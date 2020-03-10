#include "stdafx.h"
#include"LineJig.h"

#include<vector>
using namespace std;
LineJig::~LineJig()
{
	if (m_pEnt != NULL) {
		delete m_pEnt;

		m_pEnt = NULL;
	}
}

AcEdJig::DragStatus LineJig::sampler()
{
	
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
			| AcEdJig::kNoNegativeResponseAccepted
			| AcEdJig::kNoZeroResponseAccepted
			| AcEdJig::kNullResponseAccepted));
	AcGePoint3d ptTemp;

	AcEdJig::DragStatus status = acquirePoint(ptTemp);

	if (ptTemp != ptCurrent) {

		ptCurrent = ptTemp;

	}
	else if (status == DragStatus::kNormal) {

		return AcEdJig::kNoChange;
	}
	return status;
}
AcDbEntity * LineJig::entity() const
{
	return m_pEnt;
}

bool LineJig::update()
{
	return m_pEnt->move(ptCurrent);
	
}

void LineJig::DoIt(vector<AcDbLine*> &vecLine)
{
	if (0 == vecLine.size())
	{
		return;
	}
	lines = vecLine;

	
	setDispPrompt(_T("/n移动到: "));
	if (AcEdJig::kNormal == drag())
	{
		for (int i=0;i<(int)vecLine.size();i++)
		{

			PostToModelSpace(vecLine[i]);
		}
		
	}
	//else {

	//	for (int i = 0; i < (int)vecLine.size(); i++)
	//	{

	//		delete vecLine[i];
	//		vecLine[i] = NULL;

	//		delete m_Ent;
	//		m_Ent = NULL;
	//	}
	//}


}
void LineJig::DoIt(AcDbObjectIdArray & ids)
{

	if (0 == ids.length())
	{
		return;
	}
	if (NULL != m_pEnt)
	{
		delete m_pEnt;
		m_pEnt = NULL;
	}

	m_pEnt = new CDatabaseJigEntity(ids);
	setDispPrompt(_T("/n移动到: "));
	if (AcEdJig::kNormal == drag())
	{

		m_pEnt->transform();
	}
	
	delete m_pEnt;
	m_pEnt = NULL;

}
 AcDbObjectId  LineJig:: PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb) {

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