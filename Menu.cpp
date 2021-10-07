#include "Menu.h"
#include "Config.h"
#include "QLink.h"
#include "Setting.h"

#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <QTimerEvent>
#include <memory>

Menu::Menu(QWidget *parent):QWidget(parent),qlink(nullptr),setting(nullptr)
{
    setWindowTitle(tr("Menu"));
    // 设置 widget 大小
    //resize(Config::menuWidth, Config::menuHeight);
    setFixedSize(Config::menuWidth, Config::menuHeight);
    startButton.reset(new QPushButton("开始新游戏",this));
    startButton->resize(Config::menuButtonWidth,Config::menuButtonHeight);
    startButton->setFont(Config::menuButtonFont);
    startButton->move(Config::menuWidth/2 - Config::menuButtonWidth/2,Config::menuHeight/3);
    connect(&*startButton,&QPushButton::clicked,this, QOverload<>::of(&Menu::start));

    loadButton.reset(new QPushButton("载入游戏",this));
    loadButton->resize(Config::menuButtonWidth,Config::menuButtonHeight);
    loadButton->setFont(Config::menuButtonFont);
    loadButton->move(Config::menuWidth/2 - Config::menuButtonWidth/2,Config::menuHeight/3+100);
    connect(&*loadButton,&QPushButton::clicked,this, QOverload<>::of(&Menu::load));

    quitButton.reset(new QPushButton("退出游戏",this));
    quitButton->resize(Config::menuButtonWidth,Config::menuButtonHeight);
    quitButton->setFont(Config::menuButtonFont);
    quitButton->move(Config::menuWidth/2 - Config::menuButtonWidth/2,Config::menuHeight/3+200);
    connect(&*quitButton,&QPushButton::clicked,this, QOverload<>::of(&Menu::quit));
}
Menu::~Menu()
{
//    if(qlink){
//        delete qlink;
//    }
//    if(setting){
//        delete setting;
//    }
}

void Menu::start()
{
//    if(setting){//如果setting点叉的情况下不会有startTimer 就不会delete setting
//        delete setting;
//    }
    setting.reset(new Setting(nullptr,this));
    setting->show();

    //hide();

}
void Menu::load()
{
//    if(qlink){
//        delete qlink;
//    }
//    if(setting){
//        delete setting;
//    }
    //绝对路径转成相对路径后去掉后缀 只保留文件名(时间)
    QString filePath = QFileDialog::getOpenFileName(this,"选择存档截图",QString::fromStdString(Config::archiveImagePath),"Image Files(*.png)");
    int first = filePath.indexOf(QString::fromStdString(Config::archiveImagePath));
    filePath = filePath.mid(first+Config::archiveImagePath.length()).remove(".png");
    QFileInfo file(QString::fromStdString(Config::archiveFilePath) + filePath);
    QFileInfo file2(QString::fromStdString(Config::archiveFilePath) + filePath + ".conf");
    if(!file.exists() | !file2.exists()){
        QMessageBox::information(this, "错误", "未发现相应存档，请检查文件名");
        return;
    }

    qlink.reset(new QLink(nullptr,this,filePath.toStdString()));
    qlink->show();
    hide();
}
void Menu::quit()
{

    exit(0);
}

void Menu::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == Config::gameoverID){
        Config::gameoverID = -1;//这行必须要加!!否则会导致执行两次(可能是多线程的问题) 然后两次delete会报错
        show();
        qlink.reset();
//        delete qlink;
//        qlink = nullptr;//一定要赋值为nullptr 不然if(qlink)还是true
        return;
    }
    if(event->timerId() == Config::settingSubmitID){
        Config::settingSubmitID = -1;
//        delete setting;
//        setting = nullptr;
        setting.reset();
        qlink.reset(new QLink(nullptr,this));
        qlink->show();
        hide();
        return;
    }

}
