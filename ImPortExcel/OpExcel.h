#pragma once
#include"CApplication.h"
#include"CRange.h"
#include"CRanges.h"
#include"CWorkbook.h"
#include"CWorkbooks.h"
#include"CWorksheet.h"
#include"CWorksheets.h"
class COpExcel
{
public:
	COpExcel();
	~COpExcel();
public:
	static bool RunExcelApp(CApplication &excelApp, bool bVisible, bool bUserControl);
	static void NewWorkBook(CApplication &excelApp, CWorkbook &excelBook);
	static bool GetWorkSheet(CWorkbook &excelBook, int i, CWorksheet &excelSheet);
	static bool OpenWorkBook(CApplication &excelApp, const TCHAR* fileName,
		CWorkbook &excelBook, bool bIsReadOnly = true);
	static long GetRowCount(CWorksheet excelSheet);


	static bool SelectFileToOpen(const TCHAR *filedesc, const TCHAR * fileExtName,
		CString &selectedFile);
private:
	/*CApplication ExcelApp;
	CRange Range;
	CWorkbook book;
	CWorkbooks books;
	CWorksheet sheet;
	CWorksheets sheets;*/
};

