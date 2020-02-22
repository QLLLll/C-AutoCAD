#pragma once
class CLayerUtil
{
public:
	CLayerUtil();
	~CLayerUtil();
	static AcDbObjectId Add(const ACHAR* layerName, const int colorIndex = 7);
	static AcDbObjectId GetLayerId(const TCHAR* layerName);
	static bool SetColor(const TCHAR* layerName,const int colorIndex);
	static void GetLayerList(AcDbObjectIdArray & lIds);
};

