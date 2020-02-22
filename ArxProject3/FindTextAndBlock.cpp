#include "stdafx.h"
#include "FindTextAndBlock.h"
#include"StringUtil.h"
#include<vector>
#include"DwgDataBaseUtil.h"
using namespace std;
CFindTextAndBlock::~CFindTextAndBlock()
{
}

void CFindTextAndBlock::Command()
{
	getLayer();

	vector<AcDbBlockReference*> vecBr;
	vector<AcDbText*>vecText;
	vector<AcDbMText*>vecMText;

	//遍历整个图

	AcDbBlockTable *pTable = NULL;

	ErrorStatus es;

	es = curDoc()->database()->getSymbolTable(pTable, AcDb::OpenMode::kForRead);

	if (es != Acad::eOk) {
		acutPrintf(L"\nerror=%d\n", es);
		return;
	}
	AcDbBlockTableRecord *beFindRec = NULL;

	if(blockName!=NULL||type==2)
	es = pTable->getAt(blockName, beFindRec);
	
	
	AcDbBlockTableIterator *iter=NULL;
	es = pTable->newIterator(iter);

	if (es != Acad::eOk) {
		acutPrintf(L"\nerror=%d\n", es);
		return;
	}
	pTable->close();

	AcDbObjectIdArray brIdArr;
	if (beFindRec != NULL) {
		beFindRec->getBlockReferenceIds(brIdArr);

		for each (AcDbObjectId id in brIdArr)
		{
			AcDbBlockReference *ref = NULL;

			es = acdbOpenObject(ref, id, AcDb::OpenMode::kForRead);

			if (isContainsLayer(ref->layer())) {

				vecBr.push_back(ref);

			}
			else {
				ref->close();
			}

		}
	}
	for (iter->start(); !iter->done(); iter->step())
	{
		AcDbBlockTableRecord *pRec = NULL;
		es=iter->getRecord(pRec, AcDb::kForRead);
		if (textName != NULL||type==1) {
			AcDbBlockTableRecordIterator * recIter = NULL;
			pRec->newIterator(recIter);
			
			for (recIter->start(); !recIter->done(); recIter->step())
			{
				AcDbEntity* pEnt = NULL;
				es = recIter->getEntity(pEnt);

				if (es == Acad::eOk&&pEnt != NULL&&pEnt->isKindOf(AcDbText::desc())) {

					AcDbText *text = AcDbText::cast(pEnt);

					if (wcscmp(text->textString(), textName) == 0) {

						vecText.push_back(text);
					}					
				}
				else if (es == Acad::eOk&&pEnt != NULL &&pEnt->isKindOf(AcDbMText::desc())) {


					AcDbMText *text = AcDbMText::cast(pEnt);

					if (wcscmp(text->text(), textName) == 0) {

						vecMText.push_back(text);
					}

				}
				else {
					pEnt->close();
				}

			}
			if (recIter != NULL) {
				delete recIter;
			}
		}
		pRec->close();
	}
	if (iter != NULL) {
		delete iter;
	}
	if (beFindRec != NULL) {
		beFindRec->close();
	}

	AcDbLayerTable * layerTable = NULL;

	es = curDoc()->database()->getLayerTable(layerTable, AcDb::kForWrite);


	AcDbObjectId l1, l2;

	
	if (!layerTable->has(L"Find_文字位置"))
	{
		AcDbLayerTableRecord *lRec = new AcDbLayerTableRecord();
		lRec->setName(L"Find_文字位置");
		AcCmColor colorYellow = AcCmColor();
		colorYellow.setColorIndex(2);
		lRec->setColor(colorYellow);
		layerTable->add(l1, lRec);

		AcDbLayerTableRecord *lRec2 = new AcDbLayerTableRecord();

		lRec2->setName(L"Find_块位置");
		AcCmColor colorBlue = AcCmColor();
		colorBlue.setColorIndex(3);
		lRec2->setColor(colorBlue);

		layerTable->add(l2, lRec2);

		lRec->close();
		lRec2->close();

		
	}
	layerTable->close();

	if (vecBr.size() > 0) {

		for (int i = 0; i < (int)vecBr.size(); i++)
		{
			AcGePoint3d ptPos = vecBr[i]->position();

			AcGePoint3d ptZero(ptPos.x, 0, 0);

			AcDbLine *line = new AcDbLine(ptZero, ptPos);

			line->setLayer(l2);

			line->setColorIndex(3);

			CDwgDataBaseUtil::PostToModelSpace(line);

			line->close();

			vecBr[i]->close();

		}
	}

	if (vecText.size() > 0) {

		for (int i = 0; i < (int)vecText.size(); i++)
		{
			AcGePoint3d ptPos = vecText[i]->position();

			AcGePoint3d ptZero(ptPos.x, 0, 0);

			AcDbLine *line = new AcDbLine(ptZero, ptPos);

			line->setLayer(l1);

			line->setColorIndex(2);

			CDwgDataBaseUtil::PostToModelSpace(line);

			line->close();

			vecText[i]->close();

		}
	}
	if (vecMText.size() > 0) {

		for (int i = 0; i < (int)vecMText.size(); i++)
		{
			AcGePoint3d ptPos = vecMText[i]->location();

			AcGePoint3d ptZero(ptPos.x, 0, 0);

			AcDbLine *line = new AcDbLine(ptZero, ptPos);

			line->setLayer(l1);

			line->setColorIndex(2);

			CDwgDataBaseUtil::PostToModelSpace(line);

			line->close();

			vecMText[i]->close();

		}
	}


}
void CFindTextAndBlock::getLayer()
{
//W2T()
	CString str(layerName);

	CStringUtil::Split(str, L" ", vecLayer1, false);

	for (int i = 0; i < (int)vecLayer1.size(); i++)
	{
		CString l = vecLayer1.at(i);

		vecLayer.push_back(l.AllocSysString());

	}

}

bool CFindTextAndBlock::isContainsLayer(ACHAR * lName)
{
	for (int i = 0; i < (int)vecLayer.size(); i++)
	{
		if (wcscmp(lName, vecLayer[i]) == 0) {

			return true;
		}
	}
	return false;
}