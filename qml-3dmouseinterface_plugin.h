#ifndef QML_3DMOUSEINTERFACE_PLUGIN_H
#define QML_3DMOUSEINTERFACE_PLUGIN_H

#include <QQmlExtensionPlugin>

class Qml_3DMouseInterfacePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    void registerTypes(const char *uri);
};

#endif // QML_3DMOUSEINTERFACE_PLUGIN_H

