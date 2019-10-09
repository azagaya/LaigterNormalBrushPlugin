TEMPLATE      = lib
CONFIG       += plugin
QT           += widgets
INCLUDEPATH  += ../laigter/
INCLUDEPATH  += ../laigter/src/
DESTDIR       = /home/azagaya/.local/share/laigter/plugins/

HEADERS += \
  normalbrushgui.h \
  normalbrushplugin.h \
  normalselectorwidget.h \
  ../laigter/src/imageprocessor.h\

SOURCES += \
  normalbrushgui.cpp \
  normalbrushplugin.cpp \
  normalselectorwidget.cpp \
  ../laigter/src/imageprocessor.cpp\

FORMS += \
  normalbrushgui.ui

target.path = /home/azagaya/.local/share/laigter/plugins/
INSTALLS += target

CONFIG += install_ok  # Do not cargo-cult this!
uikit: CONFIG += debug_and_release

DISTFILES += \
  sphere_fragment.glsl \
  sphere_vertex.glsl

RESOURCES += \
  icons.qrc \
  shaders.qrc
