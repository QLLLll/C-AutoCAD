
#include "StdAfx.h"
#include "resource.h"
#include"LineJig.h"
#include<vector>
using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

int colorIndex = 1;

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CEcdZiFuLXApp : public AcRxArxApp {

public: struct PtTxt
{
	AcGePoint3d pt;
	ACHAR * txt;

};

public:
	CEcdZiFuLXApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	static void ECDMyGroupECDZiFuLX() {
		
		AcDbObjectIdArray entIds;
		AcDbObjectIdArray lineIds;

		if (SelectEnts(L"选择文字：\n", NULL, entIds) != true) {
			return;
		}

		ACHAR aStr[133] = {};

		if (acedGetString(0,TEXT("请输入匹配的字符：\n"),aStr) != RTNORM) {
			acutPrintf(L"没有输入文字。\n");
			return;
		}
		vector<AcDbLine*>vecLine;
		AcDbText * tt = NULL;
		for (int i=0;i<entIds.length();i++)
		{
			AcDbText * text = NULL;

			if (acdbOpenObject(text, entIds[i], AcDb::kForRead) != ErrorStatus::eOk) {

				continue;

			}

			ACHAR* txtTemp = text->textString();
			AcGePoint3d pt=text->position();

			if (tt == NULL) {

				tt = AcDbText::cast(text->clone());

			}

			CString str;
			

			if (StrEqual(txtTemp,aStr)) {

				AcDbLine * ll = new AcDbLine(pt, pt);
				ll->setColorIndex(colorIndex);

				lineIds.append(PostToModelSpace(ll));

				ll->close();
				/*vecLine.push_back(ll);*/

			}
			delete txtTemp;
			txtTemp = NULL;

			text->close();

		}

		tt->setTextString(aStr);
		tt->setColorIndex(colorIndex);
		lineIds.append(PostToModelSpace(tt));
		tt->close();
		LineJig lineJig = LineJig();

		lineJig.DoIt(lineIds);
		colorIndex++;

	}

	static bool SelectEnts(ACHAR *prompt, resbuf *rb, AcDbObjectIdArray& entIds) {

		ads_name selName;

		acutPrintf(prompt);

		int rc = acedSSGet(NULL, NULL, NULL, rb, selName);
		acutPrintf(L"rc=%d\n", rc);
		if (rc == RTNORM) {

			int length;

			acedSSLength(selName, &length);

			for (int i = 0; i < length; i++)
			{
				ads_name name;

				acedSSName(selName, i, name);

				AcDbObjectId oId;

				if (acdbGetObjectId(oId, name) == ErrorStatus::eOk) {

					entIds.append(oId);
				}
			}
			acedSSFree(selName);
			return true;
		}
		else {
			acedSSFree(selName);
			return false;
		}

	}
	static AcDbObjectId PostToModelSpace(AcDbEntity *pEnt, AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase()) {

		if (pEnt == NULL) {
			return AcDbObjectId::kNull;
		}
		AcDbBlockTable *pTable;

		AcDbBlockTableRecord *blkTblRec;

		AcDbObjectId objId;

		pDb->getBlockTable(pTable, AcDb::OpenMode::kForRead);
		pTable->getAt(ACDB_MODEL_SPACE, blkTblRec, AcDb::OpenMode::kForWrite);

		pTable->close();

		Acad::ErrorStatus status = blkTblRec->appendAcDbEntity(objId, pEnt);

		if (status != Acad::eOk) {

			blkTblRec->close();

			delete pEnt;
			pEnt = NULL;

			return AcDbObjectId::kNull;

		}

		blkTblRec->close();
		pEnt->close();

		return objId;
	}
	static bool StrEqual(ACHAR* txtTemp, ACHAR* aStr) {
		CString str1(txtTemp);
		CString str2(aStr);

		wchar_t removeStr = '*';

		str2.Remove(removeStr);

		if (str1.Find(str2, 0) != -1) {
			return true;
		}
		else {
			return false;
		}


	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CEcdZiFuLXApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CEcdZiFuLXApp, ECDMyGroup, ECDZiFuLX, ECDZiFuLX, ACRX_CMD_MODAL, NULL)


