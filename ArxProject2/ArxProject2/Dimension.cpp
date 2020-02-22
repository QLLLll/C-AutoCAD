#include "stdafx.h"
#include "Dimension.h"
#include"DwgDataBaseUtil.h"
#include"GePointUtil.h"
#include"MathUtil.h"

CDimension::CDimension()
{
}


CDimension::~CDimension()
{
}

AcDbObjectId CDimension::AddDimAligned(const AcGePoint3d & pt1, const AcGePoint3d & pt2, const AcGePoint3d & ptLine, ACHAR * text, AcDbObjectId dimStyle)
{
	AcDbAlignedDimension *dim = new AcDbAlignedDimension(pt1, pt2, ptLine, text, dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(dim);
}

AcDbObjectId CDimension::AddDimAligned(const AcGePoint3d & pt1, const AcGePoint3d & pt2, const AcGePoint3d & ptLine, AcGeVector3d textOffset,ACHAR * text, AcDbObjectId dimStyle)
{
	AcDbAlignedDimension *dim = new AcDbAlignedDimension(pt1, pt2, ptLine, text, dimStyle);

	AcDbAlignedDimension *dimOpen = NULL;

	AcDbObjectId dimId = CDwgDataBaseUtil::PostToModelSpace(dimOpen);

	if (acdbOpenObject(dimOpen, dimId, AcDb::OpenMode::kForWrite) == Acad::eOk) {

		dimOpen->setDimtmove(1);

		AcGePoint3d ptText = dimOpen->textPosition();

		ptText = ptText + textOffset;

		dimOpen->setTextPosition(ptText);

		dimOpen->close();

	}
	return dimId;

}

AcDbObjectId CDimension::AddDimRotated(const double rotation, const AcGePoint3d & pt1, const AcGePoint3d & pt2, const AcGePoint3d & ptLine, ACHAR * text, AcDbObjectId dimStyle)
{
	AcDbRotatedDimension *dim = new AcDbRotatedDimension(rotation,pt1, pt2, ptLine, text, dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(dim);
}

AcDbObjectId CDimension::AddDimRadial(const AcGePoint3d & center, const AcGePoint3d & chordPoint, double leaderLength, const ACHAR * dimText, AcDbObjectId dimStyle)
{
	AcDbRadialDimension *dim = new AcDbRadialDimension(center, chordPoint, leaderLength, dimText, dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(dim);
}

AcDbObjectId CDimension::AddDimRadial(const AcGePoint3d & center, double angle, double radius, double leaderLength, const ACHAR * dimText, AcDbObjectId dimStyle)
{
	AcGePoint3d chordPoint=CGePointUtil::PolarPoint(center,angle,radius);

	AcDbRadialDimension *dim = new AcDbRadialDimension(center, chordPoint, leaderLength, dimText, dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(dim);
}

AcDbObjectId CDimension::AddDimDiametric(const AcGePoint3d & chordPoint, const AcGePoint3d & farChordPoint, double leaderLength, const ACHAR * dimText, AcDbObjectId dimStyle)
{
	AcDbDiametricDimension *dim = new AcDbDiametricDimension(chordPoint, farChordPoint, leaderLength, dimText, dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(dim);
}

AcDbObjectId CDimension::AddDimDiametric(const AcGePoint3d & centerPt, double angle, double radius, double leaderLength, const ACHAR * dimText, AcDbObjectId dimStyle)
{
	AcGePoint3d chordPoint;
	AcGePoint3d farChordPoint;

	chordPoint = CGePointUtil::PolarPoint(centerPt, angle, radius);
	farChordPoint= CGePointUtil::PolarPoint(centerPt, angle+CMathUtil::PI(), radius);
	AcDbDiametricDimension *dim = new AcDbDiametricDimension(chordPoint, farChordPoint, leaderLength, dimText, dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(dim);
}

AcDbObjectId CDimension::AddDim2LineAngular(const AcGePoint3d& ptStart1, const AcGePoint3d& ptEnd1,
	const AcGePoint3d& ptStart2, const AcGePoint3d& ptEnd2,
	const AcGePoint3d& ptArc, const TCHAR* dimText,
	AcDbObjectId dimStyle)
{
	AcDb2LineAngularDimension *pDim = new AcDb2LineAngularDimension(
		ptStart1, ptEnd1, ptStart2, ptEnd2, ptArc, dimText, dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(pDim);
}

AcDbObjectId CDimension::AddDim3PtAngular(const AcGePoint3d& ptCenter, const AcGePoint3d& ptEnd1,
	const AcGePoint3d& ptEnd2, const AcGePoint3d& ptArc,
	const TCHAR* dimText, AcDbObjectId dimStyle)
{
	AcDb3PointAngularDimension *pDim = new AcDb3PointAngularDimension(
		ptCenter, ptEnd1, ptEnd2, ptArc, dimText, dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(pDim);
}

AcDbObjectId CDimension::AddDimOrdinate(Adesk::Boolean xAxis, const AcGePoint3d& ptStart,
	const AcGePoint3d& ptEnd, const TCHAR* dimText, AcDbObjectId dimStyle)
{
	AcDbOrdinateDimension *pDim = new AcDbOrdinateDimension(xAxis,
		ptStart, ptEnd, dimText, dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(pDim);
}

AcDbObjectIdArray CDimension::AddDimOrdinate(const AcGePoint3d& ptDef, const AcGePoint3d& ptTextX,
	const AcGePoint3d& ptTextY)
{
	AcDbObjectId dimId;
	AcDbObjectIdArray dimIds;

	dimId = AddDimOrdinate(Adesk::kTrue, ptDef, ptTextX);
	dimIds.append(dimId);
	dimId = AddDimOrdinate(Adesk::kFalse, ptDef, ptTextY);
	dimIds.append(dimId);

	return dimIds;
}

AcDbObjectIdArray CDimension::AddDimOrdinate(const AcGePoint3d& ptDef, const AcGeVector3d& vecOffsetX,
	const AcGeVector3d& vecOffsetY)
{
	AcGePoint3d ptTextX = ptDef + vecOffsetX;
	AcGePoint3d ptTextY = ptDef + vecOffsetY;

	return AddDimOrdinate(ptDef, ptTextX, ptTextY);
}

AcDbObjectId CDimension::AddDim3PointAngular(const AcGePoint3d & centerPoint, const AcGePoint3d & xLine1Point, const AcGePoint3d & xLine2Point, const AcGePoint3d & arcPoint, const ACHAR * dimText, AcDbObjectId dimStyle)
{
	AcDb3PointAngularDimension *dim=new AcDb3PointAngularDimension(centerPoint,xLine1Point,xLine2Point,arcPoint,dimText,dimStyle);

	return CDwgDataBaseUtil::PostToModelSpace(dim);

}
