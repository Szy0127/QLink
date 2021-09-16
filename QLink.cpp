#include "QLink.h"
#include "Element.h"
#include "Config.h"

#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QKeyEvent>
#include <iostream>
#include <set>
#include <cmath>
#include <algorithm>
#include <memory>
#include <fstream>

#define millisecond 1000


const QColor QLink::linkLineColor = QColor(255,0,0);
const QColor QLink::hintColor = QColor(255,255,0);

QLink::QLink(QWidget *parent,std::string configPath)
    : QWidget(parent),hint(nullptr),path("test"),animationRemain{}
{
    gameover = false;
    stop = false;
    Config::load(configPath);
    // 设置窗口的标题
    setWindowTitle(tr("QLink"));
    // 设置 widget 大小
    resize(Config::width, Config::height);

    initTimer();
    initData();
    singlePlayer = Config::playerNumber == 1;
    createBlocks();
    findSolutions();
    findEmptySpace();
    createPlayers();
}
QLink::~QLink()
{
    delete frameUpdateTimer;
    delete propCreateTimer;
}

void QLink::initData()
{
    keyToDirection[Qt::Key_Up] = 4;
    keyToDirection[Qt::Key_Down] = 3;
    keyToDirection[Qt::Key_Left] = 2;
    keyToDirection[Qt::Key_Right] = 1;
    keyToDirection[Qt::Key_W] = 4;
    keyToDirection[Qt::Key_A] = 2;
    keyToDirection[Qt::Key_S] = 3;
    keyToDirection[Qt::Key_D] = 1;
    keyToPlayerID[Qt::Key_Up] = 1;
    keyToPlayerID[Qt::Key_Down] = 1;
    keyToPlayerID[Qt::Key_Left] = 1;
    keyToPlayerID[Qt::Key_Right] = 1;
    keyToPlayerID[Qt::Key_W] = 2;
    keyToPlayerID[Qt::Key_A] = 2;
    keyToPlayerID[Qt::Key_S] = 2;
    keyToPlayerID[Qt::Key_D] = 2;

    xbegin = (Config::width - Config::numberOfBlocksRow * Block::width)/2;
    ybegin = (Config::height - Config::numberOfBlocksColumn * Block::height)/2;

    xleft = xright = xbegin ;
    ytop = ybottom = ybegin ;
    while(xleft - Element::stepx > 0){
        xleft -= Element::stepx;
    }

    while(xright + Element::stepx < Config::width){
        xright += Element::stepx;
    }

    while(ytop - Element::stepy > 0){
        ytop -= Element::stepy;
    }
    while(ybottom + Element::stepy < Config::height){
        ybottom += Element::stepy;
    }
}
void QLink::initTimer()
{
    timeRemain = Config::timeLimitation ;
    frameUpdateTimer = new QTimer(this);
    connect(frameUpdateTimer, &QTimer::timeout, this, QOverload<>::of(&QLink::update));
    frameUpdateTimer->start(millisecond/Config::fps);

    propCreateTimer = new QTimer(this);
    connect(propCreateTimer , &QTimer::timeout, this, QOverload<>::of(&QLink::createOneProp));
    propCreateTimer->start(Config::propFrequency*millisecond);

    statusUpdateTimer = new QTimer(this);
    connect(statusUpdateTimer, &QTimer::timeout ,this , QOverload<>::of(&QLink::statusUpdate));
    statusUpdateTimer->start(millisecond);

}

void QLink::statusUpdate()
{
    if(!singlePlayer){
        for(auto &player:players){
            player.updateDizzy();
            player.updateFreeze();
        }
    }

    for(int i = 0 ;i < Prop::animationAmount ; i++){
        if(animationRemain[i]){
            animationRemain[i]--;
        }
    }
    if(hint){
        hint->secondsRemain--;
        if(!hint->secondsRemain){
            hintFinish();
        }
    }
    timeRemain--;
    if(!timeRemain){
        gameover = true;
    }
    if(solutions.empty()){//判断暂时放在这里
        gameover = true;
    }
}
void QLink::paintEvent(QPaintEvent *)
{

    // 基于该 widget 构建一个 painter，准备开始画图
    QPainter painter(this);
    // 设置 painter 的抗锯齿，让画出来的线更平滑
    painter.setRenderHint(QPainter::Antialiasing);
    // 设置 painter 的坐标系装换
    //painter.translate(width() / 2, height() / 2);

    for(const auto &block:blocks){
        if(block->getPlayerIndex() == 0){ // 先画没被玩家选中的 这样被玩家选中的方块的边框可以覆盖没有被选中的
            block->draw(painter);
        }
    }
    for(const auto &block:blocks){
        if(block->getPlayerIndex() > 0){
             block->draw(painter);
        }
    }
    for(const auto &player:players){
        player.draw(painter);
    }

    drawLinkLines(painter);

    drawHint(painter);
    for(const auto &prop:props){
        prop.draw(painter);
    }
    drawTexts(painter);//文字不会完全覆盖 故最后画
}
void QLink::drawHint(QPainter &painter)
{
    if(!hint){
        return;
    }

    QPen pen;
    pen.setColor(hintColor);

    pen.setWidth(Block::penWidth);

    painter.setBrush(Qt::NoBrush);

    if(hint->status){
        pen.setStyle(Qt::DotLine);
    }else{
        pen.setStyle(Qt::SolidLine);
    }
    painter.setPen(pen);
    painter.drawRect(hint->solution.block1.x,hint->solution.block1.y,Block::width,Block::height);
    painter.drawRect(hint->solution.block2.x,hint->solution.block2.y,Block::width,Block::height);

    hint->status = !hint->status;

}
void QLink::drawTexts(QPainter &painter)
{
    QFont font("宋体",15,QFont::Bold,true);
    painter.setFont(font);
    painter.setPen(Qt::blue);
    painter.drawText(10,30,QString("时间:"));
    if(addTimeDisplay()){
        painter.setPen(Qt::red);
    }
    painter.drawText(100,30,QString::number(timeRemain));
    painter.setPen(Qt::blue);

    if(singlePlayer){
        painter.drawText(10,70,QString("分数:"));
        painter.drawText(100,70,QString::number(players[0].getScore()));
    }else{
        for(int i = 0 ; i < Config::playerNumber ; i++){
            painter.drawText(10,30+40*(i+1),QString("玩家").append(QString::number(i+1)).append(QString("分数：")));
            painter.drawText(150,30+40*(i+1),QString::number(players[i].getScore()));
        }
    }
    painter.drawText(100,90,QString::number(solutions.size()));


    if(gameover){
        painter.setPen(Qt::red);
        painter.drawText(500,500,QString("GameOver"));
        frameUpdateTimer->stop();
        propCreateTimer->stop();
    }
}
void QLink::drawLinkLines(QPainter &painter)//绘制完成后直接在此函数中删除对应的block
{
    if(linkLines.empty()){
        return;
    }
    QPen pen;

    pen.setWidth(5);
    painter.setBrush(Qt::NoBrush);
    auto lineIterator = linkLines.begin();//一组待消的砖块
    while(lineIterator!=linkLines.end()){


        int playerIndex = lineIterator->blocksEliminated.first->getPlayerIndex();
        pen.setColor(Player::playerColor[playerIndex-1]);
        painter.setPen(pen);

        if(lineIterator->turnPoints.size() == 0){
            painter.drawLine(lineIterator->blocksEliminated.first->x + Block::width/2 , lineIterator->blocksEliminated.first->y + Block::height/2,lineIterator->blocksEliminated.second->x + Block::width/2 , lineIterator->blocksEliminated.second->y + Block::height/2);
        }else if(lineIterator->turnPoints.size() == 1){
            painter.drawLine(lineIterator->blocksEliminated.first->x + Block::width/2 , lineIterator->blocksEliminated.first->y + Block::height/2,lineIterator->turnPoints[0].x + Block::width/2 , lineIterator->turnPoints[0].y + Block::height/2);
            painter.drawLine(lineIterator->turnPoints[0].x + Block::width/2 , lineIterator->turnPoints[0].y + Block::height/2,lineIterator->blocksEliminated.second->x + Block::width/2 , lineIterator->blocksEliminated.second->y + Block::height/2);
        }else{//=2
            painter.drawLine(lineIterator->blocksEliminated.first->x + Block::width/2 , lineIterator->blocksEliminated.first->y + Block::height/2,lineIterator->turnPoints[0].x + Block::width/2 , lineIterator->turnPoints[0].y + Block::height/2);
            painter.drawLine(lineIterator->turnPoints[0].x + Block::width/2 , lineIterator->turnPoints[0].y + Block::height/2,lineIterator->turnPoints[1].x + Block::width/2 , lineIterator->turnPoints[1].y + Block::height/2);
            painter.drawLine(lineIterator->turnPoints[1].x + Block::width/2 , lineIterator->turnPoints[1].y + Block::height/2,lineIterator->blocksEliminated.second->x + Block::width/2 , lineIterator->blocksEliminated.second->y + Block::height/2);
        }


        lineIterator->count++;//到0后停止动画 删除砖块
        if(lineIterator->count == Config::animationDuration * Config::fps){
            removeTwoBlocks(lineIterator->blocksEliminated.first,lineIterator->blocksEliminated.second);

            lineIterator = linkLines.erase(lineIterator);//删除此动画 这里智能指针会释放这两个+若干个拐点的block的内存
            if(lineIterator==linkLines.end()){
                break;
            }
        }else{
            lineIterator++;
        }
    }
}


void QLink::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && flashEnabled()){
        int x = event->x();
        int y = event->y();
        int ox = players[0].x;
        int oy = players[0].y;
        if(flashTo(x,y)){
            removeEmptyspace(players[0]);
            addEmptySpace(ox,oy);
            //如果flash到一个flash prop上 在flashTo中再次获得flash的功能 但在此处会再次(瞬间)关闭 导致flash到flash上不可再次flash
            //这个虽然看上去是问题 但是实际上 如果一个玩家获得了flash的功能 可以因此消除屏幕上所有flash 似乎没有意义 不妨保留这个设定
            flashFinish();//flash成功会提前结束倒计时
        }
    }

}
void QLink::keyPressEvent(QKeyEvent *event)
{
    if(gameover || stop){
        return;
    }

    int key = event->key();
    switch (event->key()){
    case Qt::Key_P:
        if(stop){
            stop = false;
            frameUpdateTimer->start(1000/Config::fps);
            propCreateTimer->start(Config::propFrequency*1000);
        }else{
            stop = true;
            frameUpdateTimer->stop();
            propCreateTimer->stop();
        }
        return;
    }
    if(!keyToDirection.count(event->key())){
        return;
    }
    int direction = keyToDirection[key];
    int playerID = keyToPlayerID[key];

    int ox = players[playerID-1].x;
    int oy = players[playerID-1].y;
    bool canMove = movePlayer(players[playerID-1],direction);
    if(canMove){
        removeEmptyspace(players[playerID-1]);
        addEmptySpace(ox,oy);
    }
}

bool QLink::flashTo(int x, int y)
{
    //点击的坐标是连续的 转化为离散的间隔为Element::step的坐标
    x = x > xbegin ? (x - xbegin) / Block::width * Block::width + xbegin : xbegin - ((xbegin - x) / Block::width+1) * Block::width;
    y = y > ybegin ? (y - ybegin) / Block::height * Block::height + ybegin : ybegin - ((ybegin - y) / Block::height+1) * Block::height;
    //超过边界则自动贴着边界
    if(x < xleft){
        x = xleft;
    }
    if(x >= xright){
        x = xright - Block::width;
    }
    if(y < ytop){
        y = ytop;
    }
    if(y >= ybottom){
        y = ybottom - Block::height;
    }

    int ox = players[0].x;
    int oy = players[0].y;
    players[0].moveTo(x,y);
    for(auto &block:blocks){
        if(collide(players[0],*block)){
            if(blocksAccessible.count(*block) && !block->isEliminated()){
                collideWithBlock(players[0],block);
                int i = block->code / 100;
                int j = block->code % 100;
                for(int k = 1 ; k <= 4 ; k++){
                    if(!map[i+dx[k]][j+dy[k]]){
                        players[0].move(dx[k],dy[k]);
                        break;
                    }
                }
            }else{
                players[0].moveTo(ox,oy);
                return false;
            }
        }
    }
    for(auto &prop:props){
        if(collide(players[0],prop)){
            collideWithProp(prop);
        }
    }
    return true;
}

QLink::Point QLink::getRandomSpace()
{
    //外部保证!emptySpace.empty()
    int loc = rand() % emptySpace.size();//在空白位置种随机一个位置

    int index = 0;
    auto empty = emptySpace.begin();
    while(index < loc){
        empty++;
        index++;
    }
    return *empty;
}

void QLink::removeTwoBlocks(std::shared_ptr<Block> block1, std::shared_ptr<Block> block2)
{
    blocks.remove_if([block1,block2](std::shared_ptr<Block> b){return b->code == block1->code|| b->code == block2->code;});
    map[block1->code/100][block1->code%100] = false;
    map[block2->code/100][block2->code%100] = false;
    updateBlocksAccessible(*block1,*block2);
    findSolutions();

    //如果被提示了 消除提示动画
    if(removeHint(*block1,*block2)){
        getRandomHint();
    }

    addEmptySpace(*block1);
    addEmptySpace(*block2);
}
bool QLink::removeHint(const Block &block1, const Block &block2)
{
    if(!hint){
        return false;
    }
    int code1 = hint->solution.block1.code;
    int code2 = hint->solution.block2.code;
    if(code1 == block1.code || code1 == block2.code || code2 == block1.code || code2 == block2.code){
        hint.release();
        return true;
    }
    return false;
}
void QLink::collideWithBlock(Player &player, std::shared_ptr<Block> block)
{
    for(auto &player:players){//如果选定的方块在消除前被另一个玩家选定 则失去该方块   可改
        if(player.getBlock() && player.getBlock()->code == block->code){
            player.removeBlock();
        }
    }
    block->setChosen(player.id);
    if(player.getBlock() && player.getBlock()->code!=block->code){ // 虽然link里有code相等不可连接的判断 但这里还是要加 不然连续两次触碰同一个block后会有问题
        Solution solution(*player.getBlock(),*block);
        if(solutions.count(solution)){//因为已经维护了解的集合 所以这里直接判断用户的两个block是否构成解即可 若是解则寻找路径绘制动画(本身也是link的过程)
            linkLines.push_back(LinkLine(player.getBlock(),block,getTurnPoints(*player.getBlock(),*block)));
            player.addScore();
            player.getBlock()->setEliminate();
            block->setEliminate();
            player.removeBlock();
        }else{
            player.getBlock()->setUnchosen();
            player.setBlock(block);
        }
    }else{
        player.setBlock(block);
    }
}

void QLink::collideWithProp(Prop &prop)
{
    switch (prop.type) {
    case Prop::ADDTIME: addTimeStart();break;
    case Prop::HINT: hintStart();break;
    case Prop::FLASH: flashStart(); break;
    case Prop::SHUFFLE: shuffleStart();break;

    }
    props.remove(prop);
    //collideWithBlock 中不能updateEmptySpace(block) 因为block的动画是其本身 需要消除后才能空出位置 而prop直接消失即可
    addEmptySpace(prop);

}

void QLink::collideWithProp(Prop &prop,Player &player)
{
    switch (prop.type) {
    case Prop::ADDTIME: addTimeStart();break;
    case Prop::HINT: hintStart();break;
    case Prop::SHUFFLE: shuffleStart();break;
    case Prop::FREEZE:freezeStart(players[2-player.id]);break;
    case Prop::DIZZY:dizzyStart(players[2-player.id]);break;
    }
    props.remove(prop);
    //collideWithBlock 中不能updateEmptySpace(block) 因为block的动画是其本身 需要消除后才能空出位置 而prop直接消失即可
    addEmptySpace(prop);
}
bool QLink::movePlayer(Player &player, int direction)
{
    int movex = dx[direction];
    int movey = dy[direction];

    player.move(movex,movey);

    /* 处理边界 第一种 不处理 一直往一个方向可以直接出地图 看不见但可以操作
     *  第二种 边界是四堵墙 到达边界后无法再向此方向移动 √
     *  第三种 从上方边界出去后会从下方边界再进来 但实现这个比较复杂 因为WIDTH不一定是width的整数倍 单纯取模会导致前后方块模型不重合 (collision里的简单判断逻辑)
     * 但个逻辑感觉应该写在基类的move里 基类的构造函数需要一个surface对象限制move的范围 但是存在嵌套定义
     */
    if(player.x < 0 || player.x + player.width > Config::width || player.y < 0 || player.y + player.height > Config::height){
        player.move(-movex,-movey);
        return false;//与边界碰撞就不会与其他元素碰撞
    }
    for(int i = 0 ; i < Config::playerNumber ; i++){//与别的玩家碰撞不可移动
        if(i != player.id-1 && collide(players[i],player)){
            player.move(-movex,-movey);
            return false;
        }
    }

    for(auto &block:blocks){//碰方块后完成选中或消除的效果 玩家不可以移动到该位置
        if(collide(player,*block)){
            player.move(-movex,-movey);
            if(!block->isEliminated()){
                collideWithBlock(player,block);
            }
            return false;
        }
    }

    for(auto &prop:props){//碰道具后道具消失 玩家可以移动到该位置
        if(collide(player,prop)){
            if(singlePlayer){
                collideWithProp(prop);
            }else{
                collideWithProp(prop,player);
            }
            return true;
        }
    }
    return true;
}


void QLink::createBlocks()
{
    srand(time(0));
    map = new bool*[Config::numberOfBlocksRow+2];
    int *count = new int[Block::type]();//加括号会初始化为0
    int max = Config::numberOfBlocksRow * Config::numberOfBlocksColumn / Block::type;
    for(int i = 0 ; i <= Config::numberOfBlocksRow + 1 ; i++){//从1开始 0表示紧挨着的白色方格 判断时有用
        map[i] = new bool[Config::numberOfBlocksColumn+2];
        for(int j = 0 ; j <= Config::numberOfBlocksColumn + 1 ;j++){
            if(i != 0 && i != Config::numberOfBlocksRow + 1 && j!=0 && j!= Config::numberOfBlocksColumn + 1){
                bool valid = false;
                int r;
                while(!valid){
                    r = rand()%Block::type;
                    if(++count[r]<=max){//改这里可以测试无解
                        valid = true;
                    }
                }
                int code = i*100+j;
                //当指向该内存的最后一个指针被销毁时 调用第二个参数(deleter) 自动释放内存
                std::shared_ptr<Block> block(new Block(xbegin+(i-1)*Block::width,ybegin+(j-1)*Block::height,Block::BlockColor[r],code));
                                             //,[](Block *b){std::cout<<"delete"<<b->code<<std::endl;delete b;});

                blocks.push_back(block);//这里用move会崩溃
                map[i][j] = true;
                if(i == 1 || i == Config::numberOfBlocksRow || j == 1 || j == Config::numberOfBlocksColumn){
                    blocksAccessible.insert(*block);
                }
            }else{
                map[i][j] = false;
            }
        }
    }
}
void QLink::addEmptySpace(const Element &e)
{
    emptySpace.insert(Point(e));//编译器会自动优化 如果这里加了std::move 会产生copy elision
}
void QLink::removeEmptyspace(const Element &e)
{
    emptySpace.erase(Point(e));
}
void QLink::addEmptySpace(int x, int y)
{
    emptySpace.insert(Point(x,y));
}
void QLink::findEmptySpace()
{

    int x,y;
    // 可以直接从左上角开始二重循环 加上block的判断 但实际上中间一大块完整的block区域一定非空白
    for (x = xleft ; x < xright ; x += Element::stepx) { //上 完整k1行
        for(y = ytop ; y < ybegin ; y += Element::stepy){
            emptySpace.insert(Point(x,y));
        }
    }
    for (x = xleft; x < xbegin ; x += Element::stepx) { //左 去掉左上矩形的k2列
        for(y = ybegin; y < ybottom ; y += Element::stepy){
            emptySpace.insert(Point(x,y));
        }
    }

    int xend = xbegin + Config::numberOfBlocksRow * Block::width;
    int yend = ybegin + Config::numberOfBlocksColumn * Block::height;
    for (x = xend ; x < xright ; x += Element::stepx) { //右 去掉右上矩形的k3列
        for(y = ybegin ; y < ybottom ; y += Element::stepy){
            emptySpace.insert(Point(x,y));
        }
    }
    for (x = xbegin ; x < xright ; x += Element::stepx) { //下
        for(y = yend ; y < ybottom ; y += Element::stepy){
            emptySpace.insert(Point(x,y));
        }
    }
}

std::shared_ptr<Block> QLink::findBlockByCode(int code)const
{
    for(const auto &block:blocks){
        if(block->code == code){
            return block;
        }
    }
    return std::shared_ptr<Block>(new Block(xbegin+(code/100-1)*Block::width,ybegin+(code%100-1)*Block::height,Block::BlockColor[0],-1));
    //return nullptr;
}

void QLink::updateBlocksAccessible(const Block &block1,const Block &block2)
{
    std::set<Block>::iterator blockIt;
    for(blockIt = blocksAccessible.begin() ; blockIt != blocksAccessible.end();){//把消除的两个block先删了
        if(blockIt->code == block1.code || blockIt->code == block2.code){
            blockIt = blocksAccessible.erase(blockIt);//官方api写的类型是迭代器 但如果直接erase(block1)也行 但这样就不能加const
        }else{
            blockIt++;
        }
    }

    int x,y;
    int code[2] = {block1.code,block2.code};
    for(int i = 0 ; i < 2 ; i++){
        x = code[i] / 100;
        y = code[i] % 100;
        if(x != 1 && map[x-1][y]){
            blocksAccessible.insert(*findBlockByCode(code[i]-100));
        }
        if(x != Config::numberOfBlocksRow && map[x+1][y]){
            blocksAccessible.insert(*findBlockByCode(code[i]+100));
        }
        if(y != 1 && map[x][y-1]){
            blocksAccessible.insert(*findBlockByCode(code[i]-1));
        }
        if(y != Config::numberOfBlocksColumn && map[x][y+1]){
            blocksAccessible.insert(*findBlockByCode(code[i]+1));

        }
    }
}


void QLink::findSolutions()
{
    solutions.clear();
    for(const auto &block1:blocksAccessible){//遍历可触碰的block 加入新的解
        for(const auto &block2:blocksAccessible){
            if(link(block1,block2)){
                Solution newSolution(block1,block2);
                solutions.insert(std::move(newSolution));
            }
        }
    }
}
std::set<int> QLink::extendOneBlock(int code)const
{
    std::set<int> res;
    int x = code/100;
    int y = code%100;
    int left = x-1;
    int right = x+1;
    int up = y-1;
    int down = y+1;
    while(up >= 0 && !map[x][up]){
        res.insert(x*100+up);
        up--;
    }
    while(down <= Config::numberOfBlocksColumn + 1 && !map[x][down]){
        res.insert(x*100+down);
        down++;
    }
    while(left >= 0 && !map[left][y]){
        res.insert(left*100+y);
        left--;
    }
    while(right <= Config::numberOfBlocksRow + 1 && !map[right][y]){
        res.insert(right*100+y);
        right++;
    }
    return res;
}
bool QLink::link(const Block &block1,const Block &block2)
{
    //返回是否可以连接 若可连接 在linkLines中加入相关block
    if(block1.code == block2.code){
        return false;
    }
    if(block1.color != block2.color){
        return false;
    }

    if(block1.isNextTo(block2)){//相邻 直接判断
        return true;
    }
    //不相邻 两个block分别四个方向延申 有重合则为拐点(单个)

    std::set<int> setA = extendOneBlock(block1.code);//两个block延申集合
    std::set<int> setB = extendOneBlock(block2.code);

    std::vector<int> setC;//两个block延申集合的交集
    std::set_intersection(setA.begin(),setA.end(),setB.begin(),setB.end(),std::back_inserter(setC));
    if(!setC.empty()){//两个block分别延申后的集合有交集则一个拐点两条直线可连接
        return true;
    }//否则遍历其中一个block延申的集合 将其中每个block再次延申 若与另一个block延申的集合有交集 则两个拐点三条直线可连接

    std::set<int> setD;//某一block延申集合中的一个block延申集合
    for(const auto &block:setA){
        setD = extendOneBlock(block);
        std::set_intersection(setD.begin(),setD.end(),setB.begin(),setB.end(),std::back_inserter(setC));
        if(!setC.empty()){
             return true;
        }
    }
    return false;
}

void QLink::findBlocksAccessible()
{
    for(const auto &block:blocks){
        int x = block->code / 100;
        int y = block->code % 100;
        if(!map[x-1][y] || !map[x+1][y] || !map[x][y-1] || !map[x][y+1]){
            blocksAccessible.insert(*block);
        }
    }
}

std::vector<QLink::Point> QLink::getTurnPoints(Block &block1, Block &block2)
{

    std::vector<QLink::Point> turnPoints; //连接点需要做动画或者其他功能 故需要通过code找回block对象

    if(!block1.isNextTo(block2)){//不相邻

        std::set<int> setA = extendOneBlock(block1.code);
        std::set<int> setB = extendOneBlock(block2.code);
        std::vector<int> setC;


        std::set_intersection(setA.begin(),setA.end(),setB.begin(),setB.end(),std::back_inserter(setC));
        if(setC.empty()){
            std::set<int> setD;
            for(auto &block:setA){
                setD = extendOneBlock(block);
                std::set_intersection(setD.begin(),setD.end(),setB.begin(),setB.end(),std::back_inserter(setC));
                if(!setC.empty()){
                    turnPoints.push_back(Point(*findBlockByCode(block)));
                    turnPoints.push_back(Point(*findBlockByCode(*setC.begin())));
                    break;
                }
            }
        }else{//作为拐点 必为空白(周围一圈或被消除的block) 不存在于blocks中 但为了统一类型 生成一个暂时的新的block作为绘制line使用 绘制完delete
            turnPoints.push_back(*(findBlockByCode(*setC.begin())));
        }
    }
    return turnPoints;
}
void QLink::createOneProp()
{
    if(emptySpace.empty()){
        return;
    }
    Point empty = getRandomSpace();
    int type;
    if(singlePlayer){
        type = rand() % Prop::singlePlayerAmount;
    }else{
        type = rand() % Prop::multiPlayerAmount;
        if(type == Prop::FLASH){
            type = Prop::DIZZY;
        }
    }
    Prop prop(empty.x,empty.y,Prop::color,type);
    props.push_back(std::move(prop));
    removeEmptyspace(prop);
}
void QLink::createPlayers()
{
    for (int i = 0; i < Config::playerNumber; i++)
    {
        Point empty = getRandomSpace();
        Player player(empty.x,empty.y,Player::playerColor[i],i+1);
        players.push_back(std::move(player));
        removeEmptyspace(player);
    }
}

void QLink::getRandomHint()//外部保证!solutions.empty()
{
    int loc = rand() % solutions.size();//随机解的位置
    int index = 0;//当前遍历到的解的位置
    for(const auto &solution : solutions){
        if(index == loc){
            hint.reset(new Hint(solution,Config::hintDuration));//会free原来指向的内存
        }
        index++;
    }
}

void QLink::shuffleStart()
{
    hint.reset();
    for(auto &player:players){
        player.removeBlock();
    }
    //打乱后所有block只是交换颜色 位置并没有变动 blocksAccessible基于位置 不变
    //但是这个集合因为使用set去重 保存的是复制的对象而不是指针 故全部失效 需要重新找

    blocksAccessible.clear();
    solutions.clear();

    for(auto &block:blocks){
        if(!block->isEliminated()){//动画中的玩家选择取消
            block->setUnchosen();
        }
    }
    for(auto &block1:blocks){// 交换 n^2 / n = n次
        for(auto &block2:blocks){
            int flag = rand() % blocks.size();
            if(flag == 0 && !block1->isEliminated() && !block2->isEliminated()){//不交换
                std::swap(block1->color,block2->color);
            }
        }
    }

    findBlocksAccessible();
    findSolutions();
}

void QLink::addTimeStart()
{
    timeRemain += 30;
    animationRemain[Prop::ADDTIME] = Config::animationDuration;
}

void QLink::hintStart()
{
    //如果还在hint的持续时间里 直接延长时间 不会换hint
    if(hint){
        hint->secondsRemain = Config::hintDuration;
    }else{
        getRandomHint();
    }

}
void QLink::hintFinish()
{
    hint.reset();
}
void QLink::flashStart()
{
    animationRemain[Prop::FLASH] = Config::flashDuration;
}
void QLink::flashFinish()
{
    animationRemain[Prop::FLASH] = 0;
}
bool QLink::flashEnabled() const
{
    return animationRemain[Prop::FLASH];
}

bool QLink::addTimeDisplay() const
{
    return animationRemain[Prop::ADDTIME];
}
void QLink::freezeStart(Player &player)
{
    player.setFreeze(Config::freezeDuration);
}

void QLink::dizzyStart(Player &player)
{
    player.setDizzy(Config::dizzyDuration);
}


