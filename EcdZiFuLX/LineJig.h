#pragma once
#include "dbjig.h"
#include<vector>
#include"DatabaseJigEntity.h"
using namespace std;
class LineJig :public AcEdJig
{
public:
	LineJig():m_pEnt(NULL){}
	~LineJig();
	virtual DragStatus sampler();
	virtual bool update();	
	virtual AcDbEntity* entity() const;
	void DoIt(vector<AcDbLine*> &vecLine);
	void DoIt(AcDbObjectIdArray &s);
	AcDbObjectId  PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase());
private:
	AcGePoint3d ptCurrent;
	vector<AcDbLine*> lines;
	CDatabaseJigEntity *m_pEnt;
	

};

