#include <QApplication>

#include "QLink.h"
#include "Menu.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
//    QLink qlink(nullptr);
//    qlink.show();
    Menu menu;
    menu.show();
    return app.exec();
}
