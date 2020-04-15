

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include<vector>
#define PI 3.1415926
using namespace std;

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CQiangToLiangApp : public AcRxArxApp {

public:
	CQiangToLiangApp () : AcRxArxApp () {}

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
	// function of the CQiangToLiangApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ECDMyGroupMyCommand () {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
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
	// ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void ECDMyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CQiangToLiangApp class.
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
	// ACED_ADSFUNCTION_ENTRY_AUTO(CQiangToLiangApp, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	static void ECDQiangToLiangEcdQ2L(void)
	{
		ErrorStatus es=ErrorStatus::eOk;

		vector<AcDbLine*>vecLines;

		ads_name aName;
		if (acedSSGet(NULL, NULL, NULL, NULL, aName) != RTNORM) {
			return;
		}

		int len = 0;
		acedSSLength(aName, &len);

		AcDbObjectIdArray ids;

		for (int i = 0; i<len; i++)
		{
			ads_name temp;

			acedSSName(aName, i, temp);

			AcDbObjectId tempId;
			acdbGetObjectId(tempId, temp);

			ids.append(tempId);

		}
		acedSSFree(aName);

		for (int i = 0; i<ids.length(); i++)
		{
			AcDbLine * l = NULL;

			if (acdbOpenObject(l, ids[i], AcDb::kForWrite) == Acad::eOk) {

				vecLines.push_back(l);
			}
		}
		acutPrintf(L"before=%d", vecLines.size());
		AcGeIntArray  delIndex;

		vector<AcDbLine*>vvL;
		vector<AcGePoint3d>vecPt;
		
		for (int i = 0; i<(int)vecLines.size(); i++)
		{
			AcDbLine * l1 = vecLines[i];
			
			//记录被删除的元素个数
			for (int j = i+1; j<(int)vecLines.size(); j++)
			{


				AcDbLine *l2 = vecLines[j];

				AcGeVector3d vec = l1->startPoint() - l2->endPoint();
				AcGeVector3d vec2 = l1->startPoint() - l2->startPoint();

				double angle = vec.angleTo(vec2);
				double aa = angle / PI * 180;
				//AcGePoint3d ptClosed, ptC2;

				AcGePoint3d pt11 = l1->startPoint();
				AcGePoint3d pt12 = l1->endPoint();

				AcGePoint3d pt21 = l2->startPoint();
				AcGePoint3d pt22 = l2->endPoint();
				
				

				vecPt.push_back(pt11);
				vecPt.push_back(pt12);
				vecPt.push_back(pt21);
				vecPt.push_back(pt22);

				//寻找最短
				int indexF = 0, indexE = 1;
				double minDis = vecPt[0].distanceTo(vecPt[1]);
				
				
					for (int k = 0; k < (int)vecPt.size(); k++)
					{

						for (int m = k + 1; m < (int)vecPt.size(); m++)
						{

							double d = vecPt[k].distanceTo(vecPt[m]);
							if (d < minDis&&d>10) {
								indexF = k;
								indexE = m;
								minDis = d;
							}
						}
					}
				


				AcGeVector3d vec3 = vecPt[indexF] - vecPt[indexE];
								
				AcGeVector3d vec4 = l1->startPoint() - l1->endPoint();

				if (vec3 == vec4) {
					vec4 = l2->startPoint() - l2->endPoint();
				}

				double angle2 = vec3.angleTo(vec4);
				double aa2 = angle2 / PI * 180;

			

			/*	l1->getClosestPointTo(l2->startPoint(), ptClosed, Adesk::kTrue);
				l1->getClosestPointTo(l2->endPoint(), ptC2, Adesk::kTrue);

				double dis = ptClosed.distanceTo(l2->startPoint());
				double dis2 = ptC2.distanceTo(l2->endPoint());*/


				//在同一直綫上
				/*if(fabs(angle/PI*180)<=0.5||fabs(angle/PI*180)>=179.5){*/
				if (fabs(aa2) < 0.3 || fabs(aa2 - 180) < 0.3) {

					AcGePoint3d temp1, temp2;

					if (fabs(vecPt[0].x - vecPt[1].x) < 1) {

						if (vecPt[0].y > vecPt[1].y) {
							temp1 = vecPt[0];
							vecPt[0] = vecPt[1];
							vecPt[1] = temp1;
						}

					}
					else {
						if (vecPt[0].x > vecPt[1].x)
						{
							temp1 = vecPt[0];
							vecPt[0] = vecPt[1];
							vecPt[1] = temp1;
						}
					}

					if (fabs(vecPt[2].x - vecPt[3].x) < 1) {

						if (vecPt[2].y > vecPt[3].y) {
							temp2 = vecPt[2];
							vecPt[2] = vecPt[3];
							vecPt[3] = temp2;
						}

					}
					else {
						if (vecPt[2].x > vecPt[3].x)
						{
							temp2 = vecPt[2];
							vecPt[2] = vecPt[3];
							vecPt[3] = temp2;
						}
					}

					AcGeVector3d vecT1 = vecPt[0] - vecPt[2];
					AcGeVector3d vecT2 = vecPt[1] - vecPt[3];

					double angle3 = vecT1.angleTo(vecT2);
					double aa3 = angle3 / PI * 180;
					if ((vecPt[0]!=vecPt[2]&&vecPt[1]!=vecPt[3])&&(fabs(aa3) < 0.3 || fabs(aa3 - 180) < 0.3))
						//寻找最长
					{
						int inF = 0, inE = 1;
						double maxDis = vecPt[0].distanceTo(vecPt[1]);

						for (int k = 0; k < (int)vecPt.size(); k++)
						{

							for (int m = k + 1; m < (int)vecPt.size(); m++)
							{

								double d = vecPt[k].distanceTo(vecPt[m]);
								if (d > maxDis) {
									inF = k;
									inE = m;
									maxDis = d;
								}
							}
						}



						//sort(vecPt.begin(), vecPt.end(), comp);
						//if(l1Len>=l2Len){

						//	if(len1>len2){
						//		l1->setEndPoint(pt21);
						//	}
						//	else{
						//		l1->setEndPoint(pt22);
						//	}
						//	/*l2->erase();
						//	l2->close();
						//	l2=NULL;*/
						//	vecLines.erase(vecLines.begin() + j - m, vecLines.begin() + j + 1 - m);
						//	m++;//删除了一个，下标发生了变化，但是j还是原来的位置，所以要多减一个

						//}
						//else{

						//	if(len3>len4){
						//		l1->setStartPoint(pt21);
						//		l1->setEndPoint(pt12);
						//	}else{
						//		l1->setStartPoint(pt12);
						//		l1->setEndPoint(pt22);
						//	}
						//	
						//}

						/*l1->setStartPoint(vecPt[0]);
						l1->setEndPoint(vecPt[3]);*/
						AcDbLine *line = new AcDbLine(vecPt[inE], vecPt[inF]);

						vvL.push_back(line);
						delIndex.append(j);

					}
				}
				vecPt.clear();
			}
			
		}
		acutPrintf(L"\ndelIndex=%d\n", delIndex.length());
		acutPrintf(L"vvL=%d\n", vvL.size());
		acutPrintf(L"after=%d", vecLines.size());

		//for (size_t i=0;i<vecLines.size()&&!delIndex.contains(i);i++)
		//{
		//	/*if(!delIndex.contains(i))*/
		//	vecLines[i]->setColorIndex(1);
		//	/*else
		//		vecLines[i]->setColorIndex(2);*/
		//	vecLines[i]->close();
		//
		//}

		for (size_t i = 0; i<vvL.size(); i++)
		{

			vvL[i]->setColorIndex(1);
			PostToModelSpace(vvL[i]);

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
	static bool comp(AcGePoint3d pt1, AcGePoint3d pt2) {
		double ax = pt1.x - pt2.x;
		double ay = pt1.y - pt2.y;
		if (fabs(ax) <fabs(ay)) {
			return pt1.y < pt2.y;
		}
		else {
			return pt1.x < pt2.x;
		}
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CQiangToLiangApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CQiangToLiangApp, MyLispFunction, false)
ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDQiangToLiang, EcdQ2L, EcdQ2L, ACRX_CMD_TRANSPARENT, NULL)
