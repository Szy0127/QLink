#pragma once
#include <QWidget>
#include <QPainter>
#include <list>
#include <set>
#include <memory>

#include "Element.h"
#include "Game.h"
//! [0]
class QLink : public QWidget
{
    Q_OBJECT
//    static const QColor linkLineColor;
//    static const QColor hintColor;

public:
    // 屏幕宽高  方块个数宽高 游戏结束时间 玩家数
    QLink(QWidget *parent = nullptr,std::string configPath = "./config.config");
    ~QLink();


protected:

    std::string path;
    Game *game;//若不设置为指针 game必须在QLink的构造函数的初始化列表中进行初始化 否则会自动进行 导致无法进行Config.load操作

    QTimer *frameUpdateTimer;
    QTimer *statusUpdateTimer;

    void initTimer();
    void updateStatus();

    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

};
//! [0]
