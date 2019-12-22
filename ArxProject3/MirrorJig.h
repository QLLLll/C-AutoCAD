#pragma once
#include<vector>
#include "dbjig.h"
#include"DatabaseJigEntity.h"
class MirrorJig :
	public AcEdJig
{
public:
	MirrorJig();
	~MirrorJig();
	virtual DragStatus sampler();
	virtual bool update();
	virtual AcDbEntity* entity() const;
	void DoIt(AcGePoint3d& ptBase ,AcDbObjectIdArray &idArr, bool bCopy=false);
	AcDbLine *mirrorLine;
	
private:
	AcGePoint3d ptCurrent;	
	AcGePoint3d ptBase;
	CDatabaseJigEntity	*m_pEnt;
	AcGeMatrix3d		m_Xform;
};

