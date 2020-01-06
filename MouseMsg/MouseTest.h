#pragma once
class MouseTest
{
public:
	MouseTest();
	~MouseTest();
	//函数的声明部分
	void regCaps();   //注册命令
	void unregCaps();  //注销命令
//public:
	//static BOOL CapsDone;   //判断是否注册了该命令
};
BOOL toCaps(MSG* pMsg);
