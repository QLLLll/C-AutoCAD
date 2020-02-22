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
#include"RegMsg.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")
#define PI 3.14159265358979323846

BOOL func(MSG *msg);

AcDbObjectId oId;
AcGeMatrix3d matx1;
AcGeMatrix3d matx2;
RegMsg * reg = NULL;

BOOL func(MSG *msg) {

	int zdFx = 0;

	if (msg->message == WM_MOUSEWHEEL) {

		zdFx = (short)HIWORD(msg->wParam);
		acutPrintf(L"\n%d", zdFx);
		if (zdFx == 120) {

			AcDbEntity * pEnt = NULL;

			ErrorStatus es = acdbOpenObject(pEnt, oId, AcDb::kForWrite);
			if (es == Acad::eOk) {

				pEnt->transformBy(matx1);
				pEnt->close();
			}

		}
		else if (zdFx == -120) {

			AcDbEntity * pEnt = NULL;

			ErrorStatus es = acdbOpenObject(pEnt, oId, AcDb::kForWrite);

			if (es == Acad::eOk) {

				pEnt->transformBy(matx2);
				pEnt->close();
			}
		}
		return TRUE;
	}


	return FALSE;
}

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CMouseWheelMsgApp : public AcRxArxApp {

public:
	CMouseWheelMsgApp() : AcRxArxApp() {}

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


	static void ECDMyGroupMyRegMsg() {

		acDocManager->lockDocument(acDocManager->curDocument());

		if (reg == NULL) {
			reg = new RegMsg();

			AcRxClass *cls = AcDbEntity::desc();

			AcDbEntity * pEnt = NULL;

			AcGePoint3d pickPoint;

			if (CSelectUtil::PromptSelectEntity(L"Ñ¡ÔñÊµÌå£º", cls, pEnt, pickPoint, true)) {

				oId = pEnt->objectId();
				pEnt->close();
				matx1 = AcGeMatrix3d::rotation(1.0 / 18 * PI, AcGeVector3d::kZAxis, pickPoint);
				matx2 = AcGeMatrix3d::rotation(-1.0 / 18 * PI, AcGeVector3d::kZAxis, pickPoint);

				reg->reg();
			}
		}

	}
	static void ECDMyGroupMyUnRegMsg() {

		acDocManager->unlockDocument(acDocManager->curDocument());

		if (reg != NULL) {

			reg->unReg();

			reg = NULL;

			delete reg;
		}
	}

};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CMouseWheelMsgApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CMouseWheelMsgApp, ECDMyGroup, MyRegMsg, MyRegMsg, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CMouseWheelMsgApp, ECDMyGroup, MyUnRegMsg, MyUnRegMsg, ACRX_CMD_MODAL, NULL)

