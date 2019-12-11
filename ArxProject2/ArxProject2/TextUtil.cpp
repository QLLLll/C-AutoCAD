#include "stdafx.h"
#include "TextUtil.h"
#include "DwgDataBaseUtil.h"

CTextUtil::CTextUtil()
{
}


CTextUtil::~CTextUtil()
{
}

AcDbObjectId CTextUtil::AddText(AcGePoint3d & ptInsert, const wchar_t * text, AcDbObjectId style, double height, double rotation)
{
	AcDbText* t= new AcDbText(ptInsert, text, style, height, rotation);

	return CDwgDataBaseUtil::PostToModelSpace(t);
}

AcDbObjectId CTextUtil::AddMText(AcGePoint3d & ptInsert, wchar_t * text, AcDbObjectId style, double height, double width)
{
	AcDbMText *mt = new AcDbMText();
	mt->setContents(text);
	mt->setTextHeight(height);
	mt->setWidth( width);
	mt->setLocation(ptInsert);
	mt->setDatabaseDefaults();

	return CDwgDataBaseUtil::PostToModelSpace(mt);
}
