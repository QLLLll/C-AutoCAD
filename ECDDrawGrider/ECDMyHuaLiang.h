#pragma once
#include<vector>
class ECDMyHuaLiang
{
public:
	ECDMyHuaLiang();
	~ECDMyHuaLiang();
	void Command();

private :
	AcDbEntity *GetEntity(const ACHAR *str);

	AcDbObjectId AddLayer(AcDbDatabase *db, const ACHAR *layerName, AcCmColor color);

	void CutLine(AcDbEntity *ent1, AcDbEntity *ent2, std::vector<AcDbLine*>listLines);

	AcGePoint3d GetCenterPt(AcDbEntity * ent);

};

