#include "stdafx.h"
#include "EllipseUtil.h"
#include "DwgDataBaseUtil.h"
#include"GePointUtil.h"
#include"ConvertUtil.h"

CEllipseUtil::CEllipseUtil()
{
}


CEllipseUtil::~CEllipseUtil()
{
}

AcDbObjectId CEllipseUtil::Add(const AcGePoint3d &ptCenter, const AcGeVector3d &vecNormal,
	const AcGeVector3d &majorAxis, double ratio) {

	AcDbEllipse *pEllipse = new AcDbEllipse(ptCenter, vecNormal, majorAxis, ratio);

	return CDwgDataBaseUtil::PostToModelSpace(pEllipse);
}

AcDbObjectId CEllipseUtil::Add(const AcGePoint2d &pt1, const AcGePoint2d &pt2) {

	AcGePoint3d ptCenter;

	ptCenter = CGePointUtil::GetMiddlePoint(CConvertUtil::ToPoint3d(pt1), CConvertUtil::ToPoint3d(pt2));

	AcGeVector3d vecNormal(0, 0, 1);

	AcGeVector3d majorAxis(fabs(pt1.x - pt2.x)/2, 0, 0);

	double ratio = fabs((pt1.y - pt2.y) / (pt1.x - pt2.x));

	return Add(ptCenter, vecNormal, majorAxis, ratio);



}

