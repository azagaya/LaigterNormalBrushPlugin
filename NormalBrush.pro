TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets
INCLUDEPATH  += ../laigter/
INCLUDEPATH  += ../laigter/src/
DESTDIR       = /home/azagaya/.local/share/laigter/plugins/

HEADERS += \
  src/normalbrushgui.h \
  src/normalbrushplugin.h \
  src/normalselectorwidget.h \
  ../laigter/src/imageprocessor.h\

SOURCES += \
  src/normalbrushgui.cpp \
  src/normalbrushplugin.cpp \
  src/normalselectorwidget.cpp \
  ../laigter/src/imageprocessor.cpp\

FORMS += \
  src/normalbrushgui.ui

target.path = /home/azagaya/.local/share/laigter/plugins/
INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

DISTFILES += \
  shaders/sphere_fragment.glsl \
  shaders/sphere_vertex.glsl

RESOURCES += \
  icons.qrc \
  shaders.qrc
