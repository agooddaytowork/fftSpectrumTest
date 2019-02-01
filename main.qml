import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.4

Window {
    id: mainWindow
    visible: true
    width: 1800
    height: 600
    title: qsTr("Hello World")


    Rectangle{
        id: drumRec
        width: 50
        height: 50
        radius: 25
        x:20
        border.width: 1
        border.color: "black"

        Label{
            text: "DRUM"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Connections{
            target: fftV
            onBeatDetected:
            {
                if(n==1)
                {
                     drumRec.color = "black"
                }


            }

            onClearBeatDetect:
            {
                drumRec.color = "white"
            }
        }

    }
    Rectangle{
        id: snareRec
        width: 50
        height: 50
        radius: 25
        x:100
        border.width: 1
        border.color: "black"
        Label{
            text: "SNARE"
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Connections{
            target: fftV
            onBeatDetected:
            {
                if(n==2)
                {
                   snareRec.color = "black"
                }



            }

            onClearBeatDetect:
            {
                snareRec.color = "white"
            }
        }

    }

//    Label{
//        id: subBand0Label

//        text: fftV.getSubandInstantEnergy(0)
//    }

//    Rectangle{
//        id: testRec

//        width: 100
//        anchors.bottom: parent.bottom
//        anchors.left: parent.left
//        height: 1
//        color: "black"
//    }


    Row
    {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 100
        anchors.left: parent.left
        spacing: 1
        Repeater
        {

            model:120
            Rectangle{
                id: testRec

                width: mainWindow.width/120 - 1
                 anchors.bottom: parent.bottom


//                anchors.left: parent.left
                height: 1
                color: "black"

                Connections{
                    target: fftV

                    onNewInstantEnergyAvailable:
                    {
//                        console.log(fftV.getSubandInstantEnergy(index))
//                      subBand0Label.text = fftV.getSubandInstantEnergy(0)
                        testRec.height =  fftV.getSubandInstantEnergy(index) * 500
                    }
                }
            }

        }
    }

    Row
    {
        x:0
        spacing: 1
//        width: parent.width
        height: 100
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        Repeater{
            model: 120

            Rectangle{
                id: labelRec
                property int  thisShitIndex: index
                height: 100
                width: mainWindow.width/120 - 1
                color: "grey"

                Label{


                    text: (labelRec.thisShitIndex*22050/512) + 22050/1024+ "Hz"
    //                text: "a"
                    rotation: 90

                    color: "white"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                }
            }


        }
    }

}
