
/*

程序使用控制台的方式。
使用一个数组来进行模拟地图

*/

#include<iostream>
#include<vector>
#include<random>
using namespace std;


namespace RandomHelper
{
	static random_device rd;
	static mt19937 mt(rd());

	///<summary>
	///使用伯努利分布根据一个概率值生成一个布尔值
	///</summary>
	bool RandomBernoulliBool(float probablity = 0.5f)
	{
		std::bernoulli_distribution dist(probablity);

		return dist(mt);
	}


}

enum ChuckType
{
	None = -1,
	Wall = 0,
	Road
};
class MapGenerator;
typedef ChuckType (*Rule)(int _x, int _y,MapGenerator&_generator);

///<summary>
///将Chuck映射到字符
///用来在控制台上进行输出
///</summary>
char MapChuckTypeToChar(ChuckType type);


class MapGenerator
{
public:
	MapGenerator(int _width, int _height,float _initProbability) :mWidth(_width),mHeight(_height),mGenerateCount(0),mInitProbability(_initProbability)
	{
		mMap.resize(mWidth*mHeight,ChuckType::None);
	}
	void Init();//初始化的方法
	void Generate();
	void PrintMap();
	void SetRule(Rule _rule);
	int Width() { return mWidth; }
	int Height() { return mHeight; }
	int GenerateCount() { return mGenerateCount; }
	inline ChuckType GetChuckType(int _x, int _y) { return mMap[_y*mWidth+_x]; }
private:
	vector<ChuckType> mMap;
	Rule mRule;//表示生成所用的规则的指针
	int mWidth;
	int mHeight;
	int mGenerateCount;
	float mInitProbability;
};

void MapGenerator::Init()
{
	for (int y = 0; y < mHeight; y++) {

		for (int x = 0; x < mWidth; x++)
		{
			if (x <= 1 || x >= mWidth - 2 || y <=1 || y >= mHeight - 2)
			{
				mMap[y*mWidth + x] = ChuckType::Wall;
			}
			else
			{
				if (RandomHelper::RandomBernoulliBool(mInitProbability))
				{
					mMap[y*mWidth + x] = ChuckType::Wall;
				}
				else {
					mMap[y*mWidth + x] = ChuckType::Road;
				}


			}
		}
	}
}
void MapGenerator::Generate()
{
	for (int y = 2; y < mHeight-2; y++)
	{
		for (int x = 2; x < mWidth-2; x++)
		{
			mMap[y*mWidth + x] = mRule(x,y,*this);
		}
	}
	mGenerateCount++;
}
void MapGenerator::PrintMap()
{
	for (int y = 0; y < mHeight; y++)
	{
		for (int x = 0; x < mWidth; x++)
		{
			cout << MapChuckTypeToChar(mMap[y*mWidth + x]);
		}
		cout << endl;
	}
}
void MapGenerator::SetRule(Rule _rule)
{
	mRule = _rule;
}


///<summary>
///该类中封装了一些规则方法
///</summary>
class RuleGenerator
{
public:
	static ChuckType WallIs4FloorIs5(int _x,int _y,MapGenerator&_generator)
	{
		if (isOutOfRange(_x, _y, _generator)) return ChuckType::None;

		int wallCount = getWallCountInRange(_x,_y,1,_generator);

		if (_generator.GetChuckType(_x, _y) == ChuckType::Wall)
		{
			return wallCount >= 4 ? ChuckType::Wall : ChuckType::Road;
		}
		else
		{
			return wallCount >= 5 ? ChuckType::Wall : ChuckType::Road;
		}
	}
	static ChuckType WallIs5OrWallInTwoIs2(int _x, int _y, MapGenerator&_generator)
	{
		if (isOutOfRange(_x, _y, _generator)) return ChuckType::None;

		int wallCountInRange1 = getWallCountInRange(_x, _y, 1, _generator);
		int wallCountInRange2 = getWallCountInRange(_x,_y,2,_generator);
		
		return wallCountInRange1 >= 5 || wallCountInRange2 <= 2 ? ChuckType::Wall : ChuckType::Road;
	}
private:
	static inline bool isOutOfRange(int _x,int _y, MapGenerator&_generator)
	{
		if (_x < 0 || _y < 0 || _x >= _generator.Width() || _y >= _generator.Height()) return true;
		return false;
	}
	static int getWallCountInRange(int _x, int _y, int _range,MapGenerator&_generator);
};

int RuleGenerator::getWallCountInRange(int _x, int _y, int _range, MapGenerator&_generator)
{
	int wallCount = 0;
	
	int minX = max(0,_x-_range), maxX = min(_generator.Width()-1,_x+_range);
	int minY = max(0, _y - _range),maxY = min(_generator.Height() - 1, _y + _range);

	
	for (int i = minX; i <= maxX;i++)
	{
		if (_generator.GetChuckType(i, minY) == ChuckType::Wall) wallCount++;
		if (_generator.GetChuckType(i, maxY) == ChuckType::Wall) wallCount++;
	}
	for (int i = minY+1; i < maxY; i++)
	{
		if (_generator.GetChuckType(minX, i) == ChuckType::Wall) wallCount++;
		if (_generator.GetChuckType(maxX, i) == ChuckType::Wall) wallCount++;
	}
	return wallCount;
}


char MapChuckTypeToChar(ChuckType type)
{
	switch (type)
	{
	case None:
		return 'N';
	case Wall:
		return '*';
	case Road:
		return ' ';
	}
	return '?';
}

int main()
{
	MapGenerator generator(72,30,0.45f);

	generator.Init();
	generator.SetRule(RuleGenerator::WallIs5OrWallInTwoIs2);

	do
	{
		system("cls");
		printf("开始第%d次地图生成\n",generator.GenerateCount());
		generator.Generate();
		generator.PrintMap();
		printf("地图生成完毕\n");

	} while (getchar());

	return 0;
}


