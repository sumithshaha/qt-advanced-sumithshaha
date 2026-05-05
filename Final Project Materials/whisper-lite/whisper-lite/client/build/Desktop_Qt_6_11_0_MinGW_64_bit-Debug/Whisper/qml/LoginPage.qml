import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Whisper

Page {
    id: page
    signal loginRequested(string nickname, string serverUrl)

    background: Rectangle {
        color: Material.backgroundColor
    }

    ColumnLayout {
        anchors.centerIn: parent
        width: Math.min(parent.width - 48, 400)
        spacing: 16

        // App branding
        Label {
            text: qsTr("Whisper")
            font.pixelSize: 48
            font.weight: Font.Bold
            Layout.alignment: Qt.AlignHCenter
            color: Material.accent
        }
        Label {
            text: qsTr("End-to-end encrypted chat")
            font.pixelSize: 14
            opacity: 0.7
            Layout.alignment: Qt.AlignHCenter
            Layout.bottomMargin: 24
        }

        // Inputs
        TextField {
            id: nickField
            Layout.fillWidth: true
            placeholderText: qsTr("Nickname")
            text: Settings.lastNickname
            selectByMouse: true
            maximumLength: 32
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText

            // Block whitespace at the start (Tero will absolutely test this)
            validator: RegularExpressionValidator {
                regularExpression: /^\S.{0,31}$/
            }
        }

        TextField {
            id: urlField
            Layout.fillWidth: true
            placeholderText: qsTr("Server URL (wss://…)")
            text: Settings.lastServerUrl
            selectByMouse: true
            inputMethodHints: Qt.ImhUrlCharactersOnly | Qt.ImhNoAutoUppercase
        }

        Switch {
            id: darkSwitch
            text: qsTr("Dark mode")
            checked: Settings.darkMode
            onToggled: Settings.darkMode = checked
        }

        Button {
            id: connectBtn
            Layout.fillWidth: true
            Layout.topMargin: 16
            text: qsTr("Connect")
            highlighted: true
            enabled: nickField.acceptableInput && urlField.text.startsWith("ws")

            onClicked: page.loginRequested(nickField.text.trim(), urlField.text.trim())
        }

        Label {
            Layout.fillWidth: true
            Layout.topMargin: 8
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
            opacity: 0.6
            font.pixelSize: 12
            text: qsTr("Tip: emulator → wss://10.0.2.2:8765\nDevice with adb reverse → wss://localhost:8765")
        }
    }
}
