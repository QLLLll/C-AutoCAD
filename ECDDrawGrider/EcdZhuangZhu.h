#pragma once
class EcdZhuangZhu
{
public:
	EcdZhuangZhu();
	~EcdZhuangZhu();
	void Command();
private :
	AcDbObjectId AddLayer(AcDbDatabase *db, const ACHAR *layerName, AcCmColor color);
};

