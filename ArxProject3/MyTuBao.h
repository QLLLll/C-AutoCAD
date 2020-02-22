#pragma once
class MyTuBao
{
public:
	MyTuBao();
	MyTuBao(AcGePoint3dArray arr);
	~MyTuBao();
	
	void  GetTuBao(AcGePoint3dArray& ptGetArr);

	void GetRec(AcGePoint3dArray & pArray);

private :
	void optimizePointsArray(AcGePoint3dArray & currPointsArray , double tol = 1.0E-7);
	long long dist(AcGePoint3d p1, AcGePoint3d p2);
	bool cmp1(AcGePoint3d p1, AcGePoint3d p2);
	bool cmp2(AcGePoint3d p1, AcGePoint3d p2, AcGePoint3d pointMin);
	double xmul(AcGePoint3d a, AcGePoint3d b, AcGePoint3d c);//¡Á³Ë
	void selectMinPoint(AcGePoint3dArray & s);
	void quickSort(AcGePoint3dArray & s, int l, int r);
private:
	AcGePoint3dArray ptArr;
};

