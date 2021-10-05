#ifndef SETTING_H
#define SETTING_H

#include "QLink.h"
#include <QWidget>
#include <QRadioButton>
#include <QPushButton>
class Setting final: public QWidget
{
public:
    Setting(QWidget *parent = nullptr,QWidget *menu = nullptr);
    QWidget *menu;
    QRadioButton *radioButtonSingle;
    QRadioButton *radioButtonMulti;
    QPushButton *confirm;
    void submit();

};

#endif // SETTING_H
