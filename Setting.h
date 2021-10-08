#ifndef SETTING_H
#define SETTING_H

#include "QLink.h"
#include <QWidget>
#include <QRadioButton>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <memory>
class Setting final: public QWidget
{
public:
    Setting(QWidget *parent = nullptr,QWidget *menu = nullptr);
    void keyPressEvent(QKeyEvent *event) override;
    //menu不能用智能指针 因为setting销毁后menu一直存在 没有必要也不能销毁
    QWidget *menu;

    std::unique_ptr<QLabel> labelMode;
    std::unique_ptr<QLabel> labelmn;
    std::unique_ptr<QLabel> labelBlock;
    std::unique_ptr<QLabel> labelTime;
    std::unique_ptr<QLabel> labelProp;
    std::unique_ptr<QLabel> labelHint;
    std::unique_ptr<QLabel> labelAdd;
    std::unique_ptr<QLabel> labelFlash;
    std::unique_ptr<QLabel> labelFreeze;
    std::unique_ptr<QLabel> labelDizzy;
    std::unique_ptr<QRadioButton> radioButtonSingle;
    std::unique_ptr<QRadioButton> radioButtonMulti;

    std::unique_ptr<QSpinBox> spinBoxn;
    std::unique_ptr<QSpinBox> spinBoxm;
    std::unique_ptr<QSpinBox> spinBoxBlock;
    std::unique_ptr<QSpinBox> spinBoxTime;
    std::unique_ptr<QSpinBox> spinBoxProp;
    std::unique_ptr<QSpinBox> spinBoxHint;
    std::unique_ptr<QSpinBox> spinBoxAdd;
    std::unique_ptr<QSpinBox> spinBoxFlash;
    std::unique_ptr<QSpinBox> spinBoxFreeze;
    std::unique_ptr<QSpinBox> spinBoxDizzy;

    std::unique_ptr<QPushButton> confirm;


    void submit();
    void setSpinBox(std::unique_ptr<QSpinBox> &box,int vmin, int vmax,int step,int vdefault,int x,int y);
    void setLabel(std::unique_ptr<QLabel> &label,QString text,int x,int y);
    void checkSingle();
    void checkMulti();
    void changeMaxMN();
};

#endif // SETTING_H
