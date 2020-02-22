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
//----- CMyEditorReactor.cpp : Implementation of CMyEditorReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CMyEditorReactor.h"

//-----------------------------------------------------------------------------
CMyEditorReactor::CMyEditorReactor (const bool autoInitAndRelease) : AcEditorReactor(), mbAutoInitAndRelease(autoInitAndRelease) {
	if ( autoInitAndRelease ) {
		if ( acedEditor )
			acedEditor->addReactor (this) ;
		else
			mbAutoInitAndRelease =false ;
	}
}

//-----------------------------------------------------------------------------
CMyEditorReactor::~CMyEditorReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CMyEditorReactor::Attach () {
	Detach () ;
	if ( !mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->addReactor (this) ;
			mbAutoInitAndRelease =true ;
		}
	}
}

void CMyEditorReactor::Detach () {
	if ( mbAutoInitAndRelease ) {
		if ( acedEditor ) {
			acedEditor->removeReactor (this) ;
			mbAutoInitAndRelease =false ;
		}
	}
}

AcEditor *CMyEditorReactor::Subject () const {
	return (acedEditor) ;
}

bool CMyEditorReactor::IsAttached () const {
	return (mbAutoInitAndRelease) ;
}

void CMyEditorReactor::commandWillStart(const ACHAR* cmdStr)
{

	acutPrintf(TEXT("\n%s命令将要开始执行."), cmdStr);

}

void CMyEditorReactor::commandEnded(const TCHAR * cmdStr)
{

	acutPrintf(L"\n%s命令执行结束.", cmdStr);

}
