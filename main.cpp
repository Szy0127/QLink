#include <QApplication>

#include "QLink.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QLink qlink(nullptr);
    qlink.show();
    return app.exec();
}
