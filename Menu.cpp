#include "Menu.h"
#include "Config.h"
#include "QLink.h"

#include <iostream>
Menu::Menu(QWidget *parent):QWidget(parent)
{
    setWindowTitle(tr("QLinkMenu"));
    // 设置 widget 大小
    resize(Config::menuWidth, Config::menuHeight);

    startButton = new QPushButton("开始新游戏",this);
    startButton->resize(Config::menuButtonWidth,Config::menuButtonHeight);
    startButton->setFont(Config::menuButtonFont);
    startButton->move(Config::menuWidth/2 - Config::menuButtonWidth/2,Config::menuHeight/3);
    connect(startButton,&QPushButton::clicked,this, QOverload<>::of(&Menu::start));

    loadButton = new QPushButton("载入游戏",this);
    loadButton->resize(Config::menuButtonWidth,Config::menuButtonHeight);
    loadButton->setFont(Config::menuButtonFont);
    loadButton->move(Config::menuWidth/2 - Config::menuButtonWidth/2,Config::menuHeight/3+100);
    connect(loadButton,&QPushButton::clicked,this, QOverload<>::of(&Menu::load));

    quitButton = new QPushButton("退出游戏",this);
    quitButton->resize(Config::menuButtonWidth,Config::menuButtonHeight);
    quitButton->setFont(Config::menuButtonFont);
    quitButton->move(Config::menuWidth/2 - Config::menuButtonWidth/2,Config::menuHeight/3+200);
    connect(quitButton,&QPushButton::clicked,this, QOverload<>::of(&Menu::quit));
}
Menu::~Menu()
{
    delete startButton;
    delete loadButton;
    delete quitButton;
    if(qlink){
        delete qlink;
    }
}

void Menu::start()
{
    qlink = new QLink(nullptr);
    qlink->show();
    hide();
}
void Menu::load()
{
    qlink = new QLink(nullptr,"test1");
    qlink->show();
    hide();
}
void Menu::quit()
{

    exit(0);
}

