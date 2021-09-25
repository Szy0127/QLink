#include <memory>
#include "Element.h"
#include <iostream>
#include <QFile>
#include <fstream>
const int Element::stepx = 65;
const int Element::stepy = 65;
const int Block::width = Element::stepx;
const int Block::height = Element::stepy;
const int Block::penWidth = 4;
const int Block::typeAmount = 6;
const int Block::eliminateTag = 0b1000;
const QColor Block::BlockColor[Block::typeAmount]= {QColor(50,100,200),QColor(100,100,100),QColor(100,50,100),QColor(50,0,200),QColor(50,200,0),QColor(200,100,20)};
const QColor Player::playerColor[2]= {QColor(255,50,50),QColor(50,50,200)};
const int Player::width = Element::stepx;
const int Player::height = Element::stepy;

const QColor Prop::color = QColor(255,255,0);
const int Prop::width = Element::stepx;
const int Prop::height = Element::stepy;

const char Prop::character[7] = "AFHSFD";//与Prop::enum顺序一致

bool collide(const Element &a,const Element &b)
{
    return a.x == b.x && a.y == b.y;//游戏界面以正方形方块为一个单位 不会出现重合部分的情况
}
void Element::move(int dx,int dy)//-1 0 1
{
    x += dx * stepx;
    y += dy * stepy;
}
Block::Block(int ix,int iy,int t,int c):Element(ix,iy),type(t),status(0),code(c)
{
    QString path(QString::number(type)+".png");
    image.load(path);
    image = image.scaledToWidth(Block::width).scaledToHeight(Block::height);
}
void Block::draw(QPainter &painter)const
{
    QPen pen;
    int index = getPlayerIndex();//动画中的边框颜色和初始状态一样
    if(index > 0){
        pen.setColor(Player::playerColor[index-1]);
    }else{
        pen.setColor(QColor(0,0,0));
    }

    pen.setWidth(penWidth);
    pen.setStyle(Qt::SolidLine);
//    QBrush brush;
//    brush.setColor(BlockColor[type]);
//    brush.setStyle(Qt::SolidPattern);
//    painter.setPen(pen);
//    painter.setBrush(brush);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);
    painter.drawImage(x,y,image);
    painter.drawRect(x,y,width,height);//x y 是左上角


}

void Block::setEliminate(){
    status |= eliminateTag;
}
bool Block::isEliminated()const{
    return status & eliminateTag;
}

int Block::getPlayerIndex()const{
    return status &~ eliminateTag;
}
void Block::setUnchosen(){
    status = 0;
}
void Block::setChosen(int playerID)
{
    status = playerID;
}

Player::Player(int ix,int iy,QColor ic,int i):Element(ix,iy,ic),block(nullptr),score(0),dizzySecondsRemain(0),freezeSecondsRemain(0),id(i){}
void Player::draw(QPainter &painter)const
{
    if(isFreeze()){
        painter.setBrush(Qt::black);
    }else{
        painter.setBrush(color);
    }
    if(isDizzy()){
        QPen pen;
        pen.setColor(Prop::color);
        pen.setWidth(Block::penWidth);
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);
    }else{
        painter.setPen(Qt::NoPen);
    }
    painter.drawEllipse(x,y,width,height);
}
void Player::move(int dx, int dy)
{
    if(isFreeze()){
        return;
    }
    if(isDizzy()){
        x -= dx * stepx;
        y -= dy * stepy;
    }else{
        x += dx * stepx;
        y += dy * stepy;
    }

}
void Player::setBlock(std::shared_ptr<Block> b)
{
    block = b;
}
void Player::removeBlock()
{
    block = nullptr;
}
std::shared_ptr<Block> Player::getBlock()const
{
    return block;
}
void Player::addScore()
{
    score += 10;
}
int Player::getScore()const
{
    return score;
}
void Player::moveTo(int xx, int yy)
{
    x = xx;
    y = yy;
}
bool Player::isFreeze()const
{
    return freezeSecondsRemain;
}
bool Player::isDizzy()const
{
    return dizzySecondsRemain;
}
void Player::setFreeze(int t)
{
    freezeSecondsRemain = t;
}
void Player::setDizzy(int t)
{
    dizzySecondsRemain = t;
}
void Player::updateFreeze()
{
    if(!freezeSecondsRemain){
        return;
    }
    freezeSecondsRemain--;
}
void Player::updateDizzy()
{
    if(!dizzySecondsRemain){
        return;
    }
    dizzySecondsRemain--;
}
void Prop::draw(QPainter &painter)const
{
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawEllipse(x,y,width,height);
    int fontSize = 20;
    QFont font("宋体",fontSize,QFont::Bold,true);
    painter.setFont(font);
    painter.setPen(Qt::red);
    painter.drawText(x + width/2 - fontSize/2, y + height/2 + fontSize/2,QString(Prop::character[type]));
}
