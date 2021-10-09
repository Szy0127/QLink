# SJTU_SEP 
![Image text](https://github.com/Szy0127/QLink/blob/main/sample.png)
### 本地依赖文件说明   
- archive文件夹保存存档 其中file中保存了可读的.conf配置文件及不可读的二进制文件 载入游戏时选择图片即可 无对应存档会报错 但如果名字没改 改了内容 游戏会崩溃(待优化)
- images文件夹保存了游戏需要加载的人物、方块、道具图片 无对应图片会报错 可自定义图片   
- default.conf文件保存了最近一次游戏的配置 可在本地修改也可在游戏中修改 这个文件随意修改也会有问题 需要在setting.cpp中限制   

### 代码文件说明
1. Config.h/cpp 保存了QLink整体的配置信息 分为以下三部分 第一部分会保存到本地文件
 - 可改动：方块数量、大小、道具时间等每局游戏可自定义的变量
 - 不可改动：存储路径、目录大小、字体大小等
 - 不同类间传递：QLink游戏结束或Setting设置完成提交时产生的startTimer信号ID
2. Element.h/cpp 定义了一些游戏需要的元素 基类有QImage*成员 可优化 `Element`的三个派生类如下
 - `Prop` 道具 有加时、打乱、提示、闪现、眩晕、冻结
 - `Block` 方块 玩家需要消除的对象 
 - `Player` 玩家 在`namespace gameSZY`的两个字典中设置玩家对应的移动操作可支持大于二人游戏
3. Game.h/cpp 保存了连连看游戏主体的逻辑 使用基础的数据结构与函数 与Qt无关 定义如下私有结构体
 - `Point` 记录二维坐标 自定义比pair使用方便一点
 - `Solution` 表示两个block构成的一组解
 - `LinkLine` 两个block及其拐点构成的线段动画
 - `Hint` 提示的解与剩余时间   
    
  维护以下成员变量
 - 玩家集合`std::vector<Player> players`
 - 方块集合`std::list<std::shared_ptr<Block>> blocks`
 - 道具集合`std::list<Prop> props`
 -  空白位置集合`std::set<Point> emptySpace` 生成道具
 -  玩家可碰方块集合`std::set< Block> blocksAccessible` 便于找解
 -  解的集合`std::set< Solution> solutions`
 -  提示`std::unique_ptr<Hint> hint`
 -  地图`bool **map` 用于判断两个方块是否可以连接
 -  方块连接的动画集合`std::list<LinkLine> linkLines`
 -  动画剩余时间(加时与闪现)`int animationRemain[Prop::animationAmount]`
 -  游戏剩余时间`int timeRemain`
 -  是否单人游戏`bool singlePlayer` 便于单双人不同逻辑时的判断
4. QLink.h/cpp 在Game的基础上增加了交互 是真正的游戏界面
 - `paintEvent` 绘制
 - `keyPressEvent`存档 移动
 - `mousePressEvent`闪现
 - `QTimer` 更新状态
5. Menu.h/cpp 菜单 游戏结束后会返回到菜单 不会退出程序
 - Start 开始新游戏 跳转到Setting
 - Load 载入游戏 archive文件中的存档
 - Quit 退出程序
6. Setting.h/cpp 用户设置Config中的可改动部分 并保存 限制了上下限
 - 游戏模式(单双人)
 - 方块大小
 - 方块数量(上下限会随方块大小变动 保证屏幕放下 Config中设置了最大屏幕大小)
 - 加时、打乱、提示、闪现、眩晕、冻结持续时间

### 一些问题
- set集合存的对象涉及到复制构造函数 效率可能较低
- Element用指针存图像 而派生类会进行复制 指针问题（已经进行处理 但可能不完美）
- 用线性结构存block 这是在逻辑和效率权衡下选择的，但不可避免出现定位block从O(1)变成O(n)的结果 是否可以兼顾
- 绘制动画是找turnPoint的算法需要通过构造不存在的Block得到坐标 虽然调用方便但是不仅导致了不存在的Block的出现 还浪费了一次构造函数和虚构函数的时间
- 最严重的问题！用二进制存档，读档时永远不会报错(只要read正确字节数)但如果这些字节数据出现了问题，是在程序后续运行中无法解析的，这些错误没有办法catch住 ，只能用其他方法额外限制存档文件的正确性(如比较哈希)解析数据时程序的崩溃没有办法吞下而不暴露在用户目前