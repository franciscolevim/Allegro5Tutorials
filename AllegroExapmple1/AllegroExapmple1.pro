TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

#Можно по отдельности подключать
#LIBS += -lallegro
#LIBS += -lallegro_dialog
#LIBS += -lallegro_image
#А можно пакетом
LIBS += `pkg-config --libs allegro-5.0 allegro_dialog-5.0 allegro_ttf-5.0 allegro_font-5.0 allegro_primitives-5.0 allegro_image-5.0`

QMAKE_CXXFLAGS += -std=gnu++14
