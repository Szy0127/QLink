#pragma once
#include <QWidget>
#include <QPainter>
#include <list>
#include <set>
#include <memory>

#include "Element.h"
#include "Game.h"

//! [0]
class QLink final : public QWidget
{
    Q_OBJECT
//    static const QColor linkLineColor;
//    static const QColor hintColor;

public:
    // 屏幕宽高  方块个数宽高 游戏结束时间 玩家数
    QLink(QWidget *parent = nullptr,QWidget *menu = nullptr,std::string gameFilePath = "");
    ~QLink();


protected:
    QWidget *menu;//便于回到菜单 这里使用startTimer(qlink) + timerEvent(menu)的方法
    std::unique_ptr<Game> game;//若不设置为指针 game必须在QLink的构造函数的初始化列表中进行初始化 否则会自动进行 导致无法进行Config.load操作

    QTimer *frameUpdateTimer;
    QTimer *statusUpdateTimer;

    bool questioned;//游戏结束时只询问一次是否回到主菜单
    void initTimer();
    void updateStatus();

    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
    void save();
    void gameover();

};
//! [0]
