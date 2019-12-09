#pragma once
class CPolylineUtil
{
public:
	CPolylineUtil();
	~CPolylineUtil();
	
static	AcDbObjectId  CPolylineUtil::Add(const AcGePoint2dArray &points, double width=0);
static	AcDbObjectId  CPolylineUtil::Add(const AcGePoint2d &ptStart, const AcGePoint2d &ptEnd, double width = 0);
};

