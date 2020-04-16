

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
	struct mySt
	{
		AcDbLine *line;
		bool isStart;
		AcGePoint3d pt;
	};

	CQiangToLiangApp() : AcRxArxApp() {}

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
	static void ECDMyGroupMyCommand() {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void ECDMyGroupMyPickFirst() {
		ads_name result;
		int iRet = acedSSGet(ACRX_T("_I"), NULL, NULL, NULL, result);
		if (iRet == RTNORM)
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
	static void ECDMyGroupMySessionCmd() {
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
	static int ads_MyLispFunction() {
		//struct resbuf *args =acedGetArgs () ;

		// Put your command code here

		//acutRelRb (args) ;

		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM);
	}
	static void ECDQiangToLiangEcdQ2L(void)
	{
		ErrorStatus es = ErrorStatus::eOk;

		vector<AcDbLine*>vecLines;

		ads_name aName;
		if (acedSSGet(NULL, NULL, NULL, NULL, aName) != RTNORM) {
			return;
		}

		int len = 0;
		acedSSLength(aName, &len);

		AcDbObjectIdArray ids;

		for (int i = 0; i < len; i++)
		{
			ads_name temp;

			acedSSName(aName, i, temp);

			AcDbObjectId tempId;
			acdbGetObjectId(tempId, temp);

			ids.append(tempId);

		}
		acedSSFree(aName);

		for (int i = 0; i < ids.length(); i++)
		{
			AcDbLine * l = NULL;

			if (acdbOpenObject(l, ids[i], AcDb::kForWrite) == Acad::eOk) {

				vecLines.push_back(l);
			}
		}
		acutPrintf(L"before=%d", vecLines.size());
		AcGeIntArray  delIndex;
		AcGeIntArray changeIndex;

		vector<AcDbLine*>vvL;
		vector<AcGePoint3d>vecPt;
		//十字路口連綫
		for (int ww = 0; ww < (int)vecLines.size(); ww++)
		{
			AcDbLine * l1 = vecLines[ww];

			//记录被删除的元素个数
			for (int j = ww + 1; j < (int)vecLines.size(); j++)
			{

				AcDbLine *l2 = vecLines[j];

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

				//在同一直綫上

				if (fabs(aa2) < 1 || fabs(aa2 - 180) < 1) {

					AcGePoint3d temp1, temp2;

					if (fabs(vecPt[0].x - vecPt[1].x) < 15) {

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

					if (fabs(vecPt[2].x - vecPt[3].x) < 15) {

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

					AcGeVector3d vecT1 = vecPt[0] - vecPt[1];
					AcGeVector3d vecT2 = vecPt[1] - vecPt[2];
					if (vecPt[1] == vecPt[2])
						vecT2 = vecPt[1] - vecPt[3];


					double angle3 = vecT1.angleTo(vecT2);
					double aa3 = angle3 / PI * 180;

					if ((vecPt[0] != vecPt[2] && vecPt[1] != vecPt[3]) && (fabs(aa3) < 5 || fabs(aa3 - 180) < 5))
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

						/*l1->setStartPoint(vecPt[inE]);
						l1->setEndPoint(vecPt[inF]);*/

AcDbLine * l = new AcDbLine(vecPt[inE], vecPt[inF]);

changeIndex.append(ww);
delIndex.append(j);

vvL.push_back(l);
					}
				}

				vecPt.clear();
			}

		}
		acutPrintf(L"\ndelIndex=%d\n", delIndex.length());
		acutPrintf(L"vvL=%d\n", vvL.size());
		acutPrintf(L"after=%d", vecLines.size());

		int cI = 0;
		for (int i = 0; i < (int)vecLines.size(); i++)
		{

			if (changeIndex.contains(i) && cI < vvL.size()) {
				vecLines[i]->setColorIndex(2);
				vecLines[i]->setStartPoint(vvL[cI]->startPoint());
				vecLines[i]->setEndPoint(vvL[cI]->endPoint());
				delete vvL[cI];
				vvL[cI] = NULL;

				vecLines[i]->close();
				++cI;
			}
			else if (delIndex.contains(i)) {
				vecLines[i]->erase();
				vecLines[i]->close();
				vecLines[i] = NULL;
			}
			else {
				vecLines[i]->setColorIndex(1);

				vecLines[i]->close();
			}
		}

		vector<AcDbLine*>vecL2;
		for (size_t i = 0; i < vecLines.size(); i++)
		{

			AcDbLine * l = NULL;

			if (vecLines[i] != NULL) {
				if (acdbOpenObject(l, vecLines[i]->objectId(), AcDb::kForWrite) == Acad::eOk) {

					vecL2.push_back(l);
					l->setColorIndex(1);
				}
			}
		}


		/*vecLines.clear();

		for (int i = 0; i < ids.length(); i++)
		{
			AcDbLine * l = NULL;

			if (acdbOpenObject(l, ids[i], AcDb::kForWrite) == Acad::eOk) {

				vecLines.push_back(l);
			}
		}*/

		AcGePoint3dArray ptArr;
		vector<mySt>vecSt;

		acutPrintf(L"\nvecL2=%d", vecL2.size());

		for (int i = 0; i < (int)vecL2.size(); i++)
		{
			AcDbLine * l1 = vecL2[i];
			AcGePoint3d l1S = l1->startPoint();
			AcGePoint3d l1E = l1->endPoint();
			AcGeVector3d vecL1 = l1S - l1E;
			for (int j = i + 1; j < (int)vecL2.size(); j++)
			{
				AcDbLine *l2 = vecL2[j];

				AcGePoint3d l2S = l2->startPoint();
				AcGePoint3d l2E = l2->endPoint();
				AcGeVector3d vecL2 = l2S - l2E;

				double angle2 = vecL1.angleTo(vecL2);
				double aa2 = angle2 / PI * 180;




				l1->intersectWith(l2, AcDb::kOnBothOperands, ptArr, 0, 0);

				int n = ptArr.length();

				if ((fabs(aa2) < 1 || fabs(aa2 - 180) < 1) && n == 0)
				{
					continue;
				}


				AcGePoint3d ptC1, ptC2;
				

				/*l1->getClosestPointTo(l2S, ptC1, Adesk::kTrue);
				l1->getClosestPointTo(l2E, ptC2, Adesk::kTrue);

				AcDbLine *ll = new AcDbLine(ptC1, ptC2);

				ll->setColorIndex(2);

				PostToModelSpace(ll);

				ll->close();*/

				//InLine(l1, ptC1, vecSt);
				//InLine(l1, ptC2, vecSt);

				//InLine(l2, ptC1, vecSt);
				//InLine(l2, ptC2, vecSt);
				l1->intersectWith(l2, AcDb::kExtendBoth , ptArr, 0, 0);

				if (ptArr.length() >= 1) {

					for (int n = 0; n < ptArr.length(); n++)
					{
						InLine(l1, ptArr[n],vecSt);
						InLine(l2, ptArr[n], vecSt);
					}

				}
				ptArr.removeAll();
			}
		}

		for (int i = 0; i < (int)vecL2.size(); i++)
		{
			for (int j = 0; j < (int)vecSt.size();j++)
			{
				if (vecSt[j].line == vecL2[i]) {

					if (vecSt[j].isStart) {
						vecL2[i]->setStartPoint(vecSt[j].pt);
					}
					else {
						vecL2[i]->setEndPoint(vecSt[j].pt);
					}
					break;
				}

			}

			vecL2[i]->close();
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
		if (fabs(ax) < fabs(ay)) {
			return pt1.y < pt2.y;
		}
		else {
			return pt1.x < pt2.x;
		}
	}

private:

	static void InLine(AcDbLine * l1, const  AcGePoint3d & pt,vector<mySt>&vecSt)
	{
		static int colorI = 1;

		AcGePoint3d pt1 = l1->startPoint();
		AcGePoint3d pt2 = l1->endPoint();

		if (pt1 == pt || pt2 == pt) {
			return;
		}

		double dis1 = pt1.distanceTo(pt2);
		double dis2 = pt1.distanceTo(pt);
		double dis3 = pt2.distanceTo(pt);

		AcGeVector3d vec1 = pt1 - pt2;
		AcGeVector3d vec2 = pt1 - pt;
		AcGeVector3d vec3 = pt2 - pt;

		double minLen = (dis2 > dis3 ? dis3 : dis2);

		if (minLen > 500) {
			return;
		}

		double angle2 = vec1.angleTo(vec2);
		double aa2 = angle2 / PI * 180;

		double angle3 = vec1.angleTo(vec3);
		double aa3 = angle3 / PI * 180;
		double max1 = (dis2 > dis3 ? dis2 : dis3);

		if ((fabs(aa3) < 1 || fabs(aa3 - 180) < 1) && (fabs(aa2) < 1 || fabs(aa2 - 180) < 1))
		{
			if (dis2 > dis1) {
				
				if (max1 > 10 * dis1) {

					return;

				}

				mySt st;
				st.line = l1;
				st.isStart = false;
				st.pt = pt;

				vecSt.push_back(st);

			}
			else if (dis3 > dis1) {
				
				if (max1 > 10 * dis1) {
					return;
				}
				mySt st;
				st.line = l1;
				st.isStart = true;
				st.pt = pt;
				vecSt.push_back(st);
				
			}
		}

	}


};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CQiangToLiangApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CQiangToLiangApp, MyLispFunction, false)
ACED_ARXCOMMAND_ENTRY_AUTO(CQiangToLiangApp, ECDQiangToLiang, EcdQ2L, EcdQ2L, ACRX_CMD_TRANSPARENT, NULL)
