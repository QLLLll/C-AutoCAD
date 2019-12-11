#pragma once
class CCircleUtil
{
public:
	CCircleUtil();
	~CCircleUtil();
	// 添加圆
	static AcDbObjectId Add(const AcGePoint3d &centerPoint, const AcGeVector3d &normal, double radius,
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase());
	static AcDbObjectId Add(const AcGePoint3d &centerPoint, double radius,
		AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase());
	// 两点法
	static AcDbObjectId Add(const AcGePoint2d &point1, const AcGePoint2d &point2);
	// 三点法
	static AcDbObjectId Add(const AcGePoint2d &pt1, const AcGePoint2d &pt2, const AcGePoint2d &pt3);
};

