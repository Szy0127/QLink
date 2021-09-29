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


QLink::QLink(QWidget *parent,std::string gameFilePath)
    : QWidget(parent)
{

    // 设置窗口的标题
    setWindowTitle(tr("QLink"));
    // 设置 widget 大小
    resize(Config::width, Config::height);

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
        return;
    case Qt::Key_V:
        game->save("test1");
        return;
    case Qt::Key_B:
        game->load("test1");
    }
    game->move(event->key());
}

