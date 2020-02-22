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
//----- ECDDwgScaleCO.cpp : Implementation of ECDDwgScaleCO
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "ECDDwgScaleCO.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 ECDDwgScaleCO::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	ECDDwgScaleCO, AcDbObject,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, ECDDWGSCALECO,
ECDCUSTOMOBJDBXAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
ECDDwgScaleCO::ECDDwgScaleCO () : AcDbObject () {

	m_infoScale = 0;
	m_lableScale = 0;

}

ECDDwgScaleCO::~ECDDwgScaleCO () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus ECDDwgScaleCO::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (ECDDwgScaleCO::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	pFiler->writeItem(m_infoScale);
	pFiler->writeItem(m_lableScale);


	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus ECDDwgScaleCO::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbObject::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > ECDDwgScaleCO::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ECDDwgScaleCO::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	switch (version)
	{
	case 1:
		pFiler->readItem(&m_infoScale);
		pFiler->readItem(&m_lableScale);
		break;
	}
	return (pFiler->filerStatus ()) ;
}

//- Dxf Filing protocol
Acad::ErrorStatus ECDDwgScaleCO::dxfOutFields (AcDbDxfFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbObject::dxfOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	es =pFiler->writeItem (AcDb::kDxfSubclass, _RXST("ECDDwgScaleCO")) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (kDxfInt32, ECDDwgScaleCO::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....
	pFiler->writeItem(AcDb::kDxfInt32, m_infoScale);
	pFiler->writeItem(AcDb::kDxfInt32, m_lableScale);


	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus ECDDwgScaleCO::dxfInFields (AcDbDxfFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbObject::dxfInFields (pFiler) ;
	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("ECDDwgScaleCO")) )
		return (pFiler->filerStatus ()) ;
	//----- Object version number needs to be read first
	struct resbuf rb ;
	pFiler->readItem (&rb) ;
	if ( rb.restype != AcDb::kDxfInt32 ) {
		pFiler->pushBackItem () ;
		pFiler->setError (Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32) ;
		return (pFiler->filerStatus ()) ;
	}
	Adesk::UInt32 version =(Adesk::UInt32)rb.resval.rlong ;
	if ( version > ECDDwgScaleCO::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ECDDwgScaleCO::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	/*while ( es == Acad::eOk && (es =pFiler->readResBuf (&rb)) == Acad::eOk ) {
		switch ( rb.restype ) {
			//----- Read params by looking at their DXF code (example below)
			//case AcDb::kDxfXCoord:
			//	if ( version == 1 )
			//		cen3d =asPnt3d (rb.resval.rpoint) ;
			//	else 
			//		cen2d =asPnt2d (rb.resval.rpoint) ;
			//	break ;
			//.....

			default:
				//----- An unrecognized group. Push it back so that the subclass can read it again.
				pFiler->pushBackItem () ;
				es =Acad::eEndOfFile ;
				break ;
		}
	}*/
	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {

		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode,
			TEXT("\nError:expected object version group code %d"),
			AcDb::kDxfInt32);
		return pFiler->filerStatus();
	}
	else {
		m_infoScale = rb.resval.rlong;
	}

	pFiler->readItem(&rb);
	if (rb.restype != AcDb::kDxfInt32) {

		pFiler->pushBackItem();
		pFiler->setError(Acad::eInvalidDxfCode,
			TEXT("\nError:expected object version group code %d"),
			AcDb::kDxfInt32);
		return pFiler->filerStatus();
	}
	else {
		m_lableScale = rb.resval.rlong;
	}


	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	/*if ( es != Acad::eEndOfFile )
		return (Acad::eInvalidResBuf) ;*/

	return (pFiler->filerStatus ()) ;
}

void ECDDwgScaleCO::Set(int infoScale, int lableScale)
{

	assertWriteEnabled();
	m_infoScale = infoScale;
	m_lableScale = lableScale;

}

int ECDDwgScaleCO::GetInfoScale() const
{
	assertReadEnabled();

	return m_infoScale;
}

int ECDDwgScaleCO::GetLableScale() const
{
	assertReadEnabled();
	return m_lableScale;

}


