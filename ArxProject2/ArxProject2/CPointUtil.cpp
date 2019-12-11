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
