#include "StdAfx.h"
#include "resource.h"
#include<math.h>
#include<vector>
using namespace std;

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")
int index = 1;
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdQieXianApp : public AcRxArxApp {
public:	struct MyAttr
{
	ACHAR* attrName;
	ACHAR* attrVal;

};

public:
	CEcdQieXianApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}
	static void ECDMyGroupEcdMakeBJ() {

		AcDbCircle *cir = new AcDbCircle(AcGePoint3d::kOrigin, AcGeVector3d::kZAxis, 50);

		AcGePoint3d pt(70, 0, 0);


		AcGePoint3d ptCenter = cir->center();
		double r = cir->radius();

		AcGePoint3d ptSet = pt;

		double dis = ptSet.distanceTo(ptCenter);

		double sinangle = r / dis;

		double angle = asin(sinangle);

		double len = sqrt(dis*dis - r*r);

		AcGeVector3d vec = ptCenter - ptSet;
		AcGeVector3d vec2 = ptCenter - ptSet;

		vec.rotateBy(angle, AcGeVector3d::kZAxis);

		vec2.rotateBy(-angle, AcGeVector3d::kZAxis);

		AcGePoint3d end1 = ptSet + vec.normal()*len;
		AcGePoint3d end2 = ptSet + vec2.normal()*len;

		AcDbLine * l1 = new AcDbLine(ptSet, end1);
		AcDbLine * l2 = new AcDbLine(ptSet, end2);


		AcGePoint2dArray vertexPts;
		AcGeDoubleArray vertexBulges;

		vertexPts.append(AcGePoint2d(ptSet.x, ptSet.y));
		vertexPts.append(AcGePoint2d(end1.x, end1.y));
		vertexPts.append(AcGePoint2d(end2.x, end2.y));

		double dBulge = 0.0;

		dBulge = tan((angle*2) / 4.0);

		vertexBulges.append(0);
		vertexBulges.append(dBulge);
		vertexBulges.append(0);

		AcDbHatch* pHatch = new AcDbHatch();

		AcGeVector3d normal(0.0, 0.0, 1.0);
		pHatch->setNormal(normal);
		pHatch->setElevation(0.0);

		pHatch->setAssociative(Adesk::kFalse);


		pHatch->setPattern(AcDbHatch::kPreDefined, TEXT("SOLID"));

		pHatch->setHatchStyle(AcDbHatch::kNormal);

		pHatch->appendLoop(AcDbHatch::kExternal, vertexPts, vertexBulges);

		pHatch->setColorIndex(4);

		AcDbDatabase *db = acdbHostApplicationServices()->workingDatabase();
		AcDbObjectId recId;
		//构造块

		AcDbBlockTable *pBlkTable = NULL;

		if (db->getSymbolTable(pBlkTable, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
		{
			delete cir;
			cir = NULL;
			delete l1;
			l1 = NULL;
			delete l2;
			l2 = NULL;
			delete pHatch;
			pHatch = NULL;

			return;
		}

		if (pBlkTable->has(L"LLL_BJ")) {

			pBlkTable->getAt(L"LLL_BJ", recId);
			pBlkTable->close();

		}
		else {

			AcDbBlockTableRecord * pBlkRec = new AcDbBlockTableRecord();

			pBlkRec->setName(L"LLL_BJ");

			pBlkRec->appendAcDbEntity(cir);
			pBlkRec->appendAcDbEntity(l1);
			pBlkRec->appendAcDbEntity(l2);
			pBlkRec->appendAcDbEntity(pHatch);

			//增加块定义
			AcDbAttributeDefinition *attrDef1 = new AcDbAttributeDefinition(AcGePoint3d(-10, -10, 0), L"1", L"标号", L"请输入标号：");

			attrDef1->setHeight(20);

			pBlkRec->appendAcDbEntity(attrDef1);

			pBlkRec->setOrigin(ptCenter);
			pBlkTable->add(recId, pBlkRec);
			
			cir->close();
			l1->close();
			l2->close();
			pHatch->close();

			attrDef1->close();
			
			pBlkRec->close();
			
			pBlkTable->close();
			acutPrintf(L"块构建成功.\n");
		}
		//插入块
		ads_point adPt;

		if (acedGetPoint(NULL, L"请插入标记\n", adPt) != RTNORM) {

			return;

		}

		vector<MyAttr> vecA;
		CString str;

		str.Format(L"%d", index++);
		MyAttr att = { L"标号",str.AllocSysString() };
		vecA.push_back(att);
		InsertBlkWidthAttr(recId, asPnt3d(adPt), vecA);


	}
	static void ECDMyGroupEcdYuanQX() {
		// Put your command code here

		ads_name sName;
		ads_point pt;
		ads_point setPt;

		AcDbObjectId cirId;

		if (acedEntSel(L"\n请选择圆", sName, pt) != RTNORM) {

			return;
		}
		acdbGetObjectId(cirId, sName);

		AcDbCircle *cir = NULL;

		if (acdbOpenObject(cir, cirId, AcDb::kForRead) != ErrorStatus::eOk) {
			acutPrintf(L"选择对象是圆，不是圆形的多段线\n");
			return;
		}
		if (acedGetPoint(NULL, L"选择切线开始点：\n", setPt) != RTNORM) {

			cir->close();
			return;

		}

		AcGePoint3d ptCenter = cir->center();
		double r = cir->radius();

		AcGePoint3d ptSet = asPnt3d(setPt);

		double dis = ptSet.distanceTo(ptCenter);

		double sinangle = r / dis;

		double angle = asin(sinangle);

		double len = sqrt(dis*dis - r*r);


		/*	AcDbLine * l1 = new AcDbLine(ptCenter, ptSet);
			AcDbLine * l2 = new AcDbLine(ptCenter, ptSet);

			l1->transformBy(AcGeMatrix3d::rotation(angle, AcGeVector3d::kZAxis, ptSet));
			l2->transformBy(AcGeMatrix3d::rotation(-angle, AcGeVector3d::kZAxis, ptSet));*/

		AcGeVector3d vec = ptCenter - ptSet;
		AcGeVector3d vec2 = ptCenter - ptSet;

		vec.rotateBy(angle, AcGeVector3d::kZAxis);

		vec2.rotateBy(-angle, AcGeVector3d::kZAxis);

		AcGePoint3d end1 = ptSet + vec.normal()*len;
		AcGePoint3d end2 = ptSet + vec2.normal()*len;

		AcDbLine * l1 = new AcDbLine(ptSet, end1);
		AcDbLine * l2 = new AcDbLine(ptSet, end2);

		PostToModelSpace(l1);
		PostToModelSpace(l2);
		l1->close();
		l2->close();
		cir->close();

	}
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase()) {

		if (pEnt == NULL) {
			return AcDbObjectId::kNull;
		}
		AcDbBlockTable *pTable;

		AcDbBlockTableRecord *blkTblRec;

		AcDbObjectId objId;

		pDb->getBlockTable(pTable, AcDb::OpenMode::kForRead);
		pTable->getAt(ACDB_MODEL_SPACE, blkTblRec, AcDb::OpenMode::kForWrite);

		pTable->close();

		Acad::ErrorStatus status = blkTblRec->appendAcDbEntity(objId, pEnt);

		if (status != Acad::eOk) {

			blkTblRec->close();

			delete pEnt;
			pEnt = NULL;

			return AcDbObjectId::kNull;

		}

		blkTblRec->close();
		pEnt->close();

		return objId;
	}
	static AcDbObjectId InsertBlkWidthAttr(AcDbObjectId blkRecId, AcGePoint3d &ptInsert, vector<MyAttr> attrs, AcDbDatabase * db = acdbHostApplicationServices()->workingDatabase())
	{
		if (blkRecId.isNull())
			return blkRecId;
		AcDbBlockTableRecord *rec;

		AcDbBlockReference *br = new AcDbBlockReference(ptInsert, blkRecId);

		if (acdbOpenObject(rec, blkRecId, AcDb::OpenMode::kForRead) != ErrorStatus::eOk)
		{
			delete br;

			return AcDbObjectId::kNull;
		}

		if (rec->hasAttributeDefinitions()) {

			AcDbBlockTableRecordIterator *pItr = NULL;

			rec->newIterator(pItr);

			for (pItr->start(); !pItr->done(); pItr->step()) {

				AcDbEntity* ent = NULL;


				if (pItr->getEntity(ent) == ErrorStatus::eOk) {

					AcDbAttributeDefinition *def = AcDbAttributeDefinition::cast(ent);

					if (def != NULL) {

						AppendAttrDef2Br(br, attrs, def);

						ent->close();
					}
				}
			}
			delete pItr;
		}
		rec->close();

		AcDbObjectId brId = PostToModelSpace(br);

		br->close();
		return brId;


	}
	static void AppendAttrDef2Br(AcDbBlockReference * br, vector<MyAttr> attrs, AcDbAttributeDefinition * def)
	{

		AcDbAttribute *attr = new AcDbAttribute();
		attr->setPropertiesFrom(br);
		attr->setLayer(br->layerId());
		attr->setInvisible(def->isInvisible());
		attr->setPosition(def->position());
		attr->setHeight(def->height());
		attr->setWidthFactor(def->widthFactor());
		attr->setRotation(def->rotation());
		attr->setHorizontalMode(def->horizontalMode());
		attr->setVerticalMode(def->verticalMode());
		attr->setAlignmentPoint(def->alignmentPoint());
		attr->setTextStyle(def->textStyle());
		attr->setAttributeFromBlock(br->blockTransform());


		ACHAR *pStr = def->tag();

		for (size_t i = 0; i < attrs.size(); i++)
		{

			//pStr = attrs[i].attrName;
			if (_tcscmp(attrs[i].attrName, pStr) == 0) {

				attr->setTag(pStr);
				acutDelString(pStr);
				attr->setTextString(attrs[i].attrVal);
				break;
			}
		}

		attr->setFieldLength(def->fieldLength());
		br->appendAttribute(attr);

		attr->close();

	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdQieXianApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdQieXianApp, ECDMyGroup, EcdYuanQX, EcdYuanQX, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CEcdQieXianApp, ECDMyGroup, EcdMakeBJ, EcdMakeBJ, ACRX_CMD_MODAL, NULL)


