//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"SelectUtil.h"
#include"DwgDataBaseUtil.h"
#include"MathUtil.h"
#include"GetInputUtil.h"
#include"ConvertUtil.h"
#include<vector>
using namespace std;
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")


//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CTongJiAndLieBiaoApp : public AcRxArxApp {

	struct blockInfo {

		ACHAR *blockName;
		int blockNum;
		AcDbObjectId blockId;

	};

public:
	CTongJiAndLieBiaoApp () : AcRxArxApp () {}

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
	
	static void ECDMyGroupMyKuaiTongJI () {
		
		AcDbTable *table = new AcDbTable();
		AcGePoint3d insertPt;

		table->insertRows(0, 50, 1);
		table->insertColumns(0, 200,3);
		table->setWidth(600);


		table->setValue(0, 0, L"图块数量统计");
	
		AcCmColor *colorRed = new AcCmColor();
		colorRed->setColorIndex(1);
		
		AcCmColor *colorYellow = new AcCmColor();
		colorYellow->setColorIndex(2);

		AcCmColor *colorGreen = new AcCmColor();
		colorGreen->setColorIndex(4);

		table->setContentColor(0, 0, *colorRed);
		table->setAlignment(0, 0, AcDb::CellAlignment::kMiddleCenter);
		table->setTextHeight(0, 0, 40);
		

		table->setValue(1, 0, L"块缩略图");
		table->setValue(1, 1, L"块名称");
		table->setValue(1, 2, L"块数量");
	
		table->setContentColor(1, 0, *colorRed);
		table->setContentColor(1, 1, *colorRed);
		table->setContentColor(1, 2, *colorRed);
		
		table->setAlignment(1, 0, AcDb::CellAlignment::kMiddleCenter);
		table->setAlignment(1, 1, AcDb::CellAlignment::kMiddleCenter);
		table->setAlignment(1, 2, AcDb::CellAlignment::kMiddleCenter);

		table->setTextHeight(1, 0, 25);
		table->setTextHeight(1, 1, 25);
		table->setTextHeight(1, 2, 25);


		AcRxClass *cls = AcDbBlockReference::desc();

		AcDbObjectIdArray entIds;
		vector<blockInfo*>vecInfo;

		if (CSelectUtil::PromptSelectEnts(L"\n选择块：", NULL, cls, entIds)) {

			for (int i = 0; i < entIds.length(); i++)
			{
				AcDbBlockReference * pBr = NULL;

				if (acdbOpenObject(pBr, entIds[i], AcDb::OpenMode::kForRead)==ErrorStatus::eOk) {

					AcDbObjectId bId=pBr->blockTableRecord();
				
					pBr->close();

					blockInfo * bI = NULL;

					for (int j = 0; j < (int)vecInfo.size(); j++)
					{

						if (vecInfo.at(j)->blockId == bId) {

							bI = vecInfo.at(j);
							break;

						}

					}
					if (bI == NULL) {

						bI = new blockInfo();
						bI->blockNum = 1;
						bI->blockId = bId;

						AcDbBlockTableRecord * rec = NULL;

						if (acdbOpenObject(rec, bId, AcDb::OpenMode::kForRead) == ErrorStatus::eOk) {
							rec->getName(bI->blockName);
						}
						rec->close();
						vecInfo.push_back(bI);
					}
					else {

						bI->blockNum += 1;

					}



				}


			}

		}

		int huiZong = 0;

		for (int m = 0; m < (int)vecInfo.size(); m++)
		{

			blockInfo*bInfo = vecInfo.at(m);

			table->insertRows(m + 2, 25);
			table->setTextHeight(m + 2, 1, 15);
			table->setTextHeight(m + 2, 2, 25);
			table->setAlignment(m + 2, 1, AcDb::CellAlignment::kMiddleCenter);
			table->setAlignment(m + 2, 2, AcDb::CellAlignment::kMiddleCenter);
			table->setBlockTableRecordId(m + 2, 0, bInfo->blockId, true);

			

			table->setValue(m + 2, 1, bInfo->blockName);
			ACHAR num[10];

			wsprintfW(num,L"%d", bInfo->blockNum);

			table->setValue(m + 2, 2, num);


			huiZong += bInfo->blockNum;
			table->setContentColor(m + 2, 0, *colorYellow);
		
			table->setContentColor(m + 2, 1, *colorGreen);
			table->setContentColor(m + 2, 2, *colorGreen);
		}

		int kinds = (int)vecInfo.size();
		ACHAR num[20], hz1[20]=L"共", hz2[20];
		wsprintfW(num, L"%d", huiZong);
		wsprintfW(hz2 ,L"%d", kinds);


		StrCatW(num, L"个");
		StrCatW(hz1, hz2);
		StrCatW(hz1, L"种");



		table->insertRows(kinds + 2, 25);
		table->setValue(kinds+2, 0, hz1);
		table->setValue(kinds + 2, 1, L"汇总");
		table->setValue(kinds + 2, 2, num);

		table->setContentColor(kinds + 2, 0, *colorYellow);
		table->setContentColor(kinds + 2, 1, *colorRed);
		table->setContentColor(kinds + 2, 2, *colorGreen);

		table->setAlignment(kinds + 2, 0, AcDb::CellAlignment::kMiddleCenter);
		table->setAlignment(kinds + 2, 1, AcDb::CellAlignment::kMiddleCenter);
		table->setAlignment(kinds + 2, 2, AcDb::CellAlignment::kMiddleCenter);

		table->setTextHeight(kinds + 2, 0, 25);
		table->setTextHeight(kinds + 2, 1, 25);
		table->setTextHeight(kinds + 2, 2, 25);

		if (CGetInputUtil::GetPoint(L"\n插入表格：", insertPt)) {


			table->setPosition(insertPt);

			CDwgDataBaseUtil::PostToModelSpace(table);

			table->close();
		}
		delete colorRed;
		delete colorYellow;
		delete colorGreen;



	}

	


	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CTongJiAndLieBiaoApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CTongJiAndLieBiaoApp, ECDMyGroup, MyKuaiTongJI, MyKuaiTongJI, ACRX_CMD_MODAL, NULL)


