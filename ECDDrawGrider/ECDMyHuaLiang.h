#pragma once
#include<vector>
class ECDMyHuaLiang
{
public:
	ECDMyHuaLiang();
	~ECDMyHuaLiang();

	AcDbEntity *GetEntity(const ACHAR *str);

	AcDbObjectId AddLayer(AcDbDatabase *db, const ACHAR *layerName, short colorIndex);

	void CutLine(AcDbEntity *ent1, AcDbEntity *ent2, std::vector<AcDbLine*>listLines);

	AcGePoint3d GetCenterPt(AcDbEntity * ent);

	void Command();

};

