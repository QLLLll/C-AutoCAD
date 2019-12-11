#pragma once
class CSplieUtil
{
public:
	CSplieUtil();
	~CSplieUtil();

	static AcDbObjectId CSplieUtil::Add(const AcGePoint3dArray &points,
		int order, double fitTolerance);

	static AcDbObjectId CSplieUtil::Add(const AcGePoint3dArray &points,
		const AcGeVector3d startTangent, AcGeVector3d endTangent,
		int order, double fitTolerance);
};

