#pragma once
#include<vector>
using namespace std;
class EcdKuaiJX
{
public:
	EcdKuaiJX();
	~EcdKuaiJX();

	void Command();

	void MirrorText(AcDbText *text, AcGeLine3d mirrorLine);
	void MirrorText(AcDbMText *mText, AcGeLine3d mirrorLine);

private :	
	void GetTextBoxCorners(AcDbText &dbText, AcGePoint3d &pt1, AcGePoint3d &pt2,
		AcGePoint3d &pt3, AcGePoint3d &pt4);
	void MyMirror(vector<AcDbEntity *> listEnt, AcGeLine3d l3d, char xY);

private:
	vector<AcDbEntity *> listVec;
	AcDbObjectIdArray listOId;
};

