/**
 * @file qml-3dmouseinterface_plugin.cpp
 * @brief A mouse interface for qt3d scene3d
 * @author Lorenzo Lucignano
 * @version 1.0
 * @date 2015-07-02
 */

#include "qml-3dmouseinterface_plugin.h"
#include "3dmouseinterface.h"

#include <qqml.h>

void Qml_3DMouseInterfacePlugin::registerTypes(const char *uri)
{
    qmlRegisterType<MouseInterface3D>(uri, 1, 0, "MouseInterface3D");
}




