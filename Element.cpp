#include <memory>
#include "Element.h"
#include <Config.h>
#include <iostream>
#include <QFileInfo>
#include <fstream>
//实际上这里写的非常奇怪 用了很多的变量但值是一样的  希望以后可以做到实现不一样的
int Element::stepx = Config::blockSize;
int Element::stepy = Config::blockSize;
int Block::width = Element::stepx;
int Block::height = Element::stepy;
int Block::imageSize = 65;
const int Block::penWidth = 4;
const int Block::typeAmount = 6;
const int Block::eliminateTag = 0b1000;
const QColor Block::BlockColor[Block::typeAmount]= {QColor(50,100,200),QColor(100,100,100),QColor(100,50,100),QColor(50,0,200),QColor(50,200,0),QColor(200,100,20)};
const QColor Player::playerColor[2]= {QColor(0,80,150),QColor(255,50,50)};
int Player::width = Element::stepx;
int Player::height = Element::stepy;
int Player::imageSize = Block::imageSize;

const QColor Prop::color = QColor(255,255,0);
int Prop::width = Element::stepx;
int Prop::height = Element::stepy;

std::string Prop::typeName[Prop::typeAmount] = {"time","flash","hint","shuffle","freeze","dizzy"};

Element::~Element()
{
    delete image;
}

bool collide(const Element &a,const Element &b)
{
    return a.x == b.x && a.y == b.y;//游戏界面以正方形方块为一个单位 不会出现重合部分的情况
}
void Element::move(int dx,int dy)//-1 0 1
{
    x += dx * stepx;
    y += dy * stepy;
}
void Element::draw(QPainter &painter)const
{
    painter.drawImage(x,y,*image);//x y 是左上角
}
void swapBlocks(Block &block1,Block &block2){
    std::swap(block1.type,block2.type);
    std::swap(block1.image,block2.image);

}
Block::Block(int ix,int iy,int t,int c):Element(ix,iy),status(0),code(c),type(t)
{
    getImage();
}
Block::Block(const Block &r):Element(r.x,r.y)//只有solution会复制block image是没用的
{
    status = r.status;
    code = r.code;
    type = r.type;
}
void Block::getImage()
{

    QString path(QString::fromStdString(Config::imagePath)+QString::number(type)+"_"+QString::number(imageSize)+".png");
    QFileInfo file(path);
    if(!file.exists()){
        throw "方块图片缺失";
    }
    image = new QImage(path);
    //image.reset(new QImage(path));
    *image = image->scaledToWidth(width).scaledToHeight(height);
}
void Block::getImageSize()
{
    if(width <= 35){
        imageSize = 30;
        return;
    }
    if(35 < width && width <=45){
        imageSize = 40;
        return;
    }
    if(45 < width && width <=55){
        imageSize = 50;
        return;
    }
    if(55 < width && width <=65){
        imageSize = 60;
        return;
    }
    if(65 < width && width <=75){
        imageSize = 70;
        return;
    }
    imageSize = 80;

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
    painter.setBrush(Qt::NoBrush);
    painter.setPen(pen);
    Element::draw(painter);
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
Block::~Block()
{
    //已在基类delete
    //delete image;//solultion等维护的数据涉及到block的复制 如果复制构造函数将image指向的内容共享 此处delete会崩溃
}
Player::Player(int ix,int iy,int i):Element(ix,iy),block(nullptr),score(0),dizzySecondsRemain(0),freezeSecondsRemain(0),id(i)
{
    getImage();
}
Player::Player(const Player &r):Element(r.x,r.y),block(r.block),score(0),dizzySecondsRemain(0),freezeSecondsRemain(0),id(r.id)
{
    getImage();
}
void Player::getImage()
{
    QString path(QString::fromStdString(Config::imagePath)+"p"+QString::number(id)+"_"+QString::number(imageSize)+".png");
    QFileInfo file(path);
    if(!file.exists()){
        throw "人物图片缺失";
    }
    image = new QImage(path);
    //image.reset(new QImage(path));
    *image = image->scaledToWidth(width).scaledToHeight(height);
}
void Player::getImageSize()
{
    if(width <= 35){
        imageSize = 30;
        return;
    }
    if(35 < width && width <=45){
        imageSize = 40;
        return;
    }
    if(45 < width && width <=55){
        imageSize = 50;
        return;
    }
    if(55 < width && width <=65){
        imageSize = 60;
        return;
    }
    if(65 < width && width <=75){
        imageSize = 70;
        return;
    }
    imageSize = 80;

}
void Player::draw(QPainter &painter)const
{
    Element::draw(painter);
    painter.setBrush(Qt::NoBrush);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    if(isFreeze()){
        pen.setColor(Qt::black);
        pen.setWidth(Block::penWidth+6);
        painter.setPen(pen);
        painter.drawEllipse(x,y,width,height);
    }

    if(isDizzy()){
        pen.setColor(Prop::color);
        pen.setWidth(Block::penWidth+3);
        painter.setPen(pen);
        painter.drawEllipse(x,y,width,height);
    }
    pen.setColor(playerColor[id-1]);
    pen.setWidth(Block::penWidth);
    painter.setPen(pen);
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
Player::~Player()
{

}
void Prop::draw(QPainter &painter)const
{
    Element::draw(painter);
}
Prop::Prop(int ix,int iy,int t):Element(ix,iy),type(t)
{
    getImage();
}
void Prop::getImage()
{
    QString path(QString::fromStdString(Config::imagePath)+QString::fromStdString(typeName[type])+".png");
    QFileInfo file(path);
    if(!file.exists()){
        throw "道具图片缺失";
    }
    image = new QImage(path);
    //image.reset(new QImage(path));
    *image = image->scaledToWidth(width).scaledToHeight(height);
}
Prop::Prop(const Prop& r):Element(r.x,r.y),type(r.type)
{
    getImage();
}
Prop::~Prop()
{

}
