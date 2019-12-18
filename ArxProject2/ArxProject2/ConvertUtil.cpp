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
AcGePoint2d CConvertUtil::ToPoint2d(AcGePoint3d pt)
{
	return AcGePoint2d(pt.x,pt.y);
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

AcGePoint3d CConvertUtil::WcsToUcsPoint(const AcGePoint3d & point)
{
	AcGePoint3d pt;

	struct resbuf rbFrom, rbTo;
	
	rbFrom.restype = RTSHORT;
	rbFrom.resval.rint = 0;//from WCS

	rbTo.restype = RTSHORT;
	rbTo.resval.rint = 1;//to UCS

	acedTrans(asDblArray(point), &rbFrom, &rbTo, Adesk::kFalse, asDblArray(pt));

	return pt;
}

AcGePoint3d CConvertUtil::UcsToWcsPoint(const AcGePoint3d & point)
{
	AcGePoint3d pt;
	struct resbuf rbFrom, rbTo;

	rbFrom.restype = RTSHORT;
	rbFrom.resval.rint = 1;//from UCS

	rbTo.restype = RTSHORT;
	rbTo.resval.rint = 0;//to WCS

	acedTrans(asDblArray(point), &rbFrom, &rbTo, Adesk::kFalse, asDblArray(pt));

	return pt;
}
