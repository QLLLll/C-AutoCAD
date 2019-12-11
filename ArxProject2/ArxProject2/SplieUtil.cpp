#include "stdafx.h"
#include "SplieUtil.h"
#include"DwgDataBaseUtil.h"

CSplieUtil::CSplieUtil()
{
}

CSplieUtil::~CSplieUtil()
{
}

AcDbObjectId CSplieUtil::Add(const AcGePoint3dArray & points, int order, double fitTolerance)
{
	AcDbSpline *spline =new  AcDbSpline(points, order, fitTolerance);

	return CDwgDataBaseUtil::PostToModelSpace(spline);

}

AcDbObjectId CSplieUtil::Add(const AcGePoint3dArray & points, const AcGeVector3d startTangent, AcGeVector3d endTangent, int order, double fitTolerance)
{
	AcDbSpline *spline = new AcDbSpline(points, startTangent, endTangent, order, fitTolerance);

	return CDwgDataBaseUtil::PostToModelSpace(spline);
}
