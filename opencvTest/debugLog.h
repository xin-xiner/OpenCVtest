#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#define log(v) DebugLog::getLogFile()->wirteLog(v);//DebugLog::getLogFile()->wirteLog(" ")
#define logLine(v) log(v);log("\n");
#define writeLog(v) log(#v);log(v);log("\n");
#define disp(x) std::cout<<#x<<std::endl<<x<<std::endl;//log(#x);log("\n");log(x);log("\n");
#define logMatrix(v,row,col) for(int i = 0;i<row;i++){ for(int j = 0;j<col-1;j++){log(v[i*col+j]);log("\t");log("\t");}logLine(v[i*col+col-1]);}logLine("\n");
class DebugLog
{
private:
	DebugLog()//µ¥ÀýÄ£Ê½
	{
		File.open("log.txt");
	};

public:
	std::ofstream File;
	template<class T>
	void wirteLog(std::vector<T> v)
	{

		for (int i = 0; i < v.size(); i++)
		{
			File << v[i] << std::endl;
		}
	}

	template<class T>
	void wirteLog(T v)
	{
		File << v;// << std::endl;
	}

	static DebugLog* getLogFile()//getInstance()
	{
		static DebugLog* instance;
		if (instance == NULL)
		{
			instance = new DebugLog();
		}
		return instance;
	}

	
};