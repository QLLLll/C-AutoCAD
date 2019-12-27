#include "StdAfx.h"
#include "ECDTriangleObj.h"

//-----------------------------------------------------------------------------
Adesk::UInt32 ECDTriangleObj::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	ECDTriangleObj, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, ECDTRIANGLEOBJ,
ECDCUSTOMOBJDBXAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
ECDTriangleObj::ECDTriangleObj () : AcDbEntity () {
}

ECDTriangleObj::ECDTriangleObj(AcGePoint3d pt1, AcGePoint3d pt2, AcGePoint3d pt3)
{
	m_verts[0] = pt1;
	m_verts[1] = pt2;
	m_verts[2] = pt3;

}

ECDTriangleObj::~ECDTriangleObj () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus ECDTriangleObj::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (ECDTriangleObj::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	for (int i = 0; i < 3; i++)
	{

		pFiler->writeItem(m_verts[i]);

	}

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus ECDTriangleObj::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > ECDTriangleObj::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < ECDTriangleObj::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	for (int i = 0; i < 3; i++)
	{

		pFiler->readItem(&m_verts[i]);

	}

	return (pFiler->filerStatus ()) ;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean ECDTriangleObj::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;
	for (int i = 0; i < 3; i++) {

		int nextIndex = i + 1;
		if (i == 2) {
			nextIndex = 0;
		}

		AcGePoint3d points[2];
		points[0] = m_verts[i];
		points[1] = m_verts[nextIndex];

		mode->geometry().polyline(2, points);

	}
	AcDbFace *pFace = GetFace();
	pFace->worldDraw(mode);
	delete pFace;

	return Adesk::kTrue;

}


Adesk::UInt32 ECDTriangleObj::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	return (AcDbEntity::subSetAttributes (traits)) ;
}

	//- Osnap points protocol
Acad::ErrorStatus ECDTriangleObj::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds)) ;
}

Acad::ErrorStatus ECDTriangleObj::subGetOsnapPoints (
	AcDb::OsnapMode osnapMode,
	Adesk::GsMarker gsSelectionMark,
	const AcGePoint3d &pickPoint,
	const AcGePoint3d &lastPoint,
	const AcGeMatrix3d &viewXform,
	AcGePoint3dArray &snapPoints,
	AcDbIntArray &geomIds,
	const AcGeMatrix3d &insertionMat) const
{
	assertReadEnabled () ;
	return (AcDbEntity::subGetOsnapPoints (osnapMode, gsSelectionMark, pickPoint, lastPoint, viewXform, snapPoints, geomIds, insertionMat)) ;
}

//- Grip points protocol
Acad::ErrorStatus ECDTriangleObj::subGetGripPoints (
	AcGePoint3dArray &gripPoints, AcDbIntArray &osnapModes, AcDbIntArray &geomIds
) const {
	assertReadEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new getGripPoints() method below (which is the default implementation)

	return (AcDbEntity::subGetGripPoints (gripPoints, osnapModes, geomIds)) ;
}

Acad::ErrorStatus ECDTriangleObj::subMoveGripPointsAt (const AcDbIntArray &indices, const AcGeVector3d &offset) {
	assertWriteEnabled () ;
	//----- This method is never called unless you return eNotImplemented 
	//----- from the new moveGripPointsAt() method below (which is the default implementation)

	return (AcDbEntity::subMoveGripPointsAt (indices, offset)) ;
}

Acad::ErrorStatus ECDTriangleObj::subGetGripPoints (
	AcDbGripDataPtrArray &grips, const double curViewUnitSize, const int gripSize, 
	const AcGeVector3d &curViewDir, const int bitflags
) const {
	assertReadEnabled () ;

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subGetGripPoints (grips, curViewUnitSize, gripSize, curViewDir, bitflags)) ;
}

Acad::ErrorStatus ECDTriangleObj::subMoveGripPointsAt (
	const AcDbVoidPtrArray &gripAppData, const AcGeVector3d &offset,
	const int bitflags
) {
	assertWriteEnabled () ;

	//----- If you return eNotImplemented here, that will force AutoCAD to call
	//----- the older getGripPoints() implementation. The call below may return
	//----- eNotImplemented depending of your base class.
	return (AcDbEntity::subMoveGripPointsAt (gripAppData, offset, bitflags)) ;
}

AcDbFace * ECDTriangleObj::GetFace()
{
	AcDbFace *pFace = new AcDbFace(m_verts[0], m_verts[1], m_verts[2]);
	pFace->setColorIndex(this->colorIndex());
	return pFace;
}

