#include "test.h"
#include "game.h"
#include <iostream>
Test::Test()
{

}
/*
0,1,2,1,0,4
5,3,2,0,1,2
3,4,5,3,4,5
5,4,3,2,1,0
1,0,2,4,3,5
1,3,0,4,2,5
5,2,3,1,4,0
0,1,4,5,2,3
*/
//blocks[行][列]
void Test::case1_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[0][0],game.blocks[0][1]) == false);
}
void Test::case2_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[0][0],game.blocks[4][0]) == false);
}
void Test::case3_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[0][0],game.blocks[0][4]) == true);
}

void Test::case4_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[0][1],game.blocks[0][3]) == true);
}

void Test::case5_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[0][1],game.blocks[4][0]) == false);
}

void Test::case6_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[2][1],game.blocks[3][1]) == true);
}

void Test::case7_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[2][1],game.blocks[2][5]) == false);
}

void Test::case8_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[6][3],game.blocks[7][1]) == false);
}

void Test::case9_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[4][5],game.blocks[5][5]) == true);
}

void Test::case10_testcase(){
    Game game;
    QVERIFY(game.link(game.blocks[2][5],game.blocks[4][5]) == true);
}




//QTEST_MAIN(SimpleTest)
