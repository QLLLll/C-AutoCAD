#include "stdafx.h"
#include "MoveJig.h"

CMoveJig::~CMoveJig()
{
	if (NULL != m_pEnt)
	{
		delete m_pEnt;
		m_pEnt = NULL;
	}
}
AcEdJig::DragStatus CMoveJig::sampler()
{
	DragStatus stat;
	setUserInputControls((UserInputControls)
		(AcEdJig::kAccept3dCoordinates
			| AcEdJig::kNoNegativeResponseAccepted
			| AcEdJig::kNoZeroResponseAccepted
			| AcEdJig::kNullResponseAccepted));
	static AcGePoint3d pointTemp;
	stat = acquirePoint(m_ToPoint, m_FromPoint);
	if (pointTemp != m_ToPoint)
		pointTemp = m_ToPoint;
	else if (stat == AcEdJig::kNormal)
		stat = AcEdJig::kNoChange;
	return stat;
}
Adesk::Boolean CMoveJig::update()
{
	//m_Xform.setToTranslation(m_ToPoint - m_FromPoint);
	m_Xform.setToMirroring(AcGeLine3d(m_FromPoint,m_ToPoint));
	
	m_pEnt->setXform(m_Xform, m_FromPoint, m_ToPoint);
	return Adesk::kTrue;
}
AcDbEntity * CMoveJig::entity() const
{
	return m_pEnt;
}
void CMoveJig::doIt(const AcDbObjectIdArray &ids, bool bCopy)
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
	setDispPrompt(_T("/nÒÆ¶¯µ½: "));
	if (AcEdJig::kNormal == drag())
	{
		if (bCopy)
		{
			m_pEnt->transformedCopy();
		}
		else
		{
			m_pEnt->transform();
		}
	}
	m_idsC = m_pEnt->m_copyIds;
	delete m_pEnt;
	m_pEnt = NULL;
}

void CMoveJig::UpdateDoIt(const AcDbObjectIdArray & ids, bool bCopy)
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

	m_Xform.setToMirroring(AcGeLine3d(m_FromPoint, m_ToPoint));

	m_pEnt->setXform(m_Xform, m_FromPoint, m_ToPoint);

	m_pEnt->transformedCopy();

	m_idsC = m_pEnt->m_copyIds;
	delete m_pEnt;
	m_pEnt = NULL;
}
