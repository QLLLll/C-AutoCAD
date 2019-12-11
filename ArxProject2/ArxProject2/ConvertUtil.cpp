#include "stdafx.h"
#include "ConvertUtil.h"


CConvertUtil::CConvertUtil()
{
}


CConvertUtil::~CConvertUtil()
{
}

AcGePoint3d CConvertUtil::ToPoint3d(AcGePoint2d  pt)
{


	return AcGePoint3d(pt.x,pt.y,0);
}
