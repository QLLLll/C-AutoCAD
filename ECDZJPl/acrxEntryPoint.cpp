// (C) Copyright 2002-2012 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include<vector>
using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CECDZJPlApp : public AcRxArxApp {

public:
	CECDZJPlApp () : AcRxArxApp () {}

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
	
	
	static void ECDMyGroupECDZjPl () {
		// Put your command code here
		ECDMyPL();
	}

	static void ECDMyPL() {

		int index = 2;
		AcGePoint3d ptStart;
		ads_point pt1;
		if (/*!CGetInputUtil::GetPoint(L"\n请输入第一个点：", ptStart)*/acedGetPoint(NULL, L"\n请输入第一个点：", pt1) != RTNORM) {
			return;
		}
		ptStart = asPnt3d(pt1);
		AcGePoint3d ptPrevious, ptCurrent;
		AcGePoint3d ptZj;
		ads_point ptCur;
		ptPrevious = ptStart;
		double *ptPre = asDblArray(ptPrevious);

		AcDbObjectId polyId;
		
		int rc;
		TCHAR keyword[20];
		
		AcDbPolyline * pl = NULL;
		int plIndex = 0;

		vector<AcDbPolyline*>vecPl;
		vector<AcGeVector3d>vecVt;
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

					

					AcDbPolyline * temp = AcDbPolyline::cast(vecPl[plIndex-1]->clone());

					AcGePoint2d ptTemp;

					temp->getPointAt(temp->numVerts() - 1, ptTemp);

					ptPrevious = AcGePoint3d(ptTemp.x, ptTemp.y,0);
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
					for (size_t i=0;i<vecPl.size();i++)
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
					 pl= new AcDbPolyline();

					//pl->assertWriteEnabled(false, true);

					pl->addVertexAt(pl->numVerts(), AcGePoint2d(ptPrevious.x, ptPrevious.y));
					pl->addVertexAt(pl->numVerts(), AcGePoint2d(ptCurrent.x, ptCurrent.y));
					AcDbPolyline * plCo = AcDbPolyline::cast(pl->clone());
					polyId = PostToModelSpace(pl);


					vecPl.push_back(plCo);
					plIndex++;

					pl->close();
					pl = NULL;

					AcGeVector3d vec1 = (ptCurrent - ptPrevious).normal();

					AcGeVector3d vec2 = vec1.rotateBy(3.1415926 / 2, AcGeVector3d::kZAxis);
					AcGeVector3d vec3 = -vec2;

					vecVt.push_back(vec1);
					vecVt.push_back(vec2);
					vecVt.push_back(vec3);


				}
				else if (index > 2) {

					

					GetZjPt(vecVt, ptPrevious, ptCurrent, ptZj);


					if (acdbOpenObject(pl, polyId, AcDb::OpenMode::kForWrite) != ErrorStatus::eOk)
						return;
					pl->addVertexAt(index - 1, AcGePoint2d(ptZj.x, ptZj.y), 0, 0, 0);
					
					AcDbPolyline * plCo = AcDbPolyline::cast(pl->clone());

					vecPl.push_back(plCo);
					plIndex++;
					pl->close();
					vecVt.clear();

					AcGeVector3d vec1 = (ptZj - ptPrevious).normal();

					AcGeVector3d vec2 = vec1.rotateBy(3.1415926 / 2, AcGeVector3d::kZAxis);
					AcGeVector3d vec3 = -vec2;

					vecVt.push_back(vec1);
					vecVt.push_back(vec2);
					vecVt.push_back(vec3);


				}
				index++;

				if(index>3)
				ptPrevious = ptZj;
				else {
					ptPrevious = ptCurrent;
				}
				ptPre = asDblArray(ptPrevious);
				break;

			}
			acedInitGet(NULL, TEXT("U Y O"));
		
			rc = acedGetPoint(ptPre, L"\n请输入下一个点或[回退U/返回退Y/完成O]:", ptCur);
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
static 	int GetPointReturnCode(const AcGePoint3d & basePoint, const TCHAR * prompt, AcGePoint3d & point)
	{
		AcGePoint3d ucsBasePoint = basePoint;

		int nReturn = acedGetPoint(asDblArray(ucsBasePoint), prompt, asDblArray(point));

		if (nReturn == RTNORM) {

			//point = CConvertUtil::UcsToWcsPoint(point);
		}

		return nReturn;


	}
static void GetZjPt(vector<AcGeVector3d>&vecVt,const AcGePoint3d ptPre, const AcGePoint3d ptCurrent, AcGePoint3d & pt) {

	AcGeVector3d vec = ptCurrent - ptPre;


	double angle = 2*3.1415926;

	AcGeVector3d vecMin;

	for (size_t i=0;i<vecVt.size();i++)
	{
		double an=vec.angleTo(vecVt[i]);

		if (an < angle) {
			angle = an;
			vecMin = vecVt[i];

		}
	}

	pt =ptPre+ vecMin*cos(angle)*vec.length();
}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CECDZJPlApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CECDZJPlApp, ECDMyGroup, ECDZjPl, ECDZjPl, ACRX_CMD_MODAL, NULL)


