#include "stdafx.h"
#include "StringUtil.h"


CStringUtil::CStringUtil()
{
}


CStringUtil::~CStringUtil()
{
}

void CStringUtil::Split(const CString & text, const TCHAR * separator, std::vector<CString>& result, bool bAddEmptyItem)
{

	int prev = 0;
	int current = 0;		// 找到的当前匹配和上一个匹配
	CString strCopyText = text;
	strCopyText.TrimRight(TEXT(" "));

	// 第一次先查找目标字符串，有没有包含分隔符
	if (strCopyText.Find(separator, 0) < 0)
	{
		if (strCopyText.GetLength() > 0)
		{
			result.push_back(strCopyText);
		}
		return;
	}

	// 循环提取中间的字符
	int length = strCopyText.GetLength();
	bool bExit = false;
	while (1)
	{
		current = strCopyText.Find(separator, prev);
		if (current == -1)		// 到最后一组了
		{
			current = length;
			bExit = true;
		}

		// 上一个分隔符之后和这个分隔符之前的字符，就是有效的字符串
		CString item = strCopyText.Mid(prev, current - prev);
		if (bAddEmptyItem || item.GetLength() > 0)
		{
			result.push_back(item);
		}

		if (bExit)
		{
			break;
		}

		prev = current + (int)_tcslen(separator);
	}

}

CString CStringUtil::Join(const std::vector<CString>& items, const TCHAR * separator)
{
	CString strResult;
	for (int i = 0; i < (int)items.size(); i++)
	{
		strResult += items[i] + separator;
	}

	strResult.TrimRight(separator);

	return strResult;
}
