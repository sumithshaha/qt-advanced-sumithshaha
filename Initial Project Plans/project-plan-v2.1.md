---
title: "Qt Advanced Project Plan: End-to-End Encrypted Chat Application (14-Day Build, Online Demonstration)"
author: "Sumith Shaha (1902402)"
course: "TT00GD57-3001 Qt Advanced, Metropolia UAS"
home_institution: "Tampere University of Applied Sciences (TAMK)"
date: "25 April 2026"
version: "2.1"
status: "Revised for online demonstration"
demonstration_date: "Tuesday, 5 May 2026 (online via Microsoft Teams)"
---

# Qt Advanced Project Plan
## End-to-End Encrypted Chat Application — 14-Day Build

| Field | Value |
|---|---|
| Author | Sumith Shaha |
| Student number | 1902402 |
| Course | TT00GD57-3001 Qt Advanced |
| Teaching institution | Metropolia University of Applied Sciences |
| Home institution | Tampere University of Applied Sciences (TAMK) |
| Document date | 25 April 2026 |
| Version | 2.1 |
| Status | Revised for online demonstration |
| Demonstration date | Tuesday, 5 May 2026 |
| Demonstration mode | Online via Microsoft Teams |
| Target grade | 5 / 5 |

### Version history

| Version | Date | Author | Change |
|---|---|---|---|
| 0.1 | 18 Apr 2026 | Sumith Shaha | Initial outline |
| 0.5 | 19 Apr 2026 | Sumith Shaha | Wireframes and architecture drafted |
| 1.0 | 20 Apr 2026 | Sumith Shaha | First complete version, six-week schedule |
| 2.0 | 21 Apr 2026 | Sumith Shaha | Compressed to fourteen-day schedule; all scope retained |
| 2.1 | 25 Apr 2026 | Sumith Shaha | Revised for online demonstration via Microsoft Teams |

### How this plan differs from Version 1.0

Version 1.0 assumed a six-week build window with approximately fifteen hours of work per week. Version 2.0 assumes a fourteen-day build window with approximately ten to twelve focused hours per working day, ending at the demonstration scheduled for Tuesday the 5th of May 2026. The two plans target the same finished product and the same grade; they differ only in how the work is distributed across calendar time.

The scope has been deliberately retained in full rather than cut, because three conditions that normally force scope reduction do not apply to this project. The development toolchain — Qt Creator 19.0.0 Enterprise with Qt 6.11.0 — is already installed and a Qt Quick hello-world project already runs, which eliminates the single largest source of unplanned time in Qt projects. The student has prior experience deploying applications to the CSC cPouta cloud platform from an earlier course project, which removes the learning curve from the TLS and systemd configuration that would otherwise consume most of a day. The available daily hours (ten to twelve of focused work) are higher than the part-time rate assumed in the six-week plan, which materially increases what fits into the schedule.

The compression manifests in three visible ways. Section 5 is rewritten as a day-by-day schedule with fourteen entries, each carrying a binary pass-or-fail checkpoint the student can tick at the end of the day rather than a weekly milestone that only becomes verifiable after several days have passed. The risk register in section 6 gains three new rows that are specific to the compressed timeline, most importantly a two-hour blocker rule that bounds the cost of any single unresolvable problem. Section 9, the presentation plan, is updated to reflect that the backup demonstration video is recorded on day thirteen rather than in a generic "final week" slot. All other sections — vision, requirements, wireframes, architecture, quality assurance, tools, references, appendices — are identical to Version 1.0 because the project itself has not changed, only the calendar on which it will be built.

Version 2.1 then revises the demonstration plan to account for the demonstration being delivered online via Microsoft Teams rather than in person. The technical scope, the architecture, the schedule for days one through twelve, and the wireframes are all unchanged from version 2.0. What changes is how the demonstration is staged: an Android screen-mirroring tool replaces the physical phone-on-camera approach, the engineered "network drop" is performed by stopping the server process on the cPouta VM rather than by disconnecting the laptop, the equipment list shifts towards good audio and a backup internet path, and days thirteen and fourteen incorporate a Teams-based dry run alongside the existing dress rehearsal. The risk register adds two rows specific to online delivery and revises one existing row to reflect that connectivity failure now affects the demonstration's ability to take place at all rather than merely the live chat link.

### Table of contents

1. Project description and vision
2. Requirements
3. UI wireframes
4. Architecture overview
5. Fourteen-day schedule
6. Risk register
7. Quality assurance plan
8. Tools and resources
9. Presentation plan
10. References
11. Appendices

### How to view the wireframe images

The wireframes in section 3 and the architecture diagram in section 4 are embedded in this Markdown document as self-contained SVG code blocks. An SVG is a text-based image format that a compatible viewer renders as an actual picture. The most convenient way to see the pictures while reading the document is to open this file in Visual Studio Code and press `Ctrl+Shift+V`, which opens a side-by-side Markdown preview that renders SVG inline. Typora, Obsidian, and most modern Markdown previewers handle inline SVG identically. If a final PDF is required for submission, the preview pane in Visual Studio Code can be exported to PDF via the browser print dialog, which preserves the SVG rendering accurately.

For in-conversation viewing without opening any external tool, the Claude chat interface renders these same SVG blocks as live visual widgets when asked, and all sixteen wireframes were rendered this way in the conversation that produced this document.

---

## 1. Project description and vision

### 1.1 One-sentence pitch

A cross-platform Qt/QML chat application in which messages are end-to-end encrypted on the sender's device, relayed as opaque ciphertext by a self-hosted Qt/C++ WebSocket server, and decrypted only by the intended recipient, so that the server operator is structurally incapable of reading user conversations.

### 1.2 Problem statement

Mainstream consumer chat tools perform transport encryption between the client and the provider's servers, but the server itself holds the message content in plaintext, either permanently or at least transiently during routing. The same provider-readable pattern reappears in most student and hobby chat projects because building true end-to-end encryption on top of a real network protocol and a real cross-platform UI framework is considered difficult. This project is a focused demonstration that it is in fact tractable within fourteen days using Qt 6.11, libsodium, and a disciplined architecture, and that the resulting system exhibits clearly different security properties from a naive design.

### 1.3 Vision of the finished product

At the demonstration on the 5th of May, the finished product is a matched triple. On one Windows 11 laptop, the desktop client connects over WebSocket Secure to a headless Qt/C++ server running on an Ubuntu 24.04 virtual machine hosted on CSC cPouta at the domain chat.sumith.dev, authenticated by a real Let's Encrypt certificate. On one physical Android phone running Android 14 or newer, the Android client connects to the same server. Both clients have generated X25519 long-term keypairs on first launch, which are stored in the operating system's secure credential store; their public halves are published to the server on login, and their fingerprints are rendered as Signal-style 60-digit safety numbers for aloud verification. Every outgoing message is encrypted with XChaCha20-Poly1305 against the recipient's public key before it leaves the local process, so that the server sees only opaque ciphertext bytes and routes them without decryption. Files and images can be attached, chunked, encrypted per chunk, and reassembled by the recipient. The Android client displays local notifications when messages arrive in the background, respects the Android application lifecycle, and adapts its layout to phone-sized screens. The Windows client uses a wide split view with a contacts pane on the left and the conversation pane on the right, and lives as a system-tray icon when the main window is closed. Message history is persisted locally in an SQLite database. The network stack survives server restarts and network interruptions with exponential-backoff reconnection, and messages composed while offline are queued locally and flushed on reconnection.

### 1.4 Target users and personas

Three representative personas shape the design decisions without claiming that the product serves them perfectly. Anna, aged thirty-four, is a freelance journalist based in Helsinki who covers sensitive topics and needs a way to have short ad-hoc conversations with sources who already trust her personally but not any particular platform; she uses a Windows laptop at the office and an Android phone when she travels. The TAMK thesis circle is a group of four final-year Software Engineering students who collaborate on a shared bachelor's thesis and want a private channel for drafts and screenshots without relying on the university's institutional chat platform. Jussi, aged fifty-one, is an IT consultant working with small municipalities in Pirkanmaa who occasionally needs to exchange sensitive configuration details with a single contact inside a client organisation and is uncomfortable doing so over email or commercial messengers. All three share a common shape: they are adults who understand what encryption is at a conceptual level, they care about a specific property the mainstream alternatives do not give them, and they will tolerate a small amount of first-run friction in exchange for that property.

### 1.5 Context of use

The application is used in short bursts rather than continuous conversations. On desktop it runs as a windowed application during working hours and minimises to the system tray when not in focus. On Android it is checked in response to notifications, used briefly, and then backgrounded. Network conditions include reliable home and office Wi-Fi, Finnish mobile networks in the Tampere region with occasional coverage gaps on trains, and airport Wi-Fi with captive portals. The application must remain usable across all of these, including surviving a thirty-second loss of connectivity without user-visible failure beyond a reconnecting banner.

### 1.6 Target platforms

Windows 11 on the desktop covers the development machine and the most common working environment for the target users; Android covers API levels 28 through 36, spanning Android 9 through Android 16, which reaches virtually all phones currently in use in Finland. Supporting both simultaneously is the central architectural challenge of the project: it forces a clean separation between presentation and logic and demonstrates the cross-platform value proposition of Qt Quick. iOS is excluded because the author does not have access to macOS hardware. The server runs on Ubuntu 24.04 LTS inside a CSC cPouta virtual machine.

### 1.7 The single most important feature

If the project had to be reduced to a single feature, it would be the server-opaque message. Every other feature in the system is present in dozens of existing Qt chat tutorials; what distinguishes this project is the ability to demonstrate live during the final presentation that the server is routing bytes it cannot read. The acceptance criteria, the demonstration choreography, and the architectural decisions are all organised around this property.

> **Key decision.** All other features are secondary to the server-opaque property. When scope pressure arises during the fourteen days, features that threaten the demonstrability of this property are cut; features that reinforce it are preserved.

### 1.8 Scope

In-scope features are nickname-based login, WebSocket Secure transport over TLS 1.2 or newer, end-to-end encrypted one-to-one text messaging, a presence list of online users, local message history persistence, file and image sharing with per-chunk encryption, public-key fingerprint display and trust-on-first-use pinning, logout with local key wipe, a dark-mode toggle, Android local notifications when messages arrive in the background, an Android runtime permission prompt for notifications, a Windows system-tray icon with toast notifications, reconnection with exponential backoff and heartbeat-based dead-connection detection, an offline message queue that flushes on reconnection, and adaptive layouts at a six-hundred-pixel breakpoint.

Out of scope, explicitly, are phone-number or email verification of identity, multi-device synchronisation of a single identity across several devices, group chat with the Messaging Layer Security protocol, the Signal Double Ratchet and therefore forward secrecy, iOS support, voice or video calling, message editing or deletion, server-side search, federation with other servers, and any form of monetisation. Firebase Cloud Messaging for push while the application is killed is documented in the plan and the presentation but is not implemented; the project uses local notifications delivered while a WebSocket is alive, which is sufficient to demonstrate the pattern.

### 1.9 Success criteria

The project is considered successful if all five of the following statements are demonstrably true on the 5th of May. First, the application builds cleanly from a fresh checkout on both Windows 11 and Android using Qt Creator 19.0.0 and Qt 6.11.0. Second, two clients — one Windows desktop and one physical Android phone — exchange text messages, images, and files in real time through the cPouta server over real TLS. Third, a network interruption of thirty seconds causes no data loss and produces a calm reconnecting indicator in the UI, and messages typed during the interruption are delivered afterwards. Fourth, a server log shown during the demonstration exhibits only high-entropy ciphertext for message bodies, and a deliberate attempt to tamper with a ciphertext produces a visible integrity failure in the receiving client. Fifth, the architecture, threat model, and reflection on limitations are explained in the presentation with sufficient precision that the instructor accepts the project as an honest implementation rather than a surface-level demonstration.

### 1.10 Non-goals

This project is not a production Signal replacement, and the plan does not pretend otherwise. It lacks forward secrecy, deniability, and metadata protection. It has not been audited. Its user interface will not match the polish of a commercial messenger. It does not attempt to defend against a compromised endpoint, a malicious first-contact public-key substitution that the user declines to verify, or a state-level adversary with access to the network and the server. These limitations are acknowledged openly, and the project's value lies in understanding them and being able to explain them.

---

## 2. Requirements

Requirements are numbered, categorised as functional (FR) or non-functional (NFR), tagged with the rubric dimension they most contribute to (F = Functionality, A = Architecture, N = Networking, U = UI/UX, P = Presentation), and prioritised using the MoSCoW convention. In the fourteen-day compressed plan, every Must-have requirement is delivered; Could-have requirements (currently dark mode and a typing indicator) are the first cuts if any single day slips by more than two hours.

### 2.1 Functional requirements

| ID | Requirement | Rubric | MoSCoW |
|---|---|---|---|
| FR-01 | The user shall be able to enter a nickname on first launch and have the client generate a long-term X25519 keypair automatically. | F, A | Must |
| FR-02 | The server shall reject connection attempts that claim a nickname already in use by a connected client. | F, N | Must |
| FR-03 | The client shall establish a WSS connection to the server over TLS 1.2 or newer and display a clear connected / connecting / reconnecting / failed state at all times. | N, U | Must |
| FR-04 | The server shall maintain an in-memory list of online users and broadcast a presence update to all connected clients whenever the list changes. | F, A | Must |
| FR-05 | A client shall be able to send an end-to-end encrypted text message to another online user, where encryption and decryption occur only on the two endpoints. | F, A | Must |
| FR-06 | The receiving client shall display an integrity failure if a received ciphertext cannot be authenticated. | F, N | Must |
| FR-07 | The client shall persist sent and received messages locally in SQLite and reload conversation history on startup. | F, A | Must |
| FR-08 | The user shall be able to attach an image or file to a conversation, which is chunked, encrypted per chunk, and reassembled on the recipient side. | F, N | Should |
| FR-09 | The client shall display the 60-digit safety-number fingerprint of any peer and allow the user to mark a peer as verified. | F, U | Should |
| FR-10 | The client shall display a blocking warning whenever a peer's public key changes from a previously seen value (trust-on-first-use enforcement). | F, N | Must |
| FR-11 | The user shall be able to log out, which wipes the local keypair and message database after an explicit confirmation. | F | Should |
| FR-12 | The user shall be able to toggle between light and dark theme from a settings page, and the choice shall persist. | U | Could |
| FR-13 | On Android, incoming messages that arrive while the application is backgrounded shall produce a system notification visible on the lock screen. | F, U | Should |
| FR-14 | On Android API 33 and above, the application shall request the POST_NOTIFICATIONS runtime permission before attempting to post its first notification. | F | Must |
| FR-15 | On Windows, the application shall display a system-tray icon and post toast notifications for messages received while the window is not focused. | F, U | Should |
| FR-16 | The client shall automatically reconnect to the server after a transport failure using exponential backoff with random jitter. | N | Must |
| FR-17 | Messages composed while the client is not connected shall be queued locally and transmitted in order upon reconnection. | N, F | Must |
| FR-18 | The client shall detect a dead connection using WebSocket ping/pong and tear down the socket if several pongs are missed. | N | Must |
| FR-19 | The server shall apply a token-bucket rate limit per connection and close connections that exceed it. | A, N | Should |
| FR-20 | The main chat layout shall adapt between a two-pane split view on wide screens and a stacked single-pane view on narrow screens at a six-hundred-pixel breakpoint. | U, A | Must |

### 2.2 Non-functional requirements

| ID | Requirement | Rubric | MoSCoW |
|---|---|---|---|
| NFR-01 | Cold startup of the desktop client shall be under two seconds on the development machine. | U | Should |
| NFR-02 | First-message round-trip latency on a good network shall be under 500 milliseconds. | N | Should |
| NFR-03 | The client shall survive a 30-second network interruption without losing messages or crashing. | N | Must |
| NFR-04 | No operation triggered from the QML layer shall block the GUI thread for more than 100 milliseconds. | U, A | Must |
| NFR-05 | TLS 1.2 or newer shall be mandatory on the WSS transport. | N | Must |
| NFR-06 | The server shall enforce a per-connection binary-frame size limit of 10 mebibytes. | N, A | Should |
| NFR-07 | The UI shall adapt across the six-hundred-pixel breakpoint at runtime, not merely at launch. | U | Must |
| NFR-08 | The platform style shall be selected per operating system: Material on Android, FluentWinUI3 or Fusion on Windows. | U, A | Should |
| NFR-09 | The UI shall meet basic accessibility expectations: sufficient contrast in both themes, focusable controls, keyboard navigation on desktop. | U | Should |
| NFR-10 | Persisted ciphertext in the local database shall not be accompanied by plaintext on disk. | A | Must |
| NFR-11 | The Android process shall stay under approximately 150 megabytes of resident memory in normal use. | A | Should |
| NFR-12 | The build shall produce no warnings from qmllint on the default configuration. | A | Should |

---

## 3. UI wireframes

The wireframes that follow render as visual images in any Markdown viewer that supports inline SVG (VS Code preview with `Ctrl+Shift+V`, Typora, Obsidian). Each wireframe is a single self-contained SVG block preceded by a figure number and caption and followed by a short explanation of what the wireframe communicates.

The visual convention throughout is deliberate greyscale line-art rather than coloured mockups, because greyscale wireframes are read as structural proposals rather than visual proposals, and structure is the proper subject of a planning document. Phone mockups are drawn inside a rounded-rectangle phone frame with approximately 360 pixels of interior width, and desktop mockups are drawn inside a window frame with a fake title bar and approximately 1000 pixels of interior width. Where a screen behaves differently on the two platforms, both variants appear on the same SVG canvas so that the adaptation is visible at a glance.

### Figure 1 — Splash / launch screen

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 520" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="520" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="460" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <circle cx="48" cy="44" r="5" fill="#9E9E9E"/>
  <circle cx="64" cy="44" r="5" fill="#9E9E9E"/>
  <circle cx="80" cy="44" r="5" fill="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Desktop</text>
  <circle cx="440" cy="230" r="44" fill="none" stroke="#424242" stroke-width="2"/>
  <path d="M422 225 L438 241 L462 215" fill="none" stroke="#424242" stroke-width="3"/>
  <text x="440" y="310" fill="#212121" font-size="20" text-anchor="middle" font-weight="600">QtChat</text>
  <text x="440" y="336" fill="#424242" font-size="13" text-anchor="middle">End-to-end encrypted messaging</text>
  <rect x="380" y="370" width="120" height="6" rx="3" fill="#E0E0E0"/>
  <rect x="380" y="370" width="72" height="6" rx="3" fill="#9E9E9E"/>
  <text x="440" y="396" fill="#9E9E9E" font-size="11" text-anchor="middle">Loading…</text>
  <rect x="940" y="30" width="240" height="460" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="420" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <rect x="1040" y="52" width="40" height="8" rx="4" fill="#212121"/>
  <circle cx="1060" cy="260" r="28" fill="none" stroke="#424242" stroke-width="2"/>
  <path d="M1048 256 L1058 266 L1074 250" fill="none" stroke="#424242" stroke-width="3"/>
  <text x="1060" y="310" fill="#212121" font-size="14" text-anchor="middle" font-weight="600">QtChat</text>
  <text x="1060" y="330" fill="#424242" font-size="10" text-anchor="middle">Secure messaging</text>
  <rect x="1020" y="360" width="80" height="4" rx="2" fill="#E0E0E0"/>
  <rect x="1020" y="360" width="48" height="4" rx="2" fill="#9E9E9E"/>
  <text x="440" y="460" fill="#9E9E9E" font-size="11" text-anchor="middle">Desktop ~1000 px</text>
  <text x="1060" y="460" fill="#9E9E9E" font-size="11" text-anchor="middle">Android ~360 px</text>
</svg>
```

The splash screen is intentionally minimal. It displays the product mark, the product name, a one-line description, and an indeterminate progress bar that is visible while the keystore is unlocked and the database is opened. Under normal conditions the screen is shown for roughly five hundred milliseconds, and it is the only moment at which the user sees a non-interactive screen.

### Figure 2 — Login with first-run key generation notice

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 540" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="540" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="480" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Sign in</text>
  <rect x="230" y="110" width="420" height="330" rx="10" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="440" y="150" fill="#212121" font-size="16" text-anchor="middle" font-weight="600">Welcome to QtChat</text>
  <text x="440" y="172" fill="#424242" font-size="12" text-anchor="middle">Choose a nickname to get started</text>
  <text x="260" y="210" fill="#424242" font-size="11">Nickname</text>
  <rect x="260" y="220" width="360" height="36" rx="6" fill="#F5F5F5" stroke="#9E9E9E"/>
  <text x="272" y="243" fill="#9E9E9E" font-size="12">sumith</text>
  <text x="260" y="278" fill="#424242" font-size="11">Server</text>
  <rect x="260" y="288" width="360" height="36" rx="6" fill="#F5F5F5" stroke="#9E9E9E"/>
  <text x="272" y="311" fill="#9E9E9E" font-size="12">wss://chat.sumith.dev:8443</text>
  <rect x="260" y="340" width="360" height="40" rx="6" fill="#F5F5F5" stroke="#E0E0E0" stroke-dasharray="4 3"/>
  <text x="272" y="358" fill="#424242" font-size="10">🔒 On first launch a keypair will be generated</text>
  <text x="272" y="372" fill="#9E9E9E" font-size="10">and stored securely in your OS credential store.</text>
  <rect x="260" y="400" width="360" height="40" rx="6" fill="#424242"/>
  <text x="440" y="425" fill="#FFFFFF" font-size="13" text-anchor="middle" font-weight="600">Sign in</text>
  <rect x="940" y="30" width="240" height="480" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="440" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <rect x="1040" y="52" width="40" height="8" rx="4" fill="#212121"/>
  <text x="1060" y="100" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Welcome</text>
  <text x="1060" y="118" fill="#424242" font-size="9" text-anchor="middle">Choose a nickname</text>
  <text x="980" y="160" fill="#424242" font-size="9">Nickname</text>
  <rect x="980" y="168" width="160" height="28" rx="5" fill="#F5F5F5" stroke="#9E9E9E"/>
  <text x="988" y="186" fill="#9E9E9E" font-size="10">sumith</text>
  <text x="980" y="216" fill="#424242" font-size="9">Server</text>
  <rect x="980" y="224" width="160" height="28" rx="5" fill="#F5F5F5" stroke="#9E9E9E"/>
  <text x="988" y="242" fill="#9E9E9E" font-size="9">wss://chat.sumith.dev</text>
  <rect x="980" y="270" width="160" height="44" rx="5" fill="#F5F5F5" stroke="#E0E0E0" stroke-dasharray="3 2"/>
  <text x="988" y="286" fill="#424242" font-size="8">🔒 A keypair will be generated</text>
  <text x="988" y="298" fill="#9E9E9E" font-size="8">and stored in Android Keystore.</text>
  <text x="988" y="310" fill="#9E9E9E" font-size="8">This takes under a second.</text>
  <rect x="980" y="340" width="160" height="36" rx="5" fill="#424242"/>
  <text x="1060" y="363" fill="#FFFFFF" font-size="11" text-anchor="middle" font-weight="600">Sign in</text>
  <text x="440" y="490" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 2a — Desktop login</text>
  <text x="1060" y="510" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 2b — Android login</text>
</svg>
```

The login screen exposes two input fields (nickname, server URL), an explanatory note that explains the first-run keypair generation step in one sentence, and a single primary button. The server URL is prefilled with `wss://chat.sumith.dev:8443` which is the author's production deployment. Nickname validation is performed client-side before the button is enabled, and server-side uniqueness enforcement happens over the wire and produces a non-blocking toast if the nickname is rejected.

### Figure 3 — Trust-on-first-use fingerprint display

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 540" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="540" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="480" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Your identity</text>
  <rect x="180" y="100" width="520" height="370" rx="10" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="440" y="140" fill="#212121" font-size="16" text-anchor="middle" font-weight="600">Your safety number</text>
  <text x="440" y="162" fill="#424242" font-size="11" text-anchor="middle">Share this with your contacts over a trusted channel</text>
  <text x="440" y="176" fill="#424242" font-size="11" text-anchor="middle">to verify that nobody is impersonating you.</text>
  <rect x="210" y="200" width="460" height="160" rx="6" fill="#F5F5F5" stroke="#E0E0E0"/>
  <text x="440" y="232" fill="#212121" font-size="18" text-anchor="middle" font-family="monospace" letter-spacing="2">42817  90334  11902</text>
  <text x="440" y="262" fill="#212121" font-size="18" text-anchor="middle" font-family="monospace" letter-spacing="2">56701  88420  17738</text>
  <text x="440" y="292" fill="#212121" font-size="18" text-anchor="middle" font-family="monospace" letter-spacing="2">44901  23155  80604</text>
  <text x="440" y="322" fill="#212121" font-size="18" text-anchor="middle" font-family="monospace" letter-spacing="2">77112  68493  05889</text>
  <text x="440" y="352" fill="#9E9E9E" font-size="10" text-anchor="middle">60-digit BLAKE2b fingerprint of your public key</text>
  <rect x="250" y="400" width="180" height="36" rx="6" fill="#FFFFFF" stroke="#424242"/>
  <text x="340" y="424" fill="#424242" font-size="12" text-anchor="middle">Copy</text>
  <rect x="450" y="400" width="180" height="36" rx="6" fill="#424242"/>
  <text x="540" y="424" fill="#FFFFFF" font-size="12" text-anchor="middle" font-weight="600">Continue</text>
  <rect x="940" y="30" width="240" height="480" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="440" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="1060" y="90" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Your safety number</text>
  <text x="1060" y="110" fill="#9E9E9E" font-size="8" text-anchor="middle">Share over a trusted channel</text>
  <rect x="980" y="130" width="160" height="150" rx="5" fill="#F5F5F5" stroke="#E0E0E0"/>
  <text x="1060" y="156" fill="#212121" font-size="11" text-anchor="middle" font-family="monospace">42817 90334</text>
  <text x="1060" y="176" fill="#212121" font-size="11" text-anchor="middle" font-family="monospace">11902 56701</text>
  <text x="1060" y="196" fill="#212121" font-size="11" text-anchor="middle" font-family="monospace">88420 17738</text>
  <text x="1060" y="216" fill="#212121" font-size="11" text-anchor="middle" font-family="monospace">44901 23155</text>
  <text x="1060" y="236" fill="#212121" font-size="11" text-anchor="middle" font-family="monospace">80604 77112</text>
  <text x="1060" y="256" fill="#212121" font-size="11" text-anchor="middle" font-family="monospace">68493 05889</text>
  <rect x="980" y="300" width="72" height="30" rx="5" fill="#FFFFFF" stroke="#424242"/>
  <text x="1016" y="320" fill="#424242" font-size="10" text-anchor="middle">Copy</text>
  <rect x="1068" y="300" width="72" height="30" rx="5" fill="#424242"/>
  <text x="1104" y="320" fill="#FFFFFF" font-size="10" text-anchor="middle">Continue</text>
  <text x="440" y="490" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 3a — Desktop TOFU</text>
  <text x="1060" y="510" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 3b — Android TOFU</text>
</svg>
```

This screen is shown once, immediately after the first successful login. The fingerprint is derived from the user's own public key using BLAKE2b-256, and the 32-byte hash is then rendered as sixty decimal digits arranged in four rows of three groups of five digits — the same format used by Signal for safety numbers. The formatting is calibrated to be short enough to read aloud on a phone call yet long enough to provide cryptographic strength. The explanatory copy is deliberately gentle because the target users are not all cryptographers.

### Figure 4 — Main chat overview

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 560" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="560" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="500" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat</text>
  <rect x="30" y="58" width="240" height="472" fill="#FFFFFF" stroke="#E0E0E0"/>
  <rect x="46" y="74" width="208" height="36" rx="6" fill="#F5F5F5"/>
  <text x="58" y="96" fill="#9E9E9E" font-size="11">Search contacts…</text>
  <text x="46" y="134" fill="#9E9E9E" font-size="10">ONLINE</text>
  <rect x="46" y="148" width="208" height="52" rx="6" fill="#E0E0E0"/>
  <circle cx="66" cy="174" r="14" fill="#9E9E9E"/>
  <text x="66" y="178" fill="#FFFFFF" font-size="10" text-anchor="middle" font-weight="600">AN</text>
  <circle cx="76" cy="186" r="4" fill="#424242"/>
  <text x="90" y="170" fill="#212121" font-size="11" font-weight="600">Anna</text>
  <text x="90" y="186" fill="#424242" font-size="10">see you tomorrow</text>
  <text x="236" y="170" fill="#9E9E9E" font-size="9" text-anchor="end">12:41</text>
  <rect x="46" y="208" width="208" height="52" rx="6" fill="#FFFFFF"/>
  <circle cx="66" cy="234" r="14" fill="#9E9E9E"/>
  <text x="66" y="238" fill="#FFFFFF" font-size="10" text-anchor="middle" font-weight="600">JU</text>
  <circle cx="76" cy="246" r="4" fill="#424242"/>
  <text x="90" y="230" fill="#212121" font-size="11" font-weight="600">Jussi</text>
  <text x="90" y="246" fill="#424242" font-size="10">sent the config</text>
  <line x1="270" y1="58" x2="270" y2="530" stroke="#9E9E9E"/>
  <rect x="270" y="58" width="580" height="36" fill="#F5F5F5"/>
  <circle cx="294" cy="76" r="12" fill="#9E9E9E"/>
  <text x="294" y="80" fill="#FFFFFF" font-size="9" text-anchor="middle" font-weight="600">AN</text>
  <circle cx="302" cy="84" r="3" fill="#424242"/>
  <text x="316" y="80" fill="#212121" font-size="12" font-weight="600">Anna</text>
  <text x="316" y="92" fill="#9E9E9E" font-size="9">verified · online</text>
  <rect x="304" y="118" width="260" height="40" rx="10" fill="#E0E0E0"/>
  <text x="316" y="134" fill="#212121" font-size="11">Hi Sumith — did you see the</text>
  <text x="316" y="148" fill="#212121" font-size="11">proposal I sent?</text>
  <rect x="556" y="188" width="280" height="40" rx="10" fill="#424242"/>
  <text x="568" y="204" fill="#FFFFFF" font-size="11">Yes, reading it now. Meeting</text>
  <text x="568" y="218" fill="#FFFFFF" font-size="11">tomorrow still works for me.</text>
  <rect x="304" y="258" width="180" height="32" rx="10" fill="#E0E0E0"/>
  <text x="316" y="278" fill="#212121" font-size="11">see you tomorrow</text>
  <rect x="280" y="478" width="560" height="40" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="300" y="502" fill="#9E9E9E" font-size="12" text-anchor="middle">📎</text>
  <text x="320" y="502" fill="#9E9E9E" font-size="11">Message Anna…</text>
  <rect x="780" y="484" width="54" height="28" rx="4" fill="#424242"/>
  <text x="807" y="502" fill="#FFFFFF" font-size="11" text-anchor="middle" font-weight="600">Send</text>
  <rect x="940" y="30" width="240" height="500" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="460" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <rect x="960" y="50" width="200" height="36" fill="#F5F5F5"/>
  <text x="1060" y="73" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">QtChat</text>
  <rect x="975" y="96" width="170" height="26" rx="5" fill="#F5F5F5"/>
  <text x="984" y="113" fill="#9E9E9E" font-size="9">Search contacts…</text>
  <rect x="975" y="132" width="170" height="42" rx="5" fill="#E0E0E0"/>
  <circle cx="990" cy="154" r="10" fill="#9E9E9E"/>
  <text x="990" y="158" fill="#FFFFFF" font-size="8" text-anchor="middle" font-weight="600">AN</text>
  <text x="1008" y="150" fill="#212121" font-size="10" font-weight="600">Anna</text>
  <text x="1008" y="166" fill="#424242" font-size="9">see you tomorrow</text>
  <rect x="975" y="180" width="170" height="42" rx="5" fill="#FFFFFF"/>
  <circle cx="990" cy="202" r="10" fill="#9E9E9E"/>
  <text x="990" y="206" fill="#FFFFFF" font-size="8" text-anchor="middle" font-weight="600">JU</text>
  <text x="1008" y="198" fill="#212121" font-size="10" font-weight="600">Jussi</text>
  <text x="1008" y="214" fill="#424242" font-size="9">sent the config</text>
  <text x="440" y="550" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 4a — Desktop SplitView</text>
  <text x="1060" y="550" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 4b — Android StackView</text>
</svg>
```

On the desktop the main overview uses a horizontal SplitView with a contacts pane on the left and the conversation pane on the right. On Android the same two panes are split across a StackView: the contacts list is the root page, and tapping a contact pushes a chat page onto the stack. The underlying data model and controller logic are the same on both platforms — only the navigation shape adapts. This is the most important demonstration of the project's cross-platform value proposition.

### Figure 5 — Contacts list with presence and verified badges

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 500" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="500" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="440" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Contacts</text>
  <text x="70" y="96" fill="#212121" font-size="14" font-weight="600">Online (3)</text>
  <rect x="54" y="112" width="772" height="48" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <circle cx="82" cy="136" r="16" fill="#9E9E9E"/>
  <text x="82" y="140" fill="#FFFFFF" font-size="10" text-anchor="middle" font-weight="600">AN</text>
  <circle cx="94" cy="148" r="5" fill="#424242"/>
  <text x="112" y="132" fill="#212121" font-size="13" font-weight="600">Anna</text>
  <rect x="155" y="120" width="54" height="14" rx="3" fill="#E0E0E0"/>
  <text x="182" y="131" fill="#424242" font-size="9" text-anchor="middle">VERIFIED</text>
  <text x="112" y="150" fill="#424242" font-size="11">active now</text>
  <rect x="54" y="172" width="772" height="48" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <circle cx="82" cy="196" r="16" fill="#9E9E9E"/>
  <text x="82" y="200" fill="#FFFFFF" font-size="10" text-anchor="middle" font-weight="600">JU</text>
  <circle cx="94" cy="208" r="5" fill="#424242"/>
  <text x="112" y="192" fill="#212121" font-size="13" font-weight="600">Jussi</text>
  <text x="112" y="210" fill="#424242" font-size="11">active now · fingerprint not verified</text>
  <rect x="54" y="232" width="772" height="48" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <circle cx="82" cy="256" r="16" fill="#9E9E9E"/>
  <text x="82" y="260" fill="#FFFFFF" font-size="10" text-anchor="middle" font-weight="600">TE</text>
  <circle cx="94" cy="268" r="5" fill="#424242"/>
  <text x="112" y="252" fill="#212121" font-size="13" font-weight="600">Teemu</text>
  <text x="112" y="270" fill="#424242" font-size="11">active now · new key — verify before messaging</text>
  <rect x="940" y="30" width="240" height="440" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="400" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="1060" y="80" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Contacts</text>
  <rect x="975" y="100" width="170" height="42" rx="5" fill="#FFFFFF" stroke="#E0E0E0"/>
  <circle cx="990" cy="121" r="10" fill="#9E9E9E"/>
  <circle cx="998" cy="130" r="3" fill="#424242"/>
  <text x="1008" y="118" fill="#212121" font-size="10" font-weight="600">Anna</text>
  <rect x="1040" y="110" width="38" height="11" rx="2" fill="#E0E0E0"/>
  <text x="1059" y="119" fill="#424242" font-size="7" text-anchor="middle">VERIFIED</text>
  <text x="1008" y="134" fill="#424242" font-size="9">active now</text>
  <rect x="975" y="150" width="170" height="42" rx="5" fill="#FFFFFF" stroke="#E0E0E0"/>
  <circle cx="990" cy="171" r="10" fill="#9E9E9E"/>
  <circle cx="998" cy="180" r="3" fill="#424242"/>
  <text x="1008" y="168" fill="#212121" font-size="10" font-weight="600">Jussi</text>
  <text x="1008" y="184" fill="#424242" font-size="8">not verified</text>
  <text x="440" y="490" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 5a — Desktop</text>
  <text x="1060" y="490" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 5b — Android</text>
</svg>
```

The contacts list shows each peer with avatar initials, a presence dot, the display name, an optional verified badge indicating a fingerprint check has been completed, and a secondary status line. The secondary status is deliberately information-dense: for a verified contact it says "active now", for an unverified contact it says "active now · fingerprint not verified", and for a contact whose key has changed it says "active now · new key — verify before messaging", which visually stands out as something requiring attention even in greyscale.

### Figure 6 — Chat conversation with reconnecting banner

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 540" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="540" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="480" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Anna (verified)</text>
  <rect x="30" y="58" width="820" height="28" fill="#E0E0E0"/>
  <text x="440" y="76" fill="#424242" font-size="11" text-anchor="middle">⟳ Reconnecting to server… (attempt 2)</text>
  <rect x="30" y="86" width="820" height="44" fill="#FFFFFF" stroke="#E0E0E0"/>
  <circle cx="60" cy="108" r="14" fill="#9E9E9E"/>
  <text x="60" y="112" fill="#FFFFFF" font-size="10" text-anchor="middle" font-weight="600">AN</text>
  <text x="82" y="104" fill="#212121" font-size="13" font-weight="600">Anna</text>
  <text x="82" y="120" fill="#9E9E9E" font-size="10">verified · last seen 12:41</text>
  <rect x="54" y="172" width="300" height="44" rx="10" fill="#E0E0E0"/>
  <text x="66" y="192" fill="#212121" font-size="11">Hi Sumith — did you see the</text>
  <text x="66" y="206" fill="#212121" font-size="11">proposal I sent?</text>
  <rect x="526" y="246" width="300" height="44" rx="10" fill="#424242"/>
  <text x="538" y="266" fill="#FFFFFF" font-size="11">Yes, reading it now. Meeting</text>
  <text x="538" y="280" fill="#FFFFFF" font-size="11">tomorrow still works for me.</text>
  <rect x="54" y="320" width="200" height="32" rx="10" fill="#E0E0E0"/>
  <text x="66" y="340" fill="#212121" font-size="11">see you tomorrow</text>
  <rect x="526" y="380" width="300" height="44" rx="10" fill="none" stroke="#424242" stroke-dasharray="3 2" stroke-width="1.5"/>
  <text x="538" y="400" fill="#424242" font-size="11">Going for lunch, will read it</text>
  <text x="538" y="414" fill="#424242" font-size="11">carefully this afternoon.</text>
  <text x="818" y="436" fill="#9E9E9E" font-size="9" text-anchor="end">⟳ queued — will send when online</text>
  <rect x="54" y="456" width="772" height="40" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="70" y="480" fill="#9E9E9E" font-size="12">📎</text>
  <text x="96" y="480" fill="#9E9E9E" font-size="11">Message Anna…</text>
  <rect x="766" y="462" width="54" height="28" rx="4" fill="#424242"/>
  <text x="793" y="480" fill="#FFFFFF" font-size="11" text-anchor="middle" font-weight="600">Send</text>
  <rect x="940" y="30" width="240" height="480" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="440" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <rect x="960" y="50" width="200" height="28" fill="#E0E0E0"/>
  <text x="1060" y="68" fill="#424242" font-size="9" text-anchor="middle">⟳ Reconnecting… (attempt 2)</text>
  <rect x="960" y="78" width="200" height="36" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="974" y="94" fill="#424242" font-size="10">&lt;</text>
  <circle cx="994" cy="96" r="10" fill="#9E9E9E"/>
  <text x="994" y="100" fill="#FFFFFF" font-size="8" text-anchor="middle">AN</text>
  <text x="1010" y="93" fill="#212121" font-size="10" font-weight="600">Anna</text>
  <rect x="975" y="130" width="140" height="32" rx="7" fill="#E0E0E0"/>
  <text x="983" y="146" fill="#212121" font-size="9">did you see the</text>
  <rect x="1005" y="170" width="140" height="32" rx="7" fill="#424242"/>
  <text x="1013" y="186" fill="#FFFFFF" font-size="9">Yes, reading it now.</text>
  <rect x="1005" y="260" width="140" height="32" rx="7" fill="none" stroke="#424242" stroke-dasharray="2 2" stroke-width="1"/>
  <text x="1013" y="276" fill="#424242" font-size="9">Going for lunch…</text>
  <rect x="975" y="445" width="170" height="34" rx="5" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="985" y="465" fill="#9E9E9E" font-size="9">📎 Message…</text>
  <rect x="1110" y="451" width="30" height="22" rx="3" fill="#424242"/>
  <text x="1125" y="465" fill="#FFFFFF" font-size="8" text-anchor="middle">→</text>
  <text x="440" y="530" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 6a — Desktop</text>
  <text x="1060" y="530" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 6b — Android</text>
</svg>
```

The conversation view carries several composable UI states. The reconnecting banner appears at the top of the chat area whenever the client is not in the connected state and disappears automatically when connectivity is restored. Outgoing messages are right-aligned with a dark fill, incoming messages are left-aligned with a light fill, and a queued outgoing message is rendered with a distinct outlined dashed style that makes its transient status immediately obvious. The composer footer is pinned to the bottom and rises above the on-screen keyboard on Android.

### Figure 7 — Image attachment in bubble

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 440" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="440" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="380" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Anna</text>
  <rect x="54" y="90" width="260" height="200" rx="10" fill="#E0E0E0"/>
  <rect x="66" y="102" width="236" height="144" fill="#9E9E9E"/>
  <path d="M66 246 L118 180 L150 210 L200 148 L302 246 Z" fill="#424242"/>
  <circle cx="258" cy="134" r="10" fill="#FFFFFF"/>
  <text x="66" y="268" fill="#212121" font-size="10">whiteboard_notes.png  · 1.2 MB</text>
  <text x="66" y="282" fill="#9E9E9E" font-size="9">12:42 · tap to open</text>
  <rect x="486" y="306" width="340" height="40" rx="10" fill="#424242"/>
  <text x="498" y="326" fill="#FFFFFF" font-size="11">Got it — clean handwriting for once.</text>
  <rect x="940" y="30" width="240" height="380" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="340" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="1060" y="78" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Anna</text>
  <rect x="975" y="98" width="150" height="140" rx="7" fill="#E0E0E0"/>
  <rect x="983" y="106" width="134" height="100" fill="#9E9E9E"/>
  <path d="M983 206 L1015 160 L1033 180 L1065 140 L1117 206 Z" fill="#424242"/>
  <circle cx="1095" cy="130" r="6" fill="#FFFFFF"/>
  <text x="983" y="222" fill="#212121" font-size="8">whiteboard_notes.png</text>
  <text x="983" y="232" fill="#9E9E9E" font-size="7">1.2 MB · 12:42</text>
  <rect x="995" y="258" width="150" height="26" rx="6" fill="#424242"/>
  <text x="1003" y="274" fill="#FFFFFF" font-size="8">Got it — clean handwriting</text>
  <text x="440" y="430" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 7a — Desktop</text>
  <text x="1060" y="430" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 7b — Android</text>
</svg>
```

An image attachment renders inline in the message bubble. The thumbnail is shown at a bounded aspect ratio, the filename and size are shown below the image, and the full-size image opens in a dedicated viewer on tap. The thumbnail is decrypted on-demand by the asynchronous image provider described in the architecture section, which keeps the UI thread free while the image is being decrypted and downscaled.

### Figure 8 — Settings

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 520" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="520" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="460" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Settings</text>
  <text x="58" y="92" fill="#9E9E9E" font-size="10">ACCOUNT</text>
  <rect x="54" y="106" width="772" height="52" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="72" y="128" fill="#212121" font-size="12" font-weight="600">Nickname</text>
  <text x="72" y="146" fill="#424242" font-size="11">sumith</text>
  <rect x="54" y="170" width="772" height="74" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="72" y="192" fill="#212121" font-size="12" font-weight="600">Your safety number</text>
  <text x="72" y="210" fill="#424242" font-size="10" font-family="monospace">42817 90334 11902 · 56701 88420 17738 · 44901 23155 80604 · 77112 68493 05889</text>
  <text x="58" y="276" fill="#9E9E9E" font-size="10">APPEARANCE</text>
  <rect x="54" y="290" width="772" height="44" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="72" y="316" fill="#212121" font-size="12" font-weight="600">Dark mode</text>
  <rect x="770" y="300" width="44" height="24" rx="12" fill="#424242"/>
  <circle cx="802" cy="312" r="9" fill="#FFFFFF"/>
  <text x="58" y="360" fill="#9E9E9E" font-size="10">DANGER ZONE</text>
  <rect x="54" y="374" width="772" height="54" rx="6" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="72" y="396" fill="#212121" font-size="12" font-weight="600">Log out and wipe this device</text>
  <text x="72" y="414" fill="#424242" font-size="10">Deletes your keypair and message history.</text>
  <rect x="710" y="384" width="100" height="34" rx="4" fill="#FFFFFF" stroke="#424242"/>
  <text x="760" y="405" fill="#424242" font-size="11" text-anchor="middle">Log out</text>
  <rect x="940" y="30" width="240" height="460" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="420" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="1060" y="78" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Settings</text>
  <text x="975" y="100" fill="#9E9E9E" font-size="8">ACCOUNT</text>
  <rect x="975" y="108" width="170" height="38" rx="5" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="985" y="122" fill="#212121" font-size="10" font-weight="600">Nickname</text>
  <text x="985" y="138" fill="#424242" font-size="9">sumith</text>
  <rect x="975" y="154" width="170" height="56" rx="5" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="985" y="168" fill="#212121" font-size="10" font-weight="600">Safety number</text>
  <text x="985" y="184" fill="#424242" font-size="7" font-family="monospace">42817 90334 11902 56701</text>
  <text x="985" y="194" fill="#424242" font-size="7" font-family="monospace">88420 17738 44901 23155</text>
  <text x="985" y="204" fill="#424242" font-size="7" font-family="monospace">80604 77112 68493 05889</text>
  <rect x="975" y="236" width="170" height="32" rx="5" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="985" y="256" fill="#212121" font-size="10">Dark mode</text>
  <rect x="1108" y="244" width="30" height="16" rx="8" fill="#424242"/>
  <circle cx="1130" cy="252" r="6" fill="#FFFFFF"/>
  <rect x="975" y="294" width="170" height="52" rx="5" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="985" y="310" fill="#212121" font-size="9" font-weight="600">Log out and wipe</text>
  <rect x="1070" y="300" width="68" height="22" rx="3" fill="#FFFFFF" stroke="#424242"/>
  <text x="1104" y="315" fill="#424242" font-size="9" text-anchor="middle">Log out</text>
  <text x="440" y="510" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 8a — Desktop</text>
  <text x="1060" y="510" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 8b — Android</text>
</svg>
```

Settings is deliberately short: account identity with the user's own safety number visible and copyable, a dark-mode toggle, and a danger-zone card that logs out and wipes local state. The wipe is guarded by a confirmation dialog that explicitly states the consequences.

### Figure 9 — Peer info and fingerprint verification

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 520" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="520" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="460" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Anna · Peer info</text>
  <circle cx="440" cy="116" r="28" fill="#9E9E9E"/>
  <text x="440" y="123" fill="#FFFFFF" font-size="14" text-anchor="middle" font-weight="600">AN</text>
  <text x="440" y="168" fill="#212121" font-size="16" text-anchor="middle" font-weight="600">Anna</text>
  <text x="440" y="186" fill="#424242" font-size="11" text-anchor="middle">first seen 8 April 2026 · active now</text>
  <rect x="140" y="216" width="600" height="164" rx="8" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="158" y="244" fill="#212121" font-size="13" font-weight="600">Anna's safety number</text>
  <text x="158" y="260" fill="#9E9E9E" font-size="10">Confirm these digits match on both sides — in person, on video, or over a trusted call.</text>
  <text x="440" y="296" fill="#212121" font-size="14" text-anchor="middle" font-family="monospace" letter-spacing="1">11902  56701  88420  17738</text>
  <text x="440" y="320" fill="#212121" font-size="14" text-anchor="middle" font-family="monospace" letter-spacing="1">44901  23155  80604  77112</text>
  <text x="440" y="344" fill="#212121" font-size="14" text-anchor="middle" font-family="monospace" letter-spacing="1">68493  05889  42817  90334</text>
  <rect x="240" y="400" width="180" height="38" rx="6" fill="#FFFFFF" stroke="#424242"/>
  <text x="330" y="424" fill="#424242" font-size="12" text-anchor="middle">Not now</text>
  <rect x="460" y="400" width="180" height="38" rx="6" fill="#424242"/>
  <text x="550" y="424" fill="#FFFFFF" font-size="12" text-anchor="middle" font-weight="600">Mark as verified</text>
  <rect x="940" y="30" width="240" height="460" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="420" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="1060" y="78" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Peer info</text>
  <circle cx="1060" cy="118" r="18" fill="#9E9E9E"/>
  <text x="1060" y="124" fill="#FFFFFF" font-size="10" text-anchor="middle" font-weight="600">AN</text>
  <text x="1060" y="156" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Anna</text>
  <rect x="975" y="186" width="170" height="150" rx="5" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="985" y="204" fill="#212121" font-size="9" font-weight="600">Safety number</text>
  <text x="1060" y="244" fill="#212121" font-size="10" text-anchor="middle" font-family="monospace">11902 56701</text>
  <text x="1060" y="260" fill="#212121" font-size="10" text-anchor="middle" font-family="monospace">88420 17738</text>
  <text x="1060" y="276" fill="#212121" font-size="10" text-anchor="middle" font-family="monospace">44901 23155</text>
  <text x="1060" y="292" fill="#212121" font-size="10" text-anchor="middle" font-family="monospace">80604 77112</text>
  <text x="1060" y="308" fill="#212121" font-size="10" text-anchor="middle" font-family="monospace">68493 05889</text>
  <text x="1060" y="324" fill="#212121" font-size="10" text-anchor="middle" font-family="monospace">42817 90334</text>
  <rect x="975" y="360" width="80" height="32" rx="5" fill="#FFFFFF" stroke="#424242"/>
  <text x="1015" y="380" fill="#424242" font-size="10" text-anchor="middle">Not now</text>
  <rect x="1065" y="360" width="80" height="32" rx="5" fill="#424242"/>
  <text x="1105" y="380" fill="#FFFFFF" font-size="10" text-anchor="middle">Verify</text>
  <text x="440" y="510" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 9a — Desktop</text>
  <text x="1060" y="510" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 9b — Android</text>
</svg>
```

The peer verification screen is the counterpart to the user's own fingerprint screen (Figure 3) and is the primary mechanism for countering the trust-on-first-use weakness: a user who cares about impersonation resistance compares these sixty digits out of band with their peer. Marking a peer as verified updates the local contact record and is reflected back in the contacts list as the verified badge.

### Figure 10 — Error states

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 620" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="620" fill="#FFFFFF"/>
  <rect x="30" y="30" width="820" height="180" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Anna</text>
  <rect x="30" y="58" width="820" height="58" fill="#424242"/>
  <text x="50" y="82" fill="#FFFFFF" font-size="13" font-weight="600">⚠ Anna's security key has changed</text>
  <text x="50" y="102" fill="#FFFFFF" font-size="11">This may mean Anna reinstalled QtChat — or someone is impersonating her. Verify the new fingerprint before continuing.</text>
  <rect x="700" y="72" width="130" height="28" rx="4" fill="#FFFFFF"/>
  <text x="765" y="91" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Review fingerprint</text>
  <text x="440" y="200" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 10a — Key-mismatch banner</text>
  <rect x="30" y="230" width="820" height="160" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="230" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="248" fill="#424242" font-size="12" text-anchor="middle">QtChat — Anna</text>
  <rect x="500" y="282" width="300" height="44" rx="10" fill="#424242" stroke="#212121" stroke-width="2"/>
  <text x="512" y="302" fill="#FFFFFF" font-size="11">This message could not be sent</text>
  <text x="512" y="316" fill="#FFFFFF" font-size="11">right now.</text>
  <text x="792" y="340" fill="#9E9E9E" font-size="9" text-anchor="end">⚠ failed · tap to retry</text>
  <rect x="736" y="298" width="60" height="22" rx="3" fill="#FFFFFF"/>
  <text x="766" y="313" fill="#212121" font-size="10" text-anchor="middle">Retry</text>
  <text x="440" y="380" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 10b — Failed-message retry</text>
  <rect x="30" y="410" width="820" height="180" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="410" width="820" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="440" y="428" fill="#424242" font-size="12" text-anchor="middle">QtChat — Anna</text>
  <rect x="54" y="456" width="772" height="40" rx="6" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="70" y="481" fill="#424242" font-size="11">📎</text>
  <text x="96" y="481" fill="#9E9E9E" font-size="11">Message Anna…</text>
  <rect x="750" y="462" width="70" height="28" rx="4" fill="#9E9E9E"/>
  <text x="785" y="481" fill="#FFFFFF" font-size="11" text-anchor="middle">12 s</text>
  <text x="54" y="524" fill="#424242" font-size="11">Server rate limit reached. Sending re-enabled in 12 s.</text>
  <rect x="54" y="540" width="772" height="4" rx="2" fill="#E0E0E0"/>
  <rect x="54" y="540" width="280" height="4" rx="2" fill="#424242"/>
  <text x="440" y="580" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 10c — Rate-limit cooldown</text>
</svg>
```

Three distinct failure modes share this figure because they deserve to be designed as a coherent family. A key-mismatch is the most serious and earns a full-width banner that blocks further messaging until acknowledged. A failed individual message is rendered as a bubble with a heavier border and a clear retry affordance. A rate-limit cooldown is the mildest and is expressed as a disabled send button showing a countdown plus a quiet progress bar.

### Figure 11 — Empty states

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 460" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="460" fill="#FFFFFF"/>
  <rect x="30" y="30" width="400" height="400" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="30" y="30" width="400" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="230" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Contacts</text>
  <circle cx="230" cy="180" r="44" fill="none" stroke="#9E9E9E" stroke-width="2" stroke-dasharray="4 3"/>
  <text x="230" y="186" fill="#9E9E9E" font-size="26" text-anchor="middle">💬</text>
  <text x="230" y="260" fill="#212121" font-size="14" text-anchor="middle" font-weight="600">No conversations yet</text>
  <text x="230" y="282" fill="#424242" font-size="11" text-anchor="middle">Share your nickname with someone to start.</text>
  <rect x="130" y="328" width="200" height="36" rx="6" fill="#FFFFFF" stroke="#424242"/>
  <text x="230" y="351" fill="#424242" font-size="12" text-anchor="middle">Show my nickname</text>
  <rect x="450" y="30" width="400" height="400" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="450" y="30" width="400" height="28" rx="8" fill="#E0E0E0" stroke="#9E9E9E"/>
  <text x="650" y="48" fill="#424242" font-size="12" text-anchor="middle">QtChat — Anna</text>
  <rect x="450" y="58" width="400" height="38" fill="#FFFFFF" stroke="#E0E0E0"/>
  <circle cx="478" cy="78" r="12" fill="#9E9E9E"/>
  <text x="478" y="82" fill="#FFFFFF" font-size="9" text-anchor="middle" font-weight="600">AN</text>
  <text x="496" y="76" fill="#212121" font-size="11" font-weight="600">Anna</text>
  <text x="496" y="90" fill="#9E9E9E" font-size="9">active now</text>
  <text x="650" y="200" fill="#9E9E9E" font-size="26" text-anchor="middle">✉</text>
  <text x="650" y="240" fill="#212121" font-size="13" text-anchor="middle" font-weight="600">No messages yet</text>
  <text x="650" y="260" fill="#424242" font-size="10" text-anchor="middle">This is the beginning of your</text>
  <text x="650" y="274" fill="#424242" font-size="10" text-anchor="middle">encrypted conversation with Anna.</text>
  <rect x="470" y="388" width="360" height="32" rx="5" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="482" y="409" fill="#9E9E9E" font-size="11">Say hello…</text>
  <rect x="940" y="30" width="240" height="400" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="960" y="50" width="200" height="360" rx="6" fill="#FFFFFF" stroke="#E0E0E0"/>
  <text x="1060" y="74" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Contacts</text>
  <circle cx="1060" cy="170" r="30" fill="none" stroke="#9E9E9E" stroke-width="2" stroke-dasharray="3 2"/>
  <text x="1060" y="176" fill="#9E9E9E" font-size="18" text-anchor="middle">💬</text>
  <text x="1060" y="226" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">No conversations yet</text>
  <text x="230" y="450" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 11a — Empty contacts</text>
  <text x="650" y="450" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 11b — Empty conversation</text>
  <text x="1060" y="450" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 11c — Android empty</text>
</svg>
```

Empty states are often skipped in student projects and are one of the cheapest ways to signal product-grade polish. Both the empty contacts list and the empty conversation carry a single-sentence explanation plus an action.

### Figure 12 — Android notification permission prompt

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 600 520" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="600" height="520" fill="#FFFFFF"/>
  <rect x="180" y="30" width="240" height="460" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="200" y="50" width="200" height="420" rx="6" fill="#424242" opacity="0.7"/>
  <rect x="220" y="200" width="160" height="200" rx="10" fill="#FFFFFF"/>
  <text x="300" y="230" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Allow QtChat to send</text>
  <text x="300" y="244" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">you notifications?</text>
  <text x="300" y="274" fill="#424242" font-size="9" text-anchor="middle">So you can see new messages</text>
  <text x="300" y="286" fill="#424242" font-size="9" text-anchor="middle">when QtChat is in the background.</text>
  <rect x="238" y="310" width="124" height="24" rx="12" fill="#FFFFFF" stroke="#424242"/>
  <text x="300" y="326" fill="#424242" font-size="10" text-anchor="middle">Allow</text>
  <rect x="238" y="344" width="124" height="24" rx="12" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="300" y="360" fill="#9E9E9E" font-size="10" text-anchor="middle">Don't allow</text>
  <text x="300" y="510" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 12 — POST_NOTIFICATIONS prompt</text>
</svg>
```

On Android 13 (API 33) and newer, applications must request the POST_NOTIFICATIONS permission at runtime before posting any notification. The application triggers this prompt exactly once, immediately after the first successful login, so that the user associates the permission with the moment they are most likely to understand why it is being requested.

### Figure 13 — Android lock-screen notification

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 600 520" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="600" height="520" fill="#FFFFFF"/>
  <rect x="180" y="30" width="240" height="460" rx="32" fill="#F5F5F5" stroke="#9E9E9E"/>
  <rect x="200" y="50" width="200" height="420" rx="6" fill="#212121"/>
  <text x="300" y="108" fill="#FFFFFF" font-size="28" text-anchor="middle" font-weight="300">12:43</text>
  <text x="300" y="128" fill="#9E9E9E" font-size="11" text-anchor="middle">Monday, 4 May</text>
  <rect x="218" y="160" width="164" height="80" rx="10" fill="#FFFFFF"/>
  <circle cx="238" cy="180" r="10" fill="#424242"/>
  <text x="238" y="184" fill="#FFFFFF" font-size="9" text-anchor="middle" font-weight="600">Q</text>
  <text x="256" y="178" fill="#424242" font-size="8">QtChat · now</text>
  <text x="218" y="204" fill="#212121" font-size="11" font-weight="600">Anna</text>
  <text x="218" y="220" fill="#424242" font-size="10">New message</text>
  <rect x="218" y="254" width="164" height="56" rx="10" fill="#FFFFFF"/>
  <circle cx="238" cy="274" r="10" fill="#424242"/>
  <text x="238" y="278" fill="#FFFFFF" font-size="9" text-anchor="middle" font-weight="600">Q</text>
  <text x="256" y="272" fill="#424242" font-size="8">QtChat · 2 min ago</text>
  <text x="218" y="296" fill="#212121" font-size="10" font-weight="600">Jussi · 1 new message</text>
  <text x="300" y="510" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 13 — Lock-screen notification</text>
</svg>
```

Lock-screen notifications deliberately do not include message previews. The rationale is that messages are end-to-end encrypted and a preview on the lock screen would leak plaintext past the security boundary the rest of the application maintains.

### Figure 14 — Windows system tray and toast

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 900 340" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="900" height="340" fill="#FFFFFF"/>
  <rect x="30" y="30" width="840" height="240" rx="8" fill="#F5F5F5" stroke="#9E9E9E"/>
  <text x="450" y="52" fill="#424242" font-size="12" text-anchor="middle">Windows desktop</text>
  <rect x="30" y="240" width="840" height="30" fill="#212121"/>
  <rect x="30" y="240" width="40" height="30" fill="#424242"/>
  <text x="50" y="260" fill="#FFFFFF" font-size="13" text-anchor="middle">⊞</text>
  <text x="100" y="260" fill="#FFFFFF" font-size="11">Search</text>
  <rect x="700" y="242" width="170" height="26" fill="#212121"/>
  <circle cx="722" cy="255" r="8" fill="#9E9E9E"/>
  <text x="722" y="259" fill="#FFFFFF" font-size="9" text-anchor="middle" font-weight="600">Q</text>
  <text x="820" y="255" fill="#FFFFFF" font-size="10">20.45</text>
  <text x="820" y="266" fill="#FFFFFF" font-size="9">05/05/26</text>
  <rect x="600" y="100" width="260" height="110" rx="8" fill="#FFFFFF" stroke="#9E9E9E"/>
  <circle cx="622" cy="124" r="10" fill="#424242"/>
  <text x="622" y="128" fill="#FFFFFF" font-size="9" text-anchor="middle" font-weight="600">Q</text>
  <text x="640" y="122" fill="#424242" font-size="9">QtChat</text>
  <text x="612" y="150" fill="#212121" font-size="12" font-weight="600">Anna sent you a message</text>
  <text x="612" y="170" fill="#424242" font-size="10">Click to open the conversation.</text>
  <rect x="612" y="182" width="80" height="22" rx="3" fill="#FFFFFF" stroke="#424242"/>
  <text x="652" y="196" fill="#424242" font-size="10" text-anchor="middle">Open</text>
  <text x="450" y="316" fill="#9E9E9E" font-size="11" text-anchor="middle">Figure 14 — Windows tray and toast</text>
</svg>
```

On Windows the application lives as a tray icon when the main window is closed rather than exiting the process. Incoming messages produce a toast notification with the sender's display name and no preview (matching the Android rationale).

### Figure 15 — Navigation flow diagram

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 520" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="520" fill="#FFFFFF"/>
  <text x="700" y="38" fill="#212121" font-size="14" text-anchor="middle" font-weight="600">Screen navigation flow</text>
  <rect x="60" y="70" width="150" height="54" rx="8" fill="#F5F5F5" stroke="#424242"/>
  <text x="135" y="94" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Splash</text>
  <text x="135" y="110" fill="#9E9E9E" font-size="10" text-anchor="middle">Fig 1</text>
  <path d="M210 97 L270 97" stroke="#424242" marker-end="url(#arr)"/>
  <rect x="270" y="70" width="150" height="54" rx="8" fill="#F5F5F5" stroke="#424242"/>
  <text x="345" y="94" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Login</text>
  <text x="345" y="110" fill="#9E9E9E" font-size="10" text-anchor="middle">Fig 2 · first run only ↓</text>
  <path d="M345 124 L345 160" stroke="#424242" stroke-dasharray="4 3" marker-end="url(#arr)"/>
  <rect x="270" y="160" width="150" height="54" rx="8" fill="#F5F5F5" stroke="#424242" stroke-dasharray="4 3"/>
  <text x="345" y="184" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">TOFU fingerprint</text>
  <text x="345" y="200" fill="#9E9E9E" font-size="10" text-anchor="middle">Fig 3 · first run</text>
  <path d="M420 187 L520 187 L520 110 L540 110" stroke="#424242" marker-end="url(#arr)"/>
  <path d="M420 97 L540 97" stroke="#424242" marker-end="url(#arr)"/>
  <rect x="540" y="70" width="150" height="80" rx="8" fill="#E0E0E0" stroke="#424242"/>
  <text x="615" y="94" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Main (Contacts)</text>
  <text x="615" y="110" fill="#9E9E9E" font-size="10" text-anchor="middle">Fig 4, Fig 5</text>
  <path d="M690 110 L780 110" stroke="#424242" marker-end="url(#arr)"/>
  <rect x="780" y="80" width="150" height="60" rx="8" fill="#E0E0E0" stroke="#424242"/>
  <text x="855" y="104" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Conversation</text>
  <text x="855" y="120" fill="#9E9E9E" font-size="10" text-anchor="middle">Fig 6 · Fig 7</text>
  <path d="M855 140 L855 190" stroke="#424242" marker-end="url(#arr)"/>
  <rect x="780" y="190" width="150" height="60" rx="8" fill="#F5F5F5" stroke="#424242"/>
  <text x="855" y="214" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Peer verify</text>
  <text x="855" y="230" fill="#9E9E9E" font-size="10" text-anchor="middle">Fig 9</text>
  <path d="M690 130 L690 260 L780 260" stroke="#424242" marker-end="url(#arr)"/>
  <rect x="780" y="270" width="150" height="60" rx="8" fill="#F5F5F5" stroke="#424242"/>
  <text x="855" y="294" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">Settings</text>
  <text x="855" y="310" fill="#9E9E9E" font-size="10" text-anchor="middle">Fig 8</text>
  <rect x="970" y="70" width="250" height="60" rx="8" fill="#F5F5F5" stroke="#9E9E9E" stroke-dasharray="4 3"/>
  <text x="1095" y="94" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Reconnecting banner</text>
  <text x="1095" y="110" fill="#9E9E9E" font-size="9" text-anchor="middle">overlays conversation</text>
  <rect x="970" y="140" width="250" height="60" rx="8" fill="#F5F5F5" stroke="#9E9E9E" stroke-dasharray="4 3"/>
  <text x="1095" y="164" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Key-mismatch warning</text>
  <text x="1095" y="180" fill="#9E9E9E" font-size="9" text-anchor="middle">blocks conversation</text>
  <rect x="970" y="210" width="250" height="60" rx="8" fill="#F5F5F5" stroke="#9E9E9E" stroke-dasharray="4 3"/>
  <text x="1095" y="234" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Failed-message retry</text>
  <rect x="970" y="280" width="250" height="60" rx="8" fill="#F5F5F5" stroke="#9E9E9E" stroke-dasharray="4 3"/>
  <text x="1095" y="304" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Rate-limit cooldown</text>
  <rect x="270" y="260" width="150" height="60" rx="8" fill="#F5F5F5" stroke="#424242"/>
  <text x="345" y="284" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Permission prompt</text>
  <text x="345" y="298" fill="#9E9E9E" font-size="9" text-anchor="middle">Android · Fig 12</text>
  <path d="M345 215 L345 260" stroke="#424242" stroke-dasharray="4 3" marker-end="url(#arr)"/>
  <rect x="60" y="260" width="150" height="60" rx="8" fill="#F5F5F5" stroke="#424242"/>
  <text x="135" y="284" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Notification</text>
  <text x="135" y="298" fill="#9E9E9E" font-size="9" text-anchor="middle">Fig 13 / Fig 14</text>
  <defs>
    <marker id="arr" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="8" markerHeight="8" orient="auto">
      <path d="M0 0 L10 5 L0 10 Z" fill="#424242"/>
    </marker>
  </defs>
</svg>
```

The navigation flow covers the full product on a single page. Rectangular nodes with solid borders are persistent screens, the dark-filled node is the main screen, and dashed-border nodes are transient overlays or conditional first-run screens.

### Figure 16 — System architecture

```svg
<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 1400 720" width="100%" font-family="Inter, Roboto, sans-serif">
  <rect width="1400" height="720" fill="#FFFFFF"/>
  <text x="700" y="38" fill="#212121" font-size="16" text-anchor="middle" font-weight="600">System architecture</text>
  <rect x="40" y="70" width="640" height="440" rx="10" fill="#F5F5F5" stroke="#424242"/>
  <text x="60" y="96" fill="#212121" font-size="14" font-weight="600">Client (Windows 11 · Android)</text>
  <rect x="70" y="116" width="580" height="100" rx="8" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="90" y="140" fill="#212121" font-size="12" font-weight="600">QML presentation layer</text>
  <text x="90" y="156" fill="#424242" font-size="11">Qt Quick Controls + Material / Fusion</text>
  <text x="90" y="190" fill="#9E9E9E" font-size="10">Responsibility: rendering, navigation, trivial local state</text>
  <rect x="70" y="230" width="580" height="260" rx="8" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="90" y="254" fill="#212121" font-size="12" font-weight="600">C++ controllers &amp; services</text>
  <rect x="88" y="270" width="175" height="66" rx="6" fill="#F5F5F5" stroke="#424242"/>
  <text x="176" y="290" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">WsClient</text>
  <text x="176" y="306" fill="#424242" font-size="9" text-anchor="middle">QWebSocket + TLS</text>
  <text x="176" y="318" fill="#424242" font-size="9" text-anchor="middle">reconnect · heartbeat</text>
  <text x="176" y="330" fill="#424242" font-size="9" text-anchor="middle">offline queue</text>
  <rect x="278" y="270" width="175" height="66" rx="6" fill="#F5F5F5" stroke="#424242"/>
  <text x="366" y="290" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">CryptoService</text>
  <text x="366" y="306" fill="#424242" font-size="9" text-anchor="middle">libsodium</text>
  <text x="366" y="318" fill="#424242" font-size="9" text-anchor="middle">X25519 + XChaCha20</text>
  <text x="366" y="330" fill="#424242" font-size="9" text-anchor="middle">QtKeychain storage</text>
  <rect x="468" y="270" width="175" height="66" rx="6" fill="#F5F5F5" stroke="#424242"/>
  <text x="556" y="290" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">MessageStore</text>
  <text x="556" y="306" fill="#424242" font-size="9" text-anchor="middle">SQLite WAL</text>
  <text x="556" y="318" fill="#424242" font-size="9" text-anchor="middle">worker thread</text>
  <rect x="88" y="350" width="260" height="62" rx="6" fill="#F5F5F5" stroke="#424242"/>
  <text x="218" y="370" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">ChatController</text>
  <text x="218" y="386" fill="#424242" font-size="9" text-anchor="middle">facade · owns Ws/Crypto/Store</text>
  <rect x="360" y="350" width="283" height="62" rx="6" fill="#F5F5F5" stroke="#424242"/>
  <text x="501" y="370" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">MessageModel</text>
  <text x="501" y="386" fill="#424242" font-size="9" text-anchor="middle">QAbstractListModel</text>
  <rect x="88" y="420" width="555" height="54" rx="6" fill="#F5F5F5" stroke="#424242"/>
  <text x="365" y="440" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Platform glue</text>
  <text x="365" y="456" fill="#424242" font-size="9" text-anchor="middle">Android JNI · Windows QSystemTrayIcon</text>
  <rect x="720" y="70" width="200" height="440" rx="10" fill="#E0E0E0" stroke="#424242"/>
  <text x="820" y="96" fill="#212121" font-size="14" text-anchor="middle" font-weight="600">Common</text>
  <text x="820" y="112" fill="#9E9E9E" font-size="10" text-anchor="middle">shared static library</text>
  <rect x="740" y="130" width="160" height="80" rx="6" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="820" y="152" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">Protocol</text>
  <text x="820" y="168" fill="#424242" font-size="9" text-anchor="middle">envelope types</text>
  <text x="820" y="180" fill="#424242" font-size="9" text-anchor="middle">serialisation</text>
  <rect x="740" y="224" width="160" height="80" rx="6" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="820" y="246" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">CryptoService</text>
  <text x="820" y="262" fill="#424242" font-size="9" text-anchor="middle">encrypt · decrypt</text>
  <text x="820" y="274" fill="#424242" font-size="9" text-anchor="middle">fingerprint60</text>
  <rect x="740" y="318" width="160" height="80" rx="6" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="820" y="340" fill="#212121" font-size="11" text-anchor="middle" font-weight="600">MessageStore</text>
  <text x="820" y="356" fill="#424242" font-size="9" text-anchor="middle">schema + queries</text>
  <rect x="960" y="70" width="400" height="440" rx="10" fill="#F5F5F5" stroke="#424242"/>
  <text x="980" y="96" fill="#212121" font-size="14" font-weight="600">Server (Ubuntu 24.04, cPouta)</text>
  <rect x="990" y="116" width="360" height="80" rx="6" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="1010" y="138" fill="#212121" font-size="12" font-weight="600">ChatServer</text>
  <text x="1010" y="156" fill="#424242" font-size="10">QWebSocketServer (SecureMode)</text>
  <text x="1010" y="170" fill="#424242" font-size="10">accepts connections · owns Sessions</text>
  <rect x="990" y="210" width="360" height="80" rx="6" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="1010" y="232" fill="#212121" font-size="12" font-weight="600">Session (per client)</text>
  <text x="1010" y="250" fill="#424242" font-size="10">parses envelope · enforces rate limit</text>
  <text x="1010" y="264" fill="#424242" font-size="10">routes MESSAGE / FILE_CHUNK by 'to'</text>
  <text x="1010" y="278" fill="#424242" font-size="10">never decrypts · never stores content</text>
  <rect x="990" y="304" width="360" height="60" rx="6" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="1010" y="326" fill="#212121" font-size="12" font-weight="600">RateLimiter</text>
  <text x="1010" y="344" fill="#424242" font-size="10">token bucket per socket</text>
  <text x="1010" y="358" fill="#424242" font-size="10">close with CloseCodePolicyViolated</text>
  <rect x="990" y="378" width="360" height="116" rx="6" fill="#FFFFFF" stroke="#9E9E9E"/>
  <text x="1010" y="400" fill="#212121" font-size="12" font-weight="600">Deployment</text>
  <text x="1010" y="418" fill="#424242" font-size="10">systemd unit · Let's Encrypt certbot</text>
  <text x="1010" y="432" fill="#424242" font-size="10">chat.sumith.dev:8443 · journald logs</text>
  <text x="1010" y="466" fill="#9E9E9E" font-size="10">no persistent state · restart-tolerant</text>
  <path d="M680 290 L960 290" stroke="#424242" stroke-width="2" marker-end="url(#a2)"/>
  <text x="820" y="280" fill="#212121" font-size="12" text-anchor="middle" font-weight="600">WSS / TLS 1.2+</text>
  <text x="820" y="306" fill="#9E9E9E" font-size="10" text-anchor="middle">server sees ciphertext only</text>
  <defs>
    <marker id="a2" viewBox="0 0 10 10" refX="9" refY="5" markerWidth="8" markerHeight="8" orient="auto">
      <path d="M0 0 L10 5 L0 10 Z" fill="#424242"/>
    </marker>
  </defs>
</svg>
```

The system architecture is described in detail in section 4.

---

## 4. Architecture overview

### 4.1 System overview

The system is deliberately composed of three parts, not two. The obvious two — client application and server application — are joined by a third, a shared common library that holds protocol definitions, cryptographic operations, and persistence code that both sides benefit from sharing. This three-way split is the single most important architectural decision in the project, and it is the one that most directly contributes to the grade-5 descriptor "well-designed, reusable components".

### 4.2 Client architecture

The client is a single Qt Quick application built from one CMake target, producing a Windows executable and an Android APK from the same sources. The presentation layer is written entirely in QML, using Qt Quick Controls with a platform-appropriate style (Material on Android, FluentWinUI3 on Windows with a Fusion fallback). The QML layer is deliberately thin: it handles rendering, navigation, and trivial local state such as the current text in a composer field, but every non-trivial operation is delegated to C++ controllers exposed as QML singletons or as list models. Three service classes carry most of the logic — WsClient wraps QWebSocket and adds reconnection, heartbeat, and offline queueing; CryptoService wraps libsodium and QtKeychain; MessageStore owns a QSqlDatabase on a dedicated worker thread and exposes pagination to QML via a QAbstractListModel subclass. The facade between QML and these services is ChatController, a QML singleton that owns the three services, orchestrates the encrypt-send-persist pipeline on outgoing messages, and the decrypt-persist-model-update pipeline on incoming messages.

### 4.3 Server architecture

The server is a headless Qt Console application with no Qt GUI dependency. Its top-level object is ChatServer, which owns a QWebSocketServer in SecureMode, a QHash of currently-connected sessions keyed by nickname, an in-memory registry of published public keys, and a RateLimiter. Each accepted QWebSocket becomes a Session child object that parses JSON envelopes, enforces the per-socket rate limit, and routes ciphertext frames to their recipient without decryption. The server is structurally incapable of reading message content because the cryptographic operations that would make it possible are simply not linked into its binary: the server links only libsodium's public-key-registry and fingerprint functions for debugging, not any decryption code. Presence and public-key state are kept only in memory, which means the server is trivially restart-tolerant.

### 4.4 Common shared library

The common library is a pure C++ static library with no Qt Quick or Qt GUI dependencies, holding three small modules. Protocol defines the JSON envelope shape, the message-type enumeration, and a version constant; both client and server include this header and thus cannot accidentally diverge on the wire format. CryptoService wraps libsodium: keypair generation, encryptForPeer, decryptFromPeer, fingerprint60, and QtKeychain-backed secret storage. MessageStore wraps QSqlDatabase with migrations, schema, and query helpers.

### 4.5 QML versus C++ split

The rule-of-thumb applied throughout the project is that QML handles presentation and trivial local state, and C++ handles networking, cryptography, persistence, and business logic. A QML file may bind to a C++ controller property and invoke Q_INVOKABLE methods; it must not make network requests, perform encryption, or touch the database. A C++ service may emit signals that QML binds to; it must not reach into QML types or assume a specific QML layout.

### 4.6 Network protocol

All client-server traffic flows over a single WSS connection on TCP port 8443. TLS 1.2 or newer is mandatory. Control messages are JSON objects sent via sendTextMessage. File transfers use a mixed pattern: a JSON FILE_OFFER negotiates the transfer, and the actual payload chunks are sent via sendBinaryMessage with a compact 21-byte binary header carrying a magic byte, a 16-byte file ID, and a 4-byte little-endian sequence number, followed by up to 64 KiB of encrypted payload.

### 4.7 Message envelope format

Every JSON envelope carries a version field `v`, a type field `t`, a UUID `id` for ACK correlation, and type-specific fields. The complete set of envelope types is HELLO, HELLO_OK, ERROR, PRESENCE, KEY_REQ, KEY_RESP, MESSAGE, ACK, FILE_OFFER, FILE_OFFER_ACK, FILE_CHUNK, FILE_COMPLETE, and LOGOUT.

### 4.8 End-to-end encryption design

Message confidentiality and integrity are provided by libsodium's `crypto_box_easy` primitive, which combines X25519 key exchange with XChaCha20-Poly1305 authenticated encryption. Each message uses a freshly random 24-byte nonce, prepended to the ciphertext; the 16-byte Poly1305 authentication tag is included in the ciphertext and is verified automatically on the receiving side, so that any tampering produces a clean decryption failure. Long-term identity keys are static X25519 keypairs generated on first launch; the secret half lives in the operating system's secure credential store via QtKeychain. Identity verification between peers uses Signal-style safety numbers: a BLAKE2b-256 hash of the public key is rendered as 60 decimal digits grouped into twelve blocks of five.

### 4.9 Deployment

The Windows client is built in release mode, bundled with `windeployqt`, and ships OpenSSL's DLLs next to the executable. The Android client is built via Qt Creator's Android kit using Gradle 9.3.1, AGP 9.0.0, NDK 27.2.12479018, and JDK 21, with KDAB's `android_openssl` project pulled via CMake FetchContent. The release APK is signed with a keystore held outside the repository. The server is deployed to a CSC cPouta Ubuntu 24.04 VM as a systemd unit running under an unprivileged user account; TLS certificates are obtained from Let's Encrypt via certbot.

### 4.10 Data persistence

The client persists conversation history in a local SQLite database opened in WAL journal mode with STRICT tables. Ciphertext is stored verbatim in the `messages.ciphertext` column; the decrypted plaintext never touches disk. The server has no persistent state.

### 4.11 Threat model summary

The system is designed to resist passive server eavesdropping, full compromise of the server (the attacker gets only the ciphertexts and public keys), passive and active network interception, and tampering of message bodies in flight. It does not resist a compromised endpoint, a malicious substitution of a peer's public key on first contact (mitigated by fingerprint verification), metadata collection, or the lack of forward secrecy inherent to static long-term keys.

### 4.12 Technology stack

| Layer | Technology | Version |
|---|---|---|
| UI | QML with Qt Quick Controls | Qt 6.11.0 |
| Language | C++ | C++20 |
| IDE | Qt Creator | 19.0.0 Enterprise |
| Build system | CMake | ≥ 3.21 |
| Networking | Qt WebSockets | 6.11 |
| TLS | OpenSSL via Qt | 3.x |
| Cryptography | libsodium | 1.0.19+ |
| Secret storage | QtKeychain | frankosterfeld |
| Client database | SQLite via QSqlDatabase | bundled |
| Server OS | Ubuntu LTS | 24.04 |
| Android toolchain | NDK | 27.2.12479018 |
| Java | OpenJDK | 21 |
| Android build | Gradle / AGP | 9.3.1 / 9.0.0 |
| Android OpenSSL | KDAB android_openssl | master |
| Certificates | Let's Encrypt via certbot | latest |

---

## 5. Fourteen-day schedule

> **Two-hour blocker rule.** Any single task that cannot progress within two hours of focused effort must be parked and a different workstream resumed. In a fourteen-day plan with no slack, continuing to grind on a blocked task past two hours is the single fastest way to miss the demonstration.

> **Day-eleven-ready rule.** The project must be in a demonstrable end-to-end state by the end of day eleven, not the end of day fourteen. The final three days are polish, testing, and rehearsal on top of an already-deliverable project. If the demonstration is rescheduled earlier on short notice, day eleven is the fallback point.

### 5.1 Day-by-day breakdown

**Day 1 — Tuesday, 21 April 2026 — Skeleton and scaffolding.** Create the Git repository and push to GitHub. Build the three-subdirectory CMake tree (client, server, common). Verify `qt_add_qml_module` configuration with a single QML file. Implement a minimal QWebSocketServer on localhost port 8080 (non-TLS initially) and a QWebSocket client that connects and echoes a string. **Checkpoint:** Client connects to local server, exchanges a plaintext hello. **Hours:** ~10.

**Day 2 — Wednesday, 22 April 2026 — WsClient wrapper and QML shell.** Wrap the QWebSocket in a WsClient class exposed to QML as a singleton. Build a minimal QML login screen with nickname input and a minimal empty ChatView. Implement the HELLO and HELLO_OK envelopes in the shared protocol header. Server rejects duplicate nicknames. **Checkpoint:** Two clients can connect with different nicknames on localhost and the server rejects a duplicate. **Hours:** ~11.

**Day 3 — Thursday, 23 April 2026 — Presence and user list.** Add the PRESENCE envelope and server-side broadcast on connect/disconnect. Build a UserListPane in QML showing online users with a presence dot. Implement basic reconnection skeleton (real backoff comes on day 5). **Checkpoint:** Connecting a second client causes the first client's list to update within one second. **Hours:** ~10.

**Day 4 — Friday, 24 April 2026 — Cryptography and first end-to-end.** Add libsodium via vcpkg on Windows. Implement CryptoService in the common library (keypair generation, encryptForPeer, decryptFromPeer, fingerprint60). Write Qt Test unit tests for round-trip, tamper detection, wrong-key rejection, and fingerprint stability. Add KEY_REQ and KEY_RESP envelopes. Wire CryptoService into MESSAGE send/receive. **Checkpoint:** Two clients exchange an encrypted MESSAGE envelope and the server log shows only base64 ciphertext for the body. **Hours:** ~12.

**Day 5 — Saturday, 25 April 2026 — Networking robustness and UI polish.** Implement exponential-backoff reconnection with random jitter, capped at thirty seconds. Add heartbeat ping every twenty seconds with three-missed-pong teardown. Build the reconnecting banner in ChatView. Refactor MessageModel as a proper QAbstractListModel with roles for text, senderId, timestamp, sentByMe, and status. Polish ChatBubble for both sent and received styles. **Checkpoint:** Kill the server process for twenty seconds; the client shows the reconnecting banner and reconnects cleanly without any manual action. **Hours:** ~11.

**Day 6 — Sunday, 26 April 2026 — Persistence and trust-on-first-use.** Move QSqlDatabase operations to a dedicated worker thread. Enable SQLite WAL mode with STRICT tables. Implement MessageStore save, load, and keyset pagination. Build the TOFU screen for the user's own fingerprint display, and the peer verification screen. Test QtKeychain secret storage on Windows. **Checkpoint:** Restart the client; conversation history loads within a second, and the keypair persists across the restart. **Hours:** ~11.

**Day 7 — Monday, 27 April 2026 — cPouta deployment (highest-risk day).** Provision the Ubuntu 24.04 VM on cPouta. Point the `chat.sumith.dev` A record at the VM's public IP. Install Qt runtime and build tooling on the VM. Build the server for Linux and transfer the binary via scp. Run `certbot certonly --standalone` for `chat.sumith.dev` on port 80. Create the systemd unit running under an unprivileged `qtchat` user with a tight read-only ACL on the Let's Encrypt private key. Switch the desktop client to `wss://chat.sumith.dev:8443`. **Checkpoint:** The desktop client on the development machine connects to the cPouta server over real TLS and an end-to-end encrypted message goes through. **Hours:** ~12. Start early because this day has the highest variance in completion time.

**Day 8 — Tuesday, 28 April 2026 — Android build configuration (second-highest-risk day).** Select the Android kit in Qt Creator. Configure Gradle 9.3.1, AGP 9.0.0, NDK 27.2.12479018, and JDK 21 in the project's `android/gradle.properties`. Add KDAB `android_openssl` via CMake FetchContent. Check prebuilt libsodium static libraries for all four ABIs into `/third_party`. Author the AndroidManifest template with INTERNET and POST_NOTIFICATIONS permissions. Build the first APK. Install via `adb install` on the physical phone. **Checkpoint:** The Android application starts on the phone and connects to the cPouta server over real TLS. **Hours:** ~12.

**Day 9 — Wednesday, 29 April 2026 — First Android-to-desktop end-to-end and adaptive layout.** Send the first end-to-end encrypted message from the Android phone to the Windows desktop and verify decryption and display. Send in the reverse direction. Fix adaptive layout for phone screens: convert the SplitView to a StackView where width is below six hundred pixels. Test the six-hundred-pixel breakpoint by resizing the desktop window from 1200 to 400 pixels and back. Implement POST_NOTIFICATIONS runtime permission via `QtAndroidPrivate::requestPermission` or `QPermission`. **Checkpoint:** A message composed on the Android phone reaches the Windows desktop within two seconds and displays correctly, and the Android app prompts for the notification permission exactly once after login. **Hours:** ~11.

**Day 10 — Thursday, 30 April 2026 — Notifications (both platforms).** Write the JNI NotificationHelper.java for Android local notifications using NotificationCompat. Trigger a notification on incoming message when the application is backgrounded. Verify lock-screen appearance shows only sender name (no preview). Add QSystemTrayIcon on Windows with a toast on incoming messages when the main window is not focused. **Checkpoint:** Background the Android app, send a message from the desktop, and the notification appears on the lock screen without message preview. **Hours:** ~10.

**Day 11 — Friday, 1 May 2026 (Vappu, Finnish May Day) — Files and images (split with public holiday).** This is a Finnish public holiday, so plan reduced hours. Work early morning on FILE_OFFER, FILE_OFFER_ACK, FILE_CHUNK, and FILE_COMPLETE envelopes, sixty-four-kilobyte chunk size, and `crypto_secretstream_xchacha20poly1305` per file. Enjoy Vappu in the afternoon. Return in the evening for image preview rendering in ChatBubble. Any overflow moves to the morning of day 12. **Checkpoint:** One image sent from Android to Windows successfully, renders inline in the recipient's chat. **Hours:** ~6–8.

**Day 12 — Saturday, 2 May 2026 — Error states and polish.** Finish any overflow from day 11 in the morning. Implement the key-mismatch blocking banner (Figure 10a). Add the failed-message retry bubble with heavier border (Figure 10b). Implement the rate-limit cooldown on the composer (Figure 10c). Add the empty states for contacts and conversations (Figure 11). Wire the dark-mode toggle to a `Theme` singleton. Accessibility quick pass: focus indicators, contrast check in both themes, keyboard navigation on desktop. **Checkpoint:** All ten functionality edge cases from section 7 pass manual testing on both platforms. **Hours:** ~11.

**Day 13 — Sunday, 3 May 2026 — Integration tests, online-demo setup, and backup demonstration.** Write Qt Test integration tests that run a QWebSocketServer in-process on an ephemeral port: connect-reconnect test, queue-flush test, thousand-message stress test. Run the full manual test checklist end-to-end on Windows 11 and on the physical Android phone; file screenshots. Install scrcpy on Windows (`choco install scrcpy` or download from the project's GitHub releases). Enable USB debugging in Developer Options on the physical Android phone, plug it in, and confirm that scrcpy mirrors the phone's screen with low latency. Configure the Android phone for mobile-hotspot mode and verify the laptop can connect to it in under thirty seconds. Conduct a thirty-minute test call in Microsoft Teams with a friend or classmate to verify screen-share quality, audio levels, and the side-by-side QtChat-plus-scrcpy window arrangement from the audience perspective. Record the backup demonstration video as a screen capture of the full live demo from start to finish — including the engineered server stop, queue-flush, and tamper attempt — using the same window arrangement as the live demo. Save the video locally, on a USB flash drive, and uploaded to OneDrive. Export a journald log excerpt showing ciphertext-only traffic for the reflection slide. Draft the slide deck (twelve slides, minimal text, large diagrams). **Checkpoint:** All tests pass; scrcpy reliably mirrors the phone; Teams test call confirmed working; backup video viewable from three storage locations; slides are drafted. **Hours:** ~12.

**Day 14 — Monday, 4 May 2026 — Dress rehearsal (no new code).** Run three full dress rehearsals of the live demonstration inside an actual Microsoft Teams meeting, recording each one and watching the recording afterwards to see what the audience will see — checking window sizes, font legibility, scrcpy frame rate, audio clarity, and the timing of each engineered failure. Fix only bugs that surface during rehearsal; no new features. Rehearse the presentation script aloud three times. Charge all devices to full. Prepare the demonstration kit: laptop with power supply, Android phone (charged, USB cable connected for scrcpy, mobile hotspot configured as backup internet), wired headset or USB microphone, USB flash drive containing the backup video and exported screenshots, browser tab open to the instructor's email address as final-fallback contact, OneDrive logged in to access the cloud copy of the backup video. Confirm Teams is updated to the latest version. Early sleep. **Checkpoint:** Three Teams-recorded rehearsals played back successfully, full presentation deliverable from memory in under thirteen minutes, all backup paths verified working. **Hours:** ~9.

**Demonstration day — Tuesday, 5 May 2026.** Arrive early. Connect equipment. Deliver the demonstration. Answer questions honestly, including about the lack of forward secrecy. Rest afterwards.

### 5.2 Parallel workstreams

| Workstream | D1 | D2 | D3 | D4 | D5 | D6 | D7 | D8 | D9 | D10 | D11 | D12 | D13 | D14 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| Server core | X | X |   |   |   |   | X |   |   |   |   |   |   |   |
| Client WsClient | X | X | X |   | X |   |   |   |   |   |   |   |   |   |
| Cryptography |   |   |   | X |   |   |   |   |   |   |   |   |   |   |
| QML UI |   | X |   |   | X |   |   |   | X |   |   | X |   |   |
| SQLite + TOFU |   |   |   |   |   | X |   |   |   |   |   |   |   |   |
| cPouta + TLS |   |   |   |   |   |   | X |   |   |   |   |   |   |   |
| Android build |   |   |   |   |   |   |   | X | X |   |   |   |   |   |
| Notifications |   |   |   |   |   |   |   |   |   | X |   |   |   |   |
| Files / images |   |   |   |   |   |   |   |   |   |   | X | X |   |   |
| Testing |   |   |   |   |   |   |   |   |   |   |   |   | X |   |
| Documentation | X |   |   |   |   |   |   |   |   |   |   |   | X |   |
| Rehearsal |   |   |   |   |   |   |   |   |   |   |   |   |   | X |

### 5.3 Critical path

The critical path runs skeleton → cryptography → cPouta deployment → Android deployment → first cross-platform end-to-end → files → polish → rehearsal. The two most vulnerable transitions are day seven (cPouta TLS) and day eight (Android OpenSSL), because both depend on toolchains that have the most variance in student projects. If either day slips, day nine's cross-platform integration is blocked. This is why the two-hour blocker rule is most strictly enforced on these two days: if Let's Encrypt is being rate-limited or the Android Gradle build is failing, the student switches immediately to another parallel workstream (UI polish, documentation, QtKeychain testing) rather than grinding on the blocked task.

### 5.4 Six key checkpoints

The plan is anchored by six binary checkpoints: first WSS handshake (end of day 2), first end-to-end encrypted round-trip (end of day 4), first real-TLS handshake to the cPouta server (end of day 7), first APK installed on the physical phone (end of day 8), first Android-to-desktop encrypted message (end of day 9), and clean dry-run demonstration (end of day 13). Missing any of these requires a re-plan; passing all six guarantees a demonstrable project.

---

## 6. Risk register

| ID | Risk | L | I | Score | Mitigation | Contingency |
|---|---|---|---|---|---|---|
| R01 | OpenSSL on Android fails to deploy | H | H | 9 | Use KDAB android_openssl from day 8 morning | Fall back to ws:// for Android demo, show wss:// only on desktop |
| R02 | libsodium Android cross-compile fails | M | H | 6 | Prebuilt static libs checked into /third_party on day 8 | Use sodium-plus prebuild from well-known fork |
| R03 | Let's Encrypt rate-limit or DNS delay | M | M | 4 | Use staging endpoint for testing day 7 morning | Self-signed cert + ignoreSslErrors for demo if needed |
| R04 | CSC cPouta VM issue on day 7 | L | H | 3 | Deployment fully scripted, any Ubuntu VM takes over | Run server on laptop over ngrok for demo |
| R05 | Android release keystore loss | L | H | 3 | Encrypted vault backup immediately after creation | Generate new keystore; acceptable since not on Play Store |
| R06 | Gradle/AGP version mismatch with Qt 6.11 | M | M | 4 | Exact versions from tech-stack table | Clear .gradle and android-build caches; rebuild |
| R07 | Any single day losing >2 hours to a blocker | H | M | 6 | Two-hour blocker rule; parallel workstream switch | Park blocker, resume next day with fresh eyes |
| R08 | Demonstration rescheduled earlier than 5 May | M | H | 6 | Day-11-ready rule | Deliver a minimally-polished version from day 11 state |
| R09 | Demonstration-day connectivity failure (student's home internet or Teams) | M | H | 6 | 4G mobile hotspot configured on Android phone as backup; backup video stored locally + flash drive + OneDrive; Teams pre-tested day 13 | If home internet fails, switch laptop to phone hotspot in <30 s; if Teams fails entirely, email instructor immediately to reschedule |
| R10 | Live demo app crash | L | H | 3 | 3× dress-rehearsal on day 14 with exact script | Show recorded backup; narrate architecture live |
| R11 | Illness during the 14 days | M | H | 6 | Cut Could-haves first (dark mode, typing indicator) | Move to day-11-ready state and rest until recovery |
| R12 | Vappu (1 May) reducing day-11 hours | M | M | 4 | Schedule accounts for it; split file-sharing across days 10 evening and 12 morning | Accept one overflow day |
| R13 | Silent end-to-end corruption bug | L | H | 3 | Property-based round-trip test day 4 | Revert to last known-good commit; document |
| R14 | QtKeychain per-OS quirks | M | M | 4 | Test on Windows day 6, Android day 8 | Fall back to passphrase-wrapped file via crypto_pwhash |
| R15 | 16 KB page-size warnings on Android 15+ | L | L | 1 | Ship 16KB-aligned OpenSSL from current KDAB release | Demonstrate on Android 14 phone |
| R16 | scrcpy fails to connect or is laggy on demo day | L | M | 2 | Install and test scrcpy on day 13; verify USB debugging persistence; have a second USB cable available | Hold phone next to laptop webcam as a degraded fallback; narrate phone state aloud |
| R17 | Audio echo, low quality, or background noise during Teams demo | M | M | 4 | Wired headset or USB microphone tested in Teams call on day 13; quiet room confirmed; phone on silent except for QtChat notification sound | Mute when not speaking; describe notification sounds verbally if audio capture fails |

---

## 7. Quality assurance plan

### 7.1 Testing strategy

Quality assurance combines three layers. Unit tests using Qt Test cover the cryptographic layer in isolation: encrypt-then-decrypt round-trip on text and binary payloads, single-byte tamper detection, wrong-key decryption failure, and fingerprint stability against a fixed test vector. Integration tests launch a QWebSocketServer in-process on an ephemeral port, connect a WsClient, and use QSignalSpy and QTRY_COMPARE to assert connection, reconnection, queue flushing, and thousand-message stress. Manual tests follow a written checklist run at the end of each day's checkpoint.

### 7.2 Acceptance criteria mapped to rubric dimensions

Functionality at grade 5 requires that ten edge-case scenarios pass without user-visible failure: network drop mid-send, server restart, forged ciphertext on receive, key mismatch after TOFU, file transfer interruption and resume, unknown protocol version, permission denied, rate-limit cooldown, dark-mode toggle mid-session, and Android background-then-foreground lifecycle. Architecture requires that the common library links into both the tests binary and the production client and server without modification, and that no QML file contains networking, cryptography, or database code. Networking requires that a thirty-second network drop produces zero message loss, that reconnection uses exponential backoff with jitter, and that three missed pongs trigger teardown-and-reconnect within five seconds. UI/UX requires that the six-hundred-pixel breakpoint works by resizing from 1200 to 400 pixels and back, that dark mode follows the OS setting on launch, and that all screens are reachable by keyboard on desktop. Presentation requires that the author can deliver the threat-model paragraph from memory and that the live demonstration includes at least two engineered failures shown as features.

### 7.3 Definition of Done

A feature is considered Done when its code is reviewed (self-review against the rule in section 4.5), its unit or integration test passes, a manual walkthrough has been performed and screenshotted, and the documentation has been updated to reflect the feature's behaviour.

---

## 8. Tools and resources

Development is centred on Qt Creator 19.0.0 Enterprise with Qt 6.11.0 kits for Windows MSVC, Android arm64 and armeabi-v7a and x86_64, and desktop Linux GCC for the server. The build system is CMake 3.21 or newer using the modern `qt_add_qml_module` convention. Version control is Git with GitHub as the remote. Libsodium is installed via vcpkg on Windows, via `pkg-config` on Linux, and as a prebuilt static library per ABI checked into `third_party/` on Android. QtKeychain is pulled from `frankosterfeld/qtkeychain` via FetchContent. OpenSSL on Android comes from `KDAB/android_openssl` via FetchContent. The server is deployed to a CSC cPouta Ubuntu 24.04 VM with TLS certificates from Let's Encrypt via certbot. Icons come from SVG Repo and UXWing (no attribution required). The UI font is Inter with a system-default fallback.

Documentation lives in `/docs/` as `architecture.md`, `threat-model.md`, `error-handling.md`, and this `project-plan.md`.

---

## 9. Presentation plan

The demonstration on the 5th of May is delivered online via Microsoft Teams and is allotted ten to fifteen minutes. The opening 90 seconds frame the problem and stakes, with the slide deck shared from PowerPoint in presenter mode. The next 150 seconds switch to the live demonstration: the student shares their full Windows desktop in Teams, on which the Windows QtChat client occupies the left half of the screen and a scrcpy window mirroring the physical Android phone occupies the right half, so that the audience sees both clients simultaneously communicating through the cPouta server. During this segment the student deliberately triggers three engineered failures, each framed as a feature rather than an accident. First, a stopped-server failure: the student SSH's into the cPouta VM in a small terminal pane and runs `sudo systemctl stop qtchat-server`, at which point the reconnecting banner appears in both the Windows and Android clients within two seconds. Second, an offline-queue demonstration: while the server is stopped, the student composes a message on the desktop client, watches the queued-message UI activate, then runs `sudo systemctl start qtchat-server`, and the queued message flushes through to the phone within five seconds of reconnection. Third, a tamper attempt: the student modifies a single byte in the test path used by the manual integrity-check helper added on day twelve, and the receiving client renders the integrity-failure indicator described in section 3 figure 10a. 150 seconds on architecture walk through Figure 16 and the three-way client-server-common split. 150 seconds on encryption cover X25519, XChaCha20-Poly1305, and the Signal-style 60-digit safety number. 150 seconds on networking cover reconnection, heartbeat, and the offline queue, citing the thirty-second survival acceptance criterion. 120 seconds of honest reflection name the lack of forward secrecy and the limits of trust-on-first-use. The remaining 90 seconds are reserved for questions.

Online delivery requires a specific technical setup that must be established and tested by the end of day thirteen. The student shares their entire desktop in Teams rather than a single window, because Teams shares only one window at a time and the demonstration requires both the QtChat application and the scrcpy phone-mirror window to be visible simultaneously. The Android phone is mirrored to the desktop via scrcpy, an open-source Android screen-mirroring utility from Genymobile that connects over USB with sub-100ms latency; it is installed on Windows from its GitHub releases page or via Chocolatey with `choco install scrcpy`, and the phone must have USB debugging enabled in Developer Options. Audio is delivered through a wired headset or USB microphone rather than the laptop's built-in microphone, both to avoid echo and to ensure the QtChat notification sound is captured cleanly by the screen-share audio channel. The slide deck is run on a second monitor in PowerPoint presenter mode if one is available; otherwise it is run on the same monitor and the student switches between slides and demonstration via Alt+Tab.

The backup plan, built on day 13, is a pre-recorded demonstration video of the full live demo recorded against the same window arrangement used for the live presentation, exported as MP4 and stored in three locations: locally on the demonstration laptop, on a USB flash drive, and uploaded to OneDrive or a personal cloud account that can be reached from any browser. A backup internet path is also prepared: the student's Android phone is configured for mobile-hotspot mode before the demonstration begins, so that if the home internet connection fails the laptop can be switched to the hotspot in under thirty seconds. The instructor's email address is open in a browser tab as a final fallback, so that if Teams collapses entirely the student can immediately propose a fifteen-minute reschedule rather than scrambling for an alternative on the call.

---

## 10. References

Primary references are the Qt 6.11 documentation pages covering `qt-add-qml-module`, `qtqml-cppintegration-definetypes`, `qwebsocket`, `qwebsocketserver`, the `sslechoserver` example, `qpermission`, `qnetworkinformation`, `qsqldatabase`, and `qtest-overview`. Cryptographic references are the libsodium documentation on public-key cryptography, AEAD with XChaCha20-Poly1305, and `crypto_generichash`. Library references are `frankosterfeld/qtkeychain` and `KDAB/android_openssl` on GitHub. Deployment references are Let's Encrypt's and CSC cPouta's documentation. Course references are the Metropolia UAS TT00GD57 kickoff and planning slides. The Signal Foundation's blog post on safety numbers motivates the 60-digit rendering.

---

## 11. Appendices

### Appendix A — Glossary

**AAB** Android App Bundle. **AEAD** Authenticated Encryption with Associated Data. **AGP** Android Gradle Plugin. **APK** Android Package Kit. **cPouta** CSC's OpenStack-based cloud platform for Finnish research users. **Double Ratchet** Signal's forward-secret key-agreement protocol (out of scope). **E2E** end-to-end encryption. **FCM** Firebase Cloud Messaging (documented, not implemented). **Fingerprint** short hash of a public key for human verification. **Forward secrecy** property that past messages stay secret if long-term keys leak (not provided). **JNI** Java Native Interface. **Let's Encrypt** free automated certificate authority. **libsodium** C cryptography library. **Metadata** who talked to whom, when, how much (not protected). **MLS** Messaging Layer Security. **NDK** Native Development Kit. **QWebSocket, QWebSocketServer** Qt's WebSocket classes. **QtKeychain** OS-integrated secret storage library. **SAF** Storage Access Framework. **scrcpy** open-source Android screen-mirroring tool from Genymobile, used during the online demonstration to display the physical phone's screen as a window on the Windows desktop. **SQLite** embedded SQL database. **systemd** Linux service manager. **Teams** Microsoft Teams, the videoconferencing platform used by Metropolia for online demonstrations. **TLS** Transport Layer Security. **TOFU** Trust on First Use. **WAL** Write-Ahead Logging. **WSS** WebSocket Secure. **X25519** ECDH primitive using Curve25519. **XChaCha20-Poly1305** AEAD with a 24-byte nonce.

### Appendix B — Figure index

| Figure | Caption | Section |
|---|---|---|
| 1 | Splash screen | 3 |
| 2 | Login with first-run key generation | 3 |
| 3 | TOFU fingerprint display | 3 |
| 4 | Main chat overview | 3 |
| 5 | Contacts list | 3 |
| 6 | Chat conversation with reconnecting banner | 3 |
| 7 | Image attachment bubble | 3 |
| 8 | Settings | 3 |
| 9 | Peer verification | 3 |
| 10 | Error states | 3 |
| 11 | Empty states | 3 |
| 12 | Android notification permission prompt | 3 |
| 13 | Android lock-screen notification | 3 |
| 14 | Windows system tray and toast | 3 |
| 15 | Navigation flow | 3 |
| 16 | System architecture | 4 |

### Appendix C — Icon attributions

All icons in the final product are drawn from SVG Repo (svgrepo.com) and UXWing (uxwing.com), both permitting commercial use without attribution.

### Appendix D — Open questions

The scope is frozen for the fourteen-day build. Two items remain optional. First, whether to implement the dark mode toggle as a Must or leave it as a Could-have fallback: current decision keeps it as a Could, implemented on day 12 only if no day has slipped. Second, whether to present on the cPouta-hosted server (slightly riskier, much more impressive) or on a local loopback as a fallback (safer, less impressive): current decision is cPouta with loopback as a fallback switchable in under 30 seconds.

---

*End of Project Plan Document, version 2.1, compressed for fourteen-day build with online demonstration.*
