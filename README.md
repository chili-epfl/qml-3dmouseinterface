qml-3DMouseInterface
=============

qml-3DMouseInterface is a plugin that enables the mouse interaction with the Entities in a Qt3D scene.
The plugin is going to be obsolete once an official implementation is released.
 

The following is required for the plugin to work:

  - Ubuntu `14.04`
  - Qt `5.5` + QT3d `2.0`
  
QML API
-------

The QML API consists of the single element **MouseInterface3D**. In order to work, the item need two properties to be set:

>  - **scene3D** The id of the root Entity of a Scene3D.
>   -**camera**  The camera of Scene3D.

The slot `select` can be invoked by `MouseArea.onClick`, passing the normalized coordinate of the mouse pointer as arguments.

On `select` is called, the plugin looks for all the Entities in scene3D having:

1. Both a Qt3D::QAbstractMesh and Qt3D::QTransform components;
2. The property `interactive` true (interactive should be added as custom flag in the qml code)

Moreover, the plugin assumes that the Entities have a boolean `clicked` property, that is set when the user clicks on the object.


Desktop Build
-------------

1. Build and install qt  (http://qt-project.org/wiki/Building_Qt_5_from_Git):
	For making the compilation faster, skip the following modules in the configure step
	
	```
	-skip qttranslations -skip qtwebkit -skip qtserialport -skip qtwebkit-examples  -skip qtwebengine 	

	```

2. Build and install qt creator (http://wiki.qt.io/Building-Qt-Creator-from-Git):
	

3. Open qml-chilitags in Qt creator

  Now the Chilitags QML plugin is installed alongside Qt's QML plugins and can be used similar to any other plugin.


Android Build
-------------

In addition to the OS, Qt requirements, you need:

  - Android SDK and Android API 14 (Android version 4.0)

  - Android NDK r10d

These instructions assume `armv7-a` target architecture. For other architectures, adapt the instructions to your liking.



1. Build and install `qt` from (http://qt-project.org/wiki/Qt5ForAndroidBuilding):

  ```
  cd qt 5
  mkdir qt-build-android && cd qt-build-android
  ../configure -xplatform android-g++ -developer-build -opensource -nomake tests -nomake examples -android-sdk [android-sdk-dir]  -android-ndk [android-ndk-r10d-dir] -android-toolchain-version 4.9  -android-ndk-host linux-x86_64 -android-ndk-platform android-19 -android-arch armeabi-v7a -confirm-license -no-warnings-are-errors -skip qtactiveqt -skip qttranslations -skip qtwebkit -skip qtserialport -skip qtwebkit-examples -skip qtdoc -skip qtrepotools -skip qtqa -skip qtsystems -skip qtdocgallery -skip qtpim -skip qtwayland -skip qtx11extras -skip qtmacextras -skip qtwinextras -skip qtenginio -skip webchannel
  make
  ```

To customize for CortexA15.A7 of Tablet Samsung SM-P600, enabling neon and hard float-abi:
  
   ```
   	gedit qt5/qtbase/mkspecs/android-g++/qmake.conf
	Search for `equals(ANDROID_TARGET_ARCH, armeabi-v7a)`, comment and substitute it with 
	QMAKE_CFLAGS = -Wno-psabi -march=armv7ve -mtune=cortex-a15.cortex-a7  -mcpu=cortex-a15 -mfpu=neon-vfpv4 -mhard-float -mfloat-abi=hard -  D_NDK_MATH_NO_SOFTFP=1  -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -DANDROID -Wa,--noexecstack -fno-builtin-memmove  -ftree-vectorize -ftree-vectorizer-verbose=1 -Wno-error=cast-align
	search for line `QMAKE_LFLAGS = --sysroot=$$ANDROID_PLATFORM_ROOT_PATH` and add -Wl,--no-warn-mismatch -lm_hard
	Search for `QMAKE_LIBS_PRIVATE`, remove -lm and add -lm_hard

   ```

2. Using qml-chilitags in Qt creator:

  Remember to add 'make install' after the step 'make' in the Build Steps.
  
	In order to debug the pluging when used in a 3rd qml application, you need to make a symbolic link to the plugin library (.so) in the build dir of the application so that gdb canm load the symbols [http://visualgdb.com/gdbreference/commands/set_solib-search-path] (I cannot set it in QTC, it is overwritter...) 

QT Creator doesn't recognize the pluging
----------------------------------------
With reference to [http://doc.qt.io/qt-5/qtqml-modules-qmldir.html#writing-a-qmltypes-file]

Usually plugins get installed in [qt-build]/qtbase/qml/Plugin-name
For some obscure reason Qt doesn't create the plugins.qmltype that provides the plugin description.
In order to solve it, you have to manually create the file using the tool `qmlplugindump` in [qt-built]/qtbase/bin

For example, 
```
qmlplugindump MouseInterface3D 1.0 [qt-build]/qtbase/qml/MouseInterface3D > [qt-build]/qtbase/qml/MouseInterface3D/plugin.qmltypes
```

Running Samples
---------------

The folder [samples/] contains a simple qml demo. By clicking on the objects, those change material properties. 

