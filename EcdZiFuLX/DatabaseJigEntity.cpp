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
	

	AcDbLine* pEnt;
	for (int i = 0; i < m_Ids.length()-1; i++)
	{
		//绘制实体无需写打开
		if (Acad::eOk == acdbOpenObject(pEnt, m_Ids[i], AcDb::kForWrite))
		{
			pEnt->setEndPoint(pt);

			mode->geometry().draw(pEnt);
			pEnt->close();
		}
	}
	AcDbText* tt = NULL;
	if (Acad::eOk == acdbOpenObject(tt, m_Ids[m_Ids.length()-1], AcDb::kForWrite))
	{
		tt->setPosition(pt);

		mode->geometry().draw(tt);
		tt->close();
	}

	return (AcDbEntity::subWorldDraw(mode));
}
//用于拖动结束后，将实体变换到新位置
BOOL CDatabaseJigEntity::transform()
{
	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
	if (NULL == pTrans)
		return FALSE;
	AcDbLine *pEnt = NULL;
	AcDbObject *pObj = NULL;
	for (int i = 0; i < m_Ids.length()-1; i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForWrite))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbLine::cast(pObj);
		pEnt->setEndPoint(pt);
	}
	AcDbText * txt = NULL;

	if (Acad::eOk != pTrans->getObject(pObj, m_Ids[m_Ids.length() - 1], AcDb::kForWrite))
	{
		acTransactionManagerPtr()->abortTransaction();
		return FALSE;
	}
	txt = AcDbText::cast(pObj);
	txt->setPosition(pt);

	acTransactionManagerPtr()->endTransaction();
	return TRUE;
}
bool CDatabaseJigEntity::move(const AcGePoint3d & m_endPt) {

	pt = m_endPt;

	return true;
}

//用于拖动结束后，将实体复制到新位置
//BOOL CDatabaseJigEntity::transformedCopy()
//{
//	AcTransaction * pTrans = acTransactionManagerPtr()->startTransaction();
//	if (NULL == pTrans)
//		return FALSE;
//	AcDbEntity *pEnt=NULL;
//	AcDbEntity *pNewEnt=NULL;
//	AcDbObject *pObj=NULL;
//	AcDbBlockTableRecord *pBlkRec;
//	AcDbObjectId blkRecId;
//	for (int i = 0; i < m_Ids.length(); i++)
//	{
//		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForRead))
//		{
//			acTransactionManagerPtr()->abortTransaction();
//			return FALSE;
//		}
//		pEnt = AcDbEntity::cast(pObj);
//		if (0 == i)
//		{
//			blkRecId = pEnt->blockId();
//			if (Acad::eOk != pTrans->getObject(pObj, blkRecId, AcDb::kForWrite))
//			{
//				acTransactionManagerPtr()->abortTransaction();
//				return FALSE;
//			}
//			pBlkRec = AcDbBlockTableRecord::cast(pObj);
//		}
//		pEnt->getTransformedCopy(m_Xform, pNewEnt);
//		pBlkRec->appendAcDbEntity(pNewEnt);
//		acTransactionManagerPtr()->addNewlyCreatedDBRObject(pNewEnt);
//	}
//	acTransactionManagerPtr()->endTransaction();
//	return TRUE;
//}
