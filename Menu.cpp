#include "Menu.h"
#include "Config.h"
#include "QLink.h"

#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
Menu::Menu(QWidget *parent):QWidget(parent),qlink(nullptr)
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
    if(qlink){
        delete qlink;
    }
    qlink = new QLink(nullptr,this);
    qlink->show();
    hide();
    //connet(qlink,&QWidget::closeEvent,)
}
void Menu::load()
{
    if(qlink){
        delete qlink;
    }

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
    qlink = new QLink(nullptr,this,filePath.toStdString());
    qlink->show();
    hide();
}
void Menu::quit()
{

    exit(0);
}


