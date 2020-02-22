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
#include"ECDMyHuaLiang.h"
#include"EcdZhuangZhu.h"
#include"DwgDataBaseUtil.h"
#include"EcdKuaiJX.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CECDDrawGriderApp : public AcRxArxApp {

public:
	CECDDrawGriderApp () : AcRxArxApp () {}

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
		
	static void TESTmrcmd()
	{
		ads_name name;
		ads_point ptEnt;
		AcDbObjectId id;
		if (acedEntSel(_T("选择要镜像的实体"), name, ptEnt) != RTNORM)
		{
			acutPrintf(_T("\nno obj select"));
			return;
		}
		if (Acad::ErrorStatus::eOk != acdbGetObjectId(id, name))
		{
			return;
		}
		AcDbEntity* pEnt;
		if (Acad::ErrorStatus::eOk != acdbOpenObject(pEnt, id, AcDb::OpenMode::kForWrite))
		{
			acutPrintf(_T("\n打开实体失败"));
			return;
		}
		ads_point ptDis1;
		if (acedGetPoint(NULL, _T("\n选择镜像点1"), ptDis1) != RTNORM)
		{
			acutPrintf(_T("\nno point select"));
			pEnt->close();
			return;
		}
		ads_point ptDis2;
		if (acedGetPoint(NULL, _T("\n选择镜像点2"), ptDis2) != RTNORM)
		{
			acutPrintf(_T("\nno point select"));
			pEnt->close();
			return;
		}
		AcGePoint3d ptDis3d1 = asPnt3d(ptDis1);
		AcGePoint3d ptDis3d2 = asPnt3d(ptDis2);
		AcGeLine3d line(ptDis3d1, ptDis3d2);
		AcGeMatrix3d mat;
		mat.setToMirroring(line);
		acedInitGet(NULL, _T("Y N"));
		
		TCHAR kword[20];
		if (acedGetKword(TEXT("删除原对象？Y/N"), kword) != RTNORM)
		{
			return;
		}
		
		if (kword[0]=='N')
		{
			AcDbEntity* pEnt2 = AcDbEntity::cast(pEnt->clone());
			CDwgDataBaseUtil::PostToModelSpace(pEnt2);
			pEnt2->close();
		}
		pEnt->transformBy(mat);
		pEnt->close();
	}
//》》》》》》》》》》》》》》》》》》》》》》》》》》》》》》》》》》》》

	static void ECDMyGroupECDXieLiang() {
		ECDMyHuaLiang *drawLiang = new ECDMyHuaLiang();
		drawLiang->Command();
		delete drawLiang;
	}

	static void ECDMyGroupECDZhuangzhu() {
		EcdZhuangZhu *drawZz = new EcdZhuangZhu();
		drawZz->Command();
		delete drawZz;
	}
	
	static void ECDMyGroupECDTest() {
	
		EcdKuaiJX * ecdKuaiJx = new EcdKuaiJX();

		ecdKuaiJx->Command();

		delete ecdKuaiJx;
	
	}

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

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CECDDrawGriderApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CECDDrawGriderApp, ECDMyGroup, ECDXieLiang, ECDXieLiang, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CECDDrawGriderApp, ECDMyGroup, ECDZhuangzhu, ECDZhuangzhu, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CECDDrawGriderApp, ECDMyGroup, ECDTest, ECDTest, ACRX_CMD_MODAL, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CECDDrawGriderApp, ECDMyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)

