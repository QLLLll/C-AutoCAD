#include "StdAfx.h"
#include "resource.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdCzFZXApp : public AcRxArxApp {

public:
	CEcdCzFZXApp () : AcRxArxApp () {}

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
	
	static void ECDMyGroupEcdCzFzx () {
		

		ads_name sName;
		ads_point sPt;

		AcDbObjectId entId;

		while (acedEntSel(TEXT("请选择直线或者多段线：\n"),sName, sPt) == RTNORM) {

			acdbGetObjectId(entId, sName);

			AcDbEntity *ent = NULL;

			if (acdbOpenObject(ent, entId, AcDb::kForRead) == ErrorStatus::eOk) {

				if (ent->isA() == AcDbLine::desc()) {

					AcDbLine* l = AcDbLine::cast(ent);

					AcGePoint3d pt1 = l->startPoint();
					AcGePoint3d pt2 = l->endPoint();

					
					getFuZhuX(pt1, pt2);

				}

				else if (ent->isA() == AcDbPolyline::desc()) {

					AcDbPolyline *pl = AcDbPolyline::cast(ent);

					AcGePoint3d ptClosed;

					while (zuiJinDian(pl, ptClosed)) {
	
						AcGePoint2dArray ptArr;

						getPtArrs(ptArr, pl);

						AcGePoint3d ptStart, ptEnd;

						getDuanDian(pl, ptArr, ptClosed, ptStart, ptEnd);


						getFuZhuX(ptStart, ptEnd);

						/*AcDbLine *l1 = new AcDbLine(ptStart, ptEnd);

						l1->setColorIndex(2);

						PostToModelSpace(l1);

						l1->close();*/

					}
				}

			}


		}


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
	
	static bool zuiJinDian(AcDbPolyline *pl,AcGePoint3d & ptNear) {

		ads_point pt;

		if (acedGetPoint(NULL, TEXT("请点选polyline的一条边：\n"), pt) == RTNORM) {

			AcGePoint3d temp = asPnt3d(pt);

			pl->getClosestPointTo(temp, ptNear);
			return true;

		}
		return false;



	}
	static void getPtArrs(AcGePoint2dArray&arr, AcDbPolyline *pl) {

		for (int i = 0; i < (int)pl->numVerts(); i++)
		{
			AcGePoint2d pt;

			pl->getPointAt(i, pt);
			arr.append(pt);
		}

	}

	static void  getDuanDian(AcDbPolyline *pl, AcGePoint2dArray &ptArr,const AcGePoint3d & ptClosed, AcGePoint3d &ptStart, AcGePoint3d &ptEnd) {
		double dis = 0.0;
		pl->getDistAtPoint(ptClosed, dis);

		double temp = 0.0;
		int j = 0;
		while (temp < dis) {

			pl->getDistAtPoint(AcGePoint3d(ptArr[j].x, ptArr[j].y, 0), temp);

			j++;


		}

		ptEnd = AcGePoint3d(ptArr[j - 1].x, ptArr[j - 1].y, 0);
		ptStart= AcGePoint3d(ptArr[j - 2].x, ptArr[j - 2].y, 0);


	}

	static void getFuZhuX(const AcGePoint3d &pt1, const AcGePoint3d &pt2) {


		AcGeVector3d vecTz = (pt2 - pt1).rotateBy(3.1415926 / 2, AcGeVector3d::kZAxis).normal();
		AcGeVector3d vec = (pt2 - pt1).normal();
		double dis = pt2.distanceTo(pt1);

		double tzLen = dis / 2*10;

		double eachDis = dis / 3;

		for (int i = 0; i < 4; i++)
		{
			AcGePoint3d temp = pt1 + i*eachDis*vec;

			AcGePoint3d ptEnd = temp + vecTz*tzLen;
			AcGePoint3d ptStart = temp + vecTz*-tzLen;

			AcDbLine * line = new AcDbLine(ptStart, ptEnd);

			line->setColorIndex(1);

			PostToModelSpace(line);

			line->close();


		}

	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdCzFZXApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdCzFZXApp, ECDMyGroup, EcdCzFzx, EcdCzFzx, ACRX_CMD_MODAL, NULL)


