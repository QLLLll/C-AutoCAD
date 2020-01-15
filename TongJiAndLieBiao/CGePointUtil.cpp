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

bool CGePointUtil::IsEqual(const AcGePoint2d & firstPoint, const AcGePoint2d & secondPoint, double tol)
{
	return (fabs(firstPoint.x - secondPoint.x) < tol&&
		fabs(firstPoint.y - secondPoint.y) < tol);
}

bool CGePointUtil::IsEqual(const AcGePoint3d & firstPoint, const AcGePoint3d & secondPoint, double tol)
{
	return (fabs(firstPoint.x - secondPoint.x) < tol&&
		fabs(firstPoint.y - secondPoint.y) < tol&&
		fabs(firstPoint.z - firstPoint.z) < tol);
}

int CGePointUtil::FindPoint(const AcGePoint3dArray & points, const AcGePoint3d & point, double tol)
{
	for (int i = 0; i < points.length(); i++)
	{

		if (IsEqual(points[i], point, tol)) {
			return i;
		}

	}
	return -1;
}

int CGePointUtil::FindPoint(const AcGePoint2dArray & points, const AcGePoint2d & point, double tol)
{
	for (int i = 0; i < points.length(); i++)
	{

		if (IsEqual(points[i], point, tol)) {
			return i;
		}

	}
	return -1;
}

void CGePointUtil::FilterEqualPoints(AcGePoint3dArray & points, double tol)
{
	for (int i = points.length() - 1; i > 0; i--) {

		for (int j = 0; j < i; j++) {

			if (CMathUtil::IsEqual(points[i].x, points[j].x, tol) &&
				CMathUtil::IsEqual(points[i].y, points[j].y, tol)) {

				points.removeAt(i);
				break;

			}


		}


	}


}

void CGePointUtil::FilterEqualPoints(AcGePoint3dArray & points, const AcGePoint2d & pt, double tol)
{

	AcGePoint3dArray tempPoints;

	for (int i = 0; i < points.length(); i++)
	{

		if (CConvertUtil::ToPoint2d(points[i]).distanceTo(pt)>tol) {

			tempPoints.append(points[i]);

		}

	}

	points = tempPoints;

}
