#include "stdafx.h"
#include "EcdKuaiJX.h"
#include"DwgDataBaseUtil.h"
#include"SelectUtil.h"
#include<vector>
//#include"GePointUtil.h"
#include"GetInputUtil.h"
//#include"MathUtil.h"
//#include"ConvertUtil.h"
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
	list.clear();
	listOId.removeAll();


	AcDbBlockReference *br = NULL;

	AcRxClass *cls = AcDbBlockReference::desc();

	AcGePoint3d pt;

	if (!CSelectUtil::PromptSelectEntity(L"\n请选择要镜像的块：", cls, (AcDbEntity*&)br, pt, true))
		return;

	AcDbObjectId pBlkTblRecId;
	pBlkTblRecId = br->blockTableRecord();
	AcDbBlockTableRecord *pBlkTblRec = NULL;

	ErrorStatus es = acdbOpenObject(pBlkTblRec, pBlkTblRecId, AcDb::kForRead);

	if (es != ErrorStatus::eOk) {
		acutPrintf(L"\n获取块名出错：%d", es);
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
	char *timeStff = "LL";
	//char*  转ACHAR
	size_t len = strlen(timeStff) + 1;
	size_t converted = 0;
	wchar_t *WStr;
	WStr = (wchar_t*)malloc(len*sizeof(wchar_t));
	mbstowcs_s(&converted, WStr, len, timeStff, _TRUNCATE);

	wcscat(name, WStr);

	blkRec->setName(name);
	blkRec->setOrigin(ptPosMirror);

	for (int i = 0; i < (int)list.size(); i++) {

		AcDbEntity * ent = list.at(i);

		if (ent != NULL) {

			blkRec->appendAcDbEntity(ent);
		}
	}

	 es =pTable->add(breNewId, blkRec);

	if (es != ErrorStatus::eOk) {

		acutPrintf(L"\n%d", es);

	}

	for each (AcDbObjectId  oId in listOId)
	{

		AcDbEntity * ent = NULL;

		if (acdbOpenObject(ent,oId, AcDb::kForWrite) == ErrorStatus::eOk) {


			ent->erase();

			ent->close();

		}


	}

	listOId.removeAll();

	for (int i = 0; i < (int)list.size(); i++)
	{

		list.at(i)->close();

	}

	AcDbBlockReference *brOld = new AcDbBlockReference(ptPos, pBlkTblRec->objectId());
	AcDbBlockReference * brNew = new AcDbBlockReference(ptPosMirror, breNewId);
 
	blkRec->close();
	pTable->close();

	CDwgDataBaseUtil::PostToModelSpace(brOld);
	CDwgDataBaseUtil::PostToModelSpace(brNew);

	

	br->close();
	pBlkTblRec->close();
}

void EcdKuaiJX::MirrorText(AcDbText *text, AcGeLine3d mirrorLine)
{

	AcDbText *dbText = NULL;

	if (acdbOpenObject(dbText, (*text).objectId(), AcDb::kForWrite) != ErrorStatus::eOk)
		return;

	AcDbText * mirroredTxt = AcDbText::cast(dbText->clone());

	AcGeMatrix3d mirrorMatrix = AcGeMatrix3d::mirroring(mirrorLine);

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
	list.push_back(mirroredTxt);

	dbText->close();
	text->close();
}

void EcdKuaiJX::MirrorText(AcDbMText *mText, AcGeLine3d mirrorLine)
{
}

void EcdKuaiJX::GetTextBoxCorners(AcDbText &dbText, AcGePoint3d &pt1, AcGePoint3d &pt2, AcGePoint3d &pt3, AcGePoint3d &pt4)
{
	ads_name name;

	int result = acdbGetAdsName(name, dbText.objectId());

	resbuf * buf = acdbEntGet(name);

	ads_point point1, point2;

	acedTextBox(buf, point1, point2);

	pt1 = asPnt3d(point1);
	pt2 = asPnt3d(point2);

	AcGeMatrix3d rotMat = AcGeMatrix3d::rotation(dbText.rotation(), dbText.normal(), pt1);

	pt1 = pt1.transformBy(rotMat) + dbText.position().asVector();
	pt2 = pt2.transformBy(rotMat) + dbText.position().asVector();

	AcGeVector3d rotDir = AcGeVector3d(
		-sin(dbText.rotation()),
		cos(dbText.rotation()),
		0
		);

	AcGeVector3d linDir = rotDir.crossProduct(dbText.normal());

	double actualWidth =
		abs((pt2.asVector() - pt1.asVector()).dotProduct(linDir));

	pt3 = pt1 + linDir*actualWidth;
	pt4 = pt2 - linDir*actualWidth;

}

void EcdKuaiJX::MyMirror(vector<AcDbEntity*> listEnt, AcGeLine3d l3d, char xY)
{
	for (int i = 0; i < (int)listEnt.size(); i++)
	{

		AcDbEntity * ent = listEnt.at(i);

		AcDbEntity * ent2 = NULL;

		if (ent->isKindOf(AcDbText::desc()) || ent->isKindOf(AcDbMText::desc())) {

			listOId.append(CDwgDataBaseUtil::PostToModelSpace(ent));
		}
		else {

			ent->getTransformedCopy(AcGeMatrix3d::mirroring(l3d), ent2);


			if ((AcDbDimension *)ent2 == NULL) {

				list.push_back(ent2);

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
		listEnt.at(i)->close();
	}

}
