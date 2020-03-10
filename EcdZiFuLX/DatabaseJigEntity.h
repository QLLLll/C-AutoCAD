#pragma once
#include<vector>
using namespace std;
class CDatabaseJigEntity:public AcDbEntity
{
public:
	CDatabaseJigEntity(const vector<AcDbLine*> &vecLine) :vec(vecLine){};
	CDatabaseJigEntity(const AcDbObjectIdArray& m):m_Ids(m) {};
	~CDatabaseJigEntity();
	CDatabaseJigEntity();
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode); //实时显示改变
	
	bool CDatabaseJigEntity::move(const AcGePoint3d & m_endPt);
	BOOL transform();
	/*BOOL transformedCopy();*/
private:
	vector<AcDbLine*> vec;
	AcGePoint3d pt;
	AcDbObjectIdArray m_Ids;
	
};

