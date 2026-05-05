import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Whisper

ApplicationWindow {
    id: root
    width: 412
    height: 892
    visible: true
    title: qsTr("Whisper")

    Material.theme: Settings.darkMode ? Material.Dark : Material.Light
    Material.primary: Material.Indigo
    Material.accent: Material.Pink

    ChatController {
        id: ctrl

        onLastErrorChanged: {
            if (ctrl.lastError.length > 0)
                errorBar.show(ctrl.lastError)
        }
    }

    StackView {
        id: stack
        anchors.fill: parent

        initialItem: LoginPage {
            onLoginRequested: function(nick, urlStr) {
                Settings.lastNickname = nick
                Settings.lastServerUrl = urlStr
                ctrl.start(nick, urlStr)
                stack.push(chatPage)
            }
        }

        Component {
            id: chatPage
            ChatPage {
                controller: ctrl
                onLogoutRequested: {
                    ctrl.logout()
                    stack.pop()
                }
            }
        }
    }

    // Inline error banner — slides down from the top when ctrl.lastError changes
    Rectangle {
        id: errorBar
        function show(text) {
            errorLabel.text = text
            visible = true
            hideTimer.restart()
        }
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: errorLabel.implicitHeight + 24
        color: Material.color(Material.Red, Material.Shade700)
        visible: false
        z: 100

        Label {
            id: errorLabel
            anchors.fill: parent
            anchors.margins: 12
            color: "white"
            wrapMode: Text.Wrap
            verticalAlignment: Text.AlignVCenter
        }

        Timer {
            id: hideTimer
            interval: 4000
            onTriggered: errorBar.visible = false
        }

        Behavior on visible {
            NumberAnimation { duration: 250 }
        }
    }
}
