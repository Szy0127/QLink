TEMPLATE = app
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle
QT += testlib
SOURCES += \
        game.cpp \
        main.cpp \
        test.cpp

HEADERS += \
    game.h \
    test.h
