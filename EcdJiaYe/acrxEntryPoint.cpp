#include "StdAfx.h"
#include "resource.h"
#include<vector>
using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdJiaYeApp : public AcRxArxApp {

public:

	struct MyStut {
		AcGeVector3d vec1;
		AcGeVector3d vec2;
		AcGePoint3d pt;

	};

	CEcdJiaYeApp () : AcRxArxApp () {}

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
	
	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CEcdJiaYeApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CEcdJiaYeApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ECDMyGroupMyCommand () {
		
		ads_point pt;
		int dis = 0;

		if (acedGetPoint(NULL, L"«Î ‰»Îµ„£∫\n", pt) != RTNORM) {
			return;
		}
		if (acedGetInt(L"«Î ‰»Î∆´“∆æ‡¿Î£∫\n", &dis) != RTNORM) {
			return;
		}

		AcGePoint3d seedPoint = asPnt3d(pt);
		AcDbVoidPtrArray  ptrArr;
		acedTraceBoundary(seedPoint, false, ptrArr);

		if (ptrArr.length() != 1) {

			acutPrintf(L"±ﬂΩÁ√ª”–∑‚±’");
			return;

		}
		AcDbPolyline * pl = (AcDbPolyline*)ptrArr[0];

		AcGePoint3dArray ptArr;

		for (int i = 0; i <(int)pl->numVerts();i++)
		{
			
			AcGePoint2d pt2d;
			pl->getPointAt(i, pt2d);
			ptArr.append(AcGePoint3d(pt2d.x, pt2d.y, 0));

		}
		int total = ptArr.length();

		vector<MyStut>vecStt;

		for (int i = 0; i < total;i++)
		{
			AcGePoint3d ptC = ptArr[i];
			AcGePoint3d ptPre, ptAfter;
			ptPre = ptArr[(i + total - 1) % total];
			ptAfter = ptArr[(i + 1) % total];

			if (fabs(ptPre.x - ptC.x) <= 0.01&&fabs(ptPre.y - ptC.y) <= 0.01) {

				ptPre= ptArr[(i + total - 2) % total];

			}
			if (fabs(ptAfter.x - ptC.x) <= 0.01&&fabs(ptAfter.y - ptC.y) <= 0.01) {

				ptAfter = ptArr[(i + 2) % total];

			}

			double dis1 = ptC.distanceTo(ptPre);
			double dis2 = ptC.distanceTo(ptAfter);

			if (fabs(dis1 - dis2) <= 1.5) {

				MyStut st;


				AcGeVector3d v1 = (ptPre - ptC)*10;
				AcGeVector3d v2 = (ptAfter - ptC)*10;
				AcGePoint3d p1, p2;

				CString str,str2;

				str.Format(L"\nv1=[%5f,%5f,0]", v1.x, v1.y);
				str2.Format(L"\nv2=[%5f,%5f,0]", v2.x, v2.y);

				acutPrintf(str);
				acutPrintf(str2);

				if (fabs(v1.y) > fabs(v1.x))
				{
					

						if (v1.y < 0) {
							st.vec1 = AcGeVector3d::kYAxis * 1;
						}
						else {
							st.vec1 = AcGeVector3d::kYAxis*-1;
						}
					
				}
				else {
					if (v1.x < 0) {
						st.vec1 = AcGeVector3d::kXAxis*1;
					}
					else {
						st.vec1 = AcGeVector3d::kXAxis*-1;
					}
				}

				if (fabs(v2.y) > fabs(v2.x))
				{
					if (v2.y <0) {
						st.vec2 = AcGeVector3d::kYAxis*1;
					}
					else {
						st.vec2 = AcGeVector3d::kYAxis*-1;
					}
				}
				else {
					if (v2.x < 0) {
						st.vec2 = AcGeVector3d::kXAxis*1;
					}
					else {
						st.vec2 = AcGeVector3d::kXAxis*-1;
					}
				}
				st.pt = ptC;

				vecStt.push_back(st);

			}



		}

		vector<AcDbEntity*>vecEnts;
		AcGePoint3dArray ptArr2;

		for (int i = 0; i < (int)vecStt.size();i++)
		{
			MyStut m = vecStt[i];

			AcGeVector3d v = (m.vec1 + m.vec2);

			AcGePoint3d ptt = m.pt+v*dis;

			AcDbLine* line = new AcDbLine(m.pt, ptt);

			ptArr2.append(ptt);
			vecEnts.push_back(line);

		}

		AcDbPolyline * pl2 = new AcDbPolyline();

		for (int i = 0; i < ptArr2.length();i++)
		{
			pl2->addVertexAt(pl2->numVerts(), AcGePoint2d(ptArr2[i].x, ptArr2[i].y), 0, 0, 0);

		}
		pl2->addVertexAt(pl2->numVerts(), AcGePoint2d(ptArr2[0].x, ptArr2[0].y), 0, 0, 0);
		vecEnts.push_back(pl2);

		delete pl;
		pl = NULL;

		for (int i = 0; i < (int)vecEnts.size();i++)
		{
			vecEnts[i]->setColorIndex(1);

			PostToModelSpace(vecEnts[i]);
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
	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CEcdJiaYeApp, ECDMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void ECDMyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM )
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CEcdJiaYeApp, ECDMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void ECDMyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CEcdJiaYeApp class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.
	
	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CEcdJiaYeApp, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdJiaYeApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdJiaYeApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CEcdJiaYeApp, ECDMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CEcdJiaYeApp, ECDMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CEcdJiaYeApp, MyLispFunction, false)

