#pragma once
class CRegionUtil
{
public:
	CRegionUtil();
	~CRegionUtil();
	static void GetRegionPoints(AcDbRegion * pRegion, AcGePoint3dArray & points);	
};

