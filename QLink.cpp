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

#define millisecond 1000


QLink::QLink(QWidget *parent,std::string configPath)
    : QWidget(parent),path("test")
{

    Config::load(configPath);
    // 设置窗口的标题
    setWindowTitle(tr("QLink"));
    // 设置 widget 大小
    resize(Config::width, Config::height);

    game = new Game;
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
    if(game->gameover || game->stop){
        return;
    }


    switch (event->key()){
    case Qt::Key_P:
        if(game->stop){
            game->stop = false;
            frameUpdateTimer->start(1000/Config::fps);
        }else{
            game->stop = true;
            frameUpdateTimer->stop();
        }
        return;
    case Qt::Key_V:
        game->save("test");
        return;
    case Qt::Key_B:
        game->load("test");
    }
    game->move(event->key());
}

