#pragma once
#include<random>
namespace RandomHelper
{
	static std::random_device rd;
	static std::mt19937 mt(rd());

	///<summary>
	///使用均匀分布生成在【0，maxValue】内的数
	///</summary>
	int RandomInt(int maxValue)
	{
		 std::uniform_int_distribution<> dist(0,maxValue);
		 return dist(mt);
	}

	///<summary>
	///使用均匀分布生成在【min，max】内的数
	///</summary>
	int RandomRangeInt(int min,int max)
	{
		std::uniform_int_distribution<> dist(0,max - min);
		return dist(mt) + min;
	}

	///<summary>
	///使用伯努利分布根据一个概率值生成一个布尔值
	///</summary>
	bool RandomBernoulliBool(float probablity = 0.5f)
	{
		std::bernoulli_distribution dist(probablity);

		return dist(mt);
	}


};

