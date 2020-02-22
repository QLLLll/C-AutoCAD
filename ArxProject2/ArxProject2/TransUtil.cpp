#include "stdafx.h"
#include "TransUtil.h"
#include"DwgDataBaseUtil.h"

CTransUtil::CTransUtil()
{
}


CTransUtil::~CTransUtil()
{
}

AcDbObjectId CTransUtil::GetId(const ACHAR *tips)
{
	ads_name entName;
	ads_point pt;
	if (acedEntSel(tips, entName, pt) == RTNORM) {
	
		AcDbObjectId oId;

		acdbGetObjectId(oId, entName);

		return oId;

	}
	else {
		return AcDbObjectId::kNull;
	}


}

AcDbObjectId CTransUtil::MyClone(AcDbObjectId objId)
{
	if (objId == AcDbObjectId::kNull) 
	{
		return objId; 
	}
	else 
	{
		AcDbEntity *pEnt = NULL;

		if (acdbOpenAcDbEntity(pEnt, objId, AcDb::OpenMode::kForRead) != ErrorStatus::eOk)
			return AcDbObjectId::kNull;

		AcDbEntity *copyEnt = AcDbEntity::cast(pEnt->clone());

		AcDbObjectId copyId=CDwgDataBaseUtil::PostToModelSpace(copyEnt);

		acutPrintf(TEXT("clone suceess£¡"));

		pEnt->close();

		return copyId;

	}
}

void CTransUtil::Move(const AcGePoint3d & basePoint, const AcGePoint3d & movePoint, AcDbObjectId objId)
{
	AcDbEntity *pEnt = NULL;


	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
		return;
	
	
	AcGeMatrix3d matrxMove;

	AcGeVector3d v = movePoint - basePoint;

	matrxMove.setTranslation(v);

	pEnt->transformBy(matrxMove);

	pEnt->close();


}

void CTransUtil::Rotate(const AcGePoint3d & basePoint, const double rotation, AcDbObjectId objId)
{
	AcDbEntity *pEnt = NULL;

	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
		return;

	AcGeVector3d normal = AcGeVector3d(0, 0, 1);

	pEnt->transformBy(AcGeMatrix3d::rotation(rotation, normal, basePoint));
	acutPrintf(TEXT("rotate suceess£¡"));
	pEnt->close();
}

void CTransUtil::Scale(AcDbObjectId objId, double scale,AcGePoint3d &ptBase)
{

	AcDbEntity *pEnt = NULL;

	if (acdbOpenAcDbEntity(pEnt, objId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
		return;

	pEnt->transformBy(AcGeMatrix3d::scaling(scale, ptBase));

	pEnt->close();
	acutPrintf(TEXT("scale suceess£¡"));
}


