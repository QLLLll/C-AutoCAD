#pragma once
class CGePointUtil
{
public:
	CGePointUtil();
	~CGePointUtil();

	static AcGePoint3d CGePointUtil::GetMiddlePoint(AcGePoint3d pt1, AcGePoint3d pt2);
};

