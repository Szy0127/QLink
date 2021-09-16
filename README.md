SJTU_SEP    
9/22 剥离出游戏本身的数据结构和算法 重新设计Game类 单例 调整接口 私有的放入private  QLink类只保留paint、mouse、keyEvent 一个game的实例   
9/29 由下至上重载每一个class/struct的输入输出函数 (不涉及stl) 并增加无参数的构造函数(仅初始化使用)  
     存档时ofstream<<object 读档时 Object object;ifstream>>object;    
	   所有涉及到指针的object 均存*object
9/30调整暂停的算法
10 增加开始、结束界面 