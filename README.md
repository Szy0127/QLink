SJTU_SEP    
9/19 剥离出游戏本身的数据结构和算法 重新设计Game类 单例 调整接口 私有的放入private  QLink类只保留paint、mouse、keyEvent 一个game的实例   
9/22 保存  全用二进制
9/25 block的纯色背景改成图片 自定义图片方法：
1 找到图片  
2 用ps右键图层 右下角倒数第四个圆的 选纯色 这里使用97d9f1 调整图层顺序 魔法笔删除原图层中空白部分
3 图像 大小 暂时设置为65x65
4 导出路径为工作目录
5 代码中 QImage对象 构造函数为path paintImage方法参数为x y image  
9/29 补全存档的内容 修复了QImage无法二进制保存的bug 完成存档 完成了简单的开始界面
10/2 暂停 开始、结束界面 存档可选择 
10/3 修改了map可能内存泄漏的bug   
10/5 增加settingwindow类 将qlink和settting与menu的跳转设计为startTimer与timerEvent的形式    
10/6 用智能指针  
10/7 修改大小导致的屏幕过大过小  测试点          
10/8 ps改不同size的图片 
10/9 加道具图片 做存档 code review  答辩     

给每种不同的block加不同的分数
存档格式不对 导致二进制read直接错位 导致所有对象的成员解析出错 出现无法预估的问题 直接崩溃 trycatch无效
对于固定像素的图片用resize的api会导致失真比较严重 最好是使用像素相近的图片 事先手动resize 40 50 60 65 70 80 png准备