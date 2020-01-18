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
	int GetDirection();


private :
	AcDbPolyline *poly;
	AcDbLine *line;
	double totalArea;
	int direction;//1 左下，2左，3左上，4上，5右上，6右，7右下，8下
	vector<double> vecFenGe;
	vector<double>vecArea;
	AcGePoint3d l1Pt, l2Pt;
	AcGeVector3d pyXl;

};

