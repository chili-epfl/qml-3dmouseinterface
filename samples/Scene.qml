
import Qt3D 2.0
import Qt3D.Renderer 2.0


Entity {
    id: sceneRoot
    objectName: "sceneRoot"
    property alias camera : camera
    property alias sceneRoot : sceneRoot
    Camera {
        id: camera
        projectionType: CameraLens.PerspectiveProjection
        fieldOfView: 45
        aspectRatio: 16/9
        nearPlane : 0.1
        farPlane : 1000.0
        position: Qt.vector3d( 0.0, 0.0, -40.0 )
        upVector: Qt.vector3d( 0.0, 1.0, 0.0 )
        viewCenter: Qt.vector3d( 0.0, 0.0, 0.0 )
    }

    Configuration  {
        controlledCamera: camera
    }

    components: [
        FrameGraph {
            activeFrameGraph: Viewport {
                id: viewport
                rect: Qt.rect(0.0, 0.0, 1.0, 1.0) // From Top Left
                clearColor: Qt.rgba(0, 0.5, 1, 1)

                CameraSelector {
                    id : cameraSelector
                    camera: camera

                    ClearBuffer {
                        buffers : ClearBuffer.ColorDepthBuffer
                    }
                }
            }
        }
    ]


    CuboidMesh {
        id: sphereMesh
    //    radius: 3
    }

    Transform {
        id: sphereTransform1
        Translate {
            translation: Qt.vector3d(20, 0, 0)
        }

    }

    Entity {
        id: sphereEntity
        objectName: "Sphere"
        components: [ sphereMesh, sphereTransform1 ]
    }

    Entity {
        id:monkey
        components : [

            SceneLoader
            {
                source: "qrc:/test_scene.dae"
            }]
    }






}
