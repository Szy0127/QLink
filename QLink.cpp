#include "QLink.h"
#include "Element.h"
#include "Config.h"

#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QKeyEvent>
#include <iostream>
#include <set>
#include <cmath>
#include <algorithm>
#include <memory>
#include <fstream>
#include <QMessageBox>

#define millisecond 1000


QLink::QLink(QWidget *parent,QWidget *menu,std::string gameFilePath)
    : QWidget(parent),menu(menu),questioned(false)
{

    // 设置窗口的标题
    setWindowTitle(tr("QLink"));
    // 设置 widget 大小
    setFixedSize(Config::width, Config::height);

    if(gameFilePath.empty()){
        game = new Game;
    }else{
        game = new Game(gameFilePath);
    }

    initTimer();

}
QLink::~QLink()
{
    delete frameUpdateTimer;
    delete statusUpdateTimer;
    delete game;
}

void QLink::updateStatus()
{
    game->updateStatus();
    if(game->gameover){
        gameover();
    }
}
void QLink::initTimer()
{
    frameUpdateTimer = new QTimer(this);
    connect(frameUpdateTimer, &QTimer::timeout, this, QOverload<>::of(&QLink::update));
    frameUpdateTimer->start(millisecond/Config::fps);


    statusUpdateTimer = new QTimer(this);
    connect(statusUpdateTimer, &QTimer::timeout ,this , QOverload<>::of(&QLink::updateStatus));
    statusUpdateTimer->start(millisecond);

}


void QLink::paintEvent(QPaintEvent *)
{

    // 基于该 widget 构建一个 painter，准备开始画图
    QPainter painter(this);
    // 设置 painter 的抗锯齿，让画出来的线更平滑
    painter.setRenderHint(QPainter::Antialiasing);
    // 设置 painter 的坐标系装换
    //painter.translate(width() / 2, height() / 2);

    game->drawBlocks(painter);
    game->drawPlayers(painter);

    game->drawLinkLines(painter);

    game->drawHint(painter);
    game->drawProps(painter);
    game->drawTexts(painter);//文字不会完全覆盖 故最后画
}



void QLink::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        int x = event->x();
        int y = event->y();
        game->flash(x,y);
    }

}
void QLink::keyPressEvent(QKeyEvent *event)
{
    if(game->gameover){
        return;
    }

    switch (event->key()){
    case Qt::Key_P:
        game->pause();
        if(game->isPaused){
            QMessageBox messageBox(QMessageBox::NoIcon,"暂停", "是否存档？",QMessageBox::Yes | QMessageBox::No, this);
            int result = messageBox.exec();
            if(result == QMessageBox::Yes){
                save();
            }
        }
        return;
    }
    game->move(event->key());
}
void QLink::save()
{
    QPixmap p = grab(QRect(0,0,Config::width,Config::height));
    QString filePathName(QString::fromStdString(Config::archiveImagePath));
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh-mm-ss-zzz");
    filePathName += time;
    filePathName += ".png";
    if(p.save(filePathName,"png")){
        game->save(time.toStdString());
        QMessageBox::information(this, "提示", "保存成功！");
    }else{
        QMessageBox::information(this, "提示", "保存失败！请检查存档目录是否被修改！");
    }
}
void QLink::gameover()
{
    if(questioned){
        return;
    }
    questioned = true;
    QMessageBox messageBox(QMessageBox::NoIcon,"游戏结束", "游戏结束，是否回到菜单？",QMessageBox::Yes | QMessageBox::No, this);
    int result = messageBox.exec();
    if(result == QMessageBox::Yes){
//        if(menu){
//            menu->show();
//        }
//        hide();//如果这里用exit menu会闪一下然后一起退出 因为使用了hide 所以需要在menu里delete
        //close();
        Config::gameoverID = menu->startTimer(0);
    }
}

void QLink::closeEvent(QCloseEvent *event)
{
    event->ignore();
    QMessageBox messageBox(QMessageBox::NoIcon,"退出", "是否直接退出？(存档请在游戏界面按P)",QMessageBox::Yes | QMessageBox::No, this);
    int result = messageBox.exec();
    if(result == QMessageBox::Yes){
        Config::gameoverID = menu->startTimer(0);
        //event->accept() = exit(0) 会连主程序一起退出 所以这里回到主菜单
    }
}
