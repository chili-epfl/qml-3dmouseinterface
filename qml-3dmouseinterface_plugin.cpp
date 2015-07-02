#include "qml-3dmouseinterface_plugin.h"
#include "3dmouseinterface.h"

#include <qqml.h>

void Qml_3DMouseInterfacePlugin::registerTypes(const char *uri)
{
    qmlRegisterType<MouseInterface3D>(uri, 1, 0, "MouseInterface3D");
}




