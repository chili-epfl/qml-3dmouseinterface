TEMPLATE = lib
TARGET = qml-3DMouseInterface
QT += qml quick 3dcore 3drenderer 3dcore-private
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = MouseInterface3D

# Input
SOURCES += \
    qml-3dmouseinterface_plugin.cpp \
    3dmouseinterface.cpp

HEADERS += \
    qml-3dmouseinterface_plugin.h \
    3dmouseinterface.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

