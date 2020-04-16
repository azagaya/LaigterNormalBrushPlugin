TEMPLATE      = lib
CONFIG       += plugin
QT           += core gui widgets

TARGET = normalbrush

CONFIG += core ui c++11

isEmpty(LAIGTER_SRC){
  LAIGTER_SRC=../laigter
}


INCLUDEPATH  += $$LAIGTER_SRC/
INCLUDEPATH  += $$LAIGTER_SRC/src/

HEADERS += \
  src/normalbrushgui.h \
  src/normalbrushplugin.h \
  src/normalselectorwidget.h \
  $$LAIGTER_SRC/src/image_processor.h \
  $$LAIGTER_SRC/src/light_source.h\
  $$LAIGTER_SRC/src/texture.h \
  $$LAIGTER_SRC/src/sprite.h \
  $$LAIGTER_SRC/src/brush_interface.h

SOURCES += \
  src/normalbrushgui.cpp \
  src/normalbrushplugin.cpp \
  src/normalselectorwidget.cpp \
  $$LAIGTER_SRC/src/image_processor.cpp\
  $$LAIGTER_SRC/src/light_source.cpp \
  $$LAIGTER_SRC/src/texture.cpp \
  $$LAIGTER_SRC/src/sprite.cpp

FORMS += \
  src/normalbrushgui.ui

isEmpty(PREFIX){
  PREFIX = .local/share/laigter/plugins
}

target.path = $$PREFIX/

DESTDIR = $$PREFIX
INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

DISTFILES += \
  metadata.json \
  shaders/sphere_fragment.glsl \
  shaders/sphere_vertex.glsl

RESOURCES += \
  icons.qrc \
  shaders.qrc

unix{
    CONFIG += link_pkgconfig
    packagesExist(opencv4){
        PKGCONFIG += opencv4
        DEFINES += CV_RGBA2GRAY=COLOR_RGBA2GRAY
        DEFINES += CV_RGB2GRAY=COLOR_RGB2GRAY
        DEFINES += CV_GRAY2RGB=COLOR_GRAY2RGB
        DEFINES += CV_GRAY2RGBA=COLOR_GRAY2RGBA
        DEFINES += CV_DIST_L2=DIST_L2
    } else {
        PKGCONFIG += opencv
    }
}

win32: LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_core320.dll
win32: LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_imgproc320.dll
win32: LIBS += C:\opencv-build\install\x64\mingw\bin\libopencv_imgcodecs320.dll
win32: LIBS += C:\Qt\5.13.0\mingw73_64\lib\libQt5OpenGL.a -lopengl32

win32: INCLUDEPATH += C:\opencv\build\include
