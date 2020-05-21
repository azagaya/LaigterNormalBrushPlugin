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
  PREFIX = $$system(echo $HOME)/.local/share/laigter/plugins
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
