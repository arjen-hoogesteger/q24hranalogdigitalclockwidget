QT += widgets

HEADERS       = \
    analog24hourclock.h
SOURCES       = \
                main.cpp \
    analog24hourclock.cpp

QMAKE_PROJECT_NAME = widgets_analogclock

CONFIG += c++11

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/analogclock
INSTALLS += target
