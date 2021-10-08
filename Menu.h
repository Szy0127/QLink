#ifndef MENU_H
#define MENU_H

#include "QLink.h"

#include "Setting.h"
#include <QWidget>
#include <QPushButton>
#include <QPushButton>
#include <memory>
class Menu : public QWidget
{

private:
    std::unique_ptr<QPushButton> startButton;
    std::unique_ptr<QPushButton> loadButton;
    std::unique_ptr<QPushButton> quitButton;

public:
    Menu(QWidget *parent = nullptr);
    ~Menu();

    void timerEvent(QTimerEvent *event)override;
    void keyPressEvent(QKeyEvent *event) override;
    std::unique_ptr<QLink>  qlink;
    std::unique_ptr<Setting> setting;//如果不设置为成员 在回调函数调用后 栈上分配的setting会被释放 导致show后立刻quit

    void start();
    void load();
    void quit();

};

#endif // MENU_H
