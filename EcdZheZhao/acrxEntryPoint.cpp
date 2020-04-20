#include "StdAfx.h"
#include "resource.h"
#include"dbwipe.h"
#include<vector>
using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdZheZhaoApp : public AcRxArxApp {

public:
	CEcdZheZhaoApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CEcdZheZhaoApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.

	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CEcdZheZhaoApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ECDMyGroupee() {

		
	}

	static void ECDMyGroupTest () {
		CString dicName = L"frame";
		struct resbuf rb;
		int i = acedGetVar(dicName, &rb);
		int m = 2;
		if (i == RTNORM) {
			int m = rb.resval.rint;
			rb.restype = RTSHORT;
			rb.resval.rint = (short)2;


			int r0 = acedSetVar(dicName, &rb);
			if (r0 != RTNORM) {
				acutPrintf(L"错误：%d\n", i);
			}
		}
		else {
			acutPrintf(L"错误：%d\n", i);
		}
		AcDbObjectId polyId;
		 ECDMyPL(polyId);

		if (polyId.isNull()) {
			AfxMessageBox(L"创建多段线出错。");
			return;
		}

		AcDbPolyline *pl = NULL;
		AcGePoint2dArray ptArr;

		AcDbPolyline *pl2 = NULL;
		if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

			pl->setClosed(true);
			pl->setColorIndex(1);
			double x = 0;
			double y = 0;
			for (int i = 0; i < (int)pl->numVerts(); i++)
			{
				AcGePoint2d pt;

				pl->getPointAt(i, pt);

				x += pt.x;
				y += pt.y;

			}
			x /= pl->numVerts();
			y /= pl->numVerts();


			/*AcGeMatrix3d mtx;
			mtx.setToScaling(1.09, AcGePoint3d(x,y,0));

		ErrorStatus es=	pl->transformBy(mtx);
		acutPrintf(L"mtx=%d", es);*/
			AcDbVoidPtrArray offsetCurves;
			double wide = 10;//例如
			pl->getOffsetCurves(wide, offsetCurves);
			pl2 = (AcDbPolyline*)offsetCurves[0];
			pl->erase();
			pl->close();
			pl = NULL;
			pl2->setClosed(true);
			polyId = PostToModelSpace(pl2);
			pl2->close();
			pl2 = NULL;
		}
		if (acdbOpenObject(pl2, polyId, AcDb::OpenMode::kForWrite) == ErrorStatus::eOk) {

			AcGePoint2d ptStart;
			for (int i = 0; i < (int)pl2->numVerts(); i++)
			{

				AcGePoint2d pt;

				pl2->getPointAt(i, pt);

				ptArr.append(pt);

			}
			pl2->getPointAt(0, ptStart);
			ptArr.append(ptStart);
			pl2->erase();
			pl2->close();
			pl2 = NULL;
		}

		AcDbWipeout *pWipeout = new AcDbWipeout();

		
			ErrorStatus es;// = wipeOut->fitPointsToImage(ptArr, minPt, scale);
			pWipeout->setDatabaseDefaults();
			AcGePoint3d originPnt(AcGePoint3d::kOrigin);
			AcGeVector3d Udirection(1, 0, 0);
			AcGeVector3d Vdirection(0, -1, 0);
			pWipeout->setOrientation(originPnt, Udirection, Vdirection);
			pWipeout->setDisplayOpt(AcDbRasterImage::kTransparent, Adesk::kTrue);
			pWipeout->setDisplayOpt(AcDbRasterImage::kShow, Adesk::kTrue);
			es = pWipeout->setClipBoundaryToWholeImage();
			es = pWipeout->setClipBoundary(AcDbRasterImage::kPoly, ptArr);

		if (es == ErrorStatus::eOk) {

			PostToModelSpace(pWipeout);
			pWipeout->close();
		}
		
		 i= acedGetVar(dicName, &rb);
		
		if (i == RTNORM) {
			int m = rb.resval.rint;
			rb.restype = RTSHORT;
			rb.resval.rint = (short)m;


			int r0 = acedSetVar(dicName, &rb);
			if (r0 != RTNORM) {
				acutPrintf(L"错误：%d\n", i);
			}
		}
		else {
			acutPrintf(L"错误：%d\n", i);
		}

	}
	static  void ECDMyPL(AcDbObjectId & polyId ) {

		int index = 2;
		AcGePoint3d ptStart;
		ads_point pt1;
		if (acedGetPoint(NULL, L"\n请输入第一个点：", pt1) != RTNORM) {
			return ;
		}
		ptStart = asPnt3d(pt1);
		AcGePoint3d ptPrevious, ptCurrent;
		AcGePoint3d ptZj;
		ads_point ptCur;
		ptPrevious = ptStart;
		double *ptPre = asDblArray(ptPrevious);

		

		int rc;
		TCHAR keyword[20];

		AcDbPolyline * pl = NULL;
		int plIndex = 0;

		vector<AcDbPolyline*>vecPl;
		//vector<AcGeVector3d>vecVt;
		acedInitGet(NULL, TEXT("U Y O"));
		rc = acedGetPoint(ptPre, L"\n请输入下一个点或[回退U/返回退Y/完成O]:", ptCur);

		while (rc == RTKWORD || rc == RTNORM) {
			switch (rc) {
			case RTKWORD:
				if (acedGetInput(keyword) != RTNORM)
					return;

				if (_tcscmp(keyword, TEXT("U")) == 0) {

					plIndex--;

					if (plIndex <= 0) {
						plIndex = 0;
						acedInitGet(NULL, TEXT("U Y O"));

						rc = acedGetPoint(ptPre, L"\n请输入下一个点或[回退U/返回退Y/完成O]:", ptCur);
						continue;
					}

					if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
					{
						acedInitGet(NULL, TEXT("U Y O"));

						rc = acedGetPoint(ptPre, L"\n请输入下一个点或[回退U/返回退Y/完成O]:", ptCur);
						continue;
					}

					pl->erase();
					pl->close();



					AcDbPolyline * temp = AcDbPolyline::cast(vecPl[plIndex - 1]->clone());

					AcGePoint2d ptTemp;

					temp->getPointAt(temp->numVerts() - 1, ptTemp);

					ptPrevious = AcGePoint3d(ptTemp.x, ptTemp.y, 0);
					ptPre = asDblArray(ptPrevious);
					polyId = PostToModelSpace(temp);
					temp->close();


				}
				else if (_tcscmp(keyword, TEXT("Y")) == 0) {
					plIndex++;
					if (plIndex >= vecPl.size()) {
						plIndex = (int)vecPl.size() - 1;
						acedInitGet(NULL, TEXT("U Y O"));

						rc = acedGetPoint(ptPre, L"\n请输入下一个点或[回退U/返回退Y/完成O]:", ptCur);
						continue;
					}

					if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
					{
						acedInitGet(NULL, TEXT("U Y O"));

						rc = acedGetPoint(ptPre, L"\n请输入下一个点或[回退U/返回退Y/完成O]:", ptCur);
						continue;
					}

					pl->erase();
					pl->close();

					AcDbPolyline * temp = AcDbPolyline::cast(vecPl[plIndex]->clone());

					AcGePoint2d ptTemp;

					temp->getPointAt(temp->numVerts() - 1, ptTemp);

					ptPrevious = AcGePoint3d(ptTemp.x, ptTemp.y, 0);
					ptPre = asDblArray(ptPrevious);
					polyId = PostToModelSpace(temp);
					temp->close();

				}
				else if (_tcscmp(keyword, TEXT("O")) == 0) {
					for (size_t i = 0; i < vecPl.size(); i++)
					{
						delete vecPl[i];
						vecPl[i] = NULL;

					}
					return;

				}

				break;
			case RTNORM:
				ptCurrent = asPnt3d(ptCur);

				if (index == 2) {
					pl = new AcDbPolyline();

					//pl->assertWriteEnabled(false, true);

					pl->addVertexAt(pl->numVerts(), AcGePoint2d(ptPrevious.x, ptPrevious.y));
					pl->addVertexAt(pl->numVerts(), AcGePoint2d(ptCurrent.x, ptCurrent.y));
					AcDbPolyline * plCo = AcDbPolyline::cast(pl->clone());
					polyId = PostToModelSpace(pl);

					vecPl.push_back(plCo);
					plIndex++;

					pl->close();
					pl = NULL;

					/*AcGeVector3d vec1 = (ptCurrent - ptPrevious).normal();

					AcGeVector3d vec2 = vec1.rotateBy(3.1415926 / 2, AcGeVector3d::kZAxis);
					AcGeVector3d vec3 = -vec2;

					vecVt.push_back(vec1);
					vecVt.push_back(vec2);
					vecVt.push_back(vec3);*/


				}
				else if (index > 2) {

					//GetZjPt(vecVt, ptPrevious, ptCurrent, ptZj);

					if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
						return;
					pl->addVertexAt(index - 1, AcGePoint2d(ptCurrent.x, ptCurrent.y), 0, 0, 0);

					AcDbPolyline * plCo = AcDbPolyline::cast(pl->clone());

					vecPl.push_back(plCo);
					plIndex++;
					pl->close();
					//vecVt.clear();

					/*	AcGeVector3d vec1 = (ptZj - ptPrevious).normal();

						AcGeVector3d vec2 = vec1.rotateBy(3.1415926 / 2, AcGeVector3d::kZAxis);
						AcGeVector3d vec3 = -vec2;

						vecVt.push_back(vec1);
						vecVt.push_back(vec2);
						vecVt.push_back(vec3);*/


				}
				index++;
				ptPrevious = ptCurrent;

				/*if (index > 3)
					ptPrevious = ptCurrent;
				else {
					ptPrevious = ptCurrent;
				}*/
				ptPre = asDblArray(ptPrevious);
				break;

			}
			acedInitGet(NULL, TEXT("U Y O"));

			rc = acedGetPoint(ptPre, L"\n请输入下一个点或[回退U/返回退Y/完成O]:", ptCur);
		}
		
	}
	static 	int GetPointReturnCode(const AcGePoint3d & basePoint, const TCHAR * prompt, AcGePoint3d & point)
	{
		AcGePoint3d ucsBasePoint = basePoint;

		int nReturn = acedGetPoint(asDblArray(ucsBasePoint), prompt, asDblArray(point));

		if (nReturn == RTNORM) {

			//point = CConvertUtil::UcsToWcsPoint(point);
		}

		return nReturn;


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
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdZheZhaoApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdZheZhaoApp, ECDMyGroup, Test, Test, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CEcdZheZhaoApp, ECDMyGroup, ee, ee, ACRX_CMD_MODAL, NULL)

