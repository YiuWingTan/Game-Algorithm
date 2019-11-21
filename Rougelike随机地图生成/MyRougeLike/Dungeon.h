#pragma once
#include<vector>

///<summary>
///表示地图上的一个方形区域
///</summary>
struct Rect
{
	Rect(int _x, int _y, int _width, int _height) :x(_x),y(_y),width(_width),height(_height) {}
	Rect() {}
	int x, y;
	int width, height;
};

///<summary>
///表示地图上的一个房间
///</summary>
struct RoomRect:public Rect
{
	static const int maxRoomWidth = 6;
	static const int maxRoomHeight = 6;
	static const int minRoomWidth = 3;
	static const int minRoomHeight = 3;
};

///<summary>
///表示地图上的一个走廊区域
///</summary>
struct CorridorRect:public Rect
{
	static const int maxCorridorLength = 6;
	static const int minCooridorLength = 3;
};

///<summary>
///可以创建地图或者是走廊的方向
///</summary>
enum Direction
{
	North = 0,
	South,
	West,
	East
};

///<summary>
///地图上的每一个方块的类型
///</summary>
enum ChuckType
{
	unused = 0,
	Floor,
	OpenDoor,
	CloseDoor,
	Wall,
	UpStair,
	DownStair,
	Corridor,
};


///<summary>
///将地图上的砖块类型映射到字符的方法
///</summary>
static char MapChuckTypeToChar(ChuckType type);


///<summary>
///代表一个地牢的类
///</summary>
class Dungeon
{
private:
	std::vector<RoomRect> _rooms;
	std::vector<CorridorRect> _corridor;
	std::vector<Rect> _sides;
	std::vector<ChuckType> map;
	int _width;
	int _height;
	int _elementNum;
	int _elementCount;


public:
	Dungeon(int width,int height, int elementNum);

	///<summary>
	///生成地图的方法
	///</summary>
	void Generator();
	///<summary>
	///打印地图的方法
	///</summary>
	void PrintMap();
	///<summary>
	///清空一个地图
	///</summary>
	void Reset();
private:

	///<summary>
	///在rect所表示的范围内放置元素
	///成功放置返回true，失败返回false
	///</summary>
	bool placeRect(const Rect& rect);
	///<summary>
	///在x，y的位置上以dir表示的方向创建一个房间。
	///成功时返回true ，否则返回false
	///</summary>
	bool placeRoom(int x, int y, Direction dir, bool isFirst = false);
	///<summary>
	///地图上的两个随机房间放置楼梯
	///</summary>
	bool placeStair();
	///<summary>
	///创建走廊的方法.x,y表示创建边界
	///</summary>
	bool placeCorridor(int x,int y,Direction dir);
	///<summary>
	///判断一个点是否超越了地图的边界
	///</summary>
	bool isOutRange(int x,int y);
	///<summary>
	///判断一个矩形是否超越了地图的边界
	///</summary>
	bool isOutRange(const Rect &rect);
	///<summary>
	///在地图上创建一个随即类型的元素
	///</summary>
	bool createElement();
	///<summary>
	///在x，y的位置上以dir表示的方向创建一个随机元素。
	///</summary>
	bool createElement(int x, int y, Direction dir);
	///<summary>
	///在x，y的位置上以type类型放置一个砖块
	///</summary>
	void fillMap(int x, int y, ChuckType type);
	///<summary>
	///获取在x,y位置上的一个砖块的类型
	///</summary>
	ChuckType getChuckType(int x,int y);

};
