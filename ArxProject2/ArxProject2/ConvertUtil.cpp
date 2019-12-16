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
CString CConvertUtil::ToString(int val)
{
	CString str;
	str.Format(TEXT("%d"), val);
	return str;
}

CString CConvertUtil::ToString(double value, int precision /*= 2*/)
{
	CString strFormat, str;
	strFormat.Format(TEXT("%%.%df"), precision);
	str.Format(strFormat, value);

	return str;
}

double CConvertUtil::ToDouble(const TCHAR* str)
{
	TCHAR* pszStop = NULL;
	return _tcstod(str, &pszStop);
}