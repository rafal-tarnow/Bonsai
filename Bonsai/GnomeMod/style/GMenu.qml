import QtQuick
import QtQuick.Controls

Menu {
    id: contextMenu

    property int calculatedWidth: 0
    implicitWidth: 120
    //implicitWidth: calculatedWidth + leftPadding + rightPadding


    leftPadding: 3
    rightPadding: 3
    topPadding: 4
    bottomPadding: 4

    property bool active: true

    delegate: GMenuItem{
        id: delegateItem
        Binding{
            delegateItem.active: contextMenu.active
        }
    }


    background: Rectangle{
        color: "#ffffff"
        radius: 6

        border.width: 1
        border.color: "#e1e1e1"
    }

    // function updateLayout() {
    //     let maxItemWidth = 0;
    //     for (let i = 0; i < contentData.length; ++i) {
    //         // Sprawdzamy, czy element ma właściwość implicitWidth
    //         if (contentData[i] && contentData[i].hasOwnProperty('implicitWidth')) {
    //             if (contentData[i].implicitWidth > maxItemWidth) {
    //                 maxItemWidth = contentData[i].implicitWidth;
    //             }
    //         }
    //     }
    //     // Ustawiamy naszą właściwość na znalezione maksimum
    //     calculatedWidth = maxItemWidth;
    // }

    // onContentDataChanged: {
    //     updateLayout()
    // }

    // Component.onCompleted: {
    //     updateLayout()
    // }
}
