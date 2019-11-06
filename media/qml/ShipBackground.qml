import QtQuick 2.11
import QtQuick.Controls 2.2
Item {
    property int shakeCount:0
    Image {
        id:img
        source: "../image/shipBG.png"
        width: parent.width*1.1
        height: parent.height*1.1
        x:parent.width*(-0.05)
        y:parent.height*(-0.05)
    }
    XAnimator{
        id:shakeAnim1
        target: img
        from: parent.width*(-0.06)
        to:parent.width*(-0.04)
        duration: 10
        onStopped: {
            shakeAnim2.start();
        }
    }
    XAnimator{
        id:shakeAnim2
        target: img
        to: parent.width*(-0.06)
        from:parent.width*(-0.04)
        duration: 10
        onStopped: {
            console.log(shakeCount);
            shakeCount++;
            if(shakeCount>=3){
                shakeCount=0;
                img.x=parent.width*(-0.05);
            }

            else shakeAnim1.start();

        }
    }
    function shake(){
        shakeAnim1.start();
    }
}
