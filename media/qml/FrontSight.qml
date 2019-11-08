import QtQuick 2.0

Item {
    property color _color
    Rectangle{
        anchors.centerIn: parent
        width: 25
        height: 25
        radius: 12.5
        border.width: 2
        border.color: _color
        color: "#00000000"
    }
    Rectangle{
        width: 2
        height: 10
        color: _color
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: -12
    }
    Rectangle{
        width: 2
        height: 10
        color: _color
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenterOffset: 12
    }
    Rectangle{
        width: 10
        height: 2
        color: _color
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: 12
    }
    Rectangle{
        width: 10
        height: 2
        color: _color
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.horizontalCenterOffset: -12
    }
}
