import QtQuick 1.0

Rectangle {
    color: "blue"
    width: 200
    height: 300
    id: page
    ListModel {
        id: model
        ListElement {
            name: "January"
        }
        ListElement {
            name: "February"
        }
    }
    ListView {
        anchors.fill: parent
        model: model
        delegate: Rectangle {
            color: "red"
            width: 100
            height: 100
            Text {
                text: name
            }
        }
    }
}
