# Whisper-Lite Server

WebSocket relay server for the Whisper-Lite chat client.

## What it does

Routes JSON envelopes between connected clients by nickname. Tracks
online presence. About 200 lines of Python.

The server sees plaintext message contents — that's the explicit scope
of this build. End-to-end encryption is documented as deferred work
(see `docs/SELF_ASSESSMENT.md` in the project root).

## Wire protocol

All messages are JSON objects sent as WebSocket text frames.

### Client → Server

```json
{ "type": "hello", "nick": "alice" }
{ "type": "message", "to": "bob", "text": "Hei Tero!", "seq": 1 }
{ "type": "ping" }
```

### Server → Client

```json
{ "type": "roster", "users": [{"nick": "bob"}, ...] }
{ "type": "presence", "nick": "bob", "online": true }
{ "type": "presence", "nick": "bob", "online": false }
{ "type": "message", "from": "alice", "text": "Hei Tero!", "seq": 1, "ts": 1714900000000 }
{ "type": "pong", "ts": 1714900000000 }
{ "type": "error", "reason": "nick_taken" | "unknown_recipient" | ... }
```

## Local development

```bash
pip install -r requirements.txt
bash generate_certs.sh                         # creates cert.pem, key.pem
                                               # also prints the SPKI pin
python3 server.py --cert cert.pem --key key.pem
# Listening on wss://0.0.0.0:8765
```

## Connecting from Android emulator

The Android emulator reaches the host's `localhost` as `10.0.2.2`. The
generated certificate already includes `10.0.2.2` as a SAN, so the client
can connect to `wss://10.0.2.2:8765`.

## Connecting from physical device

```bash
adb reverse tcp:8765 tcp:8765
```

Forwards device port 8765 to host port 8765, so the device sees the
server at `wss://localhost:8765`. The cert covers `localhost` too.

## Quick local test (no TLS)

For initial debugging only:

```bash
python3 server.py --no-tls --port 8765
```
