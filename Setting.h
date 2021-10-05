#ifndef SETTING_H
#define SETTING_H

#include "QLink.h"
#include <QWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
class Setting final: public QWidget
{
public:
    Setting(QWidget *parent = nullptr,QWidget *menu = nullptr);
    QWidget *menu;

    QLabel *labelMode;
    QLabel *labelmn;
    QLabel *labelBlock;
    QLabel *labelTime;
    QLabel *labelProp;
    QLabel *labelHint;
    QLabel *labelAdd;
    QLabel *labelFlash;
    QLabel *labelFreeze;
    QLabel *labelDizzy;
    QRadioButton *radioButtonSingle;
    QRadioButton *radioButtonMulti;

    QSpinBox *spinBoxn;
    QSpinBox *spinBoxm;
    QSpinBox *spinBoxBlock;
    QSpinBox *spinBoxTime;
    QSpinBox *spinBoxProp;
    QSpinBox *spinBoxHint;
    QSpinBox *spinBoxAdd;
    QSpinBox *spinBoxFlash;
    QSpinBox *spinBoxFreeze;
    QSpinBox *spinBoxDizzy;

    QPushButton *confirm;


    void submit();
    void setSpinBox(QSpinBox *&box,int vmin, int vmax,int step,int vdefault,int x,int y);
    void setLabel(QLabel *&label,QString text,int x,int y);
    void checkSingle();
    void checkMulti();
};

#endif // SETTING_H
