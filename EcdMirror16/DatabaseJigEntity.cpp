#include "stdafx.h"
#include "DatabaseJigEntity.h"
#include <vector>
#include "acedCmdNF.h"
#define PI 3.1415926
using namespace std;
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
	AcDbEntity *pEnt = NULL;
	AcDbObject *pObj = NULL;
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
	AcDbEntity *pEnt = NULL;

	AcDbObject *pObj = NULL;
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
	AcDbEntity *pEnt = NULL;
	AcDbEntity *pNewEnt = NULL;
	AcDbObject *pObj = NULL;
	AcDbBlockTableRecord *pBlkRec;
	AcDbObjectId blkRecId;
	AcDbObjectIdArray txtIds;
	int types = 0;
	for (int i = 0; i < m_Ids.length(); i++)
	{
		if (Acad::eOk != pTrans->getObject(pObj, m_Ids[i], AcDb::kForWrite))
		{
			acTransactionManagerPtr()->abortTransaction();
			return FALSE;
		}
		pEnt = AcDbEntity::cast(pObj);
		AcDbDimension *dim = NULL, *dimC = NULL;
		AcDbText *txt = NULL;
		AcDbMText*mtxt = NULL;
		AcDbLine*line=NULL;
		AcGeVector3d vec = AcGeVector3d::kIdentity;

		dim = AcDbDimension::cast(pEnt);

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

		if (dim != NULL) {

			dim = AcDbDimension::cast(pEnt);
			vec = dim->normal();

		}
		else if (pEnt->isA() == AcDbText::desc()) {

			txtIds.append(pEnt->objectId());

			pEnt->close();
			continue;

		}
		else if (pEnt->isA() == AcDbMText::desc()) {

			txtIds.append(pEnt->objectId());

			pEnt->close();
			continue;

		}


		pEnt->getTransformedCopy(m_Xform, pNewEnt);

		if (dim != NULL) {

			dimC = AcDbDimension::cast(pNewEnt);
			dimC->setNormal(vec);

		}
		if(pNewEnt->isA()==AcDbLine::desc()){

			line=AcDbLine::cast(pNewEnt);

			ChangeLine(line,AcDbLine::cast(pEnt));


		}
		
		
		AcDbObjectId oId;


		if (dimC != NULL) {
			pBlkRec->appendAcDbEntity(oId, dimC);
			acTransactionManagerPtr()->addNewlyCreatedDBRObject(dimC);
		}
		else if(line!=NULL){
			pBlkRec->appendAcDbEntity(oId, line);
			acTransactionManagerPtr()->addNewlyCreatedDBRObject(line);
		}
		else {
			pBlkRec->appendAcDbEntity(oId, pNewEnt);
			acTransactionManagerPtr()->addNewlyCreatedDBRObject(pNewEnt);
		};

		m_copyIds.append(oId);
	}
	acTransactionManagerPtr()->endTransaction();

	for (int i = 0; i < txtIds.length(); i++)
	{
		AcDbObjectId getId;
		
			 MyMirror(txtIds[i],getId);
			 m_copyIds.append(getId);
		}



	return TRUE;

	}

	void  CDatabaseJigEntity::MyMirror(const AcDbObjectId & oId,AcDbObjectId &getId)
	{
		ads_name a,n1,l;

		acdbGetAdsName(a, oId);

		acedSSAdd(a, NULL, n1);

		acedCommandS(RTSTR, _T("MIRROR"),

			RTPICKS, n1,
			RTSTR, _T(""),
			RTPOINT, asDblArray(m_pt1),
			RTPOINT, asDblArray(m_pt2),
			RTSTR, _T("N"),
			RTSTR, _T(""),
			RTNONE);

		acedSSFree(n1);
		
		acdbEntLast(l);

		acdbGetObjectId(getId, l);

	}

	void CDatabaseJigEntity::ChangeLine(AcDbLine* newLine, AcDbLine*oLine)
	{

		AcGeVector3d vec1=oLine->startPoint()-oLine->endPoint();
	
		AcGePoint3d pt1=newLine->startPoint();
		AcGePoint3d pt2=newLine->endPoint();

		AcGePoint3d cPt=AcGePoint3d((pt1.x+pt2.x)/2,(pt1.y+pt2.y)/2,0);

		AcGeVector3d vec2=pt1-pt2;

		double angle=vec2.angleTo(vec1);
		angle=angle/PI*180;

		/*CString text;

		text.Format(L"angle=%f",angle);
		AfxMessageBox(text);*/
		if(angle>90){

		AcGeVector3d vecT=vec2.rotateBy(PI/2,AcGeVector3d::kZAxis);

		AcGePoint3d pt4=cPt+vecT.normal()*200;

		AcGeMatrix3d mtx;

		mtx.setToMirroring(AcGeLine3d(pt4,cPt));

		newLine->transformBy(mtx);



		}
		else{


			AcGeMatrix3d mtx;

			mtx.setToMirroring(AcGeLine3d(pt1,pt2));

			newLine->transformBy(mtx);

		}

	}

