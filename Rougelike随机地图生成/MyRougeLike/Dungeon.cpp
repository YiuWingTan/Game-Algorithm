#include"Dungeon.h"
#include"RandomHelper.h"
#include<iostream>

Dungeon::Dungeon(int width, int height, int elementNum) :
	_width(width), _height(height), _elementNum(elementNum), _elementCount(0)
{
	map.resize(_width*_height,ChuckType::unused);
}

void Dungeon::Generator()
{ 
	int x = _width / 2;
	int y = _height / 2;

	Direction dir = static_cast<Direction>(RandomHelper::RandomRangeInt(0,3));

	if (!placeRoom(x, y, dir, true))
	{
		std::cout << "无法生成第一个方块"<<std::endl;
		return;
	}
	_elementCount = 1;
	
	for (int i = 0; i < _elementNum; i++)
	{
		if (!createElement())
		{
			std::cout << "无法再创建更多的元素了"<<std::endl;
			return;
		}
		else _elementCount++;
	}

	//放置楼梯
	placeStair();

	//放置物体

}

void Dungeon::PrintMap()
{
	//将地图绘制出来

	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			char c = MapChuckTypeToChar(map[y*_width+x]);
			std::cout << c;
		}
		std::cout << std::endl;
	}


}

void Dungeon::Reset()
{
	_elementCount = 0;
	_rooms.clear();
	_corridor.clear();
	_sides.clear();
	map.clear();
	map.resize(_width*_height,ChuckType::unused);
}

bool Dungeon::placeRect(const Rect&rect)
{
	if (isOutRange(rect)) return false;

	
	for (int y = 0; y < rect.height; y++)
	{
		for (int x = 0; x < rect.width; x++)
		{
			//判断是否有位置来创建房间
			if (getChuckType(rect.x + x, rect.y + y) != ChuckType::unused)
				return false;
		}
	}

	//进行创建

	for (int y = -1; y <= rect.height; y++)
	{
		for (int x = -1; x <= rect.width; x++)
		{

			if (x == -1 || y == -1 || y == rect.height || x == rect.width)
			{
				//创建围墙
				//当要创建围墙的位置上没有东西的时候才创建围墙
				if (getChuckType(rect.x + x, rect.y + y) == ChuckType::unused)
				{
					fillMap(rect.x+x,rect.y+y,ChuckType::Wall);
				}
			}
			else {

				//创建地板
				fillMap(rect.x+x,rect.y+y,ChuckType::Floor);
			}
		}
	}

	return true;
}

bool Dungeon::placeRoom(int x, int y, Direction dir,bool isFirst)
{
	RoomRect room;
	room.width = RandomHelper::RandomRangeInt(RoomRect::minRoomWidth,RoomRect::maxRoomWidth);
	room.height = RandomHelper::RandomRangeInt(RoomRect::minRoomHeight,RoomRect::maxRoomHeight);

	int offset = 0;

	/*if (isFirst) {
		room.x -= room.width / 2;
		room.y -= room.height / 2;

		if (placeRect(room))
		{
			_sides.emplace_back();
		}

	}
	else */if (dir == Direction::North)
	{
		//往北生长
		room.x =x -  room.width / 2;
		room.y =y - (room.height-1);
	}
	else if (dir == Direction::South)
	{
		//往南生长
		room.x =x - room.width / 2;
		room.y =y + 1;
	}
	else if (dir == Direction::East)
	{
		//往东生长
		room.x =x+ 1;
		room.y =y -  room.height / 2;
	}
	else if (dir == Direction::West)
	{
		//往西生长
		room.x =x - (room.width-1);
		room.y =y -  room.height / 2;
	}


	if (placeRect(room))
	{
		int createNum = 1;
		while (createNum>0)
		{
			Direction randomCreateDir = static_cast<Direction>(RandomHelper::RandomInt(3));
			//成功生成了房间
			if (randomCreateDir == Direction::South&&dir != Direction::North)
			{
				_sides.emplace_back(room.x, room.y + room.height, room.width, 1);
				_sides.emplace_back(room.x - 1, room.y, 1, room.height);
				_sides.emplace_back(room.x + room.width, room.y, 1, room.height);
				createNum--;
			}
			if (randomCreateDir == Direction::North&&dir != Direction::South)
			{
				_sides.emplace_back(room.x, room.y - 1, room.width, 1);
				_sides.emplace_back(room.x - 1, room.y, 1, room.height);
				_sides.emplace_back(room.x + room.width, room.y, 1, room.height);
				createNum--;
			}
			if (randomCreateDir == Direction::West&&dir != Direction::East)
			{
				_sides.emplace_back(room.x - 1, room.y, 1, room.height);
				_sides.emplace_back(room.x, room.y + room.height, room.width, 1);
				_sides.emplace_back(room.x, room.y - 1, room.width, 1);
				createNum--;
			}
			if (randomCreateDir == Direction::East&&dir != Direction::West)
			{
				_sides.emplace_back(room.x + room.width, room.y, 1, room.height);
				_sides.emplace_back(room.x, room.y + room.height, room.width, 1);
				_sides.emplace_back(room.x, room.y - 1, room.width, 1);
				createNum--;
			}
		}

		_rooms.push_back(room);

		return true;
	}

	return false;
}

bool Dungeon::placeStair()
{
	int r = RandomHelper::RandomInt(_rooms.size()-1);
	int x = RandomHelper::RandomRangeInt(_rooms[r].x,_rooms[r].x + (_rooms[r].width-1));
	int y = RandomHelper::RandomRangeInt(_rooms[r].y,_rooms[r].y + (_rooms[r].height-1));

	//创建下层楼梯

	fillMap(x,y,ChuckType::DownStair);


	 r = RandomHelper::RandomInt(_rooms.size() - 1);
	 x = RandomHelper::RandomRangeInt(_rooms[r].x, _rooms[r].x + (_rooms[r].width - 1));
	 y = RandomHelper::RandomRangeInt(_rooms[r].y, _rooms[r].y + (_rooms[r].height - 1));

	 //创建上层楼梯
	 fillMap(x, y, ChuckType::UpStair);

	 return true;
}

bool Dungeon::placeCorridor(int x, int y, Direction dir)
{
	CorridorRect corridor;
	int randomLength = RandomHelper::RandomRangeInt(CorridorRect::minCooridorLength,CorridorRect::maxCorridorLength);



	if (RandomHelper::RandomBernoulliBool())
	{
		//创建水平走廊
		corridor.width = randomLength;
		corridor.height = 1;
		if (dir == Direction::North)
		{
			corridor.y = y - 1;
			//往左偏
			if (RandomHelper::RandomBernoulliBool())
			{
				corridor.x = x - (randomLength - 1);
			}
		}
		else if (dir == Direction::South)
		{
			corridor.y = y + 1;
			if (RandomHelper::RandomBernoulliBool())
			{
				corridor.x = x - (randomLength - 1);
			}
		}
		else if (dir == Direction::East)
		{
			corridor.x = x+1;
			corridor.y = y;
		}
		else if (dir == Direction::West)
		{
			corridor.x =x - (randomLength - 1);
			corridor.y = y;
		}

	}
	else
	{
		//创建垂直走廊
		
		corridor.width = 1;
		corridor.height = randomLength;
		if (dir == Direction::North)
		{
			corridor.x = x;
			corridor.y =y - (randomLength - 1);
		}
		else if (dir == Direction::South)
		{
			corridor.x = x;
			corridor.y =y+ 1;
		}
		else if (dir == Direction::East)
		{
			corridor.x = x + 1;
			if (RandomHelper::RandomBernoulliBool())
			{
				corridor.y = y - (randomLength - 1);
			}

		}
		else if (dir == Direction::West)
		{
			corridor.x = x - 1;
			if (RandomHelper::RandomBernoulliBool())
			{
				corridor.y = y - (randomLength - 1);
			}
		}
	}

	if (placeRect(corridor))
	{
		int createNum = 1;
		while (createNum>0)
		{
			Direction randomCreateDir = static_cast<Direction>(RandomHelper::RandomInt(3));
			//成功生成了房间
			if (randomCreateDir == Direction::South&&dir != Direction::North)
			{
				_sides.emplace_back(corridor.x, corridor.y + corridor.height, corridor.width, 1);
				_sides.emplace_back(corridor.x - 1, corridor.y, 1, corridor.height);
				_sides.emplace_back(corridor.x + corridor.width, corridor.y, 1, corridor.height);
				createNum--;
			}
			if (randomCreateDir == Direction::North&&dir != Direction::South)
			{
				_sides.emplace_back(corridor.x, corridor.y - 1, corridor.width, 1);
				_sides.emplace_back(corridor.x - 1, corridor.y, 1, corridor.height);
				_sides.emplace_back(corridor.x + corridor.width, corridor.y, 1, corridor.height);
				createNum--;
			}
			if (randomCreateDir == Direction::West&&dir != Direction::East)
			{
				_sides.emplace_back(corridor.x - 1, corridor.y, 1, corridor.height);
				_sides.emplace_back(corridor.x, corridor.y + corridor.height, corridor.width, 1);
				_sides.emplace_back(corridor.x, corridor.y - 1, corridor.width, 1);
				createNum--;
			}
			if (randomCreateDir == Direction::East&&dir != Direction::West)
			{
				_sides.emplace_back(corridor.x + corridor.width, corridor.y, 1, corridor.height);
				_sides.emplace_back(corridor.x, corridor.y + corridor.height, corridor.width, 1);
				_sides.emplace_back(corridor.x, corridor.y - 1, corridor.width, 1);
				createNum--;
			}
		}
		_corridor.push_back(corridor);
		return true;
	}
	return false;
}

inline bool Dungeon::isOutRange(int x, int y)
{
	if (x<0 || y<0 || x>=_width|| y>=_height) return true;

	return false;
}

inline bool Dungeon::isOutRange(const Rect & rect)
{
	if (rect.x-1<0 || rect.y-1<0 || rect.x-1>=_width || rect.y-1>=_height) return true;
	if (rect.x + rect.width < 0 || rect.y + rect.height < 0 ||
		rect.x + rect.width >= _width  || rect.y + rect.height >= _height) return true;
	return false;
}

bool Dungeon::createElement()
{
	int n = _sides.size();
	//每创建一个就要进行一千次尝试
	for (int t = 0; t < 1000; t++)
	{
		if (_sides.empty())
			return false;
		int r = RandomHelper::RandomInt(_sides.size() - 1);
		int x = RandomHelper::RandomRangeInt(_sides[r].x, _sides[r].x + _sides[r].width - 1);
		int y = RandomHelper::RandomRangeInt(_sides[r].y, _sides[r].y + _sides[r].height - 1);
		//各个方向进行判断
		for (int i = 0; i < 4; i++)
		{
			//随机选择一个方向
			if (createElement(x, y, static_cast<Direction>(i)))
			{
				_sides.erase(_sides.begin() + r);
				return true;
			}
		}
	}
	return false;

}

bool Dungeon::createElement(int x, int y, Direction dir)
{
	//判断x，y是否越界

	if (isOutRange(x, y)) return false;

	int dx = 0, dy = 0;

	if (dir == Direction::North)
	{
		dy = 1;
	}
	else if (dir == Direction::South)
	{
		dy = -1;
	}
	else if (dir == Direction::East)
	{
		dx = -1;
	}
	else if (dir = Direction::West)
	{
		dx = 1;
	}

	if (isOutRange(x + dx, y + dy)) return false;

	ChuckType type = getChuckType(x + dx, y + dy);

	//如果后面的砖块不是地板就返回
	if (type != ChuckType::Floor&&type!=ChuckType::Corridor) return false;

	
	//使用伯努利分布来判断创建什么元素
	
	if (RandomHelper::RandomBernoulliBool())
	{
		//创建房间
		if (placeRoom(x, y, dir))
		{
			//为房间放置门
			if(getChuckType(x+dx,y+dy) == ChuckType::Floor)
				fillMap(x,y,ChuckType::CloseDoor);
			return true;
		}
		 
	}
	else {
		//创建走廊

		if (placeCorridor(x, y, dir))
		{

			//为走廊创建门
			if (getChuckType(x + dx, y + dy) == ChuckType::Floor)
			{
				fillMap(x,y,ChuckType::CloseDoor);
			}
			else if(getChuckType(x + dx, y + dy) == ChuckType::Corridor){
				fillMap(x,y,ChuckType::Corridor);
			}
			return true;
		}

	}

	return false;
}

void Dungeon::fillMap(int x, int y, ChuckType type)
{
	if (isOutRange(x, y)) throw "The x ，y is out of range";
	map[y*_width + x] = type;
	return;
}

inline ChuckType Dungeon::getChuckType(int x, int y)
{
	return map[y*_width+x];
}

char MapChuckTypeToChar(ChuckType type)
{
	switch (type)
	{
	case unused:
		return '.';
	case Floor:
		return ' ';
	case OpenDoor:
		return '-';
	case CloseDoor:
		return '+';
	case Wall:
		return '*';
	case UpStair:
		return '>';
	case DownStair:
		return '<';
	case Corridor:
		return ' ';
	}
	return '?';
}
