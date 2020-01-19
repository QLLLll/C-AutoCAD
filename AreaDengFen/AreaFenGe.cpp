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

	//poly->close();
	delete line;
	line = NULL;

}

void CAreaFenGe::Command()
{
	acutPrintf(L"\ndir=%d,%d", direction,plHigh);

	ErrorStatus es;
	AcDbLine * lQieXian = NULL;
	AcGePoint3d ptQieDian = GetQieDian(lQieXian);

	

	acutPrintf(L"\nptQieDian=[%.2f,%.2f,%.2f]", ptQieDian.x, ptQieDian.y, ptQieDian.z);

	ptQieDian.transformBy(AcGeMatrix3d::translation(pyXl.normal() * 5));
	acutPrintf(L"\nptQieDian2=[%.2f,%.2f,%.2f]", ptQieDian.x, ptQieDian.y, ptQieDian.z);
	
	if (ptQieDian.x==0&&ptQieDian.y==0) {
		return;
	}
	

	AcDbObjectId qxId;
	lQieXian->transformBy(AcGeMatrix3d::translation(pyXl.normal() *12));

	qxId = CDwgDataBaseUtil::PostToModelSpace(lQieXian);

	lQieXian->close();

	lQieXian = NULL;

	
	for (int i = 0; i < (int)vecArea.size(); i++)
	{
		

		AcGePoint3d ptCopyQd = ptQieDian;

		AcDbPoint *pt = new AcDbPoint(ptCopyQd);

		pt->setColorIndex(3);
		CDwgDataBaseUtil::PostToModelSpace(pt);

		pt->close();


		double a = 0.0;

		int times = 0;
		double bujin = sqrt( totalArea * 1 / 2000);
		double bujinT = 0.0;

		while (a<vecArea[i]&&times<100) {
		
			times++;
			bool flag = GetPyPolyline(ptQieDian, a);

			if (!flag) {
				return;
			}
			es = acdbOpenObject(lQieXian, qxId, AcDb::kForWrite);
			lQieXian->transformBy(AcGeMatrix3d::translation(pyXl.normal() * bujin));

			lQieXian->close();

			//ptCopyQd.transformBy(AcGeMatrix3d::translation(pyXl.normal() * bujin));
			bujinT += bujin;

		
		}

		acdbOpenObject(lQieXian, qxId, AcDb::kForWrite);

		//求面积合格时，移动后的切线和poly的焦点

		es = acdbOpenObject(poly, plId, AcDb::kForRead);

		AcGePoint3dArray ptJdArr;

		poly->intersectWith(lQieXian, AcDb::Intersect::kOnBothOperands, ptJdArr, 0, 0);


		if (ptJdArr.length() >= 2) {

			AcDbLine* lFg = new AcDbLine(ptJdArr[0], ptJdArr[1]);

			CDwgDataBaseUtil::PostToModelSpace(lFg);

			lFg->close();

		}
		poly->close();

	

		AcDbPolyline *pCopyQx = (AcDbPolyline*)lQieXian->clone();
		

		pCopyQx->transformBy(AcGeMatrix3d::translation(pyXl.normal() * bujin));
		
		if (i != (int)vecArea.size() - 1) {

			qxId = CDwgDataBaseUtil::PostToModelSpace(pCopyQx);

			pCopyQx->close();
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

		if (ptJdArr.length() >= 2) {

			AcGePoint3d pt1 = ptJdArr[0], pt2 = ptJdArr[1];

			ptQieDian = AcGePoint3d((pt1.x + pt2.x) / 2, (pt1.y + pt2.y) / 2, 0);

			ptQieDian.transformBy(AcGeMatrix3d::translation(pyXl.normal() *bujin/2));	
			acutPrintf(L"\nptQieDian3=[%.2f,%.2f,%.2f]", ptQieDian.x, ptQieDian.y, ptQieDian.z);
		}

	}

}

int CAreaFenGe::GetDirection()
{
	
	AcDbExtents ext;

	poly->getGeomExtents(ext);

	AcGePoint3d ptMin = ext.minPoint();
	AcGePoint3d ptMax = ext.maxPoint();
	AcGePoint3d ptMin2(ptMax.x, ptMin.y, 0);
	AcGePoint3d ptMax2(ptMin.x, ptMax.y, 0);

	AcGeVector3d lineDirc = l2Pt - l1Pt;

	plHigh = abs(ptMax.y - ptMin.y);
	AcGeVector3d centerPt((l2Pt.x + l1Pt.x) / 2, (l2Pt.y + l1Pt.y) / 2, 0);
	//x=0的情况没判断
	//左下
	if (centerPt.x <= ptMin.x&&centerPt.y <= ptMin.y) {

		direction = 1;

		//向量向左
		if (lineDirc.x < 0) {

			pyXl = lineDirc.rotateBy(-1 * CMathUtil::PI() / 2,AcGeVector3d::kZAxis);

		}
		else {
			pyXl = lineDirc.rotateBy(CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}

	}//左
	//左
	else if (centerPt.x<ptMin.x && (centerPt.y>ptMin.y&&centerPt.y < ptMax2.y))
	{
		direction = 2;

		if (l1Pt.y < l2Pt.y) {
			pyXl = lineDirc.rotateBy(-1 * CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}
		else {
			pyXl = lineDirc.rotateBy(CMathUtil::PI() / 2, AcGeVector3d::kZAxis);
		}
	}	
	//左上
	else if (centerPt.x<ptMin.x&&centerPt.y>ptMax2.y) {
		
		direction = 3;

		if (lineDirc.x < 0) {

			pyXl = lineDirc.rotateBy( CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}
		else {
			pyXl = lineDirc.rotateBy(-1 * CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}

	}
	//上
	else if ((centerPt.x>ptMax2.x&&centerPt.x<ptMax.x) && centerPt.y>ptMax.y) {

		direction = 4;

		//向量向左
		if (lineDirc.x < 0) {

			pyXl = lineDirc.rotateBy( CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}
		else {
			pyXl = lineDirc.rotateBy(-1 * CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}

	}
	//右上
	else if (centerPt.x>ptMax.x&&centerPt.y > ptMax.y) {

		direction = 5;

		if (lineDirc.x < 0) {

			pyXl = lineDirc.rotateBy( CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}
		else {
			pyXl = lineDirc.rotateBy(-1 * CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}

	}
	//右
	else if (centerPt.x>ptMax.x && (centerPt.y > ptMin2.y&&centerPt.y < ptMax.y)) {

		direction = 6;

		if (l1Pt.y < l2Pt.y) {
			pyXl = lineDirc.rotateBy( CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}
		else {
			pyXl = lineDirc.rotateBy(-1 * CMathUtil::PI() / 2, AcGeVector3d::kZAxis);
		}

	}
    //右下
	else if (centerPt.x < ptMin2.x&&centerPt.y < ptMin2.y) {
		direction = 7;
		//向量向左
		if (lineDirc.x < 0) {

			pyXl = lineDirc.rotateBy(-1 * CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}
		else {
			pyXl = lineDirc.rotateBy(CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}
	}
	//下
	else {
		direction = 8;
		//向量向左
		if (lineDirc.x < 0) {

			pyXl = lineDirc.rotateBy(-1 * CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}
		else {
			pyXl = lineDirc.rotateBy(CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

		}
	}

	return direction;
}

//得到辅助线和封闭多段线的切点
AcGePoint3d CAreaFenGe::GetQieDian(AcDbLine * &line1)
{

	line1 = new AcDbLine(l1Pt, l2Pt);
	
	AcGePoint3dArray ptArr;

	int times = 0;

	while (ptArr.length() < 1) {

		poly->intersectWith(line1, AcDb::Intersect::kOnBothOperands, ptArr, 0, 0);

		  line1->transformBy(AcGeMatrix3d::translation(pyXl.normal() * 1));

		/*AcDbEntity * pEnt = NULL;
		line1->getTransformedCopy(AcGeMatrix3d::translation(pyXl.normal() * 1), pEnt);
		line1->transformBy(AcGeMatrix3d::translation(pyXl.normal() * 1));
		CDwgDataBaseUtil::PostToModelSpace(pEnt);

		pEnt->close();*/

		times++;


	}
	poly->close();

	if (ptArr.length() > 0)
		return ptArr[0];
	else
		return AcGePoint3d::kOrigin;
		

}

bool  CAreaFenGe::GetPyPolyline(AcGePoint3d seedPoint,double & a)
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
	acutPrintf(L"\nbound=%d", es);
	return false;
	}

	AcDbPolyline * pl = NULL;
		pl=(AcDbPolyline*)ptrArr[0];

	pl->setColorIndex(1);

	pl->getArea(a);
	/*pl->setColorIndex(1);

	CDwgDataBaseUtil::PostToModelSpace(pl);*/
	pl->erase();
	pl->close();
	pl = NULL;
	return true;
	
}
