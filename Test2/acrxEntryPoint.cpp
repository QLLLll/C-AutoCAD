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
#include"SelectUtil.h"
#include"DwgDataBaseUtil.h"
#include"GePointUtil.h"
#include"GetInputUtil.h"
#include"MathUtil.h"
#include"ConvertUtil.h"
#include"DrawRecJig.h"
#include"dbJoinEntityPE.h"
#include<algorithm>
#include<vector>
using namespace std;

struct myStruct {
	AcGePoint2d pt;
	AcGePoint2d pt2;
	double dis;

};
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CTest2App : public AcRxArxApp {

public:
	CTest2App () : AcRxArxApp () {}

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
	// function of the CTest2App class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CTest2App, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void ECDMyGroupEcdQieJiao () {
		ErrorStatus es;
		AcGeVector2d recVec2d;
		AcGeVector3d recVec3d;
		AcGePoint3d pt1, pt2;

		if (!CGetInputUtil::GetPoint(TEXT("输入矩形朝向：\n"), pt1)) {
			return;
		}
		if (!CGetInputUtil::GetPoint(pt1, TEXT(""), pt2)) {
			return;
		}
		recVec3d = pt2 - pt1;
		recVec2d = AcGeVector2d(recVec3d.x, recVec3d.y);


		DrawRecJig recJig = DrawRecJig();

		ads_point  pt;
		AcDbObjectIdArray idArr;

		while (acedGetPoint(NULL, L"\n请选择插入点：", pt) == RTNORM) {


			AcDbPolyline *pl1 = NULL;
			AcDbObjectId  oId1;

			if (recJig.DoIt(asPnt3d(pt), recVec2d, oId1)) {

				idArr.append(oId1);
			}

			if (idArr.length() == 2) {
				AcDbPolyline * temp1 = NULL;
				AcDbPolyline * temp2 = NULL;

				es = acdbOpenObject(temp1, idArr[0], AcDb::kForWrite);
				if (es != ErrorStatus::eOk) {
					return;
				}

				es = acdbOpenObject(temp2, idArr[1], AcDb::kForWrite);

				if (es != ErrorStatus::eOk) {
					temp1->close();
					return;
				}

				AcGePoint2dArray arr1, arr2;

				AcDbPolyline *maxPl = NULL;
				AcDbPolyline *minPl = NULL;

				double area1 = 0, area2 = 0;

				temp1->getArea(area1);
				temp2->getArea(area2);

				if (area1<area2) {
					maxPl = temp2;
					minPl = temp1;
				}
				else {
					maxPl = temp1;
					minPl = temp2;
				}

				AcGePoint2dArray maxArr, minArr;

				vector<myStruct>vecMinSt1, vecMinSt2;

				getPtArrs(maxArr, maxPl);
				getPtArrs(minArr, minPl);

				AcGePoint3dArray ptArr;

				es = minPl->intersectWith(maxPl, AcDb::kOnBothOperands, ptArr);

				for (int i = 0; i < ptArr.length(); i++)
				{
					AcGePoint2d pt = CConvertUtil::ToPoint2d(ptArr[i]);
					for (int j = 0; j < maxArr.length(); j++)
					{
						if (CGePointUtil::IsEqual(maxArr[j], pt, 1e-4)) {

							ptArr.removeAt(i);

						}
					}
				}

				acutPrintf(L"%d", es);

				if (es != ErrorStatus::eOk) {


					maxPl->close();
					minPl->close();
					idArr.removeAll();
					return;

				}

				if (ptArr.length() >= 2) {

					AcDbVoidPtrArray maxCurveArr1, minCurveArr2;

					es = maxPl->getSplitCurves(ptArr, maxCurveArr1);

					acutPrintf(L"%d", es);

					if (maxCurveArr1.length() <= 1) {

						ptArr.reverse();

						for (int i = 0; i < maxCurveArr1.length(); i++)
						{
							delete maxCurveArr1[i];

							maxCurveArr1[i] = NULL;

							maxCurveArr1.setLogicalLength(0);

						}
						es = maxPl->getSplitCurves(ptArr, maxCurveArr1);

						acutPrintf(L"ddd=%d\n", es);

					}

					acutPrintf(L"ptCount=%d,maxCurveCount=%d\n", ptArr.length(), maxCurveArr1.length());

					if (es != ErrorStatus::eOk) {

						maxPl->close();
						minPl->close();
						return;
					}
					es = minPl->getSplitCurves(ptArr, minCurveArr2);
					

					if (minCurveArr2.length() <= 1) {

						ptArr.reverse();

						for (int i = 0; i < minCurveArr2.length(); i++)
						{
							delete minCurveArr2[i];

							minCurveArr2[i] = NULL;

							minCurveArr2.setLogicalLength(0);

						}
						es = minPl->getSplitCurves(ptArr, minCurveArr2);

						acutPrintf(L"ssss=%d\n", es);

					}

					acutPrintf(L"ptCount=%d,minCurveCount=%d\n", ptArr.length(), minCurveArr2.length());

					if (es != ErrorStatus::eOk) {

						maxPl->close();
						minPl->close();
						return;
					}

					maxPl->close();
					minPl->close();

					vector<AcDbCurve*>vCurve1, vCurve2;


					 AcDbCurve * c=NULL;
					for (int i=0;i<maxCurveArr1.length();i++)
					{
					c=static_cast<AcDbCurve*>(maxCurveArr1[i]);
					vCurve1.push_back(c);
					if(i%2==0)
					{ c->setColorIndex(3);}
					else
					{ c->setColorIndex(4);}
					//CDwgDataBaseUtil::PostToModelSpace(c);
					}
					//c->close();
					c=NULL;
					for (int i=0;i<minCurveArr2.length();i++)
					{
					c=static_cast<AcDbCurve*>(minCurveArr2[i]);
					vCurve2.push_back(c);
					if(i%2==0)
					{c->setColorIndex(5);}
					else
					{ c->setColorIndex(2);}
					//CDwgDataBaseUtil::PostToModelSpace(c);

					}
					//c->close();

					AcDbCircle *circle = new AcDbCircle(ptArr[0], AcGeVector3d::kZAxis, 1);
					AcGePoint3dArray tempArr1, tempArr2;
				
					bool flag = false;

					for (int i = 0; i < (int)vCurve1.size(); i++)
					{

						AcDbCurve * c1 = vCurve1[i];

						c1->intersectWith(circle, AcDb::kOnBothOperands, tempArr1);
	
						for (int j = 0; j < (int)vCurve2.size(); j++)
						{

							AcDbCurve * c2 = vCurve2[j];
							c2->intersectWith(circle, AcDb::kOnBothOperands, tempArr2);

							acutPrintf(L"[%f,%f,%f],[%f,%f,%f]\n", tempArr1[0].x, tempArr1[0].y,
								tempArr1[0].z, tempArr2[0].x, tempArr2[0].y,
								tempArr2[0].z);

							if (CGePointUtil::IsEqual(tempArr1[0], tempArr2[0], 1e-4)) {

								delete c1;

								c1 = NULL;
								vCurve1.erase(vCurve1.begin() + i);

								delete c2;

								c2 = NULL;
								vCurve2.erase(vCurve2.begin() + j);
								flag = true;
								/*c1->setColorIndex(2);
								CDwgDataBaseUtil::PostToModelSpace(c1);
								c1->close();
								
								c2->setColorIndex(3);
								CDwgDataBaseUtil::PostToModelSpace(c2);
								c2->close();*/
								break;


							}
							tempArr2.setLogicalLength(0);

						}
						tempArr1.setLogicalLength(0);
					}
					delete circle;
					circle = NULL;

					if (flag == false) {

						for (int i = 0; i < (int)vCurve1.size(); i++)
						{
							delete vCurve1[i];
							vCurve1[i] = NULL;
						}

						for (int i = 0; i < (int)vCurve2.size(); i++)
						{
							delete vCurve2[i];
							vCurve2[i] = NULL;
						}
						AfxMessageBox(TEXT("两个矩形有一个角要共角"));
						continue;
					}
						es = acdbOpenObject(temp1, idArr[0], AcDb::kForWrite);
						if (es != ErrorStatus::eOk) {
							return;
						}

						es = acdbOpenObject(temp2, idArr[1], AcDb::kForWrite);

						if (es != ErrorStatus::eOk) {
							temp1->close();
							return;
						}

						temp1->erase();
						temp1->close();

						temp2->erase();
						temp2->close();


					

					AcDbJoinEntityPE* pJoinPE = AcDbJoinEntityPE::cast(vCurve1[0]->queryX(AcDbJoinEntityPE::desc()));

					if (pJoinPE != NULL) {

						Acad::ErrorStatus es = pJoinPE->joinEntity(vCurve1[0], vCurve2[0]);

						if (Acad::eOk == es) {

							// myEntity1 should updated 
							vCurve1[0]->setColorIndex(5);

							CDwgDataBaseUtil::PostToModelSpace(vCurve1[0]);

							vCurve1[0]->close();


						}
						else {
							AfxMessageBox(TEXT("修改失败"));

							delete vCurve1[0];
							delete vCurve2[0];
						}
					}
					

					

					/*vCurve1[0]->setColorIndex(3);
					vCurve2[0]->setColorIndex(4);

					CDwgDataBaseUtil::PostToModelSpace(vCurve1[0]);
					CDwgDataBaseUtil::PostToModelSpace(vCurve2[0]);
					vCurve1[0]->close();
					vCurve2[0]->close();
*/

					idArr.removeAll();


					// ptArr.setPhysicalLength(0);
				}

				else {
					maxPl->erase();
					minPl->close();

					minPl->erase();
					minPl->close();

					AfxMessageBox(TEXT("两个矩形要有交点才行！"));
					idArr.removeAll();
					continue;

					AcGePoint2d ptCenterMax = CGePointUtil::GetMiddlePoint(maxArr[0], maxArr[2]);
					AcGePoint2d ptCenterMin = CGePointUtil::GetMiddlePoint(minArr[0], minArr[2]);

					getAllMinDis(ptCenterMax, minArr, vecMinSt1);
					getAllMinDis(ptCenterMin, maxArr, vecMinSt2);


					AcGePoint2d minPt1 = vecMinSt1[0].pt;
					AcGePoint2d minPt2 = vecMinSt1[1].pt;

					AcGePoint2d maxPt1 = vecMinSt2[0].pt;
					AcGePoint2d maxPt2 = vecMinSt2[1].pt;

					AcGeVector2d vecMax = maxPt1 - maxPt2;
					AcGeVector2d vecMin = minPt1 - minPt2;

					double angle = vecMax.angleTo(vecMin);

					if (angle >= 0.5*CMathUtil::PI()) {

						AcGePoint2d temp = minPt1;
						minPt1 = minPt2;
						minPt2 = temp;

					}

					int maxIndex1 = 0, maxIndex2 = 0;
					int minIndex1 = 0, minIndex2 = 0;

					maxIndex1 = maxArr.find(maxPt1);
					maxIndex2 = maxArr.find(maxPt2);

					minIndex1 = minArr.find(minPt1);
					minIndex2 = minArr.find(minPt2);

					if ((maxIndex2 - maxIndex1 == 1) || (maxIndex1 == 3 && maxIndex2 == 0)) {


						maxPl->addVertexAt(maxIndex1 + 1, minPt1);

						if (minIndex1>minIndex2 || (minIndex1 == 0 && minIndex2 == 3)) {

							maxPl->addVertexAt(maxIndex1 + 2, minArr[(minIndex1 + 1) % minArr.length()]);
							maxPl->addVertexAt(maxIndex1 + 3, minArr[(minIndex1 + 2) % minArr.length()]);
							maxPl->addVertexAt(maxIndex1 + 4, minArr[(minIndex1 + 3) % minArr.length()]);
						}
						else if (minIndex1<minIndex2 || (minIndex1 == 3 && minIndex2 == 0)) {

							minArr.reverse();
							minIndex1 = minArr.find(minPt1);

							maxPl->addVertexAt(maxIndex1 + 2, minArr[(minIndex1 + 1) % minArr.length()]);
							maxPl->addVertexAt(maxIndex1 + 3, minArr[(minIndex1 + 2) % minArr.length()]);
							maxPl->addVertexAt(maxIndex1 + 4, minArr[(minIndex1 + 3) % minArr.length()]);
						}


					}

					if ((maxIndex2 - maxIndex1 == -1) || (maxIndex1 == 0 && maxIndex2 == 3)) {

						maxPl->addVertexAt(maxIndex2 + 1, minPt2);
						if (minIndex1>minIndex2 || (minIndex1 == 0 && minIndex2 == 3)) {

							minArr.reverse();
							minIndex2 = minArr.find(minPt2);
							maxPl->addVertexAt(maxIndex2 + 2, minArr[(minIndex2 + 1) % minArr.length()]);
							maxPl->addVertexAt(maxIndex2 + 3, minArr[(minIndex2 + 2) % minArr.length()]);
							maxPl->addVertexAt(maxIndex2 + 4, minArr[(minIndex2 + 3) % minArr.length()]);

						}
						else if (minIndex1<minIndex2 || (minIndex1 == 3 && minIndex2 == 0)) {


							maxPl->addVertexAt(maxIndex2 + 2, minArr[(minIndex2 + 1) % minArr.length()]);
							maxPl->addVertexAt(maxIndex2 + 3, minArr[(minIndex2 + 2) % minArr.length()]);
							maxPl->addVertexAt(maxIndex2 + 4, minArr[(minIndex2 + 3) % minArr.length()]);

						}
					}
					AcDbLine *l = new AcDbLine(CConvertUtil::ToPoint3d(minPt1), CConvertUtil::ToPoint3d(minPt2));

					l->setColorIndex(1);
					CDwgDataBaseUtil::PostToModelSpace(l);
					l->close();

					maxPl->setClosed(Adesk::kTrue);

					maxPl->setColorIndex(4);
					maxPl->close();

					minPl->erase();
					minPl->close();
					idArr.removeAll();

				}

			}

		}

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CTest2App, ECDMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
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
	// ACED_ARXCOMMAND_ENTRY_AUTO(CTest2App, ECDMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void ECDMyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CTest2App class.
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
	// ACED_ADSFUNCTION_ENTRY_AUTO(CTest2App, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	static void getPtArrs(AcGePoint2dArray&arr, AcDbPolyline *pl) {

		for (int i = 0; i<(int)pl->numVerts(); i++)
		{
			AcGePoint2d pt;

			pl->getPointAt(i, pt);
			arr.append(pt);
		}

	}

	static void getPtArrs(AcGePoint3dArray&arr, AcDbPolyline *pl) {

		for (int i = 0; i<(int)pl->numVerts(); i++)
		{
			AcGePoint2d pt;
			AcGePoint3d pt3d;

			pl->getPointAt(i, pt);

			pt3d = CConvertUtil::ToPoint3d(pt);
			arr.append(pt3d);
		}

	}

	static void getAllMinDis(AcGePoint2d ptCenter, AcGePoint2dArray arr2, vector<myStruct>& sts) {

		for (int j = 0; j<arr2.length(); j++)
		{
			double dis = ptCenter.distanceTo(arr2[j]);

			myStruct mSt;
			mSt.pt = arr2[j];
			mSt.dis = dis;
			mSt.pt2 = ptCenter;

			sts.push_back(mSt);

		}

		sort(sts.begin(), sts.end(), comp);
	}

	static bool comp(myStruct st1, myStruct st2)
	{
		return st1.dis<st2.dis;
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CTest2App)

ACED_ARXCOMMAND_ENTRY_AUTO(CTest2App, ECDMyGroup, EcdQieJiao, EcdQieJiao, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CTest2App, ECDMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CTest2App, ECDMyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CTest2App, MyLispFunction, false)

