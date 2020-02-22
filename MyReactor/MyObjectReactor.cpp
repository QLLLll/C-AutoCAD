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
//----- MyObjectReactor.cpp : Implementation of CMyObjectReactor
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "MyObjectReactor.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 CMyObjectReactor::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	CMyObjectReactor, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, MYOBJECTREACTOR,
ECDMYREACTORAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
CMyObjectReactor::CMyObjectReactor () : AcDbObject () {
}

CMyObjectReactor::~CMyObjectReactor () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CMyObjectReactor::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (CMyObjectReactor::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	pFiler->writeItem((AcDbSoftPointerId&)m_entId);

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CMyObjectReactor::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > CMyObjectReactor::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CMyObjectReactor::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	pFiler->readItem((AcDbSoftPointerId*)&m_entId);

	return (pFiler->filerStatus ()) ;
}

void CMyObjectReactor::modified(const AcDbObject * dbObj)
{

	AcDbCircle *pCircle = AcDbCircle::cast(dbObj);

	if (pCircle != NULL) {

		double radius = pCircle->radius();

		AcDbCircle *pLinkCircle = NULL;
		if (acdbOpenObject(pLinkCircle, m_entId, AcDb::kForWrite) == Acad::eOk) {

			pLinkCircle->setRadius(radius);

			pLinkCircle->close();

		}


	}

}

void CMyObjectReactor::erased(const AcDbObject * dbObj, Adesk::Boolean bErasing)
{

	AcDbCircle *pCircle = AcDbCircle::cast(dbObj);

	if (pCircle != NULL&&bErasing) {

		AcDbCircle *pLinkCircle = NULL;
		if (acdbOpenObject(pLinkCircle, m_entId, AcDb::kForWrite) == Acad::eOk) {

			pLinkCircle->erase();
			pLinkCircle->close();

		}
	}
}

void CMyObjectReactor::SetLinkEntId(AcDbObjectId entId)
{
	assertWriteEnabled();

	m_entId = entId;

}

