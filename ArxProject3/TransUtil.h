#pragma once
class CTransUtil
{
public:
	CTransUtil();
	~CTransUtil();

	static AcDbObjectId GetId(const ACHAR *tips=TEXT("—°‘Ò µÃÂ"));

	static AcDbObjectId MyClone(AcDbObjectId objId);

	static void Move(const AcGePoint3d &basePoint, const AcGePoint3d &movePoint, AcDbObjectId objId);

	static void Rotate(const AcGePoint3d &basePoint, const double rotataion, AcDbObjectId objId);

	static void Scale(AcDbObjectId objId,double scale, AcGePoint3d &ptBase);
};

