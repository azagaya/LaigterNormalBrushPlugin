TEMPLATE      = lib
CONFIG       += plugin
# QOpenGLWidget moved to its own module in qt6
QT           += core gui widgets opengl openglwidgets

TARGET = normalbrush

CONFIG += core ui c++17

isEmpty(LAIGTER_SRC){
  LAIGTER_SRC=../laigter
}


INCLUDEPATH  += $$LAIGTER_SRC/
INCLUDEPATH  += $$LAIGTER_SRC/src/

HEADERS += \
  src/normalbrushgui.h \
  src/normalbrushplugin.h \
  src/normalselectorwidget.h \
  $$LAIGTER_SRC/src/brush_interface.h \
  $$LAIGTER_SRC/src/processor_interface.h

SOURCES += \
  src/normalbrushgui.cpp \
  src/normalbrushplugin.cpp \
  src/normalselectorwidget.cpp

FORMS += \
  src/normalbrushgui.ui

isEmpty(PREFIX){
unix{
 PREFIX = $$system(echo $HOME)/.local/share/laigter/plugins
}
win32{
 PREFIX = $$system(echo %APPDATA%)/laigter/plugins
}
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

# The qt opengl libs come from the QT line above, only the system one is left
win32: LIBS += -lopengl32
