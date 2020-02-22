// (C) Copyright 2002-2007 by Autodesk, Inc. 
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
//----- CMyDocReactor.cpp : Implementation of CMyDocReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CMyDocReactor.h"
#include"CMyDbReactor.h"

extern CMyDbReactor *g_myDbReactor;

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(CMyDocReactor, AcApDocManagerReactor, 1)

//-----------------------------------------------------------------------------
CMyDocReactor::CMyDocReactor (const bool autoInitAndRelease) : AcApDocManagerReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if ( autoInitAndRelease ) {
		if ( acDocManager )
			acDocManager->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
}

//-----------------------------------------------------------------------------
CMyDocReactor::~CMyDocReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CMyDocReactor::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acDocManager ) {
			acDocManager->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void CMyDocReactor::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acDocManager ) {
			acDocManager->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcApDocManager *CMyDocReactor::Subject () const {
	return (acDocManager) ;
}

bool CMyDocReactor::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}

void CMyDocReactor::documentLockModeChanged(AcApDocument * pDoc,
	AcAp::DocLockMode myPreviousMode, 
	AcAp::DocLockMode myCurrentMode,
	AcAp::DocLockMode currentMode, 
	const ACHAR * pGlobalCmdName)
{
	USES_CONVERSION;
	char* cmdName = W2A(pGlobalCmdName);
	
	//wcscmp()
	if (strcmp(cmdName, "LINE") == 0) {

		acutPrintf(L"\nLine命令被当前软件禁止.");
		veto();
	}
}

void CMyDocReactor::documentToBeDestroyed(AcApDocument *pDocToDestroy) {

	pDocToDestroy->database()->removeReactor(g_myDbReactor);

}
void CMyDocReactor::documentCreated(AcApDocument * pDocCreating) {

	pDocCreating->database()->addReactor(g_myDbReactor);

}