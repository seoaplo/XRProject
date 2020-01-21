#pragma once

template <typename T>
class CustomSingleton
{
public:
	CustomSingleton()
	{

	}
	~CustomSingleton()
	{

	}
	static T& GetInstance()
	{
		static T singleton;
		return singleton;
	}
};