QT += widgets

HEADERS       = QLink.h \
    Config.h \
    Element.h
SOURCES       = QLink.cpp \
                Element.cpp \
                config.cpp \
                main.cpp

QMAKE_PROJECT_NAME = widgets_QLink

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/widgets/QLink
INSTALLS += target
