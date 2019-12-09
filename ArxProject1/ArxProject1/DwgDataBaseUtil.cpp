#include "stdafx.h"
#include "DwgDataBaseUtil.h"


CDwgDataBaseUtil::CDwgDataBaseUtil()
{
}


CDwgDataBaseUtil::~CDwgDataBaseUtil()
{
}

AcDbObjectId CDwgDataBaseUtil::PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb) {

	if (pEnt == NULL) {
		return AcDbObjectId::kNull;
	}
	AcDbBlockTable *pTable;

	AcDbBlockTableRecord *blkTblRec;

	AcDbObjectId objId;

	pDb->getBlockTable(pTable, AcDb::OpenMode::kForRead);
	pTable->getAt(ACDB_MODEL_SPACE, blkTblRec,AcDb::OpenMode::kForWrite);
	
	pTable->close();
	
	Acad::ErrorStatus status=blkTblRec->appendAcDbEntity(objId,pEnt);

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
