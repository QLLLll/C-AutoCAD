#include "stdafx.h"
#include "AreaFenGe.h"
#include "StringUtil.h"
#include"DwgDataBaseUtil.h"
#include"MathUtil.h"
#include"acedCmdNF.h"
#include"GetInputUtil.h"
CAreaFenGe::CAreaFenGe(ACHAR *fenGeStr, AcDbPolyline *&p, AcDbLine *&l)
{
	
	vector<CString> vec;

	CStringUtil::Split(W2T(fenGeStr), L",", vec, false);

	for (int i = 0; i < (int)vec.size(); i++)
	{
		vecFenGe.push_back(_wtof(vec.at(i)));
	}
	this->poly = p;
	this->line = l;

	l1Pt = l->startPoint();
	l2Pt = l->endPoint();

	if (poly->isClosed() == false) {
		poly->setClosed(true);
	}

	poly->getArea(totalArea);

	for (int i = 0; i < (int)vecFenGe.size(); i++)
	{
		vecArea.push_back(totalArea*vecFenGe.at(i));
		acutPrintf(L"%.2f", vecArea[i]);
	}
	GetDirection();
	plId = poly->objectId();
}


CAreaFenGe::~CAreaFenGe()
{

	delete line;
	line = NULL;

}

void CAreaFenGe::Command()
{

	ErrorStatus es;
	AcDbLine * lQieXian = NULL;
	AcGePoint3d ptQieDian = GetQieDian(lQieXian);

	acutPrintf(L"\nptQieDian=[%.2f,%.2f,%.2f]", ptQieDian.x, ptQieDian.y, ptQieDian.z);

	ptQieDian=ptQieDian.transformBy(AcGeMatrix3d::translation(pyXl.normal() * 5));
	acutPrintf(L"\nptQieDian2=[%.2f,%.2f,%.2f]", ptQieDian.x, ptQieDian.y, ptQieDian.z);

	if (ptQieDian.x == 0 && ptQieDian.y == 0) {
		return;
	}
	double bujin = sqrt(totalArea * 1 / 2000);

	if (bujin <= 1) {
		bujin = 1;
	}

	AcDbObjectId qxId;
	lQieXian->transformBy(AcGeMatrix3d::translation(pyXl.normal() * bujin*3));


	es = acdbOpenObject(poly, plId, AcDb::kForRead);

	AcGePoint3dArray ptJdArr;

	poly->intersectWith(lQieXian, AcDb::Intersect::kOnBothOperands, ptJdArr, 0, 0);


	if (ptJdArr.length() >= 2) {

		ptQieDian=AcGePoint3d((ptJdArr[0].x+ ptJdArr[1].x)/2, (ptJdArr[0].y + ptJdArr[1].y) / 2,0);

		ptQieDian= ptQieDian.transformBy(AcGeMatrix3d::translation(pyXl.normal() * -bujin*1.5));

	}
	poly->close();


	qxId = CDwgDataBaseUtil::PostToModelSpace(lQieXian);

	lQieXian->close();

	lQieXian = NULL;

	//AcDbPolyline * ptPy = new AcDbPolyline();

	//ptPy->setColorIndex(5);

	//AcDbObjectId ptId;

	AcGePoint3dArray ptJdArr2;


	for (int i = 0; i < (int)vecArea.size(); i++)
	{


		AcGePoint3d ptCopyQd = ptQieDian;

		double a = 0.0;
		
		while (a < vecArea[i]) {

			bool flag = GetPyPolyline(ptQieDian, a);

			if (!flag) {
				return;
			}
			es = acdbOpenObject(lQieXian, qxId, AcDb::kForWrite);
			lQieXian->transformBy(AcGeMatrix3d::translation(pyXl.normal() * bujin));

			ptJdArr2.removeAll();

			es = acdbOpenObject(poly, plId, AcDb::kForRead);
			poly->intersectWith(lQieXian, AcDb::Intersect::kExtendArg, ptJdArr2, 0, 0);

			poly->close();

			if (ptJdArr2.length() < 1) {

				lQieXian->close();
				break;
				
			}


			lQieXian->close();

		}

		acdbOpenObject(lQieXian, qxId, AcDb::kForWrite);

		es = acdbOpenObject(poly, plId, AcDb::kForRead);

		 ptJdArr.removeAll();

		poly->intersectWith(lQieXian, AcDb::Intersect::kExtendArg, ptJdArr, 0, 0);

		AcGePoint3d preCenter;
		if (ptJdArr.length() >= 2) {

			AcDbLine* lFg = new AcDbLine(ptJdArr[0], ptJdArr[1]);

			preCenter = AcGePoint3d((ptJdArr[0].x + ptJdArr[1].x) / 2, (ptJdArr[0].y + ptJdArr[1].y) / 2, 0);
			lFg->setColorIndex(2);

			CDwgDataBaseUtil::PostToModelSpace(lFg);

			lFg->close();

		}
		else {

			poly->close();
			lQieXian->erase();
			lQieXian->close();
			lQieXian = NULL;
			//ptId = CDwgDataBaseUtil::PostToModelSpace(ptPy);
			return;

		}
	
		poly->close();

		AcDbPolyline *pCopyQx = (AcDbPolyline*)lQieXian->clone();

		pCopyQx->transformBy(AcGeMatrix3d::translation(pyXl.normal() *bujin));

		if (i != (int)vecArea.size() - 1) {


			es = acdbOpenObject(poly, plId, AcDb::kForRead);

			ptJdArr.removeAll();

			poly->intersectWith(pCopyQx, AcDb::Intersect::kExtendArg, ptJdArr, 0, 0);

			if (ptJdArr.length() >= 2) {
				ptQieDian = AcGePoint3d((ptJdArr[0].x + ptJdArr[1].x) / 2, (ptJdArr[0].y + ptJdArr[1].y) / 2, 0);

				ptQieDian = AcGePoint3d((preCenter.x + ptQieDian.x) / 2, (preCenter.y + ptQieDian.y) / 2, 0);

			}
			qxId = CDwgDataBaseUtil::PostToModelSpace(pCopyQx);

			pCopyQx->close();
			poly->close();
			pCopyQx = NULL;
		}
		lQieXian->erase();
		lQieXian->close();
		lQieXian = NULL;

		//当运行到最后一个面积时，就不复制了
		if (i == (int)vecArea.size() - 1) {

			delete pCopyQx;
			pCopyQx = NULL;

			break;
		}
	}

	/*if (ptId.isNull()) {
		ptId = CDwgDataBaseUtil::PostToModelSpace(ptPy);
		ptPy->close();
	}*/
}

AcGeVector3d CAreaFenGe::GetDirection()
{
	//l2pt和l1pt是在poly侧边画的线的端点
	AcGeVector3d lineDirc = l2Pt - l1Pt;
	AcGeVector3d lineDirc2 = l2Pt - l1Pt;

	AcGePoint3d centerPt((l2Pt.x + l1Pt.x) / 2, (l2Pt.y + l1Pt.y) / 2, 0);
	AcGePoint3d ptCloset;

	poly->getClosestPointTo(centerPt, ptCloset, false);

	AcDbLine *line1 = AcDbLine::cast(line->clone());
	AcDbLine *line2 = AcDbLine::cast(line->clone());



	double len = ptCloset.distanceTo(centerPt);

	AcGeVector3d pyXl1 = lineDirc.rotateBy(-1 * CMathUtil::PI() / 2, AcGeVector3d::kZAxis);
	AcGeVector3d pyXl2 = lineDirc2.rotateBy(CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

	line1->transformBy(AcGeMatrix3d::translation(pyXl1.normal()*len*1.5));
	line2->transformBy(AcGeMatrix3d::translation(pyXl2.normal()*len*1.5));

	
	AcGePoint3dArray p3dArr1, p3dArr2;


	poly->intersectWith(line1, AcDb::Intersect::kExtendArg, p3dArr1, 0, 0);
	poly->intersectWith(line2, AcDb::Intersect::kExtendArg, p3dArr2, 0, 0);

	
	if (p3dArr1.length() > 0) {		
		pyXl = pyXl1;	
	}
	else if(p3dArr2.length()>0){		
		pyXl = pyXl2;
	}
	else {
		pyXl = AcGeVector3d(3, -1, 0);
		
	}
	
	delete line1;
	line1 = NULL;
	delete line2;
	line2 = NULL;

	return pyXl;
	
}

//得到切线
AcGePoint3d CAreaFenGe::GetQieDian(AcDbLine * &line1)
{

	line1 = new AcDbLine(l1Pt, l2Pt);

	AcGePoint3dArray ptArr;

	while (ptArr.length() < 1) {

		poly->intersectWith(line1, AcDb::Intersect::kOnBothOperands, ptArr, 0, 0);

		line1->transformBy(AcGeMatrix3d::translation(pyXl.normal() * 1));

	}
	poly->close();

	if (ptArr.length() > 0)
		return ptArr[0];
	else
		return AcGePoint3d::kOrigin;

}

bool  CAreaFenGe::GetPyPolyline(AcGePoint3d seedPoint, double & a)
{
	//ads_name pent;

	////CGetInputUtil::GetPoint(L"\n请在封闭曲线一侧输入一个点：", seedPoint);
	//ads_point pt = { seedPoint.x,seedPoint.y,seedPoint.z };
	//acedCommandS(RTSTR, L"-boundary", RTPOINT, pt, RTSTR, L"", RTNONE);
	//acdbEntLast(pent);
	////acedRedraw(pent, 3);
	//AcDbObjectId eId;

	//acdbGetObjectId(eId, pent);

	//AcDbPolyline * pEnt = NULL;

	//ErrorStatus es = acdbOpenObject(pEnt, eId, AcDb::OpenMode::kForWrite);

	//if (es != Acad::eOk) {
	//	acutPrintf(L"\n%d", es);
	//	return false;
	//}

	//pEnt->setClosed(true);

	//pEnt->getArea(a);

	//pEnt->erase();

	//pEnt->close();
	//
	//return true;

	AcDbVoidPtrArray ptrArr;
	ErrorStatus es = acedTraceBoundary(seedPoint, false, ptrArr);

	if (es != Acad::eOk) {
		acutPrintf(L"\nboundary=%d", es);
		return false;
	}

	AcDbPolyline * pl = NULL;
	pl = (AcDbPolyline*)ptrArr[0];

	pl->setColorIndex(1);

	pl->getArea(a);

	pl->erase();
	pl->close();
	pl = NULL;
	return true;

}
