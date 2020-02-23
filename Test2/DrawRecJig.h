#pragma once
#include "dbjig.h"
class DrawRecJig :
	public AcEdJig
{
public:
	DrawRecJig();
	~DrawRecJig();
	virtual DragStatus sampler();
	virtual Adesk::Boolean  update();
	virtual AcDbEntity* entity() const;
	bool DoIt(AcGePoint3d &ptCenter,AcGeVector2d recVec2d,AcDbObjectId &plId);
private:
	AcDbPolyline *pl;
	
	AcGePoint3d ptCenter;
	AcGePoint3d ptCurrent;
	AcGeVector2d recVec;
};

