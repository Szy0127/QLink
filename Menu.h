#ifndef MENU_H
#define MENU_H

#include "QLink.h"
#include <QWidget>
#include <QPushButton>
class Menu : public QWidget
{
private:
    QPushButton *startButton;
    QPushButton *loadButton;
    QPushButton *quitButton;
public:
    Menu(QWidget *parent = nullptr);
    ~Menu();
    QLink * qlink;
    void start();
    void load();
    void quit();

};

#endif // MENU_H
