import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.2
import OpenGLItem 1.0
Window {
    id:mainWindow
    visible: true
    width:400*1.8
    height:400
    OpenGLItem
       {
           id: openGLItem
           visible: true
           anchors.fill: parent
           onCrashed:{
               shipBackground.shake();
           }
       }
    ShipBackground {
        id:shipBackground
        anchors.fill: parent
        z:2
    }
    Text {
        id: speedTxt
        text: "0.0"
        color: "yellow"
        font.pointSize: 30
        font.family:mFLiHei.name
        x:40
        y:40
        z:2
    }
    Text {
        id: posTxt
        text: "方位："
        color: "purple"
        font.pointSize: 20
        font.family:weimiHei.name
        x:40
        y:120
        z:2
    }


    Connections{
        target:openGLItem
        onOutputShipSpeed:{
            speedTxt.text=(speed*180).toFixed(1);
            posTxt.text="方位："+shipx.toFixed(0)+","+shipz.toFixed(0)+","+shipy.toFixed(0);
        }
    }
    FontLoader { id: mFLiHei; source: "../font/MFLiHei_Noncommercial-Regular.ttf" }
    FontLoader {id: weimiHei; source: "../font/WenYiquan_WeimiHei.ttf"}

}
