#pragma once
#include <vector>
class CStringUtil
{
public:
	CStringUtil();
	~CStringUtil();
	
	// 以一个字符串为分隔符对字符串进行分割
	// bAddEmptyItem: 是否添加空字符串元素
	static void Split(const CString &text, const TCHAR* separator, std::vector<CString> &result, bool bAddEmptyItem = true);

	// 通过分隔符合并字符串集合
	static CString Join(const std::vector<CString> &items, const TCHAR* separator);

};

