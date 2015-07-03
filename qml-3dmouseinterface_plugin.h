/**
 * @file qml-3dmouseinterface_plugin.h
 * @brief A mouse interface for qt3d scene3d
 * @author Lorenzo Lucignano
 * @version 1.0
 * @date 2015-07-02
 */
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

