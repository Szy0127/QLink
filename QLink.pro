QT += widgets

HEADERS       = QLink.h \
    Config.h \
    Element.h \
    Game.h \
    Menu.h \
    Setting.h
SOURCES       = QLink.cpp \
                Element.cpp \
                Game.cpp \
                Menu.cpp \
                Setting.cpp \
                config.cpp \
                main.cpp

QMAKE_PROJECT_NAME = widgets_QLink

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/QLink
INSTALLS += target
