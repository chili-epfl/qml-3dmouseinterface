TEMPLATE = lib
TARGET =qml-3DMouseInterface

QT += qml quick 3dcore 3drenderer 3dcore-private

CONFIG += qt plugin c++11
CONFIG -= android_install

TARGET = $$qtLibraryTarget($$TARGET)
uri = MouseInterface3D

# Input
SOURCES += \
    qml-3dmouseinterface_plugin.cpp \
    3dmouseinterface.cpp

HEADERS += \
    qml-3dmouseinterface_plugin.h \
    3dmouseinterface.h

OTHER_FILES += qmldir


qmldir.files = qmldir

installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir


