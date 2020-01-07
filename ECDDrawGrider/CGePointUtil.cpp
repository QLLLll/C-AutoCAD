#include "stdafx.h"
#include "GePointUtil.h"

CGePointUtil::CGePointUtil()
{
}

CGePointUtil::~CGePointUtil()
{
}

AcGePoint3d CGePointUtil::GetMiddlePoint(AcGePoint3d pt1, AcGePoint3d pt2)
{
	AcGePoint3d ptCenter;

	ptCenter.x = (pt1.x + pt2.x) / 2;
	ptCenter.y = (pt1.y + pt2.y) / 2;
	ptCenter.z = (pt1.z + pt2.z) / 2;

	return ptCenter;
}

AcGePoint3d CGePointUtil::PolarPoint(const AcGePoint3d &basePt, double angle, double length)
{
	double x = basePt.x + length*cos(angle);
	double y = basePt.y + length*sin(angle);

	return AcGePoint3d(x, y, basePt.z);
}
AcGePoint3d CGePointUtil::RelativePoint(const AcGePoint3d& pt, double x, double y)
{
	AcGePoint3d ptReturn(pt.x + x, pt.y + y, pt.z);
	return ptReturn;
}
AcGePoint2d CGePointUtil::GetMiddlePoint(const AcGePoint2d &startPoint, const AcGePoint2d &endPoint)
{
	double x = (startPoint.x + endPoint.x) * 0.5;
	double y = (startPoint.y + endPoint.y) * 0.5;

	return AcGePoint2d(x, y);
}
AcGePoint2d CGePointUtil::PolarPoint(const AcGePoint2d &basePoint, double angle, double length)
{
	double x = basePoint.x + length * cos(angle);
	double y = basePoint.y + length * sin(angle);

	return AcGePoint2d(x, y);
}