import QtQuick
import mehQml

Rectangle
{
    id: root
    color: "white"
    width: 800
    height: 600

    Rectangle
    {
        id: item1
        objectName: "RedRectangle"
        x: 20
        y: 20
        width: 50
        height: 30
        color: "red"
        property int iId: 1
        property bool bSelected: false
        SelectionFrame
        {
            bSelected: parent.bSelected
        }
    }

    Rectangle
    {
        id: item2
        objectName: "YellowRectangle"
        x: 160
        y: 115
        width: 40
        height: 70
        color: "yellow"
        property int iId: 2
        property bool bSelected: false
        SelectionFrame
        {
            bSelected: parent.bSelected
        }
    }

    Rectangle
    {
        id: item3
        objectName: "GreenRectangle"
        x: 300
        y: 155
        width: 80
        height: 25
        color: "green"
        property int iId: 3
        property bool bSelected: false
        SelectionFrame
        {
            bSelected: parent.bSelected
        }
    }

    Rectangle
    {
        id: lasso
        color: "#60A0A0A0"
        visible: false
    }
}
