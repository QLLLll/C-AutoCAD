#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdJiaoDFApp : public AcRxArxApp {

public:
	CEcdJiaoDFApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	
	static void ECDMyGroupEcdJiaoDF () {
	
		AcDbLine *l1 = NULL, *l2 = nullptr;

		ErrorStatus es;

		AcDbObjectId line1Id = getObjectId(TEXT("请选择第一条线:\n"));

		if (line1Id == AcDbObjectId::kNull) {
			return;
		}

		es = acdbOpenObject(l1, line1Id, AcDb::kForWrite);

		if (es != ErrorStatus::eOk) {
			acutPrintf(L"打开直线1失败：%d", es);
			return;
		}

		AcDbObjectId line2Id = getObjectId(TEXT("请选择第二条线:\n"));

		if (line2Id == AcDbObjectId::kNull) {
			l1->close();
			return;
		}

		es = acdbOpenObject(l2, line2Id, AcDb::kForWrite);

		if (es != ErrorStatus::eOk) {
			acutPrintf(L"打开直线2失败：%d", es);
			l1->close();
			return;
		}

		int times = 0;

		if (acedGetInt(L"请选择角度平分的次数：\n", &times) != RTNORM) {

			l1->close();
			l2->close();
			return;

		}

		lPtSort(l1);
		lPtSort(l2);

		AcDbLine *temp = NULL;

		//l1在上，l2在下

		if (l1->startPoint().y < l2->startPoint().y || l1->endPoint().y < l2->endPoint().y) {

			temp = l1;
			l1 = l2;
			l2 = temp;

		}


		AcGePoint3d pt1Start, pt1End, pt2Start, pt2End;

		pt1Start = l1->startPoint();
		pt1End = l1->endPoint();

		pt2Start = l2->startPoint();
		pt2End = l2->endPoint();

		AcGeVector3d vec1 = pt1End - pt1Start;
		AcGeVector3d vec2 = pt2End - pt2Start;


		double angle = vec1.angleTo(vec2);

		double jiao = angle / 3.1415926 * 180;

		if (fabs(jiao-180)<1||fabs(jiao)<1) {

			//认为这两条线是平行的；

			double dis = pt1Start.distanceTo(pt2Start);

			double eachDis = dis / (times + 1);

			AcGeVector3d vec3 = (pt1Start - pt2Start).normal();

			

			for (int i=0;i<times;i++)
			{

				AcDbEntity *pEnt = NULL;

				AcGeMatrix3d mtx;
				mtx.setTranslation(vec3*eachDis*(i+1));

				l2->getTransformedCopy(mtx, pEnt);

				PostToModelSpace(pEnt);

				pEnt->close();

			}


		}
		else {

			double eachAngle = angle / (times+1);

			AcGePoint3dArray ptArr;

			l1->intersectWith(l2, AcDb::kExtendBoth, ptArr);

			if (ptArr.length() != 1) {

				acutPrintf(L"两条直线的交点个数不为一：%d\n", ptArr.length());

				l1->close();
				l2->close();
				return;

			}

			AcDbLine* jiXian = new AcDbLine(ptArr[0], pt2End);

			AcDbLine * startL = new AcDbLine(pt2Start, pt1Start);

			AcDbLine *endL = new AcDbLine(pt2End, pt1End);

			AcGePoint3dArray ptTemp;

			for (int i=0;i<times;i++)
			{
				jiXian->transformBy(AcGeMatrix3d::rotation(eachAngle, AcGeVector3d::kZAxis, ptArr[0]));

				jiXian->intersectWith(startL, AcDb::kExtendThis, ptTemp);

				if (ptTemp.length() != 1) {

					jiXian->transformBy(AcGeMatrix3d::rotation(-2*eachAngle, AcGeVector3d::kZAxis, ptArr[0]));
					ptTemp.setLogicalLength(0);
					jiXian->intersectWith(startL, AcDb::kExtendThis, ptTemp);
					eachAngle *= -1;

				}


				AcGePoint3d pt1 = ptTemp[0];

				ptTemp.setLogicalLength(0);


				jiXian->intersectWith(endL, AcDb::kExtendThis, ptTemp);

				AcGePoint3d pt2 = ptTemp[0];

				ptTemp.removeAll();

				AcDbLine * addL = new AcDbLine(pt1, pt2);

				PostToModelSpace(addL);

				addL->close();

			}

			delete jiXian;
			jiXian = NULL;

			delete startL;
			startL = NULL;

			delete endL;
			endL = NULL;

		}

		l1->close();
		l2->close();


	}
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase()) {

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
	static AcDbObjectId getObjectId(ACHAR* prompt) {

		ads_name sName;
		ads_point pt;

		if (acedEntSel(prompt, sName, pt) != RTNORM) {
			return AcDbObjectId::kNull;
		}
		AcDbObjectId oId;

		acdbGetObjectId(oId, sName);

		return oId;


	}

	static void lPtSort(AcDbLine* l) {


		if (l->startPoint().x > l->endPoint().x) {

			AcGePoint3d tempPt = l->startPoint();

			l->setStartPoint(l->endPoint());

			l->setEndPoint(tempPt);

		}
		else if (l->startPoint().y > l->endPoint().y) {

			AcGePoint3d tempPt = l->startPoint();

			l->setStartPoint(l->endPoint());

			l->setEndPoint(tempPt);

		}

	}
	
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdJiaoDFApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdJiaoDFApp, ECDMyGroup, EcdJiaoDF, EcdJiaoDF, ACRX_CMD_MODAL, NULL)


