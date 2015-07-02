import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Scene3D 2.0
import MouseInterface3D 1.0

ApplicationWindow {
    title: qsTr("Hello World")
    width: 640
    height: 480
    visible: true

    Item{
        anchors.fill:parent
        Scene3D {
            id:scene3Dwrapper
            anchors.fill: parent
            focus: true
            aspects: "input"
            Scene {
                id: scene3D
            }
        }
        MouseInterface3D{
            id:mouseInterface
            scene3D:scene3D.sceneRoot
            camera:scene3D.camera
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                mouseInterface.select(Qt.vector2d((2.0 * mouseX)/width - 1.0,1.0-(2.0 * mouseY)/height))
            }
        }
    }




}
