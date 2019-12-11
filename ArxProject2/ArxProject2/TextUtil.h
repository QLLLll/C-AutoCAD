#pragma once
class CTextUtil
{
public:
	CTextUtil();
	~CTextUtil();
	static AcDbObjectId CTextUtil::AddText(AcGePoint3d & ptInsert, const wchar_t *text,
	AcDbObjectId style=AcDbObjectId::kNull,double height=2.5,double rotation=0.0);

	static AcDbObjectId CTextUtil::AddMText(AcGePoint3d & ptInsert, wchar_t *text,
		AcDbObjectId style = AcDbObjectId::kNull, double height = 2.5, double width = 10);
};

