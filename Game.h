#ifndef GAME_H
#define GAME_H


#include <QPainter>
#include <list>
#include <set>
#include <memory>

#include "Element.h"

class Game final
{
    static const QColor linkLineColor;
    static const QColor hintColor;
public:
    // 屏幕宽高  方块个数宽高 游戏结束时间 玩家数
    Game(std::string path);
    Game();
    ~Game();

private:
    int dx[5] = {0,1,-1,0,0};
    int dy[5] = {0,0,0,1,-1};

    std::map<int,int>keyToDirection;
    std::map<int,int>keyToPlayerID;

    int xbegin,ybegin;//左上角方块的左上角
     //由方块的左上角定位到地图的左上角 (由于地图大小不是方块大小的整数倍 故可到达的左上角非0 0)
    //right 与 bottom 不可达到
    int xleft;
    int xright;
    int ytop;
    int ybottom;

    struct Point final{//用于判断空白位置 一个空白位置仅需要坐标 而不需要其他任何信息 如颜色、移动、绘制 故不继承Element
        int x;
        int y;
        Point(){};
        Point(int xx,int yy):x(xx),y(yy){}
        Point(const Element &e):x(e.x),y(e.y){}
        bool operator < (const Point &b)const{
            return x * 10000 + y < b.x * 10000 + b.y;
        }
    };
    struct Solution final{
        Block block1;//hint需要高亮 故不存code  set需要自定义比较 故不存地址
        Block block2;
        Solution(){};
        Solution(Block b1,Block b2):block1(b1),block2(b2){//必须用初始化列表 因为block没有默认构造函数
            if(block1.code > block2.code){
                std::swap(block1,block2);
            }
        }
        bool operator <(const Solution &b)const{
            return block1.code*10000+block2.code<b.block1.code*10000+b.block2.code;
        }
    };

    struct LinkLine final{
        //这里必须记录地址因为全局维护的block是地址 动画导致了block删除的延后性 动画完成后指针变量销毁 对应的内存地址自动释放
        std::pair<std::shared_ptr<Block>,std::shared_ptr<Block>> blocksEliminated;//连接的线段需要的点 包括两个目标点与0-2个拐点
        std::vector<Point> turnPoints;//0 1 2
        int count;//每帧-1 到0后停直动画
        LinkLine(std::shared_ptr<Block> block1,std::shared_ptr<Block> block2,std::vector<Point> tp,int c = 0):blocksEliminated(std::make_pair(block1,block2)),turnPoints(tp),count(c){}
    };

    struct Hint final{
        Solution solution;
        bool status;//每帧改变状态(画笔类型) 达到闪烁的效果
        int secondsRemain;
        Hint(){};
        Hint(Solution s,int t):solution(s),status(true),secondsRemain(t){}
    };
    //根据设定 全局只有一个hint 且可能存在也可能不存在
    //只有指针可用nullptr表示不存在 否则需要加构造函数并额外判断
    //使用unique_ptr自动释放内存 否则管理较麻烦

    Point getRandomSpace();
    std::shared_ptr<Block> findBlockByCode(int code)const;
    std::vector<Point> getTurnPoints(Block &block1,Block &block2);//为有解的两个block加入连接的动画(找拐点) 实际上这个过程和link一模一样 但是秉持车一个函数只实现一个功能的原则分开写


    std::set<int> extendOneBlock(int code)const;//将一个block四个方向空白的格子进行扩展 返回集合
    bool removeHint(const Block &a, const Block &b);//在消除两个block后 如果某一个被提示了 则删除这对提示 返回是否删除

    void removeTwoBlocks(std::shared_ptr<Block> b1,std::shared_ptr<Block> b2);//删除block的逻辑 延迟删除 故不在link后调用而是在linkLine到时后调用

    //开始道具对应的功能后 startTime 计时并得到对应的标记 通过timerEvent检测倒计时结束

protected:


    std::list<LinkLine> linkLines;//连接线段的集合

    int animationRemain[Prop::animationAmount];
    //不用timer的原因是 每个10s生成prop的逻辑是游戏本身的(不需要任何外部框架的辅助) 并且这样做便于记录游戏的状态
    int propCount;//每次statusupdate更新 如果到propFrequency createOneProp
    bool singlePlayer;

    int timeRemain;
    bool flashEnabled()const;
    bool hintDisplay()const;
    bool addTimeDisplay()const;

    //为了使得游戏的长宽可变 这里使用动态二维数组 这导致了多次new的内存分配不一定连续 保存时不可保存连续的区域
    bool **map;//true->有block false ->无block 便于算法的判断  map[column][row]  [code/100][code%100]
    /*记录了空白位置中心点的坐标 改程序中 玩家、方块、道具均为正方形且大小相同 故以空白位置为中心生成的新对象一定不会和其他对象重合 若需要修改三种对象的大小 则此逻辑需要修改
       玩家、道具、方块均占用位置且只能生成在空白位置
       文字出现的地方视为空白
*/



    std::vector<Player> players;// vector便于直接索引 且声明时不需要提前知道元素个数  (Player* = new Player 很难初始化)
    std::list<std::shared_ptr<Block>> blocks;//player选中block后需要记录 故存地址 使用智能指针管理内存
    std::list<Prop> props;

    std::unique_ptr<Hint> hint;//用指针是因为可以用nullptr表示空 否则需要加默认构造函数 及成员变量的默认构造函数 及判断为空的条件

    /*维护三个集合
     * 1 空白位置 用于生成玩家(初始) 道具
     * 2 玩家可触碰的方块(四周未被方块包围) 用于判断解的情况
     * 3 地图的所有解 用于判断有无解 及提示
     *
     * 地图状态的每次更新 基于随机道具的生成或者玩家消除一对方块 对大部分的内容还是没有影响的
     * 若每次更新或需要某集合时使用遍历的算法去判断 效率不高 故采用如下方法
     * findXXX函数是完整的遍历寻找算法 可用于初始化或随机打乱后使用
     * updateXXX是维护某集合的算法 根据游戏的逻辑进行较少的操作
     * 两种方法都是直接修改对应集合 无返回值
     *
     */
    std::set<Point> emptySpace;
    std::set<Block> blocksAccessible;//玩家可触碰的block 每次消除后更新
    std::set<Solution> solutions;//当前地图可行的所有解

    void createOneProp();
    void createBlocks();
    void createPlayers();


    //void statusUpdate();
    void initTimer();
    void initData();

    void findEmptySpace();//只在一开始调用 在任意时刻调用无效
    void addEmptySpace(Element const &e);//消除e 增加空白
    void addEmptySpace(int x,int y);//玩家移动的时候需要减一个加一个 如果存之前的player对象 开销有点大
    void removeEmptyspace(Element const &e);//增加e 减少空白

    void updateBlocksAccessible(const Block &block1,const Block &block2);//block1与block2连接后被消除 导致用户可触碰的block减少两个 增加0-6个
    void findBlocksAccessible();

    //由于2拐的存在 任意一个点的消除都可能导致两个均不与该点同行同列的点的可连接 因此无法根据某种简单的算法单步更新所有解 而是每次都需要遍历所有block
    void findSolutions();

    void getRandomHint();

    //有持续时间的道具 玩家碰到道具后调用propStart propDuration后调用propFinish
    void hintStart();
    void hintFinish();//hint.reset 特殊操作
    void shuffleStart();
    void flashStart();
    void flashFinish();//提前赋0
    void addTimeStart();

    void freezeStart(Player &player);//被debuff的玩家

    void dizzyStart(Player &player);


    bool flashTo(int x,int y);//用户点击的位置


    bool movePlayer(Player &player,int direction);//player.move(dx[direction],dy[direction])


    void collideWithBlock(Player &player,std::shared_ptr<Block> block);
    void collideWithProp(Prop &prop);
    void collideWithProp(Prop &prop,Player &player);


    bool link(Block const &block1, Block const &block2);//判断两个方块是否可以连接


public:
        bool isPaused;
        bool gameover;


        void updateStatus();
        void drawHint(QPainter &painter);
        void drawPlayers(QPainter &painter);
        void drawBlocks(QPainter &painter);
        void drawProps(QPainter &painter);
        void drawLinkLines(QPainter &painter);
        void drawTexts(QPainter &painter);

        void flash(int x,int y);//接受点击的坐标参数
        void move(int key);//接受按下的键盘参数


        //所有元素的存储放在以下两个函数中进行，使用write read 二进制存储 原因如下
        //1 使得存取完全在掌控之中 不会出现 存入两个int分别为1和2 读出一个int为12的情况
        //2 由于使用不定长容器存储 故容器大小也是未知量 大块的数据其实是作为一个整体的  而基本元素也就是write一行语句的事
        //3 二进制文件对于用户本地改动这个操作门槛较高 不然易于发生作弊的情况  如果需要可以扩展为加密保存

        //read需要事先准备好已经分配好内存的地址，不论是在堆上还是栈上申请都需要默认构造函数
        //write需要保证对象的内容无指针，且对象的长度(sizeof)固定 不然读的时候不好读
        //如果有不确定的变量使得read困难 可以在write的时候统一先write size flag等


        //save与load保存游戏相关变量 但很多数据的长度依赖Config中的值
        //若save与load中额外保存这些值 游戏中的逻辑也会用到Config 所以必须调用loadConfig
        //连续调用 会导致加载Block时调用的getImage依赖的Block::width 没有办法更新成Config::blockSize 故需要分开调用 并initData
        void save(std::string path);
        //考虑到QImage的内存存储方式是位置的 不存储QImage而是在读取后重新生成 故load后block的样式与本地image文件有关
        void load(std::string path);
        void pause();

};

#endif // GAME_H
