TARGET = zeitgeist
TEMPLATE = app
QT += core gui widgets
CONFIG += c++11

win32:DESTDIR = bin

RESOURCES += resources/resources.qrc

gcc|clang {
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
}

include(src.pri)

defined(QUAZIP, var) {
    LIBS += -lquazip
    LIBS *= -L$$QUAZIP
    SOURCES += zip.cpp
    HEADERS += zip.h
}
