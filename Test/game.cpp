#include "game.h"
#include <set>
#include <iostream>

bool collide(const Element &a,const Element &b)
{
    return a.x == b.x && a.y == b.y;//游戏界面以正方形方块为一个单位 不会出现重合部分的情况
}
void Element::move(int dx,int dy)//-1 0 1
{
    x += dx ;
    y += dy ;
}

Block::Block(int ix,int iy,int t,int c):Element(ix,iy),status(0),code(c),type(t)
{
}
Block::Block(const Block &r):Element(r.x,r.y)
{
    status = r.status;
    code = r.code;
    type = r.type;
}





std::set<int> Game::extendOneBlock(int code)const
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
    while(down <= n + 1 && !map[x][down]){
        res.insert(x*100+down);
        down++;
    }
    while(left >= 0 && !map[left][y]){
        res.insert(left*100+y);
        left--;
    }
    while(right <= m + 1 && !map[right][y]){
        res.insert(right*100+y);
        right++;
    }
    return res;
}

bool Game::link(const Block &block1,const Block &block2)
{
    //返回是否可以连接 若可连接 在linkLines中加入相关block
    if(block1.code == block2.code){
        return false;
    }
    if(block1.type != block2.type){
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
Game::Game()
{
    int type[m*n] = {0,1,2,1,0,4,5,3,2,0,1,2,3,4,5,3,4,5,5,4,3,2,1,0,1,0,2,4,3,5,1,3,0,4,2,5,5,2,3,1,4,0,0,1,4,5,2,3};
    for(int i = 0 ; i <= n + 1 ; i++){//从1开始 0表示紧挨着的白色方格 判断时有用
        for(int j = 0 ; j <= m + 1 ;j++){
            if(i != 0 && i != n + 1 && j!=0 && j!= m + 1){
                int code = i*100+j;
                //当指向该内存的最后一个指针被销毁时 调用第二个参数(deleter) 自动释放内存

                blocks[i-1][j-1] = Block(1,1,type[(i-1)*m+j-1],code);
                map[i][j] = true;
            }else{
                map[i][j] = false;
            }
        }
    }
}
