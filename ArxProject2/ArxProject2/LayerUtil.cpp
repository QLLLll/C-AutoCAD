#include "stdafx.h"
#include "LayerUtil.h"


CLayerUtil::CLayerUtil()
{
}


CLayerUtil::~CLayerUtil()
{
}

AcDbObjectId CLayerUtil::Add(const ACHAR * layerName, const int colorIndex)
{
	assert(layerName != NULL);
	assert(colorIndex >= 1 && colorIndex <= 255);

	AcDbLayerTable * lTable = NULL;

	if (acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lTable, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
		return AcDbObjectId::kNull;

	if (lTable->has(layerName)) {

		lTable->close();

		acutPrintf(TEXT("已存在该层名\n"));

		return AcDbObjectId::kNull;
	}
	else {

		AcDbLayerTableRecord *lRec = new AcDbLayerTableRecord();

		lRec->setName(layerName);
		AcCmColor *c=new AcCmColor();
		c->setColorIndex(colorIndex);
		lRec->setColor(*c);

		AcDbObjectId  lId;

		lTable->add(lId,lRec);

		lRec->close();

		delete c;

		lTable->close();
		acutPrintf(TEXT("添加成功\n"));
		return lId;
	}





}

AcDbObjectId CLayerUtil::GetLayerId(const TCHAR * layerName)
{
	assert(layerName != NULL);
	AcDbLayerTable *lTable = NULL;

	if (acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lTable, AcDb::OpenMode::kForRead) != ErrorStatus::eOk)
		return AcDbObjectId::kNull;

	AcDbObjectId lId;

	if (lTable->has(layerName)) {

		lTable->getAt(layerName, lId);

	}

	lTable->close();

	return lId;
}

bool CLayerUtil::SetColor(const TCHAR * layerName, const int colorIndex)
{
	AcDbObjectId lId = GetLayerId(layerName);
	assert(colorIndex >= 1 && colorIndex <= 255);
	if (lId.isNull()) {
		return false;
	}
	else
	{
		AcDbLayerTableRecord *lRec = NULL;

		if (acdbOpenObject(lRec, lId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
			return false;
		AcCmColor *color = new AcCmColor();

		color->setColorIndex(colorIndex);
		lRec->setColor(*color);

		delete color;

		lRec->close();

		return true;

	}

}

void CLayerUtil::GetLayerList(AcDbObjectIdArray & lIds)
{
	AcDbLayerTable *lTable = NULL;

	acdbHostApplicationServices()->workingDatabase()->getSymbolTable(lTable, AcDb::OpenMode::kForRead);

	AcDbLayerTableIterator * lIter = NULL;

	lTable->newIterator(lIter);



	for (lIter->start(); !lIter->done();lIter->step())
	{

		AcDbObjectId lId;
		lIter->getRecordId(lId);

		lIds.append(lId);

	}

	delete lIter;
}
