#include "test.h"
#include "game.h"
#include <iostream>
Test::Test()
{

}
/*
0   2,1,0
5,3,2,0,1
  4,5,4
  4,3,2,1,0
1,0,2   3,5
1,3,0,4   5
5,2,3,1,4,0
0   4,5,2,3
*/
//blocks[行][列]
void Test::case1_testcase(){//不同颜色 折线1
    Game game;
    QVERIFY(game.link(game.blocks[0][0],game.blocks[0][2]) == false);
}
void Test::case2_testcase(){//同颜色 折线3
    Game game;
    QVERIFY(game.link(game.blocks[0][0],game.blocks[0][4]) == true);
}
void Test::case3_testcase(){//同颜色 无折线
    Game game;
    QVERIFY(game.link(game.blocks[0][0],game.blocks[1][3]) == false);
}

void Test::case4_testcase(){//同颜色 相邻
    Game game;
    QVERIFY(game.link(game.blocks[2][1],game.blocks[3][1]) == true);
}

void Test::case5_testcase(){//同颜色 折线2
    Game game;
    QVERIFY(game.link(game.blocks[3][3],game.blocks[4][2]) == true);
}

void Test::case6_testcase(){//同颜色 折线4
    Game game;
    QVERIFY(game.link(game.blocks[7][3],game.blocks[5][5]) == false);
}

void Test::case7_testcase(){//同颜色 折线1
    Game game;
    QVERIFY(game.link(game.blocks[1][4],game.blocks[3][4]) == true);
}

void Test::case8_testcase(){//同颜色 折线5
    Game game;
    QVERIFY(game.link(game.blocks[0][3],game.blocks[3][4]) == false);
}

void Test::case9_testcase(){//不同颜色 折线3
    Game game;
    QVERIFY(game.link(game.blocks[0][0],game.blocks[4][0]) == false);
}

void Test::case10_testcase(){//不同颜色 折线2
    Game game;
    QVERIFY(game.link(game.blocks[1][0],game.blocks[2][1]) == false);
}




//QTEST_MAIN(SimpleTest)
