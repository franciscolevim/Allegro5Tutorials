TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += `pkg-config --libs allegro-5.0 allegro_dialog-5.0 allegro_image-5.0`

QMAKE_CXXFLAGS += -std=gnu++14
