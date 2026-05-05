# Whisper-Lite

TLS-secured WebSocket chat for Android, built with Qt 6.10.

Final project for **TT00GD57-3001 Qt Advanced**, Metropolia UAS, May 2026.

## What it is

Two Android phones can chat via a relay server. Messages travel over
TLS 1.2+ with SPKI-pinned certificate verification. The server itself
sees plaintext routing — end-to-end encryption is documented as
deferred work in `docs/SELF_ASSESSMENT.md`.

```
┌──── Android ────┐                ┌──── Android ────┐
│  alice's app    │      TLS       │  bob's app      │
│                 │←──────────────→│                 │
└─────────┬───────┘                └────────┬────────┘
          │                                 │
          ▼                                 ▼
  ┌───────────────────────────────────────────┐
  │  Whisper-Lite relay (Python asyncio + TLS)│
  │  ~200 lines — routes JSON envelopes       │
  └───────────────────────────────────────────┘
```

## Why "Lite"

The original project plan proposed end-to-end encryption with libsodium.
I delivered TLS-only after hitting integration friction with the Android
libsodium cross-compile and SPKI pin format mismatch between Android's
Network Security Config and Qt. That decision is owned upfront in
`docs/PRESENTATION.md` slide 1 and reflected in the rubric scoring in
`docs/SELF_ASSESSMENT.md`. The architectural seam where E2EE re-enters
is two functions in `src/chatcontroller.cpp` — about 20 lines of
changes plus a new `CryptoManager` class.

## Repository layout

```
whisper-lite/
├── client/                   Qt 6.10 application (Android + Desktop)
│   ├── src/                  C++ — controller, network, storage
│   ├── qml/                  Qt Quick UI (Material 3)
│   ├── android/              Manifest, network security config, certs
│   ├── tests/                Qt Test unit tests
│   └── CMakeLists.txt
├── server/                   Python 3 relay (asyncio + websockets 16)
│   ├── server.py
│   ├── generate_certs.sh
│   ├── Dockerfile
│   └── README.md             Wire protocol spec, deployment notes
└── docs/
    ├── BUILD-WINDOWS.md      16-step Windows build & demo guide
    ├── PRESENTATION.md       12-minute demo script
    └── SELF_ASSESSMENT.md    Rubric mapping + viva-voce Q&A
```

## Quick start

Full Windows walkthrough: **[docs/BUILD-WINDOWS.md](docs/BUILD-WINDOWS.md)**.

In one paragraph: install Qt 6.10.2 with the Android kit and OpenSSL,
run `generate_certs.sh` to make a self-signed dev cert plus its SPKI
pin, paste both into client resources, start `server.py`, build the
APK in Qt Creator with the Android kit, deploy to the emulator (URL
`wss://10.0.2.2:8765`) or to a phone (after `adb reverse tcp:8765
tcp:8765`, URL `wss://localhost:8765`).

## Test status

```
$ ctest --output-on-failure
Test #1: tst_store ............ 7/7 pass
```

## Wire protocol in one paragraph

JSON envelopes over WebSocket. Client sends `hello` (with nickname),
server replies with `roster` (list of online nicks) and broadcasts
`presence` events. Messages are `{type: "message", to: <nick>, text:
<plain>, seq: <n>}`. Server routes by `to` and forwards as `{type:
"message", from: <nick>, text: <plain>, seq, ts}`. Errors are `{type:
"error", reason: <string>}`.

## TLS in one paragraph

The Android Network Security Config trusts a debug-only bundled CA
(`whisper_ca.pem`). On top of that, the app independently verifies the
peer's `SHA-256(SubjectPublicKeyInfo)` against a baked-in pin
(`spki_pin.txt`). Either layer alone would be defeatable; together,
an attacker would have to modify the APK itself.

## License

MIT (see LICENSE).

## Author

Sumith Shaha · sumithshaha.shaha@metropolia.fi · May 2026
