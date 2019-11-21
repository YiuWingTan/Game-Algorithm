
#include<iostream>
#include"Dungeon.h"

using namespace std;



int main()
{
	Dungeon dungeon(79,30,100);
	do
	{
		system("cls");
		cout << "开始生成地图..................................." << endl;
		dungeon.Reset();
		dungeon.Generator();
		dungeon.PrintMap();

		cout << "地图生成完毕" << endl;

	} while (getchar());

	return 0;
}


