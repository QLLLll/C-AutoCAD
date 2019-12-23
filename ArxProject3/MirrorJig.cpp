#include "stdafx.h"
#include "MirrorJig.h"
#include"DwgDataBaseUtil.h"


MirrorJig::MirrorJig()
{
	m_pEnt = NULL;
}


MirrorJig::~MirrorJig()
{
}

AcEdJig::DragStatus MirrorJig::sampler()
{
	setUserInputControls((AcEdJig::UserInputControls)(AcEdJig::kAccept3dCoordinates |
		AcEdJig::kNoNegativeResponseAccepted |
		AcEdJig::kNullResponseAccepted));

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

bool MirrorJig::update()
{
	

	AcGeLine3d *line = new AcGeLine3d(ptBase, ptCurrent);
	mirrorLine->setEndPoint(ptCurrent);

	m_Xform.setToMirroring(*line);
	m_pEnt->setXform(m_Xform);
	delete line;
	return Adesk::kTrue;

}

AcDbEntity * MirrorJig::entity() const
{
	
	return m_pEnt;
}

void MirrorJig::DoIt(AcGePoint3d& ptBase, AcDbObjectIdArray & idArr, bool bCopy)
{
	this->ptBase = ptBase;

	mirrorLine = new AcDbLine();
	mirrorLine->setStartPoint(ptBase);

	if (idArr.length() <= 0) {
		return;
	}
	
	if (NULL != m_pEnt)
	{
		delete m_pEnt;
		m_pEnt = NULL;
	}

	m_pEnt = new CDatabaseJigEntity(idArr);
	
	setDispPrompt(L"\nÇëÒÆ¶¯Êó±ê\n");

	if (drag() == AcEdJig::DragStatus::kNormal) {
		if (bCopy)
		{
			m_pEnt->transformedCopy();
		}
		else
		{
			m_pEnt->transform();
		}
		
	}
	delete m_pEnt;
	m_pEnt = NULL;
	
}

