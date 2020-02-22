#include "stdafx.h"
#include "EcdKuaiJX.h"
#include"DwgDataBaseUtil.h"
#include"SelectUtil.h"
#include<vector>
//#include"GePointUtil.h"
#include"GetInputUtil.h"
#include"MathUtil.h"
#include"ConvertUtil.h"
#include"time.h"
using namespace std;
EcdKuaiJX::EcdKuaiJX()
{
}


EcdKuaiJX::~EcdKuaiJX()
{
}

void EcdKuaiJX::Command()
{

	
	if(!listVec.empty())
	listVec.clear();
	if(!listOId.isEmpty())
	listOId.removeAll();


	AcDbBlockReference *br = NULL;

	AcRxClass *cls = AcDbBlockReference::desc();

	AcGePoint3d pt;

	if (!CSelectUtil::PromptSelectEntity(L"\n请选择要镜像的块：", cls, (AcDbEntity*&)br, pt, true))
		return;


	//得到块名
	AcDbObjectId pBlkTblRecId;
	pBlkTblRecId = br->blockTableRecord();
	AcDbBlockTableRecord *pBlkTblRec = NULL;

	ErrorStatus es = acdbOpenObject(pBlkTblRec, pBlkTblRecId, AcDb::kForRead);

	if (es != ErrorStatus::eOk) {
		acutPrintf(L"\n获取块记录出错：%d", es);
		br->close();
		return;
	}
	ACHAR *name;
	pBlkTblRec->getName(name);

	AcGePoint3d ptPos = br->position();
	AcGePoint3d ptPosMirror;


	int ret = 0;

	CGetInputUtil::GetKeyword(L"\n请选择镜像方向[X/Y]", L"X Y", L"X", 0, ret);

	AcDbExtents bounds;

	br->bounds(bounds);

	AcGePoint3d ptMax = bounds.maxPoint();
	AcGePoint3d ptMin = bounds.minPoint();

	double maxY = abs(ptMax.y - ptMin.y);
	double maxX = abs(ptMax.x - ptMin.x);




	AcDbVoidPtrArray coll;

	br->explode(coll);

	vector<AcDbEntity *> listEnt;

	for (int i = 0; i < coll.length(); i++)
	{
		listEnt.push_back((AcDbEntity *)coll.at(i));
	}
	AcGeMatrix3d mtx = AcGeMatrix3d();

	//Y
	if (ret == 1) {

		AcGePoint3d ptEnd = ptPos + AcGeVector3d::kYAxis * 100;

		AcGeLine3d lineY = AcGeLine3d(ptPos, ptEnd);

		lineY.transformBy(mtx.setTranslation(AcGeVector3d::kXAxis*maxX));

		ptPosMirror = ptPos.transformBy(AcGeMatrix3d::mirroring(lineY));

		MyMirror(listEnt, lineY, 'Y');
	}
	else {

		AcGePoint3d ptEnd = ptPos + AcGeVector3d::kXAxis * 100;

		AcGeLine3d lineX = AcGeLine3d(ptPos, ptEnd);

		lineX.transformBy(mtx.setTranslation(AcGeVector3d::kYAxis*maxY));

		ptPosMirror = ptPos.transformBy(AcGeMatrix3d::mirroring(lineX));

		MyMirror(listEnt, lineX, 'X');

	}
	//构建新块
	AcDbObjectId breNewId;

	AcDbBlockTable  * pTable = NULL;

	es = acdbHostApplicationServices()->workingDatabase()->getSymbolTable(pTable, AcDb::OpenMode::kForWrite);

	if (es != ErrorStatus::eOk)
	{
		acutPrintf(L"\n打开块表失败：%d", es);
		br->close();
		pBlkTblRec->close();
		return;
	}

	AcDbBlockTableRecord *blkRec = new AcDbBlockTableRecord();

	blkRec->setBlockInsertUnits(pBlkTblRec->blockInsertUnits());

	time_t  m_time;

	time(&m_time);
	//char *timeStff=ctime(&m_time);

	//char *timeStff = "LL";
	//char*  转ACHAR
	/*size_t len = strlen(timeStff) + 1;
	size_t converted = 0;
	wchar_t *WStr;
	WStr = (wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, timeStff, _TRUNCATE);
	*/

	wchar_t wchar[11] = { 0 };

	swprintf(wchar, 11, L"%d", m_time);

	wcscat(name, wchar);

	blkRec->setName(name);

	//把镜像后的实体添加到新记录中
	for (int i = 0; i < (int)listVec.size(); i++) {

		AcDbEntity * ent = listVec.at(i);

		if (ent != NULL) {

			blkRec->appendAcDbEntity(ent);
		}
	}
	blkRec->setOrigin(ptPosMirror);
	es = pTable->add(breNewId, blkRec);

	if (es != ErrorStatus::eOk) {

		acutPrintf(L"\n%d", es);

	}
	//把原文字删除，因为在镜像文字之前需要把炸开后的原文字加入到模型空间
	for each (AcDbObjectId  oId in listOId)
	{

		AcDbEntity * ent = NULL;

		if (acdbOpenObject(ent, oId, AcDb::kForWrite) == ErrorStatus::eOk) {


			//ent->erase();

			ent->close();

		}


	}



	//关闭镜像后的实体
	for (int i = 0; i < (int)listVec.size(); i++)
	{

		listVec.at(i)->close();

	}

	//AcDbBlockReference *brOld = new AcDbBlockReference(ptPos, pBlkTblRec->objectId());
	AcDbBlockReference *brNew = new AcDbBlockReference(ptPosMirror, breNewId);

	//关闭新块记录
	blkRec->close();
	pTable->close();

	//CDwgDataBaseUtil::PostToModelSpace(brOld);
	CDwgDataBaseUtil::PostToModelSpace(brNew);


	/*AcDbPoint *pt1 = new AcDbPoint(ptPos);
	pt1->setColorIndex(1);
	CDwgDataBaseUtil::PostToModelSpace(pt1);

	AcDbPoint *pt2 = new AcDbPoint(ptPosMirror);
	pt2->setColorIndex(3);
	CDwgDataBaseUtil::PostToModelSpace(pt2);

	pt1->close();
	pt2->close();*/

	br->close();
	pBlkTblRec->close();
}

void EcdKuaiJX::MirrorText(AcDbText *text, AcGeLine3d mirrorLine)
{

	AcDbText *dbText = NULL;

	if (acdbOpenObject(dbText, (*text).objectId(), AcDb::kForWrite) != ErrorStatus::eOk)
		return;

	AcDbText * mirroredTxt = AcDbText::cast(dbText->clone());

	dbText->close();
	text->close();

	AcGeMatrix3d mirrorMatrix = AcGeMatrix3d::mirroring(mirrorLine);


	mirroredTxt->transformBy(mirrorMatrix);

	AcGePoint3d pt1, pt2, pt3, pt4;

	GetTextBoxCorners(*dbText, pt1, pt2, pt3, pt4);



	AcGeVector3d rotDir =
		(pt4 - pt1.asVector()).asVector();

	AcGeVector3d linDir = (pt3 - (pt1.asVector())).asVector();

	AcGeVector3d mirRotDir = rotDir.transformBy(mirrorMatrix);

	AcGeVector3d mirLinDir = linDir.transformBy(mirrorMatrix);

	if (abs(mirrorLine.direction().y) > abs(mirrorLine.direction().x)) {

		
			 mirroredTxt->mirrorInX(!mirroredTxt->isMirroredInX());

		mirroredTxt->setPosition(mirroredTxt->position() + mirLinDir);
	}
	else {
			
			mirroredTxt->mirrorInY(!mirroredTxt->isMirroredInY());

		mirroredTxt->setPosition(mirroredTxt->position() + mirRotDir);
	}
	listVec.push_back(mirroredTxt);


}

void EcdKuaiJX::MirrorText(AcDbMText *mText, AcGeLine3d mirrorLine)
{
}

void EcdKuaiJX::GetTextBoxCorners(AcDbText &dbText, AcGePoint3d &pt1, AcGePoint3d &pt2, AcGePoint3d &pt3, AcGePoint3d &pt4)
{

	bool isMx = dbText.isMirroredInX();
	bool isMy = dbText.isMirroredInY();


	ads_name name;

	int result = acdbGetAdsName(name, dbText.objectId());
	resbuf *buf = NULL;
	buf = acdbEntGet(name);


	struct resbuf *rb=NULL;
	
	acedGetVar(L"ERRNO", rb);
	if(rb!=NULL)
	acutPrintf(L"\nrb=%d", rb->resval.rint);

	ads_point point1, point2;

	if (acedTextBox(buf, point1, point2) != RTNORM) {
		
		struct resbuf *rb1=NULL;

		acedGetVar(L"ERRNO", rb1);

		acutPrintf(L"\nrbbox=%d", rb1->resval.rint);
		acutRelRb(rb1);
	}
	if (buf != NULL)
	acutRelRb(buf);
	if (rb != NULL)
	acutRelRb(rb);
	pt1 = asPnt3d(point1);
	pt2 = asPnt3d(point2);
	
	AcDbLine *line8 = new AcDbLine(pt1, pt2);

	line8->setColorIndex(220);

	CDwgDataBaseUtil::PostToModelSpace(line8);

	line8->close();

	
	AcGeVector3d vecZero = AcGePoint3d::kOrigin - pt1;

	pt1 = AcGePoint3d::kOrigin;
	pt2 = pt2 + vecZero;

	//double ro = 236.0 / 180 * CMathUtil::PI();
	//double ro1= 124.0 / 180 * CMathUtil::PI();
	
	
	double ro = dbText.rotation();

	//AcGeMatrix3d rotMat = AcGeMatrix3d::rotation(CMathUtil::PI() + ro, dbText.normal(), pt1);
	

	
	AcGeMatrix3d rotMat = AcGeMatrix3d::rotation(ro, dbText.normal(), pt1);
	
	
	//pt1 = pt1.transformBy(rotMat) + dbText.position().asVector();
	//pt2 = pt2.transformBy(rotMat) + dbText.position().asVector();

	pt1 = pt1.transformBy(rotMat);
	pt2 = pt2.transformBy(rotMat);


	AcDbLine *line2 = new AcDbLine(pt1, pt2);

	line2->setColorIndex(2);

	CDwgDataBaseUtil::PostToModelSpace(line2);

	line2->close();

	pt1=pt1+ dbText.position().asVector();
	pt2 = pt2 + dbText.position().asVector();

	AcDbLine *line4 = new AcDbLine(pt1, pt2);

	line4->setColorIndex(10);

	CDwgDataBaseUtil::PostToModelSpace(line4);

	line4->close();

	/*AcGeVector3d rotDir = AcGeVector3d(
		-sin(2*dbText.rotation()),
		cos(2*dbText.rotation()),
		0
		);*/
	AcGeVector3d rotDir = AcGeVector3d(
		-sin(ro),
		cos(ro),
		0
		);
		/*AcGeVector3d rotDir = AcGeVector3d(
		-sin(ro1),
		cos(ro1),
		0
		);*/
		rotDir = -1 * rotDir;
	
		pt2 = pt2 + 2*(pt1 - pt2);

	//AcGeVector3d linDir = rotDir.crossProduct(-1 * dbText.normal());
		AcGeVector3d linDir = rotDir.crossProduct(dbText.normal());
		
	double actualWidth =
		abs((pt2.asVector() - pt1.asVector()).dotProduct(linDir));

	

	pt3 = pt1 + linDir*actualWidth;
	pt4 = pt2 - linDir*actualWidth;
	

	AcDbPolyline *pl = new AcDbPolyline();

	pl->addVertexAt(pl->numVerts(), CConvertUtil::ToPoint2d(pt1), 0, 0, 0);
	pl->addVertexAt(pl->numVerts(), CConvertUtil::ToPoint2d(pt2), 0, 0, 0);
	pl->addVertexAt(pl->numVerts(), CConvertUtil::ToPoint2d(pt3), 0, 0, 0);
	pl->addVertexAt(pl->numVerts(), CConvertUtil::ToPoint2d(pt4), 0, 0, 0);

	pl->setColorIndex(1);
	
	CDwgDataBaseUtil::PostToModelSpace(pl);

	pl->close();


}

void EcdKuaiJX::MyMirror(vector<AcDbEntity*> listEnt, AcGeLine3d l3d, char xY)
{
	for (int i = 0; i < (int)listEnt.size(); i++)
	{

		AcDbEntity * ent = listEnt.at(i);

		AcDbEntity * ent2 = NULL;

		if (ent->isKindOf(AcDbText::desc()) || ent->isKindOf(AcDbMText::desc())) {
			AcDbObjectId textId = CDwgDataBaseUtil::PostToModelSpace(ent);
			listOId.append(textId);

			ent->close();

		}
		else {

			ent->getTransformedCopy(AcGeMatrix3d::mirroring(l3d), ent2);

			if (ent2->isKindOf(AcDbDimension::desc()) == false) {
				listVec.push_back(ent2);
				
			}

		}

		if (ent->isKindOf(AcDbText::desc())) {

			AcDbText * a = (AcDbText *)ent;

			MirrorText(a, l3d);
		}
		else if (ent->isKindOf(AcDbMText::desc())) {

			AcDbMText *a = (AcDbMText *)ent;

			MirrorText(a, l3d);
		}
		else if ((AcDbDimension *)ent != NULL) {




		}
	}
	for (int i = 0; i < (int)listEnt.size(); i++)
	{
		AcDbEntity *ent1 = listEnt.at(i);
		if (ent1->isKindOf(AcDbText::desc()) || ent1->isKindOf(AcDbMText::desc())) {

			continue;

		}
		ent1->close();
	}

}
