#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <deque>
#include "boxPusher.h"
using namespace std;

class PushBoxManager
{
public:
	//构造函数
	PushBoxManager();

	//首菜单功能
	void showMainMenu();

	//开始游戏
	void startGame();

	//游戏菜单
	void showGameMenu();

	//现在正在第几关
	int m_indexNow = 0;

	//开始游戏 中 1、系统关卡
	void Chechpoint();

	//关卡正式开始
	void StartChechpoint(int n);  //参数是第几关卡

	//存放全部地图的容器
	map<int, vector<vector<string>>> m_SumMap;

	//存放全部地图的边长的容器
	map<int, pair<int, int>> m_SumMapLenth;

	//存取当前使用地图的容器
	vector<vector<string>> m_Map;


	//打印地图
	void PrintMap();

	//创建推箱子的人
	BoxPusher bpm;

	//推箱子的人所有地图的坐标容器
	map<int, pair<int, int>> m_BoxPusherSites;
	//推箱子的人所有地图的坐标容器  未结束的
	map<int, pair<int, int>> m_UnfinishBoxPusherSites;

	//将推箱人放入地图中
	void PutBoxPusherMap();

	// 我  移动系统 推箱功能
	void marchPusher(char ch);

	//推箱子的目的地---口
	//存放所有地图“口”的坐标
	map<int,vector<pair<int,int>>> m_SumDestination;  //第一个数是 x轴  第二个数是 y轴  存放所有地图“口”的坐标


	//存取当前使用地图的“口”的容器
	vector< pair<int, int>> m_Destination;

	//将口放入地图
	void PutDesitnation();

	//是否胜利
	bool isWin = false;

	//读取文件信息
	void readFiles();

	//文件是否存在
	bool isMapFile = false;

	//保存玩家没有结束的关卡
	void SaveCheckpoint(int n,int x,int y);

	//保存玩家没有结束的关卡的容器,地图
	map<int, vector<vector<string>>> m_SaveCheckpoint;

	//完全记录保存玩家没有结束的关卡的记录放入文件
	void WriteFilesSaveCheckpoint();

	//提前加载  //完全记录保存玩家没有结束的关卡的容器
	void LoadingRecordPlay();

	//玩家未完成
	bool isPlayUnfinish = false;

	//自定义地图
	void CustomSystem();
	
	//自定义地图菜单
	void CustomMapMenu();

	//存储所有自定义地图
	map<int, vector<vector<string>>> m_CustomAllMap;

	//存储所有未完成自定义地图
	map<int, vector<vector<string>>> m_CustomUnfinishMap;

	//存储所有自定义地图的“我”的坐标
	map<int, pair<int, int>> m_CustomPusher;

	//存储未完成的自定义地图的“我”的坐标
	map<int, pair<int, int>> m_CustomUnfinishPusher;

	//存储所有自定义地图的“口”的坐标
	map<int, vector<pair<int, int>>> m_CustomDesitnation;

	//存储所有自定义地图边长
	map<int, pair<int, int>> m_CustomSideLenthMap;

	//存储自定义地图数
	int m_CustomMapCount = 0;

	//1、创建地图___自定义地图
	void CreateCustomMap();

	//自定义地图边长
	void CustomMapGameSideLenth(int x,int y);

	//开始自定义地图内容
	void CustomMapMain();

	//自定义地图的打印
	void CustomPrintMap();

	//自定义地图的移动系统和放置系统
	void CustomMoveAndPut(char ch);

	//是否进入放置状态
	bool m_isPut = false;

	//放置状态中三种可能  放箱 J   放墙 K   放口 L 
	char m_CustomPutStatus = ' ';

	//“口”的保存
	void CustomSaveDestination();

	//删除物品的状态
	bool m_CustomDelect = false;

	//读取自定义地图的系统   可以读取未完成的也可以读取自定义所有地图
	void CustomReadSystem(string FileName, bool IsCustomMapRead = true);

	//是否有自定义地图
	bool m_isCustomMap = false;

	//是否有未完成的自定义关卡
	bool m_isCustonUnfinishMap = false;

	//字符串数字转成整形数字
	int StringTurnInt(string s);

	//写入自定义地图  还可以写入问完成的
	void CustomMapInFiles(string s, map<int, vector<vector<string>>> v);

	//自定义关卡
	void CustomCheckpoint();

	//自定义关卡的菜单
	void CustomMenu();

	// 1、自创关卡  自定义关卡
	void CustomCheckpointOne();

	//2、自定义地图正式开始
	void CustomPlayGameMap();

	//2、自定义地图正式开始的移动系统
	void CustomPlayGameMove(char ch);

	//3、删除地图   自定义地图
	void CustomDelectMap();

	//修改自定义地图
	void CustomMapModify();

	//记录自定义完成后的“口”的坐标
	void CustomMapSaveDestination();

	//未完成的自定义关卡
	void CustomUnfinishCheckpoint();

	//3、退出游戏
	void exitGame();

	//析构函数
	~PushBoxManager();
};