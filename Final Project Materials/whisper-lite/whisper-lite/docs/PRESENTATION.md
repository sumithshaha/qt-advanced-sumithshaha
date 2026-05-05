# Whisper-Lite — Presentation Script (12 minutes)

> Final demo for Qt Advanced TT00GD57-3001, Metropolia UAS.
> Audience: Tero Alamäki + classmates.
> Goal: communicate the architecture honestly, demo the working app, and
> own the scope-down decision rather than hide it.

---

## Slide 0 — Title (15s)

**Whisper-Lite — TLS-secured chat for Android with Qt 6.10**
Sumith Shaha · Qt Advanced final project · May 2026

> "I built a chat app for Android using Qt 6.10. This presentation walks
> through how it's structured, demos it on stage, and explains one
> deliberate scope decision I made along the way."

---

## Slide 1 — Scope decision, owned upfront (60s)

My original project plan (v2.1) proposed end-to-end encryption using
libsodium. **I delivered TLS-only.** I want to address that directly
before the demo, not hide it in the appendix.

**What changed:** During the second sprint, the libsodium Android
cross-compile + Qt resource integration consumed more time than
budgeted. I had to choose between:

1. Ship E2EE half-working — race conditions in session-key derivation,
   undocumented edge cases, demo failures likely
2. Ship TLS-only end-to-end — every feature works, every edge case
   handled, room left for tests and polish

I picked option 2. The deliverable is **smaller in scope but larger in
quality** than option 1 would have been.

**What this means architecturally:** the wire is protected from passive
network observers (TLS 1.2+, SPKI-pinned cert). The server itself is
trusted with plaintext routing. That's the same trust model as
Slack-on-the-wire or any classic chat relay — different from Signal-style
E2EE, and I'm not pretending otherwise.

**What it means for re-introducing E2EE:** I designed the architecture
so adding it back is a localised change. `ChatController::sendText` and
`handleMessage` are the two functions that would gain `encryptFor()` /
`decryptFrom()` calls; everything else (UI, models, persistence,
reconnect, TLS) is untouched. I'll show the seam in slide 7.

---

## Slide 2 — Architecture in one diagram (60s)

```
┌────────────────────┐                  ┌────────────────────┐
│  Whisper Android   │                  │  Whisper Android   │
│  (Qt Quick QML UI) │                  │  (Qt Quick QML UI) │
│   ChatController   │                  │   ChatController   │
│   ┌─────────────┐  │                  │   ┌─────────────┐  │
│   │  WsClient   │  │                  │   │  WsClient   │  │
│   └──────┬──────┘  │                  │   └──────┬──────┘  │
│          ↓          │                  │          ↓          │
│     QSslSocket     │                  │     QSslSocket     │
└──────────┬─────────┘                  └─────────┬──────────┘
           │                                      │
           │ wss://server:8765 (TLS 1.2+,         │
           │  SPKI-pinned, self-signed dev cert)  │
           └──────────────┬───────────────────────┘
                          │
              ┌───────────▼───────────┐
              │  Whisper relay        │
              │  (Python asyncio)     │
              │  - registers nicks    │
              │  - routes by 'to'     │
              │  - sees plaintext     │
              └───────────────────────┘
```

Three things to point at:

1. **The server is intentionally minimal.** Two hundred lines of
   Python. JSON envelopes in, JSON envelopes out. No persistence, no
   authentication beyond unique nicknames.
2. **TLS pinning is non-trivial.** Self-signed dev certs need a custom
   trust path on Android — see slide 4.
3. **The architectural seam where E2EE re-enters** is one C++ file:
   `ChatController.cpp`, two functions. I'll show this in slide 7.

---

## Slide 3 — The Qt/QML boundary (60s)

The single most opinionated decision in this codebase: **all
interaction with C++ goes through one `Q_OBJECT`**, the
`ChatController`. QML never imports `WsClient` or `MessageStore`
directly.

```cpp
class ChatController : public QObject {
    Q_PROPERTY(MessageModel* messages READ messages CONSTANT)
    Q_PROPERTY(UserModel* users READ users CONSTANT)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged)
    Q_INVOKABLE void sendText(const QString &peer, const QString &text);
    Q_INVOKABLE void selectPeer(const QString &peer);
    // ...
};
```

Why? Three reasons:

1. **Testable.** I can unit-test `MessageStore` without instantiating a
   QML engine.
2. **One audit point for failure paths.** "What happens when X fails"
   is decided in one file, not scattered across QML signal handlers.
3. **Future-proof.** When E2EE comes back, all the new failure modes
   (bad MAC, missing peer key, etc.) plug into `setLastError()` —
   the QML side just keeps showing the error banner it already shows.

The `WHISPER_QML_BUILD` compile guard means the same headers compile
in unit tests (no QML registration noise) and in the real app (with
`QML_ELEMENT` registration).

---

## Slide 4 — TLS pinning, two layers (45s)

The wire is protected by two independent checks.

**Layer 1 — Android Network Security Config:** the dev cert is bundled
in `res/raw/whisper_ca.pem` and registered as a debug-only trust
anchor in `network_security_config.xml`. Release builds ignore this
entirely, so we cannot accidentally ship a permissive config to
production.

**Layer 2 — App-level SPKI pin:** `TlsConfig::acceptablePinned()`
computes `SHA-256(SubjectPublicKeyInfo)` of whatever cert the peer
presents and compares against a baked-in pin in `res/raw/spki_pin.txt`.
Even if a malicious CA somehow signed a cert for the same hostname,
the pin still catches it.

```cpp
const QByteArray expected = expectedSpkiSha256();
const QByteArray actual   = computeSpkiSha256(peerCert);
if (expected != actual) return false;  // pin mismatch — abort
```

Either layer alone would be defeatable. Together, an attacker would
need to modify the APK itself.

This is the part I'm most proud of — it's the same defence-in-depth
pattern Signal and WhatsApp use, just applied to a school project.

---

## Slide 5 — Live demo (3 minutes)

Setup on screen:

- Left half: phone (or emulator) running Whisper-Lite as `alice`
- Right half: desktop client running Whisper-Lite as `bob`
- Bottom: server terminal scrolling INFO logs

Demo sequence:

1. **(30s)** Type "Hei Tero!" on alice. Press Send. Show it appearing on
   bob with the slide-and-fade animation. Status indicators stay green.
2. **(30s)** Switch to the server terminal. Show the routed line.
   Acknowledge: yes, the server saw the plaintext — that's the scope
   decision from slide 1, not a bug.
3. **(45s)** Open SQLite databases on both sides — show local
   persistence, history survives an app restart.
4. **(30s)** Kill the server with Ctrl+C. Watch the green dot turn
   amber, then "Reconnecting (attempt 2)…", then "Reconnecting (attempt
   3)…". Restart the server. Both clients reconnect within ~2s. Send
   another message — works.
5. **(30s)** As a parting move: show that the SPKI pin works. Stop the
   server, regenerate the cert without updating the pin, restart.
   Watch both clients fail with `TLS pin/error rejected` in the log.
   Restore the matching cert. They reconnect. Pinning is real.
6. **(15s)** Quick handoff to the slides for the architectural detail.

---

## Slide 6 — Tests + quality gates (45s)

```
$ ctest --output-on-failure
Test #1: tst_store ............ Passed (7/7)
```

Seven tests, real assertions, no skips:

- **`openCreatesSchema`** — fresh DB gets the right schema
- **`insertAndLoad`** — round-trip through SQLite
- **`orderingIsByTimestamp`** — explicit ORDER BY ts, not insert order
- **`sqlInjectionDefeated`** — peer name `'; DROP TABLE messages; --`
  is stored verbatim, the table survives, querying it returns the
  injection-attempt as data, not as SQL
- **`knownPeers`** — returns distinct peers, alphabetical

What I deliberately *didn't* test: the WebSocket reconnect logic.
Mocking time inside `QTimer` is expensive; a real wall-clock test
takes 30 seconds per run. The reconnect is verified manually during
the demo (step 4), and I've tagged this as "test-coverage gap" in the
self-assessment.

---

## Slide 7 — The E2EE seam (45s)

If you give me another 20 hours:

```cpp
// In ChatController::sendText, before m_ws.sendJson(env):
const QByteArray blob = m_crypto->encryptFor(peer, m_myNick, text.toUtf8());
QJsonObject env{
    { "type", "cipher" },
    { "to",   peer },
    { "blob", QString::fromLatin1(blob) },  // base64, opaque
    { "seq",  ++m_seq },
    { "ts",   ts },
};

// In ChatController::handleCipher:
const QByteArray plain = m_crypto->decryptFrom(from, m_myNick, blob);
if (plain.isEmpty()) return;  // MAC failure — drop silently
```

That's it. Two functions, ~20 lines of changes. The crypto module
(`CryptoManager`) is a single new class that wraps libsodium's
`crypto_kx` (X25519 key exchange) and `crypto_secretbox_easy`
(XSalsa20-Poly1305 AEAD). Nicknames decide roles deterministically —
the lex-smaller side is the libsodium "client" — so both peers derive
matching session keys without a handshake.

I have working test code for this from my prototype. I just couldn't
land it cleanly within the deadline.

---

## Slide 8 — What I'd build next (30s)

In priority order:

1. **Add E2EE back** — exactly the seam shown above. Stretch goal: persist
   long-term identity keys in Android Keystore so users keep their
   identity across sessions.
2. **Group chat** — Signal "sender keys" pattern is the simple version;
   MLS (RFC 9420) is the proper version.
3. **Real production deployment** — drop the self-signed cert, get
   Let's Encrypt on a real domain hosted on CSC cPouta.
4. **WebSocket reconnect tests** — bite the bullet on time-mocking.

---

## Slide 9 — Final thought (15s)

> Picking what *not* to build is half of engineering. Whisper-Lite is
> what's left after I made that pick honestly. The seam for the next
> iteration is right there in the code, waiting.

Thank you. Questions?

---

## Q&A prep — likely Tero questions

**Q: "Why didn't you just keep the E2EE you had working from the
project plan v2.1?"**
A: That earlier project used Qt 6.4 and a different architecture. The
crypto code itself was solid, but the integration cost into the new Qt
6.10 / QML / Android-NDK toolchain — particularly cross-compiling
libsodium for three Android ABIs — was where the time went. The
*algorithm* is well-understood; the *toolchain plumbing* was what
defeated me.

**Q: "What's the threat model of TLS-only chat? Who is it actually
secure against?"**
A: Passive network observers (anyone tapping the wire), low-skill
on-path attackers without a trusted CA, and stolen device data
*if* the SQLite file gets disk-encrypted. It's *not* secure against
the server operator, a server compromise, or a forensic read of the
device. I'd put it in roughly the same trust class as company Slack on
a corporate VPN — useful, but with explicit assumptions.

**Q: "Why TLS pinning if you're not doing E2EE?"**
A: Because TLS without pinning is defeatable by anyone who can get a
cert from any trusted CA — Lavabit, DigiNotar, etc. all happened.
Pinning narrows the trust set from "every CA on Earth" to "this exact
public key." It costs ~50 lines of code and one base64 string in
resources. Cheap insurance.

**Q: "What's the most important test you wrote?"**
A: `sqlInjectionDefeated`. It feels paranoid until you read CVE
databases and realise how many shipped chat apps got this wrong. The
test stores `'; DROP TABLE messages; --` as a peer name and verifies
the table is still there afterwards. Bound parameters (not string
concatenation) make it trivially safe — but the test ensures nobody
"refactors that helper" later and breaks it.

**Q: "How long did this actually take?"**
A: ~70 hours over four weeks. Honest breakdown:
- 10h reading Qt 6.10 + Android docs and kicking the tires
- 18h on the abandoned E2EE attempt (libsodium cross-compile, ABI
  juggling, SPKI pin format mismatches between Android NSC and Qt) —
  this is what the scope decision was about
- 14h server + protocol design
- 12h QML/QtQuick UI + animations
- 8h Android-specific bits (NSC, JNI for notifications)
- 4h tests
- 4h docs + presentation

That's the truthful estimate, including the abandoned work. The rubric
estimate of 60-80h held up.

**Q: "What surprised you?"**
A: That QML's StackView did exactly what I needed for navigation
without fighting it, and that JNI for Android notifications was
easier than I expected once I had the channel registration figured
out. The hardest single thing was the SPKI pin format mismatch
between Android NSC and Qt — Android wants the pin without trailing
`=` padding, Qt's `QSslCertificate::publicKey().toDer()` gives the
SPKI block I expected. Half a debugging session, fully documented in
`tlsconfig.cpp::computeSpkiSha256`.

**Q: "How does reconnection know it's the right server after a
reconnect?"**
A: Same SPKI pin check fires on every TLS handshake. If someone swapped
the server while we were disconnected, the pin mismatch would fail the
reconnect.

**Q: "What if two users pick the same nickname?"**
A: Server replies with `{"type":"error","reason":"nick_taken"}` and
closes the connection. Client shows the error banner. There's no
nickname squatting protection between sessions — that's a known
limitation; production would tie nick to a verified identity.

**Q: "Why a dedicated server at all? Couldn't peers find each other
directly?"**
A: For three reasons: (a) NAT traversal — most phones can't accept
inbound connections, (b) presence — knowing who's online without
polling every peer, (c) offline delivery if you ever want it. The
server doesn't have to be smart; it just has to exist.
