#include "stdafx.h"
#include "BlockUtil.h"
#include"DwgDataBaseUtil.h"



BlockUtil::BlockUtil()
{
}


BlockUtil::~BlockUtil()
{
}

AcDbObjectId BlockUtil::CreateBlk(AcDbDatabase * db)
{
	AcDbBlockTable *pBlkTable;

	if (db->getSymbolTable(pBlkTable, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
		return AcDbObjectId::kNull;

	AcDbBlockTableRecord * pBlkRec = new AcDbBlockTableRecord();

	
	bool isNameOK = true;
	wchar_t  buff[40];
	if (acedGetString(1, L"\n请输入块名", buff) == RTNORM)
	{
		while (pBlkTable->has(buff)) {

			CString s;
			s.Format(_T("%s"), buff);

			AfxMessageBox(TEXT("已存在: " + s + "的块"));

			//清零
			memset(buff, 0, sizeof(buff));

			s.Format(_T("%s"), buff);

			acutPrintf("是否清零" + s + "\n");

			if (acedGetString(1, L"\n请输入块名", buff) == RTNORM)
			{
				continue;
			}
			else 
			{
				isNameOK = false;
				break;
			}

		}
	}

	if (!isNameOK)
	{

		delete pBlkRec;

		pBlkTable->close();

		return AcDbObjectId::kNull;

	}
	pBlkRec->setName(buff);

	AcDbObjectId recId;

	pBlkTable->add(recId,pBlkRec);

	// 向块表记录中添加实体
	AcGePoint3d ptStart(-10, 0, 0), ptEnd(10, 0, 0);
	AcDbLine *pLine1 = new AcDbLine(ptStart, ptEnd); // 创建一条直线

	ptStart.set(0, -10, 0);
	ptEnd.set(0, 10, 0);
	AcDbLine *pLine2 = new AcDbLine(ptStart, ptEnd); // 创建一条直线

	AcGeVector3d vecNormal(0, 0, 1);
	AcDbCircle *pCircle = new AcDbCircle(AcGePoint3d::kOrigin, vecNormal, 6);

	pBlkRec->appendAcDbEntity(pLine1);
	pBlkRec->appendAcDbEntity(pLine2);
	pBlkRec->appendAcDbEntity(pCircle);


	//增加块定义
	AcDbAttributeDefinition *attrDef = new AcDbAttributeDefinition(ptEnd, L"20", L"直径长度", L"请输入直径");

	pBlkRec->appendAcDbEntity(attrDef);


	pCircle->close();
	pLine2->close();
	pLine1->close();
	pBlkRec->close();
	pBlkTable->close();
	attrDef->close();

	return recId;
}

AcDbObjectId BlockUtil::InsertBlk(const ACHAR * blkName, AcDbDatabase * db)
{
	AcDbBlockTable *pBlkTbl;

	db->getSymbolTable(pBlkTbl, AcDb::OpenMode::kForRead);

	if (pBlkTbl->has(blkName)) {

		AcDbObjectId blkRecId;

		pBlkTbl->getIdAt(blkName, blkRecId);

		return BlockUtil::InsertBlk(blkRecId, db);

	}
	return AcDbObjectId::kNull;
}

AcDbObjectId BlockUtil::InsertBlk(AcDbObjectId blkId, AcDbDatabase * db)
{
	ads_point ptInsert;
	if (blkId.isNull()) {
		return blkId;
	}


	if (acedGetPoint(NULL, _T("清选择块参照插入点：\n"), ptInsert) != RTNORM)
		return AcDbObjectId::kNull;

	AcGePoint3d pt = asPnt3d(ptInsert);

	AcDbBlockReference *br = new AcDbBlockReference(pt, blkId);

	br->setScaleFactors(AcGeScale3d(5, 5, 5));
	br->setColorIndex(3);

	AcDbObjectId brId = CDwgDataBaseUtil::PostToModelSpace(br);

	br->close();
	return brId;

}

AcDbObjectId BlockUtil::InsertBlkWidthAttr(AcDbObjectId blkRecId, AcGePoint3d &ptInsert, AcDbDatabase * db)
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

					AppendAttrDef2Br(br, def);
					
					ent->close();
				}
			}
		}
		delete pItr;
	}
	rec->close();

	AcDbObjectId brId= CDwgDataBaseUtil::PostToModelSpace(br);

	br->close();
	return brId;


}

void BlockUtil::AppendAttrDef2Br(AcDbBlockReference * br, AcDbAttributeDefinition * def)
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


	TCHAR *pStr = def->tag();
	attr->setTag(pStr);
	acutDelString(pStr);
	attr->setFieldLength(def->fieldLength());
	attr->setTextString(def->textString());

	br->appendAttribute(attr);

	attr->close();

}

void BlockUtil::SetAttribute2Br(AcDbBlockReference * br,  const TCHAR * tag, const TCHAR * val)
{
	ads_point ptInsert;


	acedGetPoint(NULL, _T("请输入属性插入的点"), ptInsert);
	//acedGetPoint(NULL, _T("请输入终点"), movePt);

	AcGePoint3d pt1 = asPnt3d(ptInsert);

	AcDbAttribute *attr = new AcDbAttribute(pt1, val, tag);

	br->appendAttribute(attr);

	attr->close();

}
