#include "pushBoxManager.h"

int main()
{
	//实现地图  人物移动
	PushBoxManager pbm;

	while (true)
	{
		//主菜单展示
		pbm.showMainMenu();

		cout << "请选择功能：" << endl;
		char ch;
		cin >> ch;

		int chioce = 0;
		if (ch >= '1' && ch <= '3')  //访问输入错误造成bug
		{
			chioce = ch - '0';
		}
		else
		{
			system("cls");
			continue;
		}
		
		switch (chioce)
		{
		case 1:  //1、开始游戏
			pbm.startGame();
			break;
		case 2:  //2、自创地图
			pbm.CustomSystem();
			break;
		case 3:  //3、退出游戏
			pbm.exitGame();
			break;
		default:
			system("cls");
			break;
		}
	}
	
	

	return 0;
}