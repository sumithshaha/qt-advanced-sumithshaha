# Whisper-Lite — Windows Build & Demo Guide

> Target: Windows 11 with Qt 6.10.2 + Android Studio already installed.
> Both **emulator** and **physical-device** demo paths are covered.
> Scope: TLS over WebSocket. E2EE deferred (see `docs/SELF_ASSESSMENT.md`).

This guide gets you from a freshly extracted zip to a running APK in
**16 numbered steps** — no libsodium cross-compile, no SODIUM_ROOT, no
ABI directory renames. Steps that apply only to one demo path are
tagged `[EMULATOR]` or `[DEVICE]`.

---

## Part A — One-time setup

### 1. Extract the project

Extract `whisper-lite.zip` somewhere short — long Windows paths break
some Qt build rules:

```
C:\dev\whisper\
```

### 2. Open PowerShell as Administrator

```powershell
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
```

if PowerShell scripts are blocked.

### 3. Verify Qt 6.10.2 components

Open **Qt Maintenance Tool** (`C:\Qt\MaintenanceTool.exe` by default).
Under **Qt 6.10.2** make sure these are checked:

- `MinGW 13.1.0 64-bit` (for the desktop kit)
- `Android` (for the Android kit)
- `Qt 5 Compatibility Module`
- `Qt WebSockets`
- `Sources` (handy for jumping to definitions)

Under **Developer and Designer Tools** check:

- `CMake 3.30.x` (anything ≥ 3.21 works)
- `Ninja 1.12.x`
- `OpenSSL 3.0.x Toolkit` ← gives you `openssl.exe` for cert generation

If you didn't install OpenSSL through Qt, install it via Chocolatey:

```powershell
choco install openssl -y
```

### 4. Verify Android toolchain

Qt Creator → **Edit → Preferences → Devices → Android** must show all
green ticks. Versions Qt 6.10.2 expects:

| Component       | Version              |
|-----------------|----------------------|
| JDK             | 17 (any OpenJDK 17.x)|
| Android SDK     | API 35 (platform)    |
| Build-tools     | 35.x.x               |
| NDK             | 27.2.12479018        |
| Platform-tools  | latest               |

If anything is yellow, click **Setup SDK** in that pane and let Qt
Creator resolve it.

### 5. Generate the development TLS certificate

Open Git Bash:

```bash
cd /c/dev/whisper/server
bash generate_certs.sh
```

Output ends with the SPKI pin. Copy the new `cert.pem` over the bundled
one in client resources:

```powershell
copy server\cert.pem client\android\res\raw\whisper_ca.pem
```

Then open `client\android\res\raw\spki_pin.txt` in Notepad and paste
the new SPKI pin from the script's output. **Both files must come from
the same cert generation run** or the TLS handshake will fail.

> Why both? The PEM is what Android's NSC trusts at the TLS layer; the
> SPKI pin is what `TlsConfig::acceptablePinned()` re-checks at the
> application layer. Two independent gates — defence in depth.

---

## Part B — Run the server

### 6. Install Python dependencies

```powershell
cd C:\dev\whisper\server
py -3.12 -m pip install -r requirements.txt
```

(Python 3.10+ works. `websockets ≥ 13` is the only requirement.)

### 7. Start the server

```powershell
py -3.12 server.py --cert cert.pem --key key.pem
```

You should see:

```
2026-05-05 12:00:00,000 [INFO] Whisper-Lite relay listening on wss://0.0.0.0:8765
```

**Leave this terminal open** for the rest of the demo. Open a second
PowerShell window for the client work.

### 8. Allow port 8765 through Windows Firewall (one-time)

The first time the server starts, Windows pops up a firewall dialog.
Tick **Private networks** and click **Allow access**. If you missed it:

```powershell
New-NetFirewallRule -DisplayName "Whisper relay" -Direction Inbound `
    -Protocol TCP -LocalPort 8765 -Action Allow -Profile Private
```

---

## Part C — Build & test on desktop

### 9. Configure with the desktop kit

In Qt Creator: **File → Open File or Project →
`C:\dev\whisper\client\CMakeLists.txt`**.

Pick the **Desktop Qt 6.10.2 MinGW 64-bit** kit and click **Configure
Project**. CMake should configure cleanly with no errors — there's no
external dependency to find now.

### 10. Run unit tests

In Qt Creator's left sidebar → **Projects → Build & Run → Tests**
(or `Ctrl+Alt+T`). All 7 tests should turn green:

- `tst_store`: `openCreatesSchema`, `insertAndLoad`,
  `orderingIsByTimestamp`, `sqlInjectionDefeated`, `knownPeers`
  (+ initTestCase + cleanupTestCase)

### 11. Run the desktop client (sanity check)

Switch the active target to `appwhisper` and hit **Run** (Ctrl+R). The
desktop window opens with the login page. Type:

- Nickname: `desktoptest`
- URL: `wss://localhost:8765`

Click **Connect**. You'll see the **Connected** indicator turn green and
the server terminal log a `REGISTER desktoptest` line. Quit the desktop
app — you've now confirmed the C++/QML stack works against the real
server before involving Android at all.

---

## Part D — Build for Android

### 12. Switch to the Android Qt 6.10.2 kit

Qt Creator → **Projects (left sidebar) → Build & Run → Add Kit →
Android Qt 6.10.2 Clang Multi-Abi**.

Build configuration: **Release**.

### 13. Pick which ABIs to build

Under **Projects → Android Qt 6.10.2 → Build Android APK**, in the
**Build Architectures** section, tick:

- `arm64-v8a` (every modern phone)
- `x86_64`   (the Android emulator)

Skip `armeabi-v7a` and `x86` — fewer ABIs means faster builds.

### 14. Click Build (the hammer icon)

First build takes 3-5 minutes (Gradle warm-up + compiling for both
ABIs). Without libsodium in the mix, the build is significantly faster
than the full E2EE version.

If you get a **16 KB page-size warning**, that's QTBUG-144166. The
CMake linker flag `-Wl,-z,max-page-size=16384` is already in the
project, so the warning is informational only.

---

## Part E — Demo path 1: Android Emulator `[EMULATOR]`

### 15. `[EMULATOR]` Run the app

Pick the running emulator from the device dropdown and hit **Run**
(Ctrl+R). When the login page appears:

- Nickname: `alice`
- URL: `wss://10.0.2.2:8765`   ← `10.0.2.2` is the emulator's view of host `localhost`

Tap **Connect**. Status bar shows **Connected**, green dot.

For the second peer, use the desktop client (step 11) again with
nickname `bob` connecting to `wss://localhost:8765`. Now you have two
clients chatting through the relay — phone and desktop side-by-side.

### 16. `[EMULATOR]` Demo flow

1. Both clients connected, status green.
2. On `alice`'s phone, tap the hamburger → tap `bob` in the user list.
3. Type "Hei Tero!" and send.
4. Switch to `bob`'s window — message appears with a fade-in animation.
5. Open the **server terminal** and point at the routed log line.

---

## Part F — Demo path 2: Physical Android device `[DEVICE]`

### 15-alt. `[DEVICE]` Enable USB debugging

On the phone: **Settings → About phone → tap Build Number 7 times** →
**Settings → System → Developer Options → USB Debugging: ON**.

Plug the phone in. Phone shows "Allow USB debugging?" — tick "Always
allow from this computer", tap OK.

```powershell
adb devices
```

Phone listed as `device` (not `unauthorized`).

### 16-alt. `[DEVICE]` Forward port 8765 to the phone

```powershell
adb reverse tcp:8765 tcp:8765
```

Verify:

```powershell
adb reverse --list
# host-1 reverse 8765 -> 8765
```

Now `wss://localhost:8765` on the phone reaches the server on your PC.

In Qt Creator pick the phone from the device dropdown and hit **Run**.
Login page:

- Nickname: `alice`
- URL: `wss://localhost:8765`

Tap **Connect**. Green dot. Use the desktop client as `bob` (step 11)
for the second peer.

> **Tip:** if status stays yellow ("Reconnecting…"), the most likely
> cause is a stale `adb reverse`. Re-run
> `adb reverse tcp:8765 tcp:8765`. If you see a TLS error in `adb
> logcat`, your `spki_pin.txt` doesn't match the cert.

---

## Troubleshooting reference card

| Symptom                                                | Likely cause                                          | Fix                                                                       |
|--------------------------------------------------------|-------------------------------------------------------|---------------------------------------------------------------------------|
| App opens, status stays yellow forever                 | Server not listening / firewall blocking 8765         | Confirm step 7 terminal is still running; re-do step 8                    |
| `[ssl] error: handshake failure`                       | SPKI pin mismatch (cert regenerated, pin not updated) | Re-do step 5 — copy the *new* pin from the script output                  |
| `unknown_recipient` error in app                       | Other peer hasn't connected yet                       | Make sure both clients show green status before sending                   |
| QML keyboard doesn't dismiss after sending             | QTBUG-140897 (Android 16)                             | Tap outside the input field. Fix is in Qt 6.10.3.                         |
| Emulator can't reach `10.0.2.2:8765`                   | Server bound to `127.0.0.1` instead of `0.0.0.0`      | Default args use `--host 0.0.0.0`. If you changed it, change it back.     |
| Phone can't reach `localhost:8765`                     | `adb reverse` not active                              | `adb reverse tcp:8765 tcp:8765`                                           |
| First Gradle build hangs on "downloading dependencies" | First-time slow npm-style sync                        | Be patient (3–5 min on first run, ~30 sec thereafter)                     |
| `ld: warning: max page size of 16384`                  | NDK r27 + non-16KB-aligned ELF (QTBUG-144166)         | Already handled in CMakeLists. Warning is benign.                         |

---

## What to show during the live demo

A 12-minute live demo. See `docs/PRESENTATION.md` for the slide-by-slide
script — including the live "kill the server, watch reconnect" beat at
4 minutes and the honest scope-decision slide at the start.
