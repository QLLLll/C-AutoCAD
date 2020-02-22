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
#include"CMyEditorReactor.h"
#include"CMyDocReactor.h"
#include"CMyDbReactor.h"
#include"MyObjectReactor.h"
#include"SelectUtil.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

CMyEditorReactor * g_myEditorReactor = NULL;
CMyDocReactor *g_myDocReactor = NULL;
CMyDbReactor *g_myDbReactor = NULL;
//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CMyReactorApp : public AcRxArxApp {

public:
	CMyReactorApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		g_myEditorReactor = new CMyEditorReactor();
		acedEditor->addReactor(g_myEditorReactor);
		
		g_myDocReactor = new CMyDocReactor();
		acDocManager->addReactor(g_myDocReactor);

		g_myDbReactor = new CMyDbReactor();

		AcApDocumentIterator *it = acDocManager->newAcApDocumentIterator();

		for (; !it->done(); it->step()) {

			it->document()->database()->addReactor(g_myDbReactor);

		}
		delete it;


		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		acedEditor->removeReactor(g_myEditorReactor);
		delete g_myEditorReactor;

		acDocManager->removeReactor(g_myDocReactor);
		delete g_myDocReactor;

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	

	static void ECDMyGroupMyCommand () {
		
		AcDbObjectIdArray circleIds;

		AcRxClass * cls = AcDbCircle::desc();

		if (CSelectUtil::PromptSelectEnts(TEXT("\n选择两个圆："),NULL, cls, circleIds)) {

			if (circleIds.length() != 2) {

				acutPrintf(L"\n必须选择两个圆.");
				return;

			}

			AcDbDictionary *pNameDict = NULL;
			CMyObjectReactor *pObjReactorA = new CMyObjectReactor();

			pObjReactorA->SetLinkEntId(circleIds[1]);
			AcDbObjectId objReactorAId;

			Acad::ErrorStatus es = acdbHostApplicationServices()->workingDatabase()
				->getNamedObjectsDictionary(pNameDict, AcDb::kForWrite);

			pNameDict->setAt(L"CircleA", pObjReactorA, objReactorAId);

			AcDbCircle * pCircleA = NULL;

			if (acdbOpenObject(pCircleA, circleIds[0], AcDb::kForWrite) == Acad::eOk) {

				pCircleA->addPersistentReactor(objReactorAId);
				pCircleA->close();
			}


			CMyObjectReactor *pObjReactorB = new CMyObjectReactor();

			pObjReactorB->SetLinkEntId(circleIds[0]);
			AcDbObjectId objReactorBId;

			pNameDict->setAt(L"CircleB", pObjReactorB, objReactorBId);

			AcDbCircle * pCircleB = NULL;

			if (acdbOpenObject(pCircleB, circleIds[1], AcDb::kForWrite) == Acad::eOk) {

				pCircleB->addPersistentReactor(objReactorBId);
				pCircleB->close();
			}

			pNameDict->close();
		}

	}


	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CMyReactorApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CMyReactorApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)


