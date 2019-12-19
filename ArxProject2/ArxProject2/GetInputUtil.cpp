#include "stdafx.h"
#include "GetInputUtil.h"
#include"ConvertUtil.h"
#include<vector>
#include"StringUtil.h"


CGetInputUtil::CGetInputUtil()
{
}


CGetInputUtil::~CGetInputUtil()
{
}

int CGetInputUtil::GetPointReturnCode(const AcGePoint3d & basePoint, const TCHAR * prompt, AcGePoint3d & point)
{
	AcGePoint3d ucsBasePoint = CConvertUtil::WcsToUcsPoint(basePoint);

	int nReturn = acedGetPoint(asDblArray(ucsBasePoint), prompt, asDblArray(point));

	if (nReturn == RTNORM) {

		point = CConvertUtil::UcsToWcsPoint(point);
	}

	return nReturn;


}

int CGetInputUtil::GetPointReturnCode(const TCHAR * prompt, AcGePoint3d & point)
{
	int nReturn = acedGetPoint(NULL, prompt, asDblArray(point));

	if (nReturn == RTNORM) {
		point = CConvertUtil::UcsToWcsPoint(point);
	}
	return nReturn;

}

bool CGetInputUtil::GetPoint(const AcGePoint3d & basePoint, const TCHAR * prompt, AcGePoint3d & point)
{
	return (GetPointReturnCode(basePoint,prompt,point)==RTNORM);
}

bool CGetInputUtil::GetPoint(const TCHAR * prompt, AcGePoint3d & point)
{
	return (GetPointReturnCode(prompt, point) == RTNORM);
}

bool CGetInputUtil::GetKeyword(const TCHAR * prompt, const TCHAR * keywords, const TCHAR * firstDefault, int defaultKeyIndex, int & nRet)
{
	int rc;

	TCHAR kword[20];
	//firstDefault參數输入NULL表示不予许直接按ENTER键
	acedInitGet(firstDefault == NULL ? 1 : 0, keywords);

	CString strPrompt = prompt;

	if (firstDefault != NULL) {

		strPrompt += L"<";
		strPrompt += firstDefault;
		strPrompt += ">:";

	}
	else {
		strPrompt += L":";
	}
	rc = acedGetKword(strPrompt, kword);

	switch (rc) {

	case RTCAN:
		return false;//esc

	case RTNONE://enter
		nRet = defaultKeyIndex;
		return true;

	case RTNORM:
	{
		std::vector<CString>items;

		CStringUtil::Split(keywords, TEXT(" "), items);

		for (int i = 0; i < (int)items.size(); i++)
		{

			if (items[i].CompareNoCase(kword) == 0) {

				nRet = i;
				break;
			}
		}
		return true;
	}
	default:
		return false;

	}
}

bool CGetInputUtil::GetReal(const TCHAR * prompt, double defaultVal, int precision, double & ret)
{
	CString strPrompt = prompt;
	strPrompt.TrimRight();
	strPrompt.TrimRight(TEXT(":"));
	CString strDefaultValue;
	strDefaultValue.Format(TEXT("<%%.%df>:"), precision);

	strPrompt.Format(strPrompt + strDefaultValue, defaultVal);
	ret = defaultVal;
	int rc = acedGetReal(strPrompt, &ret);

	if (rc == RTNORM || rc == RTNONE) {

		return true;
	}
	else {
		return false;
	}

}
