#ifndef MENU_H
#define MENU_H

#include "QLink.h"

#include "Setting.h"
#include <QWidget>
#include <QPushButton>
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

    void timerEvent(QTimerEvent *event)override;
    QLink * qlink;
    Setting *setting;//如果不设置为成员 在回调函数调用后 栈上分配的setting会被释放 导致show后立刻quit

    void start();
    void load();
    void quit();

};

#endif // MENU_H
