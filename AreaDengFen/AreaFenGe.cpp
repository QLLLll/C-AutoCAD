#include "stdafx.h"
#include "AreaFenGe.h"
#include "StringUtil.h"
#include"DwgDataBaseUtil.h"
#include"MathUtil.h"

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

}


CAreaFenGe::~CAreaFenGe()
{

	poly->close();
	delete line;
	line = NULL;

}

void CAreaFenGe::Command()
{
	acutPrintf(L"\ndir=%d", direction);




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
