#include "Setting.h"
#include "Config.h"

#include <QRadioButton>
#include <QPushButton>
#include <QWidget>
#include <QMessageBox>
#include <QSpinBox>
#include <QLabel>
#include <memory>
#include <iostream>
Setting::Setting(QWidget *parent,QWidget *menu):QWidget(parent),menu(menu)
{
    Config::load(Config::configPath);
    setWindowTitle(tr("Setting"));
    // 设置 widget 大小
    setFixedSize(Config::settingWidth, Config::settingHeight);

    int xbegin = 80;
    int y = 50;
    setLabel(labelMode,"游戏模式",xbegin,y);
    setLabel(labelmn,"方块数量",xbegin,y+=50);
    setLabel(labelmn,"方块大小",xbegin,y+=50);
    setLabel(labelTime,"时间限制",xbegin,y+=50);
    setLabel(labelProp,"道具出现频率",xbegin,y+=50);
    setLabel(labelHint,"提示持续时间",xbegin,y+=50);
    setLabel(labelAdd,"加时时间",xbegin,y+=50);
    setLabel(labelFlash,"闪现持续时间",xbegin,y+=50);
    setLabel(labelFreeze,"眩晕持续时间",xbegin,y);
    setLabel(labelDizzy,"颠倒持续时间",xbegin,y+=50);
    labelFreeze->hide();
    labelDizzy->hide();

    radioButtonSingle.reset(new QRadioButton(this));
    radioButtonSingle->setChecked(true);
    radioButtonSingle->setText("单人");
    radioButtonSingle->move(300,50);
    radioButtonMulti.reset(new QRadioButton(this));
    radioButtonMulti->setText("双人");
    radioButtonMulti->move(400,50);


    xbegin = 300;
    y = 100;
    setSpinBox(spinBoxn,6,20,1,Config::numberOfBlocksRow,xbegin,y);
    setSpinBox(spinBoxm,6,14,1,Config::numberOfBlocksColumn,xbegin+100,y);
    setSpinBox(spinBoxBlock,40,80,5,Config::blockSize,xbegin,y+=50);
    spinBoxn->setMaximum(Config::maxWidth/spinBoxBlock->value()-2);
    spinBoxm->setMaximum(Config::maxHeight/spinBoxBlock->value()-2);
    setSpinBox(spinBoxTime,10,300,20,Config::timeLimitation,xbegin,y+=50);
    setSpinBox(spinBoxProp,1,30,5,Config::propFrequency,xbegin,y+=50);
    setSpinBox(spinBoxHint,1,20,5,Config::hintDuration,xbegin,y+=50);
    setSpinBox(spinBoxAdd,10,60,10,Config::addTime,xbegin,y+=50);
    setSpinBox(spinBoxFlash,1,20,2,Config::flashDuration,xbegin,y+=50);
    setSpinBox(spinBoxFreeze,3,20,3,Config::freezeDuration,xbegin,y);
    setSpinBox(spinBoxDizzy,1,10,2,Config::dizzyDuration,xbegin,y+=50);
    spinBoxFreeze->hide();
    spinBoxDizzy->hide();


    confirm.reset(new QPushButton(this));
    confirm->setText("开始游戏");
    confirm->setFont(Config::settingLabelFont);
    confirm->move(200,500);

    connect(&*radioButtonSingle,&QRadioButton::clicked,this,QOverload<>::of(&Setting::checkSingle));
    connect(&*radioButtonMulti,&QRadioButton::clicked,this,QOverload<>::of(&Setting::checkMulti));
    connect(&*confirm,&QPushButton::clicked,this,QOverload<>::of(&Setting::submit));
    //valueChanged 不是信号？好坑
    connect(&*spinBoxBlock,&QSpinBox::textChanged,this,QOverload<>::of(&Setting::changeMaxMN));


}

void Setting::submit()
{
    if(spinBoxn->value() * spinBoxm->value() % 2){
        QMessageBox::warning(this,"错误!","奇数个方块会导致无解，请重新设置!");
        return;
    }
    if(radioButtonSingle->isChecked()){
        Config::playerNumber = 1;
        Config::flashDuration = spinBoxFlash->value();
    }else{
        Config::playerNumber = 2;
        Config::freezeDuration = spinBoxFreeze->value();
        Config::dizzyDuration = spinBoxDizzy->value();
    }

    Config::numberOfBlocksRow = spinBoxn->value();
    Config::numberOfBlocksColumn = spinBoxm->value();
    Config::blockSize = spinBoxBlock->value();
    Config::timeLimitation = spinBoxTime->value();
    Config::propFrequency = spinBoxProp->value();
    Config::hintDuration = spinBoxHint->value();
    Config::addTime = spinBoxAdd->value();

    //如果有空的话 上下可留2排 左右可留3排
    Config::width = Config::blockSize * (Config::numberOfBlocksRow + 6);
    while(Config::width > Config::maxWidth){
        Config::width -= 2 * Config::blockSize;
    }
    Config::height = Config::blockSize * (Config::numberOfBlocksColumn + 4);
    if(Config::height > Config::maxHeight){
        Config::height -= 2 * Config::blockSize;
    }

    Config::save(Config::configPath);
    Config::settingSubmitID = menu->startTimer(0);
}

void Setting::setSpinBox(std::unique_ptr<QSpinBox> &box, int vmin, int vmax, int step, int vdefault, int x, int y)
{
    box.reset(new QSpinBox(this));
    box->setMinimum(vmin);
    box->setMaximum(vmax);
    box->setSingleStep(step);
    box->setValue(vdefault);
    box->move(x,y);
}
void Setting::setLabel(std::unique_ptr<QLabel> &label, QString text, int x, int y)
{
    label.reset(new QLabel(this));
    label->setText(text);
    label->move(x,y);
    label->setFont(Config::settingLabelFont);
}
void Setting::checkSingle()
{
    spinBoxFlash->show();
    labelFlash->show();
    labelFreeze->hide();
    labelDizzy->hide();
    spinBoxFreeze->hide();
    spinBoxDizzy->hide();
}
void Setting::checkMulti()
{
    spinBoxFlash->hide();
    labelFlash->hide();
    labelFreeze->show();
    labelDizzy->show();
    spinBoxFreeze->show();
    spinBoxDizzy->show();
}
void Setting::changeMaxMN()
{
    //至少四周各留一排给人走 这样不仅限制了ui 更保证了game中的逻辑不会报错
    //(n+2)*size <= width
    //(m+2)*size <= height
    //如果修改的时候spinBoxn->value > max 会自动调整
    spinBoxn->setMaximum(Config::maxWidth/spinBoxBlock->value()-2);
    spinBoxm->setMaximum(Config::maxHeight/spinBoxBlock->value()-2);
}
