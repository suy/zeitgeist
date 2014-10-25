TARGET = zeitgeist
TEMPLATE = app

CONFIG += c++11 debug
QT += core gui widgets

QMAKE_CXXFLAGS += \
    -Wextra \
    -Werror \
    -pedantic \
    -Wold-style-cast \
    -Wfloat-equal \
    -Wcast-qual \
    -Wcast-align \
    -Wuninitialized \
    -Winit-self \
    -Woverloaded-virtual \

artifacts_dir = tmp
MOC_DIR = $$artifacts_dir
OBJECTS_DIR = $$artifacts_dir

include(src.pri)
