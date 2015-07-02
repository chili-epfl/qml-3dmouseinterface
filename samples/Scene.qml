
import Qt3D 2.0
import Qt3D.Renderer 2.0


Entity {
    id: sceneRoot
    property alias camera : camera
    property alias sceneRoot : sceneRoot
    property alias anothersphereEntity: anothersphereEntity
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

    PhongMaterial {
        id: material1
        ambient: "lawngreen"
        diffuse: "forestgreen"
    }
    PhongMaterial {
        id: material2
        ambient: "lawngreen"
        diffuse: "forestgreen"
    }
    PhongMaterial {
        id: material3
        ambient: "lawngreen"
        diffuse: "forestgreen"
    }

    TorusMesh {
        id: torusMesh
        radius: 5
        minorRadius: 1
        rings: 100
        slices: 20
    }

    Transform {
        id: torusTransform
        Scale { scale3D: Qt.vector3d(1.5, 1, 0.5) }
        Rotate {
            angle: 45
            axis: Qt.vector3d(1, 0, 0)
        }
    }

    Entity {
        id: torusEntity
        components: [ torusMesh, material1, torusTransform ]
    }

    SphereMesh {
        id: sphereMesh
        radius: 3
    }

    Transform {
        id: sphereTransform1
        Translate {
            translation: Qt.vector3d(20, 0, 0)
        }

        Rotate {
            id: sphereRotation
            axis: Qt.vector3d(0, 1, 0)
        }
    }
    Transform {
        id: sphereTransform2
        Translate {
            translation: Qt.vector3d(-20, 0, 0)
        }

        Rotate {
            id: sphereRotation2
            axis: Qt.vector3d(0, 1, 0)
        }
    }

    Entity {
        id: sphereEntity
        property bool interactive:  true
        property bool clicked: false
        property string name:"Sphere1"

        onClickedChanged: {
            clicked ? material2.diffuse="dodgerblue": material2.diffuse="forestgreen"
            }
        components: [ sphereMesh, material2, sphereTransform1 ]
    }

    Entity {
        property bool interactive:  true
        property bool clicked: false
        property string name:"Sphere2"

        onClickedChanged: {
            clicked ? material3.diffuse="dodgerblue": material3.diffuse="forestgreen"
            }
        id: anothersphereEntity
        components: [ sphereMesh, material3, sphereTransform2 ]
    }
}
