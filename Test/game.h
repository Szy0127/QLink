#ifndef GAME_H
#define GAME_H
#include <QPainter>
#include <set>
class Element
{
    friend bool collide(const Element &a,const Element &b);
    public:
    Element(){};
    Element(int ix,int iy,QColor ic = QColor(0,0,0)):x(ix),y(iy),color(ic){}// x y 为具体坐标
    int x;//由于Qt 的paint函数是根据左上角坐标绘制 所以这里存储的xy也是左上角坐标
    int y;
    QColor color;
public:
    virtual void move(int dx,int dy);//-1 0 1


};
class Block final:public Element
{

private:
    int status;

public:
    int code;
    int type;//0~typeAmount-1

    Block(){};

    Block(int ix,int iy,int t,int c);
    Block(const Block &r);
    bool operator <(const Block &b)const{ // set
        return code < b.code;
    }
    bool operator ==(const Block &b)const{
        return code == b.code;
    }
    bool isNextTo(const Block &b)const{
        return std::abs(code - b.code)==1||std::abs(code - b.code)==100;
    }


};

class Game
{
    enum{n = 8};
    enum{m = 6};
public:
    Game();
    std::set<int> extendOneBlock(int code)const;
    bool link(Block const &block1, Block const &block2);
    int map[n+2][m+2];
    Block blocks[n][m];
};

#endif // GAME_H
