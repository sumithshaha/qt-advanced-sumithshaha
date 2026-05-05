import QtQuick
import QtQuick.Controls.Material
import Whisper

Item {
    id: root
    property int state: WsClient.Disconnected
    implicitWidth: 16
    implicitHeight: 16

    Rectangle {
        anchors.centerIn: parent
        width: 12; height: 12; radius: 6
        color: {
            switch (root.state) {
            case WsClient.Connected:    return Material.color(Material.Green)
            case WsClient.Connecting:
            case WsClient.Reconnecting: return Material.color(Material.Amber)
            case WsClient.Failed:       return Material.color(Material.Red)
            default:                    return Material.color(Material.Grey)
            }
        }

        // Pulse animation while connecting / reconnecting
        SequentialAnimation on opacity {
            loops: Animation.Infinite
            running: root.state === WsClient.Connecting ||
                     root.state === WsClient.Reconnecting
            NumberAnimation { from: 1.0; to: 0.3; duration: 600 }
            NumberAnimation { from: 0.3; to: 1.0; duration: 600 }
        }
    }
}
