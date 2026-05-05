#!/usr/bin/env python3
"""
Whisper-Lite WebSocket relay server.

Scope: TLS protects the wire, the server sees plaintext. End-to-end
encryption is documented as deferred work in docs/SELF_ASSESSMENT.md.

Run:
    bash generate_certs.sh
    python3 server.py --cert cert.pem --key key.pem
"""

import argparse
import asyncio
import json
import logging
import ssl
import time
from typing import Dict, Optional

import websockets
from websockets.asyncio.server import serve, ServerConnection

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
)
log = logging.getLogger("whisper")


class Hub:
    """Routes envelopes between connected sessions. Keeps no message history."""

    def __init__(self) -> None:
        self.clients: Dict[str, "Session"] = {}

    async def register(self, sess: "Session") -> bool:
        if sess.nick in self.clients:
            await sess.send({"type": "error", "reason": "nick_taken"})
            return False
        self.clients[sess.nick] = sess
        log.info("REGISTER %s (online=%d)", sess.nick, len(self.clients))

        # Send the new client a roster of everyone currently online
        await sess.send({
            "type": "roster",
            "users": [{"nick": n} for n in self.clients if n != sess.nick],
        })
        # Tell everyone else that this user just came online
        await self.broadcast(
            {"type": "presence", "nick": sess.nick, "online": True},
            exclude=sess.nick,
        )
        return True

    async def unregister(self, sess: "Session") -> None:
        if sess.nick and sess.nick in self.clients:
            self.clients.pop(sess.nick, None)
            log.info("UNREGISTER %s (online=%d)", sess.nick, len(self.clients))
            await self.broadcast(
                {"type": "presence", "nick": sess.nick, "online": False},
            )

    async def route(self, sender: "Session", msg: dict) -> None:
        target_nick = msg.get("to")
        if not isinstance(target_nick, str):
            await sender.send({"type": "error", "reason": "bad_to_field"})
            return
        target = self.clients.get(target_nick)
        if not target:
            await sender.send({"type": "error", "reason": "unknown_recipient"})
            return
        await target.send({
            "type": "message",
            "from": sender.nick,
            "text": msg.get("text", ""),
            "seq":  msg.get("seq", 0),
            "ts":   int(time.time() * 1000),
        })

    async def broadcast(self, payload: dict, exclude: Optional[str] = None) -> None:
        # Snapshot the items so a concurrent unregister() can't mutate the dict
        # while we're iterating. (Hit by tearing down two clients at once.)
        dead = []
        for nick, sess in list(self.clients.items()):
            if nick == exclude:
                continue
            try:
                await sess.send(payload)
            except Exception as e:
                log.warning("broadcast failed for %s: %s", nick, e)
                dead.append(nick)
        for nick in dead:
            self.clients.pop(nick, None)


class Session:
    """Per-connection state machine."""

    def __init__(self, ws: ServerConnection, hub: Hub) -> None:
        self.ws = ws
        self.hub = hub
        self.nick: Optional[str] = None

    async def send(self, obj: dict) -> None:
        await self.ws.send(json.dumps(obj))

    async def run(self) -> None:
        try:
            async for raw in self.ws:
                try:
                    msg = json.loads(raw)
                except json.JSONDecodeError:
                    await self.send({"type": "error", "reason": "bad_json"})
                    continue
                if not isinstance(msg, dict):
                    await self.send({"type": "error", "reason": "not_object"})
                    continue

                t = msg.get("type")
                if t == "hello":
                    nick = (msg.get("nick") or "").strip()
                    if not nick or len(nick) > 32:
                        await self.send({"type": "error",
                                         "reason": "hello_invalid"})
                        break
                    self.nick = nick
                    if not await self.hub.register(self):
                        break
                elif t == "message":
                    if not self.nick:
                        await self.send({"type": "error",
                                         "reason": "must_hello_first"})
                        continue
                    await self.hub.route(self, msg)
                elif t == "ping":
                    await self.send({"type": "pong",
                                     "ts": int(time.time() * 1000)})
                else:
                    await self.send({"type": "error",
                                     "reason": "unknown_type"})
        except websockets.ConnectionClosed:
            pass
        finally:
            await self.hub.unregister(self)


async def main() -> None:
    p = argparse.ArgumentParser()
    p.add_argument("--cert", default="cert.pem")
    p.add_argument("--key", default="key.pem")
    p.add_argument("--host", default="0.0.0.0")
    p.add_argument("--port", type=int, default=8765)
    p.add_argument("--no-tls", action="store_true",
                   help="DEV ONLY: run plain ws:// (no TLS)")
    args = p.parse_args()

    ssl_ctx = None
    if not args.no_tls:
        ssl_ctx = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
        ssl_ctx.minimum_version = ssl.TLSVersion.TLSv1_2
        ssl_ctx.load_cert_chain(args.cert, args.key)

    hub = Hub()

    async def handler(ws: ServerConnection) -> None:
        await Session(ws, hub).run()

    scheme = "ws" if args.no_tls else "wss"
    log.info("Whisper-Lite relay listening on %s://%s:%d",
             scheme, args.host, args.port)
    async with serve(handler, args.host, args.port, ssl=ssl_ctx,
                     ping_interval=25, ping_timeout=10):
        await asyncio.Future()


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        log.info("shutting down")
