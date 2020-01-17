#include "StdAfx.h"
#include "resource.h"
#include"OpExcel.h"
#include"ConvertUtil.h"
#include"DwgDataBaseUtil.h"
//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CImPortExcelApp : public AcRxArxApp {

public:
	CImPortExcelApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here
		::CoInitialize(NULL);
		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here
		::CoUninitialize();
		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}
	
	static void ExportToExcel() {
		// 1.获取直线、圆形集合
		AcDbObjectIdArray allEntIds = CDwgDataBaseUtil::GetAllEntityIds(L"0",acdbCurDwg());
		AcDbObjectIdArray lineIds, circleIds;
		for (int i = 0; i < allEntIds.length(); i++)
		{
			AcDbEntity *pEnt = NULL;
			if (acdbOpenObject(pEnt, allEntIds[i], AcDb::kForRead) == Acad::eOk)
			{
				if (pEnt->isKindOf(AcDbLine::desc()))
				{
					lineIds.append(pEnt->objectId());
				}
				else if (pEnt->isKindOf(AcDbCircle::desc()))
				{
					circleIds.append(pEnt->objectId());
				}

				pEnt->close();
			}
		}

		// 2.启动Excel
		CApplication excelApp;
		COpExcel::RunExcelApp(excelApp, true, true);

		// 3.新建一个WorkBook
		CWorkbook excelBook;
		COpExcel::NewWorkBook(excelApp, excelBook);

		// 4.导出直线内容
		// 获得Sheet1
		CWorksheet excelSheet1;
		COpExcel::GetWorkSheet(excelBook, 1, excelSheet1);
		CRange cells1;
		cells1.AttachDispatch(excelSheet1.get_Cells());
		// 第一行的内容
		long row = 1, col = 1;
		cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("序号")));
		cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("起点X")));
		cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("起点Y")));
		cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("起点Z")));
		cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("终点X")));
		cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("终点Y")));
		cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("终点Z")));
		// 数据行的内容
		for (int i = 0; i < lineIds.length(); i++)
		{
			AcDbLine *pLine = NULL;
			if (acdbOpenObject(pLine, lineIds[i], AcDb::kForRead) == Acad::eOk)
			{
				AcGePoint3d startPoint = pLine->startPoint();
				AcGePoint3d endPoint = pLine->endPoint();

				row = i + 2;
				col = 1;
				cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(row - 1)));
				cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(startPoint.x)));
				cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(startPoint.y)));
				cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(startPoint.z)));
				cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(endPoint.x)));
				cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(endPoint.y)));
				cells1.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(endPoint.z)));

				pLine->close();
			}
		}

		// 5.导出圆数据
		// 获得Sheet2
		CWorksheet excelSheet2;
		COpExcel::GetWorkSheet(excelBook, 2, excelSheet2);
		CRange cells2;
		cells2.AttachDispatch(excelSheet2.get_Cells());
		// 第一行的内容
		row = 1, col = 1;
		cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("序号")));
		cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("圆心X")));
		cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("圆心Y")));
		cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("圆心Z")));
		cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(TEXT("半径")));
		// 数据行的内容
		for (int i = 0; i < circleIds.length(); i++)
		{
			AcDbCircle *pCircle = NULL;
			if (acdbOpenObject(pCircle, circleIds[i], AcDb::kForRead) == Acad::eOk)
			{
				AcGePoint3d center = pCircle->center();
				double radius = pCircle->radius();

				row = i + 2;
				col = 1;
				cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(row - 1)));
				cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(center.x)));
				cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(center.y)));
				cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(center.z)));
				cells2.put_Item(COleVariant(row), COleVariant(col++), COleVariant(CConvertUtil::ToString(radius)));

				pCircle->close();
			}
		}
	}

	static void ECDMyGroupMyCommand () {
		
		ExportToExcel();
	}


	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CImPortExcelApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CImPortExcelApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)

