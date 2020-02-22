#pragma once
#include"MathUtil.h"
#include"ConvertUtil.h"
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
	static bool IsEqual(const AcGePoint2d &firstPoint, const AcGePoint2d &secondPoint, double tol = 1.0E-7);
	static bool IsEqual(const AcGePoint3d &firstPoint, const AcGePoint3d &secondPoint, double tol = 1.0E-7);
	static int FindPoint(const AcGePoint3dArray &points, const AcGePoint3d &point,
		double tol = 1.0E-7);
	static int FindPoint(const AcGePoint2dArray &points, const AcGePoint2d &point,
		double tol = 1.0E-7);
	static void FilterEqualPoints(AcGePoint3dArray &points, double tol = 1.0E-7);
	static void FilterEqualPoints(AcGePoint3dArray &points, const AcGePoint2d &pt,
		double tol = 1.0E-7);

};

