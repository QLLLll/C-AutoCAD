#pragma once
class CEllipseUtil
{
public:
	CEllipseUtil();
	~CEllipseUtil();

	static AcDbObjectId CEllipseUtil::Add(const AcGePoint3d &ptCenter, const AcGeVector3d &vecNormal,
		const AcGeVector3d &majorAxis, double ratio);

	static AcDbObjectId CEllipseUtil::Add(const AcGePoint2d &pt1, const AcGePoint2d &pt2);

};

