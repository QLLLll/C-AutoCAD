#pragma once
class CConvertUtil
{
public:
	CConvertUtil();
	~CConvertUtil();
	static AcGePoint3d CConvertUtil::ToPoint3d(AcGePoint2d pt);
	static AcGePoint2d CConvertUtil::ToPoint2d(AcGePoint3d pt);

	// 转换为字符串
	static CString ToString(int val);
	static CString ToString(double value, int precision = 2);

	// 字符串转换为特定的类型
	static double ToDouble(const TCHAR* str);

	//世界坐标转用户坐标
	static AcGePoint3d WcsToUcsPoint(const AcGePoint3d &point);

	//用户坐标转世界坐标
	static AcGePoint3d UcsToWcsPoint(const AcGePoint3d &point);

};

