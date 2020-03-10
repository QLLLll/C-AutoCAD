#include "stdafx.h"
#include "DatabaseJigEntity.h"
CDatabaseJigEntity::CDatabaseJigEntity()
{
}


CDatabaseJigEntity::~CDatabaseJigEntity()
{
}

Adesk::Boolean CDatabaseJigEntity::subWorldDraw(AcGiWorldDraw * mode)
{
	//这个地方是关键！
	mode->geometry().pushModelTransform(m_Xform);
	AcDbEntity* pEnt;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		//绘制实体无需写打开
		if (Acad::eOk == acdbOpenObject(pEnt, m_Ids[i], AcDb::kForRead))
		{
			mode->geometry().draw(pEnt);
			pEnt->close();
		}
	}



	//这句不能少，恢复现场
	mode->geometry().popModelTransform();
	return (AcDbEntity::subWorldDraw(mode));
}
//用于拖动结束后，将实体变换到新位置
BOOL CDatabaseJigEntity::transform()
{
	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
	if (NULL == pTrans)
		return FALSE;
	AcDbEntity *pEnt=NULL;
	AcDbObject *pObj=NULL;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForWrite))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbEntity::cast(pObj);
		pEnt->transformBy(m_Xform);
	}
	acTransactionManagerPtr()->endTransaction();
	return TRUE;
}
bool CDatabaseJigEntity::move() {

	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
	if (NULL == pTrans)
		return FALSE;
	AcDbEntity *pEnt=NULL;

	AcDbObject *pObj=NULL;	
	for (int i = 0; i < m_Ids.length(); i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForWrite))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbEntity::cast(pObj);
		
		pEnt->transformBy(m_Xform);
		pEnt->close();
	}
	return true;
}

//用于拖动结束后，将实体复制到新位置
BOOL CDatabaseJigEntity::transformedCopy()
{
	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
	if (NULL == pTrans)
		return FALSE;
	AcDbEntity *pEnt=NULL;
	AcDbEntity *pNewEnt=NULL;
	AcDbObject *pObj=NULL;
	AcDbBlockTableRecord *pBlkRec;
	AcDbObjectId blkRecId;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForRead))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbEntity::cast(pObj);
		if (0 == i)
		{
			blkRecId = pEnt->blockId();
			if (Acad::eOk != pTrans->getObject(pObj, blkRecId, AcDb::kForWrite))
			{
				acTransactionManagerPtr()->abortTransaction();
				return FALSE;
			}
			pBlkRec = AcDbBlockTableRecord::cast(pObj);
		}
		pEnt->getTransformedCopy(m_Xform, pNewEnt);
		pBlkRec->appendAcDbEntity(pNewEnt);
		acTransactionManagerPtr()->addNewlyCreatedDBRObject(pNewEnt);
	}
	acTransactionManagerPtr()->endTransaction();
	return TRUE;
}
