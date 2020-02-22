#pragma once
class CLineUtil
{
public:
	CLineUtil();
	~CLineUtil();
	static AcDbObjectId Add(const AcGePoint3d & startPoint, const AcGePoint3d & endPoint, AcDbDatabase  *pDb = acdbHostApplicationServices()->workingDatabase());
};

