#pragma once
#include "dbjig.h"
class DrawRecJig :
	public AcEdJig
{
public:
	DrawRecJig();
	~DrawRecJig();
	virtual DragStatus sampler();
	virtual bool update();
	virtual AcDbEntity* entity() const;
	bool DoIt(AcGePoint3d &ptCenter,AcDbObjectId &plId);
private:
	AcDbPolyline *pl;
	
	AcGePoint3d ptCenter;
	AcGePoint3d ptCurrent;
};

