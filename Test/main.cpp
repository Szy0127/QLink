#include <iostream>
#include "test.h"
#include "game.h"
using namespace std;

int main()
{
    /*Test code start*/
    Test *test = new Test();
    QTest::qExec(test);
    delete test;
    /*Test code end*/

    /*Your work should be here.*/
    Game game;
    std::cout<<game.blocks[1][0].type<<game.blocks[3][0].type<<std::endl;
    return 0;
}
