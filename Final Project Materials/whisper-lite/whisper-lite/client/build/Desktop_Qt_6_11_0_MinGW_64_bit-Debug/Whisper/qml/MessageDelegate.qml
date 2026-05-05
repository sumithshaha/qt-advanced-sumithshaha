import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts

Item {
    id: root

    required property string msgText
    required property bool   msgIncoming
    required property var    msgTimestamp

    implicitHeight: bubble.implicitHeight + 8

    Rectangle {
        id: bubble

        anchors.right:  root.msgIncoming ? undefined : parent.right
        anchors.left:   root.msgIncoming ? parent.left : undefined
        anchors.rightMargin: 12
        anchors.leftMargin:  12
        anchors.topMargin:    4
        anchors.bottomMargin: 4

        radius: 18
        color: root.msgIncoming
               ? Material.color(Material.Grey, Material.Shade800)
               : Material.color(Material.Indigo, Material.Shade400)

        // Cap bubble width to ~78% of viewport
        width: Math.min(root.width * 0.78, contentCol.implicitWidth + 28)
        implicitHeight: contentCol.implicitHeight + 16

        // Entrance animation: fade-in + slide
        opacity: 0
        x: root.msgIncoming ? -20 : 20
        Component.onCompleted: {
            opacityAnim.start()
            slideAnim.start()
        }
        NumberAnimation {
            id: opacityAnim
            target: bubble; property: "opacity"
            from: 0; to: 1; duration: 220
            easing.type: Easing.OutCubic
        }
        NumberAnimation {
            id: slideAnim
            target: bubble; property: "x"
            to: root.msgIncoming
                ? bubble.anchors.leftMargin
                : root.width - bubble.width - bubble.anchors.rightMargin
            duration: 220
            easing.type: Easing.OutCubic
        }

        ColumnLayout {
            id: contentCol
            anchors.fill: parent
            anchors.margins: 8
            spacing: 2

            Label {
                Layout.fillWidth: true
                text: root.msgText
                wrapMode: Text.Wrap
                color: "white"
                font.pixelSize: 15
            }

            Label {
                text: Qt.formatDateTime(new Date(root.msgTimestamp), "hh:mm")
                color: "white"
                opacity: 0.7
                font.pixelSize: 10
                Layout.alignment: root.msgIncoming ? Qt.AlignLeft : Qt.AlignRight
            }
        }
    }
}
