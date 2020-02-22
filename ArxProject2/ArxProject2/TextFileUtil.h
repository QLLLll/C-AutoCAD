#pragma once
#include <vector>
class CTextFileUtil
{
public:
	CTextFileUtil();
	~CTextFileUtil();
	// 保存文本文件
	static void Save(const TCHAR* fileName, const std::vector<CString> &lines);

	// 加载文件
	static void Load(const TCHAR* fileName, std::vector<CString> &lines);
};

