#ifndef ELEMENT_H
#define ELEMENT_H

#include <QPainter>
#include <list>
#include <set>
#include <memory>

class Element
{
    friend bool collide(const Element &a,const Element &b);
    public:
    static int stepx;
    static int stepy;
    Element(){};
    ~Element();
    Element(int ix,int iy,QColor ic = QColor(0,0,0)):x(ix),y(iy),color(ic){}// x y 为具体坐标
    int x;//由于Qt 的paint函数是根据左上角坐标绘制 所以这里存储的xy也是左上角坐标
    int y;
    QColor color;
public:
    virtual void move(int dx,int dy);//-1 0 1

    virtual void draw(QPainter &painter)const=0 ;
//    virtual int getWidth()const=0;
//    virtual int getHeight()const=0;

};
//enum type{ADDTIME,SHUFFLE,HINT,FLASH,FREEZE,DIZZY}
class Prop final:public Element
{

public:
    enum{typeAmount = 6};
    static int width;
    static int height;
    static const QColor color;


    static std::string typeName[typeAmount];

    enum{singlePlayerAmount = 4};
    enum{multiPlayerAmount = 5};

    //static const int 虽是常量 但本质是赋值后的变量 并非字面量  switch case的比较需要字面量 用enum
    //freeze dizzy 在双人模式下出现 作用于某一player一段时间 1和2分别对应0b01 0b10 可组合两个状态
    enum{ADDTIME,FLASH,HINT,SHUFFLE,FREEZE,DIZZY};//把需要动画的放在前面
    enum{animationAmount = 2};//记录动画时间的数组 在编译时确定大小 需要字面量 static const虽然是常量但还是变量

private:
    //Prop(const Prop&);
    Prop operator=(const Prop&);

public:

    int type;
    QImage *image;
    Prop(){};
    ~Prop();
    Prop(const Prop&r);
    Prop(int ix,int iy,QColor ic,int t);
    virtual void draw(QPainter &painter)const override;
    bool operator ==(const Prop &b)const{
        return x == b.x && y == b.y && type == b.type;//实际上不会在同一个位置存在2个以上道具 不需要判断type
    }
    void getImage();
};

class Block final:public Element
{
    friend void swapBlocks(Block &block1,Block &block2);
public:
    static int width;
    static int height;
    static int imageSize;
    static const int penWidth;
    static const QColor BlockColor[6];
    static const int typeAmount;
    static const int eliminateTag;
    /* 0-初始 1-player1选中 2-player2 用于绘制不同颜色的边框
     当玩家成功连接两个block时 这两个block不能立即删除 需要完成动画 同时不能被其他玩家再选中
     故需要另一种status表示此状态 如-1
     而考虑到动画可能需要用到玩家的信息(不同玩家不同动画) 因此“已被消除”的status需要和“被某一玩家选中”的status结合组成一种状态
     默认玩家数量 <8 及用3bit可表示 将第四位设置为1表示已被消除 status |= 0b1000
     从已被消除的状态找到对应玩家 status &= ~0b1000
     */
    static void getImageSize();//用户可选择连续的BlockSize而图片size是离散的 30 40-80
    //如果在构造函数中写 读档的执行不了 如果在getImage写 没有必要 因为这个只需要计算一次
private:  
    Block& operator=(Block &r);//维护Block的数据结构有点多 而block涉及到image 需要管理指针 必须保证没有意外的赋值操作出现 player同理
    int status;

public:
    int code;
    int type;//0~typeAmount-1
    //如果不用指针 会导致使用二进制整体存储block的时候报错 如果用智能指针 会有问题 debug不出
    //如果在draw里根据type去loadimage 会导致效率很低 很卡 所以交换的时候需要同时交换image和type
    //std::shared_ptr<QImage> image; 用智能指针会有问题
    QImage *image;
    Block(){};
    ~Block();
    Block(int ix,int iy,int t,int c);
    Block(const Block &r);//solution会复制block
    virtual void draw(QPainter &painter)const override;
    bool operator <(const Block &b)const{ // set
        return code < b.code;
    }
    bool operator ==(const Block &b)const{
        return code == b.code;
    }
    bool isNextTo(const Block &b)const{
        return std::abs(code - b.code)==1||std::abs(code - b.code)==100;
    }

    void setEliminate();//位运算的操作封装起来使用会更加友好
    bool isEliminated()const;//但是如果指定为inline 要么将定义直接放在类内写 这样会自动转换为inline 要么在类外但是必须在同一文件中定义 两种都不符合接口与实现分离的理念

    int getPlayerIndex()const;//玩家1 2 3 4  对应players[0] [1] [2] [3]    0表示未被选中
    void setUnchosen();//消除后无法被选中 故能被选中的block一定状态是0 直接赋值即可
    void setChosen(int playerID);
    void getImage();

};

class Player final:public Element
{
public:
    static int width;
    static int height;
    static int imageSize;
    static const QColor playerColor[2];
    static void getImageSize();
private:
    Player operator=(Player&);
    std::shared_ptr<Block> block;
    int score;
    int dizzySecondsRemain;
    int freezeSecondsRemain;
public:
    int id;//1 2 3 4
    QImage *image;
    Player(){};
    ~Player();
    Player(int ix,int iy,QColor ic,int i);
    Player(const Player& r);
    virtual void draw(QPainter &painter)const override;
    void setBlock(std::shared_ptr<Block> b);
    void removeBlock();
    std::shared_ptr<Block> getBlock()const;
    void addScore();
    int getScore()const;
    void moveTo(int x,int y);
    void move(int dx,int dy)override;
    bool isFreeze()const;
    bool isDizzy()const;
    void updateFreeze();
    void updateDizzy();
    void setFreeze(int t);
    void setDizzy(int t);
    void getImage();
};


#endif // ELEMENT_H
