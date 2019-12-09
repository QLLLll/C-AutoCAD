#include "stdafx.h"
#include "RunCommand.h"


RunCommand::RunCommand()
{
}


RunCommand::~RunCommand()
{
}

AcDbObjectId RunCommand::CreateLine() {
	AcGePoint3d pStart(0, 0, 0);
	AcGePoint3d pEnd(100, 100, 0);

	AcDbLine *pLine = new AcDbLine(pStart, pEnd);

	AcDbBlockTable *pBlockTable = NULL;

	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable, AcDb::OpenMode::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;

	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);

	AcDbObjectId lineId;

	pBlockTableRecord->appendAcDbEntity(lineId, pLine);

	pBlockTable->close();

	pBlockTableRecord->close();

	pLine->close();

	return lineId;
}

Acad::ErrorStatus RunCommand::ChangeColor(AcDbObjectId entId, Adesk::UInt16 coloridx) {

	AcDbEntity*pEnt;

	acdbOpenObject(pEnt, entId, AcDb::OpenMode::kForWrite);

	if (pEnt != NULL) {

		pEnt->setColorIndex(coloridx);

		pEnt->close();

		return Acad::eOk;

	}
	return Acad::eInvalidXrefObjectId;
}