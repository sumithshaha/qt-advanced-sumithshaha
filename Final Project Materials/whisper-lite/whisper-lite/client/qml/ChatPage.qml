import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Whisper

Page {
    id: page

    required property var controller
    signal logoutRequested()

    header: ToolBar {
        Material.foreground: "white"
        RowLayout {
            anchors.fill: parent
            anchors.margins: 4

            ToolButton {
                text: "\u2630"   // hamburger
                onClicked: drawer.open()
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 0
                Label {
                    text: page.controller.activePeer.length > 0
                          ? page.controller.activePeer
                          : qsTr("Whisper")
                    font.pixelSize: 18
                    font.weight: Font.Medium
                    elide: Text.ElideRight
                }
                Label {
                    text: page.controller.status
                    font.pixelSize: 12
                    opacity: 0.85
                }
            }

            StatusIndicator {
                state: page.controller.wsState
            }

            ToolButton {
                text: qsTr("Logout")
                onClicked: page.logoutRequested()
            }
        }
    }

    // ---- Drawer with online users ---------------------------------------
    Drawer {
        id: drawer
        edge: Qt.LeftEdge
        width: Math.min(page.width * 0.8, 320)
        height: page.height

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 8

            Label {
                text: qsTr("Online users")
                font.pixelSize: 18
                font.weight: Font.Bold
            }

            ListView {
                id: userList
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                model: page.controller.users
                spacing: 4

                delegate: ItemDelegate {
                    required property string nick
                    required property string pubkey
                    required property bool   online

                    width: ListView.view.width
                    enabled: online

                    contentItem: RowLayout {
                        spacing: 12
                        Rectangle {
                            width: 12; height: 12; radius: 6
                            color: parent.parent.online
                                   ? Material.color(Material.Green)
                                   : Material.color(Material.Grey)
                        }
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 0
                            Label { text: parent.parent.parent.nick; font.pixelSize: 16 }
                            Label {
                                text: parent.parent.parent.pubkey.length > 0
                                      ? parent.parent.parent.pubkey.substring(0, 16) + "…"
                                      : ""
                                visible: text.length > 0
                                font.pixelSize: 10
                                opacity: 0.55
                                font.family: "monospace"
                            }
                        }
                    }
                    onClicked: {
                        page.controller.selectPeer(nick)
                        drawer.close()
                    }
                }
            }

            Label {
                Layout.fillWidth: true
                wrapMode: Text.Wrap
                opacity: 0.6
                font.pixelSize: 11
                text: qsTr("Tap a user to start chatting. The 16-character " +
                           "string is the start of their public key — verify " +
                           "it matches their device for full assurance.")
            }
        }
    }

    // ---- Main message area ----------------------------------------------
    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // Empty state
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: page.controller.activePeer.length === 0

            ColumnLayout {
                anchors.centerIn: parent
                spacing: 8
                Label {
                    text: "\u{1F510}"
                    font.pixelSize: 64
                    Layout.alignment: Qt.AlignHCenter
                }
                Label {
                    text: qsTr("Select a user to start chatting")
                    Layout.alignment: Qt.AlignHCenter
                    opacity: 0.7
                }
            }
        }

        // Message list
        ListView {
            id: messageList
            Layout.fillWidth: true
            Layout.fillHeight: true
            visible: page.controller.activePeer.length > 0
            clip: true
            spacing: 4
            model: page.controller.messages
            verticalLayoutDirection: ListView.TopToBottom

            // Auto-scroll to bottom when a new message arrives.
            onCountChanged: positionViewAtEnd()
            Component.onCompleted: positionViewAtEnd()

            delegate: MessageDelegate {
                width: messageList.width
                msgText: model.text
                msgIncoming: model.incoming
                msgTimestamp: model.timestamp
            }
        }

        // Composer
        Rectangle {
            Layout.fillWidth: true
            visible: page.controller.activePeer.length > 0
            implicitHeight: composer.implicitHeight + 16
            color: Material.dialogColor

            RowLayout {
                id: composer
                anchors.fill: parent
                anchors.margins: 8
                spacing: 8

                TextField {
                    id: msgInput
                    Layout.fillWidth: true
                    placeholderText: qsTr("Encrypted message…")
                    selectByMouse: true
                    wrapMode: TextInput.Wrap
                    onAccepted: sendBtn.clicked()
                }
                Button {
                    id: sendBtn
                    text: qsTr("Send")
                    enabled: msgInput.text.length > 0
                    highlighted: true
                    onClicked: {
                        page.controller.sendText(page.controller.activePeer,
                                                 msgInput.text)
                        msgInput.clear()
                    }
                }
            }
        }
    }
}
