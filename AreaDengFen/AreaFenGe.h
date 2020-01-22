#pragma once
#include<vector>
using namespace std;
class CAreaFenGe
{
public:
	CAreaFenGe(ACHAR *fenGeStr,AcDbPolyline *&p,AcDbLine *&l);
	~CAreaFenGe();

public:
	void Command();
private:
	AcGeVector3d GetDirection();
	AcGePoint3d GetQieDian(AcDbLine * &line1);
	bool  GetPyPolyline(AcGePoint3d seedPoint,double &a);

private :
	AcDbPolyline *poly;
	AcDbLine *line;
	double totalArea;	
	vector<double> vecFenGe;
	vector<double>vecArea;
	AcGePoint3d l1Pt, l2Pt;
	AcGeVector3d pyXl;
	AcDbObjectId plId;

};

