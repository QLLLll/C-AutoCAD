#pragma once
class CConvertUtil
{
public:
	CConvertUtil();
	~CConvertUtil();
	static AcGePoint3d CConvertUtil::ToPoint3d(AcGePoint2d pt);

	// 转换为字符串
	static CString ToString(int val);
	static CString ToString(double value, int precision = 2);

	// 字符串转换为特定的类型
	static double ToDouble(const TCHAR* str);
};

