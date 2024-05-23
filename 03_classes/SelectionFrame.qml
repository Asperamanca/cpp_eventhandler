import QtQuick

Rectangle
{
    required property bool bSelected
    visible: bSelected
    anchors.fill: parent
    anchors.margins: -3
    color: "transparent"
    border.width: 1
    border.color: "black"
}
