#pragma once
class CGePointUtil
{
public:
	CGePointUtil();
	~CGePointUtil();

	static AcGePoint3d CGePointUtil::GetMiddlePoint(AcGePoint3d pt1, AcGePoint3d pt2);
	static AcGePoint3d CGePointUtil::PolarPoint(const AcGePoint3d &basePt, double angle,double length);
	static AcGePoint3d RelativePoint(const AcGePoint3d & pt, double x, double y);
	static AcGePoint2d GetMiddlePoint(const AcGePoint2d & startPoint, const AcGePoint2d & endPoint);
	static AcGePoint2d PolarPoint(const AcGePoint2d & basePoint, double angle, double length);
};

