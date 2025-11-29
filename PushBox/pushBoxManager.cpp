#include "pushBoxManager.h"
#include <conio.h>


//构造函数
PushBoxManager::PushBoxManager()
{

	//加载地图
	this->readFiles();

	//创建推箱子的人
	BoxPusher bpm;

	//提前加载  //完全记录保存玩家没有结束的关卡的容器
	this->LoadingRecordPlay();

	//读取自定义地图的系统   可以读取未完成的也可以
	//读取自定义所有地图
	this->CustomReadSystem("CustomAllMap.txt", true);

	//读取未完成的自定义地图
	this->CustomReadSystem("CustomUnfinishMap.txt", false);

}

//首菜单功能
void PushBoxManager::showMainMenu()
{
	cout << "************************************" << endl;
	cout << "************************************" << endl;
	cout << "********                    ********" << endl;
	cout << "******** 欢迎来到推箱子游戏 ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********    1、开始游戏     ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********    2、自创地图     ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********    3、退出游戏     ********" << endl;
	cout << "********                    ********" << endl;
	cout << "************************************" << endl;
	cout << "************************************" << endl;
}

//开始游戏
void PushBoxManager::startGame()
{
	system("cls");
	while (true)
	{
		this->showGameMenu();
		cout << "请选择功能：" << endl;
		char ch;
		cin >> ch;

		int chioce = 0;
		//访问输入错误造成bug
		if (ch >= '0' && ch <= '3')
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
		case 1:  //1、系统关卡
			this->Chechpoint();
			break;
		case 2:  //2、继续游戏
			this->isPlayUnfinish = true;
			this->Chechpoint();
			this->isPlayUnfinish = false;
			break;
		case 3:  //3、自定关卡
			this->CustomCheckpoint();
			break;
		case 0:  //0、返回
			system("cls");
			return;
		default:
			system("cls");
			break;
		}
	}
}

//游戏菜单
void PushBoxManager::showGameMenu()
{
	cout << "************************************" << endl;
	cout << "************************************" << endl;
	cout << "********                    ********" << endl;
	cout << "********    1、系统关卡     ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********    2、继续游戏     ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********    3、自定关卡     ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********    0、返回         ********" << endl;
	cout << "********                    ********" << endl;
	cout << "************************************" << endl;
	cout << "************************************" << endl;
}

//开始游戏 中 1、系统关卡
void PushBoxManager::Chechpoint()
{
	if (this->isMapFile == false)
	{
		system("cls");
		return;
	}
	while (true)
	{
		if (this->isPlayUnfinish)cout << "没完成的系统关卡：" << endl;
		else cout << "系统关卡：" << endl;
		cout << "------------------------------------" << endl;
		if (this->isPlayUnfinish)
		{
			//判断是否有未完成的关卡
			if (this->m_SaveCheckpoint.size() == 0)
			{
				cout << "没有未完成的关卡" << endl;
				system("pause");
				system("cls");
				return;
			}
			int k = 0;
			for (map<int, vector<vector<string>>>::iterator it = m_SaveCheckpoint.begin(); it != m_SaveCheckpoint.end(); it++)
			{
				cout << "\t" << it->first + 1;
				k++;
				if (k % 4 == 0)cout << endl;
			}
		}
		else
		{
			for (int i = 0; i < this->m_SumMap.size(); i++)
			{
				cout << "\t" << i + 1;
				if ((i + 1) % 4 == 0)cout << endl;
			}
		}	
		cout << endl;
		cout << "------------------------------------" << endl;
		cout << "请选择关卡：           按数字0键返回" << endl;
		int select = 0;
		string s;
		cin >> s;
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] < '0' || s[i]>'9')
			{
				system("cls");
				return;

			}
			select *= 10;
			select += s[i] - '0';
		}

		if (select == 0)
		{
			this->isPlayUnfinish = false;
			system("cls");
			return;  //返回
		}
		this->m_indexNow = select;
		this->StartChechpoint(select);
	}
	
	
}

//关卡正式开始
void PushBoxManager::StartChechpoint(int n)
{
	if (this->isPlayUnfinish)
	{
		if (this->m_SaveCheckpoint.find(n - 1) == this->m_SaveCheckpoint.end()) 
		{
			system("pause");
			return;
		}
		this->m_Map = this->m_SaveCheckpoint[n - 1];
		this->m_Destination = this->m_SumDestination[n - 1];
		this->bpm.m_x = this->m_UnfinishBoxPusherSites[n - 1].first;
		this->bpm.m_y = this->m_UnfinishBoxPusherSites[n - 1].second;
	}
	else
	{
		if (this->m_SumMap.find(n - 1) == this->m_SumMap.end())
		{
			system("pause");
			return;
		}
		this->m_Map = this->m_SumMap[n - 1];
		this->m_Destination = this->m_SumDestination[n - 1];
		this->bpm.m_x = this->m_BoxPusherSites[n - 1].first;
		this->bpm.m_y = this->m_BoxPusherSites[n - 1].second;
	}
	while (true)
	{
		system("cls");
		
		this->PrintMap();

		if (this->isWin)  //判断是否完成关卡
		{
			cout << "------ 恭喜你通过此关 ------" << endl;
			this->isWin = false;
			system("pause");
			/*system("cls");*/
			break;
		}
		//cout << this->m_Destination.size() << endl; //测试代码
		cout << "                   按数字0键返回" << endl;
		cout << "                   按M键重新开始" << endl;
		cout << "                   按B键保存返回" << endl;
		cout << "       上：W" << endl;
		cout << "左：A  下：S  右：D" << endl;

		
		char ch = _getch();
		if (ch == '0')return;
		else if (ch == 'm' || ch == 'M')  //重新开始
		{
			cout << "确定吗？" << endl;
			cout << "1、确定" << endl;
			cout << "2、否" << endl;
			int temp;
			cin >> temp;
			if (temp == 1)
			{
				this->m_indexNow = 0;
				/*if (this->isPlayUnfinish)
				{
					this->m_Map = this->m_SaveCheckpoint[n - 1];
					this->m_Destination = this->m_SumDestination[n - 1];
					this->bpm.m_x = this->m_UnfinishBoxPusherSites[n - 1].first;
					this->bpm.m_y = this->m_UnfinishBoxPusherSites[n - 1].second;
				}
				else*/
				{
					this->m_Map = this->m_SumMap[n - 1];
					this->m_Destination = this->m_SumDestination[n - 1];
					this->bpm.m_x = this->m_BoxPusherSites[n - 1].first;
					this->bpm.m_y = this->m_BoxPusherSites[n - 1].second;
				}
			}
		}
		else if (ch == 'b' || ch == 'B')  //保存返回
		{
			cout << "确定吗？" << endl;
			cout << "1、确定" << endl;
			cout << "2、否" << endl;
			int temp;
			cin >> temp;
			if (temp == 1)
			{
				this->m_indexNow = 0;
				this->SaveCheckpoint(n,this->bpm.m_x,this->bpm.m_y);
				return;
			}
		}
		else
		{
			this->marchPusher(ch);
		}
		
	}
}

//打印地图
void PushBoxManager::PrintMap()
{
	for (vector<vector<string>>::iterator it1 = this->m_Map.begin(); it1 != this->m_Map.end(); it1++)
	{
		for (vector<string>::iterator it = it1->begin(); it != it1->end(); it++)
		{
			cout << *it;
		}
		cout << endl;
	}
	cout << endl;
}

//将推箱人放入地图中
void PushBoxManager::PutBoxPusherMap()
{
	this->m_Map[this->bpm.m_y][this->bpm.m_x] = "我";
}

// 我  移动系统 推箱功能
void PushBoxManager::marchPusher(char ch)
{
	//pusher 的坐标
	int x = this->bpm.m_x;
	int y = this->bpm.m_y;
	if (ch == 'w' || ch == 'W' || ch == 's' || ch == 'S' || ch == 'a' || ch == 'A' || ch == 'd' || ch == 'D')
	{
		this->m_Map[y][x] = "  ";
		if (ch == 'w' || ch == 'W')
		{
			if (this->m_Map[y - 1][x] != "墙")
			{
				if (this->m_Map[y - 1][x] == "箱" && this->m_Map[y - 2][x] != "墙" && this->m_Map[y - 2][x] != "箱")
				{
					this->bpm.m_y--;
					this->m_Map[y - 2][x] = "箱";
				}
				else if (this->m_Map[y - 1][x] != "箱")this->bpm.m_y--;
			}
		}
		else if (ch == 's' || ch == 'S')
		{
			if (this->m_Map[y + 1][x] != "墙")
			{
				if (this->m_Map[y + 1][x] == "箱" && this->m_Map[y + 2][x] != "墙" && this->m_Map[y + 2][x] != "箱" )
				{
					this->bpm.m_y++;
					this->m_Map[y + 2][x] = "箱";
				}
				else if (this->m_Map[y + 1][x] != "箱")this->bpm.m_y++;
			}
		}
		else if (ch == 'a' || ch == 'A')
		{
			if (this->m_Map[y][x - 1] != "墙")
			{
				if (this->m_Map[y][x - 1] == "箱" && this->m_Map[y][x - 2] != "墙" && this->m_Map[y][x - 2] != "箱")
				{
					this->bpm.m_x--;
					this->m_Map[y][x - 2] = "箱";
				}
				else if (this->m_Map[y][x - 1] != "箱")this->bpm.m_x--;
			}
		}
		else if (ch == 'd' || ch == 'D')
		{
			if (this->m_Map[y][x + 1] != "墙")
			{
				if (this->m_Map[y][x + 1] == "箱" && this->m_Map[y][x + 2] != "墙" && this->m_Map[y][x + 2] != "箱")
				{
					this->bpm.m_x++;
					this->m_Map[y][x + 2] = "箱";
				}
				else if (this->m_Map[y][x + 1] != "箱")this->bpm.m_x++;
			}
		}
	}

		//保持“口”一直存在
	this->PutDesitnation();
	

	
	//更新地图中 我 的位置
	this->PutBoxPusherMap();

}

//将口放入地图
void PushBoxManager::PutDesitnation()
{
	int temp = (int)this->m_Destination.size();
	for (vector<pair<int, int>>::iterator it = this->m_Destination.begin(); it != this->m_Destination.end(); it++)
	{
		if (this->m_Map[it->second][it->first] != "箱")	this->m_Map[it->second][it->first] = "口";
		else if(this->m_Map[it->second][it->first] == "箱")temp--;
	}
	if (temp == 0)
	{
		this->isPlayUnfinish = false;
		this->m_SaveCheckpoint.erase(this->m_indexNow - 1);
		this->isWin = true;
	}
}

//读取系统关卡文件信息
void PushBoxManager::readFiles()
{
	ifstream ifs;
	ifs.open("pusherMap.txt", ios::in);

	if (!ifs.is_open())  //是否打开成功
	{
		this->isMapFile = false;
		return;
	}

	//判读文件是否存在 
	if (ifs.eof())
	{
		this->isMapFile = false;
		return;
	}

	char ch;
	ifs >> ch;

	if (ch == EOF)//判断文件是否为空
	{
		this->isMapFile = false;
		return;
	}
	this->isMapFile = true;  //文件不为空
	ifs.putback(ch);

	//临时存放数据
	string line;

	int index = 0;
	while (ifs >> line)
	{
		int pos = line.find(",");
		int rpos = line.rfind(",",pos + 1);
		string s1 = line.substr(0, pos);
		string s2 = line.substr(pos + 1, rpos);
		int x = 0, y = 0; //存放边长
		for (int i = 0; i < s1.size(); i++)
		{
			x *= 10;
			x += s1[i] - '0';
		}
		for (int i = 0; i < s2.size(); i++)
		{
			y *= 10;
			y += s2[i] - '0';
		}

		//临时存放地图的容器
		vector<vector<string>> v;
		//临时存放地图的“口”的容器
		vector<pair<int, int>> v2;
		for (int i = 0; i < y; i++)
		{
			vector<string> v1;
			for (int j = 0; j < x; j++)
			{
				ifs >> ch;
				if (ch == 'W')v1.push_back("墙");
				else if (ch == 'O')v1.push_back("  ");
				else if (ch == '#')
				{
					v1.push_back("口");
					v2.push_back(make_pair(j, i));
				}
				else if (ch == 'B')v1.push_back("箱");
				else if (ch == '@')
				{
					v1.push_back("我");
					this->m_BoxPusherSites.insert(make_pair(index, make_pair(j, i)));
				}
			}
			v.push_back(v1);
		}
		this->m_SumMap.insert(make_pair(index, v));  //index++ 更新index下一次的数
		this->m_SumDestination.insert(make_pair(index, v2));
		this->m_SumMapLenth.insert(make_pair(index, make_pair(x, y)));
		index++;
	}
	ifs.close();
}

//保存玩家没有结束的关卡  // n 是第几关
void PushBoxManager::SaveCheckpoint(int n,int x,int y)  
{
	this->m_SaveCheckpoint[n - 1] = this->m_Map;
	this->m_UnfinishBoxPusherSites[n - 1].first = x;
	this->m_UnfinishBoxPusherSites[n - 1].second = y;

}

//完全记录保存玩家没有结束的关卡的容器
void PushBoxManager::WriteFilesSaveCheckpoint()
{
	ofstream ofs("SaveChechpoint.txt", ios::out | ios::trunc);
	for (map<int, vector<vector<string>>>::iterator it = this->m_SaveCheckpoint.begin(); it != this->m_SaveCheckpoint.end(); it++)
	{
		ofs << this->m_SumMapLenth[it->first].first << "," << this->m_SumMapLenth[it->first].second << ","
			<< it->first + 1 << "," << endl;
		for (vector<vector<string>>::iterator it1 = it->second.begin(); it1 != it->second.end(); it1++)
		{
			for (vector<string>::iterator it2 = it1->begin(); it2 != it1->end(); it2++)
			{
				if (*it2 == "墙")ofs << "W";
				else if (*it2 == "  ")ofs << "O";
				else if (*it2 == "箱")ofs << "B";
				else if (*it2 == "口")ofs << "#";
				else if (*it2 == "我") ofs << "@";
				
			}
			ofs << endl;
		}

	}
	ofs.close();
}

//提前加载  //完全记录保存玩家没有结束的关卡的容器
void PushBoxManager::LoadingRecordPlay()
{
	ifstream ifs;
	ifs.open("SaveChechpoint.txt", ios::in);

	if (!ifs.is_open())
	{
		return;
	}
	if (ifs.eof())
	{
		return;
	}

	char ch;
	ifs >> ch;

	if (ch == EOF)return;  //判断有无内容

	ifs.putback(ch); //放回

	string s;
	while (ifs >> s)
	{
		int pos = s.find(",");
		int pos1 = s.find(",",pos + 1);
		int pos2 = s.find(",", pos1 + 1);
		string s1 = s.substr(0, pos);
		string s2 = s.substr(pos + 1, pos1 - pos - 1);
		string s3 = s.substr(pos1 + 1, pos2 - pos1 - 1);
		int x = 0, y = 0,index = 0; //存放边长
		for (int i = 0; i < s1.size(); i++)
		{
			x *= 10;
			x += s1[i] - '0';
		}
		for (int i = 0; i < s2.size(); i++)
		{
			y *= 10;
			y += s2[i] - '0';
		}
		for (int i = 0; i < s3.size(); i++)
		{
			index *= 10;
			index += s3[i] - '0';
		}
		index--;

		//cout << "pos = " << pos << "  pos1 = " << pos1 << "  pos2 = " << pos2 << endl;
		//cout << "s = " << s << "  s1 = " << s1 << "  s2 = " << s2 << "  s3 = " << s3 << endl;
 	//	cout << " x = " << x << "  y = " << y << "  index = " << index << endl;  ////////////////

		//临时存放地图的容器
		vector<vector<string>> v;
		//临时存放地图的“口”的容器
		vector<pair<int, int>> v2;
		for (int i = 0; i < y; i++)
		{
			vector<string> v1;
			for (int j = 0; j < x; j++)
			{
				ifs >> ch;
				if (ch == 'W')v1.push_back("墙");
				else if (ch == 'O')v1.push_back("  ");
				else if (ch == '#')
				{
					v1.push_back("口");
					v2.push_back(make_pair(j, i));
				}
				else if (ch == 'B')v1.push_back("箱");
				else if (ch == '@')
				{
					v1.push_back("我");
					this->m_UnfinishBoxPusherSites.insert(make_pair(index, make_pair(j, i)));
				}
			}
			v.push_back(v1);
		}

		////测试代码
		//for (int i = 0; i < 3; i++)
		//{
		//	for (int j = 0; j < 3; j++)
		//	{
		//		cout << v[i][j];
		//	}
		//	cout << endl;
		//}
		//system("pause");
		this->m_SaveCheckpoint.insert(make_pair(index, v));  //index++ 更新index下一次的数
	}
	ifs.close();
}

//自定义地图
void PushBoxManager::CustomSystem()
{
	system("cls");
	while (true)
	{
		this->CustomMapMenu();
		cout << "请选择功能：" << endl;
		char ch;
		cin >> ch;

		//访问输入错误造成bug
		int chioce = 0;
		if (ch >= '0' && ch <= '3')
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
		case 1:  //1、创建地图
			this->CreateCustomMap();
			break;
		case 2:  //2、修改地图
			this->CustomMapModify();
			break;
		case 3:  //3、删除地图
			this->CustomDelectMap();
			break;
		case 0:  //0、返回
			system("cls");
			return;
		default:
			system("cls");
			break;
		}
	}
}

//3、删除地图   自定义地图
void PushBoxManager::CustomDelectMap()
{
	cout << "------------------------------------" << endl;
	if (this->m_CustomAllMap.empty())
	{
		cout << "没有自定义的关卡！" << endl;
		system("pause");
		return;
	}
	int count = 0;
	for (map<int, vector<vector<string>>>::iterator it = this->m_CustomAllMap.begin(); it != this->m_CustomAllMap.end(); it++)
	{
		cout << "\t" << it->first + 1;
		count++;
		if (count % 4 == 0)cout << endl;
	}
	cout << endl;
	cout << "------------------------------------" << endl;

	cout << "请选择删除关卡：       按数字 0 返回" << endl;
	int select = 0;
	string s;
	cin >> s;
	for (int i = 0; i < s.size(); i++)
	{
		if (s[i] < '0' || s[i]>'9')
		{
			system("cls");
			return;

		}
		select *= 10;
		select += s[i] - '0';
	}
	if (select == 0)
	{
		system("cls");
		return;
	}
	this->m_CustomAllMap.erase(select - 1);
	this->m_CustomDesitnation.erase(select - 1);
	this->m_CustomMapCount--;
	this->m_CustomPusher.erase(select - 1);
	this->m_CustomSideLenthMap.erase(select - 1);
	this->m_CustomUnfinishMap.erase(select - 1);
	system("cls");
}

//自定义地图菜单
void PushBoxManager::CustomMapMenu()
{
	cout << "************************************" << endl;
	cout << "************************************" << endl;
	cout << "********                    ********" << endl;
	cout << "********    1、创建地图     ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********    2、修改地图     ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********    3、删除地图     ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********    0、返回         ********" << endl;
	cout << "********                    ********" << endl;
	cout << "************************************" << endl;
	cout << "************************************" << endl;
}

//1、创建地图___自定义地图
void PushBoxManager::CreateCustomMap()
{
	int x = 0, y = 0;
	while (true)
	{
		system("cls");
		this->CustomMapMenu();
		cout << "------------------------------------" << endl;
		cout << "长是：                 按数字 0 返回" << endl;
	
		string s;
		cin >> s;
		int i;
		for (i = 0; i < s.size(); i++)
		{
			if (s[i] < '0' || s[i]>'9')
			{
				cout << "输入错误！请重新输入！" << endl;
				system("pause");
				system("cls");
				break;
			}
			x *= 10;
			x += s[i] - '0';
		}
		if (x == 0)
		{
			system("cls");
			return;
		}
		if (i == s.size())break;
	}
	
	while (true)
	{
		system("cls");
		this->CustomMapMenu();
		cout << "------------------------------------" << endl;

		cout << "宽是：                 按数字 0 返回" << endl;
		string s;
		cin >> s;

		int i;
		for (i = 0; i < s.size(); i++)
		{
			if (s[i] < '0' || s[i]>'9')
			{
				cout << "输入错误！请重新输入！" << endl;
				system("pause");
				system("cls");
				break;
			}
			y *= 10;
			y += s[i] - '0';
		}
		if (y == 0)
		{
			system("cls");
			return;
		}
		if (i == s.size())break;
	}
	this->bpm.m_x = 0;
	this->bpm.m_y = 0;

	this->m_CustomMapCount++;
	this->m_indexNow = this->m_CustomMapCount;
	this->CustomMapGameSideLenth(x, y);
	this->m_Map[0][0] = "我";
	this->m_CustomSideLenthMap.insert(make_pair(this->m_indexNow - 1, make_pair(x, y)));
	this->CustomMapMain();
	
	this->m_CustomPusher.insert(make_pair(this->m_indexNow - 1, make_pair(this->bpm.m_x, this->bpm.m_y)));
	this->CustomMapSaveDestination();
	//cout << this->bpm.m_x << "  " << this->bpm.m_y << endl;
	//system("pause");
	
}

//自定义地图边长
void PushBoxManager::CustomMapGameSideLenth(int x,int y)
{
	vector<string> v1;
	v1.resize(x, "  ");
	vector<vector<string>> v;
	v.resize(y, v1);
	this->m_Map = v;
}

//开始自定义地图内容
void PushBoxManager::CustomMapMain()
{
	while (true)
	{
		system("cls");
		this->CustomPrintMap();
		if (this->m_isPut)cout << "处于放置状态：";
		else if (this->m_CustomDelect) cout << "处于爆破状态：";
		cout << endl;
		cout << "按 J 键进入放箱的状态" << endl;
		cout << "按 K 键进入放墙的状态" << endl;
		cout << "按 L 键进入放口的状态" << endl;
		cout << "按 I 键进入爆破的状态" << endl;
		cout << "按 B 键进入退出" << endl;

		char ch = _getch();

		
		if (ch == 'b' || ch == 'B')
		{
			cout << "确定吗？" << endl;
			cout << "1、确定" << endl;
			cout << "2、不了" << endl;
			char ch2 = _getch();
			if (ch2 - '0' == 1)
			{
				this->m_CustomAllMap[this->m_indexNow - 1] = this->m_Map;
				system("cls");
				return;
			}
			system("cls");
		}
		else this->CustomMoveAndPut(ch);


	}
}

//自定义地图的打印
void PushBoxManager::CustomPrintMap()
{
	for (vector<vector<string>>::iterator it = this->m_Map.begin(); it != this->m_Map.end(); it++)
	{
		for (vector<string>::iterator it1 = it->begin(); it1 != it->end(); it1++)
		{
			cout << *it1;
		}
		cout << endl;
	}
}

//自定义地图的移动系统和放置系统
void PushBoxManager::CustomMoveAndPut(char ch)
{
	int m_x = this->bpm.m_x;
	int m_y = this->bpm.m_y;
	int x = this->m_CustomSideLenthMap[this->m_indexNow - 1].first;
	int y = this->m_CustomSideLenthMap[this->m_indexNow - 1].second;
	if (ch == 'j' || ch == 'J')
	{
		this->m_CustomPutStatus = ch;
		this->m_isPut = true;
	}
	else if (ch == 'k' || ch == 'K')
	{
		this->m_CustomPutStatus = ch;
		this->m_isPut = true;
	}
	else if (ch == 'l' || ch == 'L')
	{
		this->m_CustomPutStatus = ch;
		this->m_isPut = true;
	}
	else if (ch == 'i' || ch == 'I')
	{
		this->m_CustomPutStatus = ch;
		this->m_CustomDelect = true;
	}
	else if (this->m_isPut) //放置系统
	{
		if (ch == 'w' || ch == 'W' || ch == 's' || ch == 'S' || ch == 'a' || ch == 'A' || ch == 'd' || ch == 'D')
		{
			this->m_Map[m_y][m_x] = "  ";
			if (ch == 'w' || ch == 'W')
			{
				if (m_y - 1 >= 0 && this->m_Map[m_y - 1][m_x] == "  ")
				{
					// J 放箱子  K 放墙  L 放口 I 破
					if (this->m_CustomPutStatus == 'j' || this->m_CustomPutStatus == 'J')
					{
						this->m_Map[m_y - 1][m_x] = "箱";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'k' || this->m_CustomPutStatus == 'K')
					{
						this->m_Map[m_y - 1][m_x] = "墙";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'l' || this->m_CustomPutStatus == 'L')
					{
						this->m_Map[m_y - 1][m_x] = "口";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'i' || this->m_CustomPutStatus == 'I')
					{
						this->m_Map[m_y - 1][m_x] = "  ";
						this->m_CustomPutStatus = ' ';
					}
				}
			}
			else if (ch == 's' || ch == 'S')
			{
				if (m_y + 1 < y && this->m_Map[m_y + 1][m_x] == "  ")
				{
					// J 放箱子  K 放墙  L 放口
					if (this->m_CustomPutStatus == 'j' || this->m_CustomPutStatus == 'J')
					{
						this->m_Map[m_y + 1][m_x] = "箱";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'k' || this->m_CustomPutStatus == 'K')
					{
						this->m_Map[m_y + 1][m_x] = "墙";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'l' || this->m_CustomPutStatus == 'L')
					{
						this->m_Map[m_y + 1][m_x] = "口";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'i' || this->m_CustomPutStatus == 'I')
					{
						this->m_Map[m_y + 1][m_x] = "  ";
						this->m_CustomPutStatus = ' ';
					}
				}
			}
			else if (ch == 'a' || ch == 'A')
			{
				if (m_x - 1 >= 0 && this->m_Map[m_y][m_x - 1] == "  ")
				{
					// J 放箱子  K 放墙  L 放口
					if (this->m_CustomPutStatus == 'j' || this->m_CustomPutStatus == 'J')
					{
						this->m_Map[m_y][m_x - 1] = "箱";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'k' || this->m_CustomPutStatus == 'K')
					{
						this->m_Map[m_y][m_x - 1] = "墙";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'l' || this->m_CustomPutStatus == 'L')
					{
						this->m_Map[m_y][m_x - 1] = "口";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'i' || this->m_CustomPutStatus == 'I')
					{
						this->m_Map[m_y][m_x - 1] = "  ";
						this->m_CustomPutStatus = ' ';
					}
				}
			}
			else if (ch == 'd' || ch == 'D')
			{
				if (m_x + 1 < x && this->m_Map[m_y][m_x + 1] == "  ")
				{
					// J 放箱子  K 放墙  L 放口
					if (this->m_CustomPutStatus == 'j' || this->m_CustomPutStatus == 'J')
					{
						this->m_Map[m_y][m_x + 1] = "箱";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'k' || this->m_CustomPutStatus == 'K')
					{
						this->m_Map[m_y][m_x + 1] = "墙";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'l' || this->m_CustomPutStatus == 'L')
					{
						this->m_Map[m_y][m_x + 1] = "口";
						this->m_CustomPutStatus = ' ';
					}
					else if (this->m_CustomPutStatus == 'i' || this->m_CustomPutStatus == 'I')
					{
						this->m_Map[m_y][m_x + 1] = "  ";
						this->m_CustomPutStatus = ' ';
					}
				}
			}
			this->m_isPut = false;
		}
	}
	else if (this->m_CustomDelect)
	{
		if (ch == 'w' || ch == 'W' || ch == 's' || ch == 'S' || ch == 'a' || ch == 'A' || ch == 'd' || ch == 'D')
		{
			this->m_Map[m_y][m_x] = "  ";
			if (ch == 'w' || ch == 'W')
			{
				if (m_y - 1 >= 0 && this->m_Map[m_y - 1][m_x] != "  ")
				{
					if (this->m_CustomPutStatus == 'i' || this->m_CustomPutStatus == 'I')
					{
						this->m_Map[m_y - 1][m_x] = "  ";
					}
				}
			}
			else if (ch == 's' || ch == 'S')
			{
				if (m_y + 1 < y && this->m_Map[m_y + 1][m_x] != "  ")
				{
					if (this->m_CustomPutStatus == 'i' || this->m_CustomPutStatus == 'I')
					{
						this->m_Map[m_y + 1][m_x] = "  ";
					}
				}
			}
			else if (ch == 'a' || ch == 'A')
			{
				if (m_x - 1 >= 0 && this->m_Map[m_y][m_x - 1] != "  ")
				{
					if (this->m_CustomPutStatus == 'i' || this->m_CustomPutStatus == 'I')
					{
						this->m_Map[m_y][m_x - 1] = "  ";
					}
				}
			}
			else if (ch == 'd' || ch == 'D')
			{
				if (m_x + 1 < x && this->m_Map[m_y][m_x + 1] != "  ")
				{
					if (this->m_CustomPutStatus == 'i' || this->m_CustomPutStatus == 'I')
					{
						this->m_Map[m_y][m_x + 1] = "  ";
					}
				}
			}
			this->m_CustomDelect = false;
		}
	}
	else if (!this->m_isPut && !this->m_CustomDelect)
	{
		if (ch == 'w' || ch == 'W' || ch == 's' || ch == 'S' || ch == 'a' || ch == 'A' || ch == 'd' || ch == 'D')
		{
			this->m_Map[m_y][m_x] = "  ";
			if (ch == 'w' || ch == 'W')
			{
				if (m_y - 1 >= 0 && this->m_Map[m_y - 1][m_x] == "  ")
				{
					m_y--;
				}
				else if (m_y - 2 >= 0 && this->m_Map[m_y - 1][m_x] != "  " && this->m_Map[m_y - 2][m_x] == "  ")
				{
					this->m_Map[m_y - 2][m_x] = this->m_Map[m_y - 1][m_x];
					this->m_Map[m_y - 1][m_x] = "  ";
					m_y--;
				}
			}
			else if (ch == 's' || ch == 'S')
			{
				if (m_y + 1 < y && this->m_Map[m_y + 1][m_x] == "  ")
				{
					m_y++;
				}
				else if (m_y + 2 < y && this->m_Map[m_y + 1][m_x] != "  " && this->m_Map[m_y + 2][m_x] == "  ")
				{
					this->m_Map[m_y + 2][m_x] = this->m_Map[m_y + 1][m_x];
					this->m_Map[m_y + 1][m_x] = "  ";
					m_y++;
				}
			}
			else if (ch == 'a' || ch == 'A')
			{
				if (m_x - 1 >= 0 && this->m_Map[m_y][m_x - 1] == "  ")
				{
					m_x--;
				}
				else if (m_x - 2 >= 0 && this->m_Map[m_y][m_x - 1] != "  " && this->m_Map[m_y][m_x - 2] == "  ")
				{
					this->m_Map[m_y][m_x - 2] = this->m_Map[m_y][m_x - 1];
					this->m_Map[m_y][m_x - 1] = "  ";
					m_x--;
				}
			}
			else if (ch == 'd' || ch == 'D')
			{
				if (m_x + 1 < x && this->m_Map[m_y][m_x + 1] == "  ")
				{
					m_x++;
				}
				else if (m_x + 2 < x && this->m_Map[m_y][m_x + 1] != "  " && this->m_Map[m_y][m_x + 2] == "  ")
				{
					this->m_Map[m_y][m_x + 2] = this->m_Map[m_y][m_x + 1];
					this->m_Map[m_y][m_x + 1] = "  ";
					m_x++;
				}
			}
		}
	}
	this->bpm.m_x = m_x;
	this->bpm.m_y = m_y;
	this->m_Map[m_y][m_x] = "我";

	/*cout << "m_x = " << m_x << "  m_y = " << m_y << "  ch = " << ch << endl;
	system("pause");*/

	/*this->CustomSaveDestination();*/

}

//“口”的保存
void PushBoxManager::CustomSaveDestination()
{
	vector<pair<int, int>> v;
	v = this->m_CustomDesitnation[this->m_indexNow - 1];
	int count = v.size();

	for (vector<pair<int, int>>::iterator it = v.begin(); it != v.end(); it++)
	{
		//cout << it->first << "  " << it->second << endl;
		//system("pause");
		if (this->m_Map[it->second][it->first] == "  ")this->m_Map[it->second][it->first] = "口";
		if (this->m_Map[it->second][it->first] == "箱")count--;
	}
	if (count == 0)this->isWin = true;
}

//读取自定义地图的系统   可以读取未完成的也可以读取自定义所有地图
void PushBoxManager::CustomReadSystem(string FileName,bool IsCustomMapRead)
{
	ifstream ifs(FileName, ios::in);
	if (!ifs.is_open()||ifs.eof())return;
	
	char ch;
	ifs >> ch;
	if (ch == EOF)return;

	ifs.putback(ch);
	if (IsCustomMapRead)this->m_isCustomMap = true;
	else this->m_isCustonUnfinishMap = true;

	string s;
	while (ifs >> s)
	{
		int pos1 = s.find(",");
		int pos2 = s.find(",", pos1 + 1);
		int pos3 = s.find(",", pos2 + 1);
		string temp1 = s.substr(0, pos1);
		string temp2 = s.substr(pos1 + 1, pos2 - pos1 - 1);
		string temp3 = s.substr(pos2 + 1, pos3 - pos2 - 1);
		int x = 0, y = 0, index = 0;
		x = this->StringTurnInt(temp1);
		y = this->StringTurnInt(temp2);
		index = this->StringTurnInt(temp3);

		this->m_CustomSideLenthMap.insert(make_pair(index - 1,make_pair(x, y)));

		vector<vector<string>>v;
		vector<pair<int, int>> Dest;
		for (int i = 0; i < y; i++)
		{
			vector<string> v1;
			for (int j = 0; j < x; j++)
			{
				ifs >> ch;
				if (ch == 'W')v1.push_back("墙");
				else if (ch == 'O')v1.push_back("  ");
				else if (ch == '#')
				{
					Dest.push_back(make_pair(j, i));
					v1.push_back("口");
				}
				else if (ch == 'B')v1.push_back("箱");
				else if (ch == '@')
				{
					//“我”的坐标存储
					if (IsCustomMapRead)this->m_CustomPusher.insert(make_pair(index - 1, make_pair(j,i)));
					else this->m_CustomUnfinishPusher.insert(make_pair(index - 1, make_pair(j, i)));
					v1.push_back("我");
				}
			}
			v.push_back(v1);
		}

		//“口”的坐标存储
		if (IsCustomMapRead)this->m_CustomDesitnation.insert(make_pair(index - 1, Dest));

		//自定义地图存储
		if (IsCustomMapRead)this->m_CustomAllMap.insert(make_pair(index - 1, v));
		else this->m_CustomUnfinishMap.insert(make_pair(index - 1, v));

		this->m_CustomMapCount = index;
		////测试代码
		//this->m_Map = v;
		//this->CustomPrintMap();
		//system("pause");
	/*	cout << this->m_CustomPusher[index - 1].first << endl;
		cout << this->m_CustomPusher[index - 1].second << endl;
		system("pause");*/

		index++;
	}
	ifs.close();
}

//字符串数字转成整形数字
int PushBoxManager::StringTurnInt(string s)
{
	int res = 0;
	for (int i = 0; i < s.size(); i++)
	{
		res *= 10;
		res += s[i] - '0';
	}
	return res;
}

//写入自定义地图  还可以写入未完成的
void PushBoxManager::CustomMapInFiles(string s, map<int, vector<vector<string>>> v)
{
	ofstream ofs;
	ofs.open(s, ios::trunc | ios::out);

	for (map<int, vector<vector<string>>>::iterator it1 = v.begin(); it1 != v.end(); it1++)
	{
		int index = it1->first + 1;
		ofs << this->m_CustomSideLenthMap[index - 1].first << "," << this->m_CustomSideLenthMap[index - 1].second << ","
			<< index << "," << endl;
		for (vector<vector<string>>::iterator it2 = (*it1).second.begin(); it2 != (*it1).second.end(); it2++)
		{
			for (vector<string>::iterator it3 = it2->begin(); it3 != it2->end(); it3++)
			{
				if (*it3 == "墙")ofs << 'W';
				if (*it3 == "  ")ofs << 'O';
				if (*it3 == "口")ofs << '#';
				if (*it3 == "箱")ofs << 'B';
				if (*it3 == "我")ofs << '@';
			}
			ofs << endl;
		}
	}

	ofs.close();
}

//自定义关卡
void PushBoxManager::CustomCheckpoint()
{
	while (true)
	{
		system("cls");
		this->CustomMenu();

		cout << "请选择功能：" << endl;
		char ch;
		cin >> ch;

		int chioce = 0;
		//访问输入错误造成bug
		if (ch >= '0' && ch <= '2')
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
		case 1:  // 1、自创关卡
			this->CustomCheckpointOne();
			break;
		case 2:  // 2、未完关卡
			this->CustomUnfinishCheckpoint();
			break;
		case 0:  // 0、返回
			system("cls");
			return;
		default:
			break;
		}
	}
}

//自定义关卡的菜单
void PushBoxManager::CustomMenu()
{
	cout << "************************************" << endl;
	cout << "************************************" << endl;
	cout << "********                    ********" << endl;
	cout << "********     1、自创关卡    ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********     2、未完关卡    ********" << endl;
	cout << "********                    ********" << endl;
	cout << "********     0、返回        ********" << endl;
	cout << "********                    ********" << endl;
	cout << "************************************" << endl;
	cout << "************************************" << endl;
}

// 1、自创关卡  自定义关卡
void PushBoxManager::CustomCheckpointOne()
{
	while (true)
	{
		system("cls");
		this->CustomMenu();
		cout << endl;
		cout << "------------------------------------" << endl;
		if (this->m_CustomAllMap.empty())
		{
			cout << "没有自定义的关卡！" << endl;
			system("pause");
			return;
		}
		int count = 0;
		for (map<int, vector<vector<string>>>::iterator it = this->m_CustomAllMap.begin(); it != this->m_CustomAllMap.end(); it++)
		{
			cout << "\t" << it->first + 1;
			count++;
			if (count % 4 == 0)cout << endl;
		}
		cout << endl;
		cout << "------------------------------------" << endl;

		cout << "请选择关卡：           按数字 0 返回" << endl;
		int select = 0;
		string s;
		cin >> s;
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] < '0' || s[i]>'9')
			{
				system("cls");
				return;

			}
			select *= 10;
			select += s[i] - '0';
		}
		if (select == 0)
		{
			system("cls");
			return;
		}

		map<int, vector<vector<string>>>::iterator it = this->m_CustomAllMap.find(select - 1);
		if (it == this->m_CustomAllMap.end())continue;

		this->m_Map = this->m_CustomAllMap[select - 1];
		this->bpm.m_x = this->m_CustomPusher[select - 1].first;
		this->bpm.m_y = this->m_CustomPusher[select - 1].second;

		this->m_indexNow = select;
		this->CustomPlayGameMap();
		if (this->isWin)
		{
			this->isWin = false;
			return;
		}
	}
}

//2、自定义地图正式开始
void PushBoxManager::CustomPlayGameMap()
{
	while (true)
	{
		system("cls");
		this->CustomPrintMap();

		cout << "                   按数字0键返回" << endl;
		cout << "                   按M键重新开始" << endl;
		cout << "                   按B键保存返回" << endl;

		char ch = _getch();

		if (ch == '0') return;
		else if (ch == 'm' || ch == 'M')
		{
			this->m_Map = this->m_CustomAllMap[this->m_indexNow - 1];
			this->bpm.m_x = this->m_CustomPusher[this->m_indexNow - 1].first;
			this->bpm.m_y = this->m_CustomPusher[this->m_indexNow - 1].second;
			continue;
		}
		else if (ch == 'b' || ch == 'B')
		{
			this->m_CustomUnfinishMap[this->m_indexNow - 1] = this->m_Map;
			this->m_CustomUnfinishPusher[this->m_indexNow - 1] = make_pair(this->bpm.m_x, this->bpm.m_y);
			return;
		}
		


		if (ch == 'w' || ch == 'W' || ch == 's' || ch == 'S' || ch == 'a' || ch == 'A' || ch == 'd' || ch == 'D')
		{
			this->CustomPlayGameMove(ch);
		}
		if (this->isWin)
		{
			system("cls");
			this->CustomPrintMap();
			cout << "恭喜你通过此关卡！" << endl;
			system("pause");
			return;
		}
	}
}

//2、自定义地图正式开始的移动系统
void PushBoxManager::CustomPlayGameMove(char ch)
{
	int m_x = this->bpm.m_x;
	int m_y = this->bpm.m_y;
	int x = this->m_CustomSideLenthMap[this->m_indexNow - 1].first;
	int y = this->m_CustomSideLenthMap[this->m_indexNow - 1].second;

	if (ch == 'w' || ch == 'W' || ch == 's' || ch == 'S' || ch == 'a' || ch == 'A' || ch == 'd' || ch == 'D')
	{
		this->m_Map[m_y][m_x] = "  ";
		if (ch == 'w' || ch == 'W')
		{
			if (m_y - 1 >= 0 && this->m_Map[m_y - 1][m_x] == "  ")m_y--;
			else if (m_y - 1 >= 0 && this->m_Map[m_y - 1][m_x] == "口") m_y--;
			else if (m_y - 2 >= 0 && this->m_Map[m_y - 1][m_x] == "箱" && (this->m_Map[m_y - 2][m_x] == "  "|| this->m_Map[m_y - 2][m_x] == "口"))
			{
				this->m_Map[m_y - 2][m_x] = this->m_Map[m_y - 1][m_x];
				this->m_Map[m_y - 1][m_x] = "  ";
				m_y--;
			}
		}
		else if (ch == 's' || ch == 'S')
		{
			if (m_y + 1 < y && this->m_Map[m_y + 1][m_x] == "  ")m_y++;
			else if (m_y + 1 < y && this->m_Map[m_y + 1][m_x] == "口")m_y++;
			else if (m_y + 2 < y && this->m_Map[m_y + 1][m_x] == "箱" && (this->m_Map[m_y + 2][m_x] == "  " || this->m_Map[m_y + 2][m_x] == "口"))
			{
				this->m_Map[m_y + 2][m_x] = this->m_Map[m_y + 1][m_x];
				this->m_Map[m_y + 1][m_x] = "  ";
				m_y++;
			}
		}
		else if (ch == 'a' || ch == 'A')
		{
			if (m_x - 1 >= 0 && (this->m_Map[m_y][m_x - 1] == "  "|| this->m_Map[m_y][m_x - 1] == "口"))
			{
				m_x--;
			}
			else if (m_x - 2 >= 0 && this->m_Map[m_y][m_x - 1] == "箱" && (this->m_Map[m_y][m_x - 2] == "  " || this->m_Map[m_y][m_x - 2] == "口"))
			{
				this->m_Map[m_y][m_x - 2] = this->m_Map[m_y][m_x - 1];
				this->m_Map[m_y][m_x - 1] = "  ";
				m_x--;

			}
		}
		else if (ch == 'd' || ch == 'D')
		{
			if (m_x + 1 < x && (this->m_Map[m_y][m_x + 1] == "  "|| this->m_Map[m_y][m_x + 1] == "口"))
			{
				m_x++;
			}
			else if (m_x + 2 < x && this->m_Map[m_y][m_x + 1] == "箱" && (this->m_Map[m_y][m_x + 2] == "  " || this->m_Map[m_y][m_x + 2] == "口"))
			{
				this->m_Map[m_y][m_x + 2] = this->m_Map[m_y][m_x + 1];
				this->m_Map[m_y][m_x + 1] = "  ";
				m_x++;
			}
		}
	}

	this->bpm.m_x = m_x;
	this->bpm.m_y = m_y;
	this->m_Map[m_y][m_x] = "我";

	//保持“口”的存在
	this->CustomSaveDestination();
}

//修改自定义地图
void PushBoxManager::CustomMapModify()
{
	while (true)
	{
		system("cls");
		this->CustomMapMenu();
		this->m_indexNow = this->m_CustomMapCount;  //////////////////

		cout << endl;
		cout << "------------------------------------" << endl;
		if (this->m_CustomAllMap.empty())
		{
			cout << "没有自定义地图！" << endl;
			system("pause");
			system("cls");
			return;
		}
		int count = 0;
		for (map<int, vector<vector<string>>>::iterator it = this->m_CustomAllMap.begin(); it != this->m_CustomAllMap.end(); it++)
		{
			cout << "\t" << it->first + 1;
			count++;
			if (count % 4 == 0)cout << endl;
		}
		cout << endl;
		cout << "------------------------------------" << endl;

		cout << "请选择要修改关卡：     按数字 0 返回" << endl;
		int select = 0;
		string s;
		cin >> s;
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] < '0' || s[i]>'9')
			{
				system("cls");
				return;

			}
			select *= 10;
			select += s[i] - '0';
		}
		if (select == 0)
		{
			system("cls");
			return;
		}

		map<int, vector<vector<string>>>::iterator it = this->m_CustomAllMap.find(select - 1);
		if (it == this->m_CustomAllMap.end())continue;

		this->m_indexNow = select;

		this->m_Map = this->m_CustomAllMap[this->m_indexNow - 1];
		this->bpm.m_x = this->m_CustomPusher[this->m_indexNow - 1].first;
		this->bpm.m_y = this->m_CustomPusher[this->m_indexNow - 1].second;
		this->CustomMapMain();

		this->m_CustomPusher[this->m_indexNow - 1] = make_pair(this->bpm.m_x, this->bpm.m_y);
		this->m_CustomAllMap[this->m_indexNow - 1] = this->m_Map;
		this->CustomMapSaveDestination();
	}
}

//保存自定义完成后的“口”的坐标
void PushBoxManager::CustomMapSaveDestination()
{
	vector<pair<int, int>> v;
	int y = 0;
	for (vector<vector<string>>::iterator it = this->m_Map.begin(); it != this->m_Map.end(); it++)
	{
		int x = 0;
		for (vector<string>::iterator it1 = it->begin(); it1 != it->end(); it1++)
		{
			if (*it1 == "口")v.push_back(make_pair(x, y));
			x++;
		}
		y++;
	}
	this->m_CustomDesitnation[this->m_indexNow - 1] = v;
	//for (vector<pair<int, int>>::iterator it = v.begin(); it != v.end(); it++)
	//{
	//	cout << it->first << "  " << it->second << endl;
	//}
	//system("pause");
}

//未完成的自定义关卡
void PushBoxManager::CustomUnfinishCheckpoint()
{
	while (true)
	{
		system("cls");
		this->CustomMenu();
		cout << endl;
		cout << "------------------------------------" << endl;
		if (this->m_CustomUnfinishMap.empty())
		{
			cout << "没有未完成的自定义的关卡！" << endl;
			system("pause");
			return;
		}
		int count = 0;
		for (map<int, vector<vector<string>>>::iterator it = this->m_CustomUnfinishMap.begin(); it != this->m_CustomUnfinishMap.end(); it++)
		{
			cout << "\t" << it->first + 1;
			count++;
			if (count % 4 == 0)cout << endl;
		}
		cout << endl;
		cout << "------------------------------------" << endl;

		cout << "请选择关卡：           按数字 0 返回" << endl;
		int select = 0;
		string s;
		cin >> s;
		for (int i = 0; i < s.size(); i++)
		{
			if (s[i] < '0' || s[i]>'9')
			{
				system("cls");
				return;

			}
			select *= 10;
			select += s[i] - '0';
		}
		if (select == 0)
		{
			system("cls");
			return;
		}

		map<int, vector<vector<string>>>::iterator it = this->m_CustomUnfinishMap.find(select - 1);
		if (it == this->m_CustomUnfinishMap.end())continue;

		this->m_Map = this->m_CustomUnfinishMap[select - 1];
		this->bpm.m_x = this->m_CustomUnfinishPusher[select - 1].first;
		this->bpm.m_y = this->m_CustomUnfinishPusher[select - 1].second;

		this->m_indexNow = select;
		this->CustomPlayGameMap();
		if (this->isWin)
		{
			this->m_CustomUnfinishMap.erase(select - 1);
			this->m_CustomUnfinishPusher.erase(select - 1);
			this->isWin = false;
			return;
		}
	}
}

//3、退出游戏
void PushBoxManager::exitGame()
{
	cout << "确定退出游戏吗？" << endl;
	cout << "1、确定" << endl;
	cout << "2、返回" << endl;

	char ch;
	cin >> ch;

	int select = 0;
	if (ch >= '1' && ch <= '3')  //访问输入错误造成bug
	{
		select = ch - '0';
	}

	if (select == 1)
	{
		this->WriteFilesSaveCheckpoint();  //保存玩家闯关未完成的记录
		this->CustomMapInFiles("CustomAllMap.txt", this->m_CustomAllMap); //保存地图到文件中

		this->CustomMapInFiles("CustomUnfinishMap.txt", this->m_CustomUnfinishMap);  //保存自创关卡的未完成的关卡

		exit(0);
	}
	system("pause");
	system("cls");
}

//析构函数
PushBoxManager::~PushBoxManager() {};