#pragma once
class CConvertUtil
{
public:
	CConvertUtil();
	~CConvertUtil();

	static AcGePoint3d CConvertUtil::ToPoint3d(const AcGePoint2d &pt);
};

