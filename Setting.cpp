#include "Setting.h"
#include "Config.h"

#include <QRadioButton>
#include <QPushButton>
#include <QWidget>
Setting::Setting(QWidget *parent,QWidget *menu):QWidget(parent),menu(menu)
{
    setWindowTitle(tr("Setting"));
    // 设置 widget 大小
    resize(Config::settingWidth, Config::settingHeight);

    radioButtonSingle = new QRadioButton(this);
    radioButtonSingle->setChecked(true);
    radioButtonSingle->setText("单人");
    radioButtonSingle->move(100,100);
    radioButtonMulti = new QRadioButton(this);
    radioButtonMulti->setText("双人");
    radioButtonMulti->move(100,200);

    confirm = new QPushButton(this);
    confirm->setText("开始游戏");
    confirm->move(300,300);

    connect(confirm,&QPushButton::clicked,this,QOverload<>::of(&Setting::submit));
}

void Setting::submit()
{
    if(radioButtonSingle->isChecked()){
        Config::playerNumber = 1;
    }else{
        Config::playerNumber = 2;
    }
    Config::save(Config::configPath);
    Config::settingSubmitID = menu->startTimer(0);
}
