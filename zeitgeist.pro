TARGET = zeitgeist
TEMPLATE = app
CONFIG += c++11

# QMake's debug/release build mechanics are complete BS. So, in order
# to build a release build, you need to remove the debug option and
# maybe add the release option (depending on the defaults of your Qt
# distribution).

mac {
    INCLUDEPATH += /usr/local/include/quazip5
    LIBS += -L/usr/local/lib -lquazip5
} unix {
    INCLUDEPATH += /usr/include/quazip5 # Directory with header files
    LIBS += -L/usr/lib64 -lquazip5 # -L<path/to> -l<library>
} win32 {
    INCLUDEPATH += C:\lib\include\quazip # Folder with header files
    LIBS += -LC:\lib\bin -lquazip # -L<path/to> -l<library>
}

# CONFIG(debug, debug|release) {
# message ("debug build")
# BUILD_DIR = build/debug
# #QMAKE_CXXFLAGS += -Wno-unused-variable
# } else {
# message ("release build")
# DEFINES += QT_NO_DEBUG_OUTPUT
# BUILD_DIR = build/release
# }

QT += core gui widgets

RESOURCES += resources/resources.qrc

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

# DESTDIR = $$BUILD_DIR
# MOC_DIR = $$BUILD_DIR
# OBJECTS_DIR = $$BUILD_DIR
# RCC_DIR = $$BUILD_DIR

include(src.pri)

# mac {
# OUTPUT_FILE = $$quote($$DESTDIR/$$TARGET\.app)
# QMAKE_POST_LINK += $$QMAKE_COPY -R $$OUTPUT_FILE "."
# } else {
# unix:OUTPUT_FILE = $$quote($$DESTDIR/$$TARGET)
# unix:QMAKE_POST_LINK += $$QMAKE_COPY $$OUTPUT_FILE "."
# }
