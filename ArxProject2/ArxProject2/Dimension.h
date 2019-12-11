#pragma once
class CDimension
{
public:
	CDimension();
	~CDimension();
	//对齐标注
	static AcDbObjectId CDimension::AddDimAligned(const AcGePoint3d &pt1, const AcGePoint3d &pt2,
		const AcGePoint3d &ptLine, ACHAR* text, AcDbObjectId dimStyle = AcDbObjectId::kNull);

	static AcDbObjectId CDimension::AddDimAligned(const AcGePoint3d &pt1, const AcGePoint3d &pt2,
		const AcGePoint3d &ptLine, AcGeVector3d textOffset=AcGeVector3d::kIdentity, ACHAR* text=NULL, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//转角标注
	static AcDbObjectId CDimension::AddDimRotated(
		const double rotation,const AcGePoint3d &pt1, const AcGePoint3d &pt2,
		const AcGePoint3d &ptLine, ACHAR* text, AcDbObjectId dimStyle = AcDbObjectId::kNull);
	//半径标注
	static AcDbObjectId AddDimRadial(const AcGePoint3d& center,
		const AcGePoint3d& chordPoint,
		double leaderLength,
		const ACHAR * dimText = NULL,
		AcDbObjectId dimStyle = AcDbObjectId::kNull);

	static AcDbObjectId AddDimRadial(const AcGePoint3d& center,
		double angle,
		double radius,
		double leaderLength=5,//引线长度
		const ACHAR * dimText = NULL,
		AcDbObjectId dimStyle = AcDbObjectId::kNull);

	/*
	*直径标注
	*/
static AcDbObjectId AddDimDiametric(
	const AcGePoint3d& chordPoint,
	const AcGePoint3d& farChordPoint,
	double leaderLength,
	const ACHAR * dimText = NULL,
	AcDbObjectId dimStyle = AcDbObjectId::kNull
	);
static AcDbObjectId AddDimDiametric(
	const AcGePoint3d& centerPt,
	double angle,double radius,
	double leaderLength,
	const ACHAR * dimText = NULL,
	AcDbObjectId dimStyle = AcDbObjectId::kNull
	);

// 角度标注
static AcDbObjectId AddDim2LineAngular(const AcGePoint3d& ptStart1,
	const AcGePoint3d& ptEnd1, const AcGePoint3d& ptStart2,
	const AcGePoint3d& ptEnd2, const AcGePoint3d& ptArc,
	const TCHAR* dimText = NULL,
	AcDbObjectId dimStyle = AcDbObjectId::kNull);
static AcDbObjectId AddDim3PtAngular(const AcGePoint3d& ptCenter,
	const AcGePoint3d& ptEnd1, const AcGePoint3d& ptEnd2,
	const AcGePoint3d& ptArc, const TCHAR* dimText = NULL,
	AcDbObjectId dimStyle = AcDbObjectId::kNull);

// 坐标标注
static AcDbObjectId AddDimOrdinate(Adesk::Boolean xAxis,
	const AcGePoint3d& ptStart, const AcGePoint3d& ptEnd,
	const TCHAR* dimText = NULL,
	AcDbObjectId dimStyle = AcDbObjectId::kNull);
static AcDbObjectIdArray AddDimOrdinate(const AcGePoint3d& ptDef,
	const AcGePoint3d& ptTextX, const AcGePoint3d& ptTextY);
static AcDbObjectIdArray AddDimOrdinate(const AcGePoint3d& ptDef,
	const AcGeVector3d& vecOffsetX, const AcGeVector3d& vecOffsetY);

};

