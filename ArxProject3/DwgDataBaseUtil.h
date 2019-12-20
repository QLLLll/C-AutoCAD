#pragma once
class CDwgDataBaseUtil
{
public:
	CDwgDataBaseUtil();
	~CDwgDataBaseUtil();

	static AcDbObjectId CDwgDataBaseUtil::PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb=acdbHostApplicationServices()->workingDatabase());
};

