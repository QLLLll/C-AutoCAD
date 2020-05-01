#include "stdafx.h"
#include "DimMirror.h"
#include"DwgDataBaseUtil.h"
#include"SelectUtil.h"

CDimMirror::CDimMirror()
{
}


CDimMirror::~CDimMirror()
{
}

void CDimMirror::Command()
{

	AcDbDimension * pDim = NULL;

	AcGePoint3d ptPick;

	AcRxClass *cls = AcDbDimension::desc();

	if (CSelectUtil::PromptSelectEntity(TEXT("\nÕˆÑ¡Ôñ±ê×¢:"), cls, (AcDbEntity*&)pDim, ptPick)) {

		AcGeVector3d oNormal = pDim->normal();

		AcGePoint3d pt = pDim->textPosition();

		AcGePoint3d ptStart = pt + AcGeVector3d::kXAxis * 100;

		AcGePoint3d ptEnd = ptStart + AcGeVector3d::kYAxis * 100;

		AcGeLine3d l3d = AcGeLine3d(ptStart, ptEnd);


		AcGeMatrix3d matx = AcGeMatrix3d::mirroring(l3d);


		AcDbDimension * pCopy = NULL;

		pDim->getTransformedCopy(matx, (AcDbEntity*&)pCopy);


		pCopy->setNormal(oNormal);

		CDwgDataBaseUtil::PostToModelSpace(pCopy);


		acutPrintf(L"\nok");

	}


}
