# Whisper-Lite — Self-Assessment

> Honest mapping of my work to Tero's published 0-5 rubric.
> Scoped down from the original E2EE proposal — the "Scope decision"
> section explains why upfront. One scorecard per dimension, with
> file::function-level evidence so I can point at code if asked.
> Followed by 18 viva-voce Q&A.

---

## Scope decision (explicit)

My project plan (v2.1) proposed end-to-end encryption using libsodium.
**I delivered TLS-only.** The deliverable bundle is named
"Whisper-Lite" rather than "Whisper" to make this explicit, not to hide
it.

Why the change: during the second sprint I underestimated the time
cost of cross-compiling libsodium for three Android ABIs and getting
the SPKI pin format right between Android NSC and Qt. With the
deadline approaching, I had two paths:

1. Ship E2EE half-working — visible bugs in session-key derivation,
   demo failures likely
2. Ship TLS-only end-to-end — every feature works, room for tests and
   polish

I picked option 2. I'd rather hand Tero a smaller thing that works
than a bigger thing that doesn't.

The architectural seam where E2EE re-enters is documented in
`docs/PRESENTATION.md` slide 7. It's two functions, ~20 lines of
changes — `ChatController::sendText` and `handleMessage`.

---

## Rubric scoring

### Functionality — weight 30%, my self-score: 4/5

**Required behaviour delivered:**
- Login by nickname + server URL (`qml/LoginPage.qml`).
- TLS-pinned WebSocket connect with auto-reconnect (`src/wsclient.cpp::connectTo`,
  `scheduleReconnect`).
- Online user list updated by `roster` and `presence` (`src/chatcontroller.cpp::handleRoster`,
  `handlePresence`).
- Send and receive messages (`src/chatcontroller.cpp::sendText`, `handleMessage`).
- Persistent local history (`src/messagestore.cpp`).
- System notification on incoming message (`src/notificationhelper.cpp::showMessage`).

**Beyond the baseline:**
- Reconnection with exponential backoff + ±20% jitter, capped at 12
  attempts and 30s ceiling (`src/wsclient.cpp::scheduleReconnect`).
- SQL-injection-safe storage (verified by `tst_store::sqlInjectionDefeated`).
- Two-layer TLS trust (Android NSC + app-level SPKI pin).

**Why not 5:** I'm honestly assessing the scope reduction here. The
original plan's E2EE was a defensible feature for a 5-rated project. I
ship without it, so I take 4. The lite scope is fully delivered, but
"fully delivered a smaller thing" isn't 5.

---

### Architecture & C++/QML quality — weight 25%, my self-score: 4/5

**What I'm proud of:**
- Single QML-facing orchestrator (`ChatController`) that owns all
  subsystems. QML never reaches into `WsClient` or `MessageStore`
  directly. That keeps `Q_PROPERTY`/`Q_INVOKABLE` boundaries clean.
- `Q_OBJECT` only where actually needed; pure data structs (`ChatMessage`,
  `UserInfo`) for plain holders.
- `WHISPER_QML_BUILD` compile guard means the same headers compile in
  unit tests (no QML registration noise) and in the real app.
- Models inherit `QAbstractListModel` properly with role names — no
  string-keyed hacks.
- `qDebug()` replaced with `qCDebug()` under named categories so logs
  can be silenced in production with one env var.
- The architectural seam for E2EE re-introduction is real, not aspirational —
  it's specifically two function bodies, no class hierarchy changes.

**Why not 5:**
- I'm holding raw `QVector<UserInfo>` inside `UserModel` and doing
  linear `indexOf` lookups. Fine for tens of users, would smell with
  thousands.
- `ChatController` is ~200 lines. Splitting `MessageRouter` and
  `PresenceTracker` would be cleaner once it grows.
- `Settings` uses `QSettings` directly; a real app would hide that
  behind an interface so I could swap to a different store in tests.

---

### Networking & error handling — weight 20%, my self-score: 5/5

This is where the lite scope actually helps me — without crypto in the
mix, the networking layer is the most polished part of the project.

**State machine:** `WsClient::State` is a closed enum
{`Disconnected, Connecting, Connected, Reconnecting, Failed`} — no
implicit "in-between" states.

**Backoff:** `kBaseDelayMs * 2^attempt` capped at 30s, with ±20%
jitter (`src/wsclient.cpp::scheduleReconnect`). Twelve attempts before
"Failed".

**Heartbeat:** `QWebSocket::ping()` every 25s while Connected.

**TLS errors:** every error type has a documented decision in
`src/tlsconfig.cpp::acceptablePinned` — `HostNameMismatch` is allowed
in `QT_DEBUG` builds only, `SelfSignedCertificate*` and `Untrusted`
allowed *only after* SPKI pin matches. Anything else rejected.

**JSON parsing:** `src/wsclient.cpp::onTextMessageReceived` checks
`QJsonParseError` and `isObject()` before dispatching. Malformed
frames dropped with a warning, never crash the app.

5/5 because the full reconnect lifecycle, heartbeat, TLS error policy,
and JSON robustness are in place and verifiable by killing the server
during the demo.

---

### UI/UX — weight 15%, my self-score: 4/5

**Strengths:**
- Material 3 styling with consistent colour tokens. Dark mode toggle
  persists across launches.
- Animated message bubbles (`qml/MessageDelegate.qml`) — fade-in plus
  side-slide that respects which side the message is on.
- Status indicator (`qml/StatusIndicator.qml`) with pulsing animation
  during reconnect, colour-coded {green, amber, red, grey}.
- Empty state ("Select a user to start chatting") with a lock emoji,
  not a blank page.
- Error banner that auto-hides after 4s instead of stacking forever.

**Why not 5:**
- No accessibility annotations (`Accessible.role`/`Accessible.name`).
- No haptics on message send / receive.
- No swipe-to-go-back (a chat app really wants this).

---

### Presentation & docs — weight 10%, my self-score: 5/5

The deliverable bundle includes:

- `README.md` (project overview)
- `docs/BUILD-WINDOWS.md` (16-step Windows walkthrough — no libsodium phase)
- `docs/PRESENTATION.md` (12-min slide-by-slide demo script with the
  scope decision owned upfront)
- `docs/SELF_ASSESSMENT.md` (this file)
- `server/README.md` (protocol spec + deployment)
- Inline comments on every public class header

Code is consistently formatted, no commented-out blocks, no `TODO`s
left unaddressed. The scope reduction is documented in three places
(README, presentation slide 1, this file) so it can't be missed.

---

### Estimated overall

```
0.30 × 4 + 0.25 × 4 + 0.20 × 5 + 0.15 × 4 + 0.10 × 5
= 1.20 + 1.00 + 1.00 + 0.60 + 0.50
= 4.30 → rounds to 4
```

I'm asking for a 4. A 5 would have required the E2EE I scoped out.
I'd rather be honest about that than overscore myself.

---

## Viva-voce Q&A — 18 likely questions

### Scope

**1. Why didn't you ship E2EE? You proposed it.**
Time. The libsodium Android cross-compile + SPKI pin format mismatch
between Android NSC and Qt cost me ~18 hours and I still didn't have a
clean integration. With three weeks left, I chose to deliver a working
TLS-only build rather than a half-broken E2EE one. The architectural
seam is real — two functions, well-documented in the presentation.

**2. Is "TLS-only chat" actually useful?**
For threats this defends against (passive eavesdropping, on-path
attackers without a trusted-CA cert) — yes. For threats it doesn't
defend against (server compromise, server operator) — no, and I don't
claim otherwise. Same trust class as Slack-on-corporate-VPN. Useful
inside its assumptions.

**3. How would you sell this to a real product manager?**
Honestly: as version 1. "We have a working chat infrastructure with
solid networking, TLS, persistence, and reconnect. Adding E2EE in
version 2 is a localised C++ change in two functions; we shipped a
working v1 first." That's a reasonable engineering plan.

### Architecture

**4. Why is there a server at all?**
Three reasons: (a) NAT traversal — most phones can't accept inbound
connections, (b) presence — knowing who's online without polling every
peer, (c) offline delivery if you want it later. The server doesn't
need to be smart; it just needs to exist.

**5. Why ChatController owns everything?**
Two reasons: testability (I can unit-test `MessageStore` without QML),
and single-point-of-policy (every "what should I do when X fails"
decision lives in one file, audited in one place).

**6. Why SQLite over a NoSQL store?**
SQLite ships with Qt and is on every target without packaging effort.
The schema is two columns and an index — I don't need a query
planner, but I do need transactional inserts. WAL mode handles that.

**7. Why QML for the UI instead of Qt Widgets?**
Two reasons: (a) the course is "Qt Advanced" and QML is the modern
path, (b) animation API is much nicer in QML — the message bubble
fade-and-slide would be 30 lines of QPropertyAnimation in widgets vs
4 lines of `NumberAnimation` in QML.

### Networking

**8. Why exponential backoff with jitter?**
Without jitter, a thundering herd is real — 100 clients losing a flaky
network all retry at exactly 1s, then 2s, then 4s. ±20% jitter spreads
the herd. Cap at 30s so a returning user doesn't wait 8 minutes for
the 8th retry.

**9. What close codes does WebSocket use, and how do you treat them?**
1000 (normal) and 1001 (going away) trigger reconnect because for our
use case the user wants the app to stay online. 1006 (abnormal) and
1011 (server error) also reconnect. 1008 (policy violation) ideally
shouldn't reconnect but I haven't implemented that distinction.

**10. What's the difference between cert pinning and SPKI pinning?**
Cert pinning binds you to a *specific certificate* — when the cert
rotates (e.g. Let's Encrypt every 90 days), the pin breaks and your
app stops working until the next release. SPKI pinning binds you to
the *public key inside the cert*, so you can rotate certs as long as
the keypair is preserved.

### Storage

**11. Why is the SQLite path per-nickname?**
So two test users on the same desktop don't trample each other's
history. Multi-device sync is out of scope.

**12. What's the threat model around the SQLite file?**
Plaintext on disk. If the device is unlocked and an attacker has
shell access, they can read it. Mitigation: SQLCipher (transparent
AES) or libsodium-encrypted blobs. Documented as v2 work.

### Engineering practices

**13. What did unit tests catch that you wouldn't have caught manually?**
The `sqlInjectionDefeated` test exists because I genuinely wasn't sure
my QSqlQuery::bindValue usage was binding correctly until the test
proved it. That kind of negative test is harder to think of for
manual testing but cheap to write as code.

**14. What didn't you test? Why?**
WebSocket reconnect under network flakiness. Either you mock time and
the QWebSocket internals (massive setup, brittle), or you stand up a
real network with `tc` rules. Neither felt worth it for a 6-week
project. I verify reconnect manually during the demo.

**15. How long did it take, and where did the time go?**
~70 hours over four weeks. Rough breakdown:
- 10h reading Qt 6.10 / Android docs and kicking the tires
- 18h on the abandoned E2EE attempt (this is what the scope decision
  was about — fully owned and documented in slide 1)
- 14h server + protocol design and rewrite (lite version)
- 12h QML/QtQuick UI
- 8h Android-specific bits (NSC, JNI for notifications)
- 4h tests
- 4h docs + presentation

**16. What surprised you?**
That the JNI for notifications was easier than I expected once I had
the channel registration figured out, and that QML's StackView did
exactly what I needed for navigation without fighting it. The hardest
single thing — and the one that triggered the scope reduction — was
the SPKI pin format mismatch between Android NSC and Qt. Documented in
`tlsconfig.cpp::computeSpkiSha256`.

**17. If you had another week, what would you do?**
Re-attempt the E2EE integration with my hard-won understanding of the
SPKI pin format and the Android-NDK toolchain. The crypto code itself
isn't the hard part; the integration plumbing is, and I now know
where the rocks are.

**18. What's the single biggest thing you learned?**
Picking what *not* to build. My instinct early in the project was to
keep cramming features in. The decision to ship lite-scope rather than
broken-full-scope was uncomfortable but right. That's a more
transferable skill than any specific Qt API.
