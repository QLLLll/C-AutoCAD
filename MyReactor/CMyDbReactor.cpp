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
//----- CMyDbReactor.cpp : Implementation of CMyDbReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "CMyDbReactor.h"

//-----------------------------------------------------------------------------
ACRX_CONS_DEFINE_MEMBERS(CMyDbReactor, AcDbDatabaseReactor, 1)

//-----------------------------------------------------------------------------
CMyDbReactor::CMyDbReactor (AcDbDatabase *pDb) : AcDbDatabaseReactor(), mpDatabase(pDb) {
	if ( pDb )
		pDb->addReactor (this) ;
}

//-----------------------------------------------------------------------------
CMyDbReactor::~CMyDbReactor () {
	Detach () ;
}

//-----------------------------------------------------------------------------
void CMyDbReactor::Attach (AcDbDatabase *pDb) {
	Detach () ;
	if ( mpDatabase == NULL ) {
		if ( (mpDatabase =pDb) != NULL )
			pDb->addReactor (this) ;
	}
}

void CMyDbReactor::Detach () {
	if ( mpDatabase ) {
		mpDatabase->removeReactor (this) ;
		mpDatabase =NULL ;
	}
}

AcDbDatabase *CMyDbReactor::Subject () const {
	return (mpDatabase) ;
}

bool CMyDbReactor::IsAttached () const {
	return (mpDatabase != NULL) ;
}

void CMyDbReactor::objectAppended(const AcDbDatabase * dwg, 
	const AcDbObject * dbObj)
{
	if (dbObj->isKindOf(AcDbEntity::desc()) && const_cast<AcDbObject*>(dbObj)
		->upgradeOpen() == Acad::eOk) {

		AcDbEntity * pEnt = AcDbEntity::cast(dbObj);
		pEnt->setColorIndex(1);

	}


}
