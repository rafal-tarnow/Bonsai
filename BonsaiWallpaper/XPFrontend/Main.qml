import QtQuick
import "./mask"
import "."

// MainWindows{
//     FontLoader {
//         id: tahomaRegular
//         source: "qrc:/assets/fonts/tahoma.ttf"
//     }

//     FontLoader {
//         id: tahomaBold
//         source: "qrc:/assets/fonts/tahomabd.ttf"
//     }
// }

MainMask{
    FontLoader {
        id: tahomaRegular
        source: "qrc:/assets/fonts/tahoma.ttf"
    }

    FontLoader {
        id: tahomaBold
        source: "qrc:/assets/fonts/tahomabd.ttf"
    }
}
