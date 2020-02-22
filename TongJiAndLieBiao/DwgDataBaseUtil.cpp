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

AcDbObjectIdArray CDwgDataBaseUtil::GetAllEntityIds(const TCHAR* layerName, AcDbDatabase *pDb)
{
	AcDbObjectIdArray entIds;		// 满足条件的实体集合
	bool bFilterLayer = false;			// 是否需要过滤图层
	AcDbObjectId layerId;
	// 获得指定图层的对象ID
	if (layerName != NULL)
	{
		AcDbLayerTable *pLayerTbl = NULL;
		acdbHostApplicationServices()->workingDatabase()
			->getSymbolTable(pLayerTbl, AcDb::kForRead);
		if (!pLayerTbl->has(layerName))
		{
			pLayerTbl->close();
			return entIds;
		}
		pLayerTbl->getAt(layerName, layerId);
		pLayerTbl->close();

		bFilterLayer = true;
	}

	// 获得块表
	AcDbBlockTable *pBlkTbl = NULL;
	pDb->getBlockTable(pBlkTbl, AcDb::kForRead);

	// 获得模型空间的块表记录
	AcDbBlockTableRecord *pBlkTblRcd = NULL;
	pBlkTbl->getAt(ACDB_MODEL_SPACE, pBlkTblRcd, AcDb::kForRead);
	pBlkTbl->close();

	// 创建遍历器，依次访问模型空间的每一个实体
	AcDbBlockTableRecordIterator *it = NULL;
	pBlkTblRcd->newIterator(it);
	for (it->start(); !it->done(); it->step())
	{
		AcDbEntity *pEnt = NULL;
		Acad::ErrorStatus es = it->getEntity(pEnt, AcDb::kForRead);
		if (es == Acad::eOk)
		{
			if (bFilterLayer)				// 过滤图层
			{
				if (pEnt->layerId() == layerId)
				{
					entIds.append(pEnt->objectId());
				}
			}
			else
			{
				entIds.append(pEnt->objectId());
			}

			pEnt->close();
		}
		else
		{
			acutPrintf(TEXT("\nCDwgDatabaseUtil::GetAllEntityIds中打开实体失败(错误代码:%d)."), (int)es);
		}
	}
	delete it;
	pBlkTblRcd->close();

	return entIds;
}
