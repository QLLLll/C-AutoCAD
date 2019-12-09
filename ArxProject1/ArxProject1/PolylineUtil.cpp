#include "stdafx.h"
#include "PolylineUtil.h"
#include"DwgDataBaseUtil.h"

CPolylineUtil::CPolylineUtil()
{
}


CPolylineUtil::~CPolylineUtil()
{
}
AcDbObjectId  CPolylineUtil::Add(const AcGePoint2dArray &points, double width) {

	int numVerts = points.length;
	AcDbPolyline *pPoly = new AcDbPolyline(numVerts);

	for (int i = 0; i < numVerts; i++)
	{
		pPoly->addVertexAt(i, points.at(i), 0, width, width);
	}
	return  CDwgDataBaseUtil::PostToModelSpace(pPoly, acdbHostApplicationServices()->workingDatabase);


}

AcDbObjectId  CPolylineUtil::Add(const AcGePoint2d &ptStart, const AcGePoint2d &ptEnd, double width = 0) {

	AcGePoint2dArray points;
	points.append(ptStart);
	points.append(ptEnd);

	return CPolylineUtil::Add(points, width);
}

