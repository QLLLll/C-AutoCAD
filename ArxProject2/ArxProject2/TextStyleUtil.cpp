#include "stdafx.h"
#include "TextStyleUtil.h"


CTextStyleUtil::CTextStyleUtil()
{
}


CTextStyleUtil::~CTextStyleUtil()
{
}

AcDbObjectId CTextStyleUtil::Add(const TCHAR * name, const TCHAR * fontFileName, const TCHAR * bigFontFileName)
{
	

	Acad::ErrorStatus es;

	AcDbTextStyleTable *pTextStyleTable = NULL;
	
	es = acdbHostApplicationServices()->workingDatabase()
		->getTextStyleTable(pTextStyleTable, AcDb::OpenMode::kForRead);
	
	

	if (es != ErrorStatus::eOk)
	{
		acutPrintf(L"\n打開文字樣式表失敗");
		return AcDbObjectId::kNull;
	}

	AcDbTextStyleTableRecord *pTextStyleRecord = new AcDbTextStyleTableRecord();
	es = pTextStyleRecord->setName(name);
	es = pTextStyleRecord->setBigFontFileName(bigFontFileName);
	es = pTextStyleRecord->setFileName(fontFileName);
	es = pTextStyleRecord->setXScale(1.0);

	AcDbObjectId styleId;

	pTextStyleTable->upgradeOpen();

	es = pTextStyleTable->add(styleId, pTextStyleRecord);

	pTextStyleRecord->close();
	pTextStyleTable->close();

	return styleId;



}

AcDbObjectId CTextStyleUtil::GetAt(const TCHAR * name)
{
	assert(name != NULL);

	AcDbTextStyleTable *pTextTable = NULL;

	Acad::ErrorStatus es;

	es = acdbHostApplicationServices()->workingDatabase()->
		getTextStyleTable(pTextTable, AcDb::OpenMode::kForRead);

	if (es != ErrorStatus::eOk) {
		acutPrintf(L"未能打開文字樣式表");
		return AcDbObjectId::kNull;
	}

	AcDbObjectId styleId;

	if (pTextTable->has(name)) {

		pTextTable->getAt(name, styleId);
		pTextTable->close();
		return styleId;

	}
	
	else {
		pTextTable->close();
		return AcDbObjectId::kNull;
	}
}

void CTextStyleUtil::GetAll(std::vector<CString>& textStyles)
{
	AcDbTextStyleTable *pTextTable = NULL;

	Acad::ErrorStatus es;

	es = acdbHostApplicationServices()->workingDatabase()->
		getTextStyleTable(pTextTable, AcDb::OpenMode::kForRead);

	if (es != ErrorStatus::eOk) {
		acutPrintf(L"未能打開文字樣式表");
		return;
	}

	AcDbTextStyleTableIterator *pIter = NULL;

	pTextTable->newIterator(pIter);

	for (pIter->start(); !pIter->done(); pIter->step())
	{
		AcDbTextStyleTableRecord *textTableRec = NULL;

		pIter->getRecord(textTableRec);

		TCHAR*szName = NULL;
		textTableRec->getName(szName);


		if (_tcsclen(szName) > 0)
		{
			textStyles.push_back(szName);
		}

		acutDelString(szName);

		textTableRec->close();

	}
	delete pIter;
	pTextTable->close();
}
