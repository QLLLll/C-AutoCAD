#include "stdafx.h"
#include "OpExcel.h"
#include <comdef.h>

COpExcel::COpExcel()
{
}


COpExcel::~COpExcel()
{
}

bool COpExcel::RunExcelApp(CApplication & excelApp, bool bVisible, bool bUserControl)
{
	CLSID clsId;
	::CLSIDFromProgID(L"Excel.Application", &clsId);
	IUnknown *pUnk = NULL;

	if (::GetActiveObject(clsId, NULL, &pUnk) == S_OK) {

		LPDISPATCH pDispatch;

		VERIFY(pUnk->QueryInterface(IID_IDispatch, (void**)&pDispatch) == S_OK);
		excelApp.AttachDispatch(pDispatch);

	}
	else {
		if (!excelApp.CreateDispatch(TEXT("Excel.Application"))) {

			AfxMessageBox(TEXT("当前计算机没有安装Excel"));
			return false;

		}
	}
	excelApp.put_Visible(bVisible);
	excelApp.put_UserControl(bUserControl);

	return true;


}

void COpExcel::NewWorkBook(CApplication & excelApp, CWorkbook & excelBook)
{
	LPDISPATCH pDispatch = excelApp.get_Workbooks();

	CWorkbooks excelBooks;
	excelBooks.AttachDispatch(pDispatch);

	COleVariant vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	pDispatch = excelBooks.Add(vOptional);
	excelBook.AttachDispatch(pDispatch);

}

bool COpExcel::GetWorkSheet(CWorkbook & excelBook, int i, CWorksheet & excelSheet)
{
	LPDISPATCH pDispatch = excelBook.get_Worksheets();
	CWorksheets excelSheets;

	excelSheets.AttachDispatch(pDispatch);

	if(i>0&&i<=excelSheets.get_Count()){
	
		LPDISPATCH pDispatch2 = excelSheets.get_Item(COleVariant(long(i)));
		excelSheet.AttachDispatch(pDispatch2);
	
		return true;
	}
	else {
		return false;
	}
}

bool COpExcel::OpenWorkBook(CApplication & excelApp, const TCHAR * fileName, CWorkbook & excelBook, bool bIsReadOnly)
{
	try {
		COleVariant vOptional((long)DISP_E_PARAMNOTFOUND
			, VT_ERROR);
		COleVariant vIsReadOnly((short)bIsReadOnly);

		LPDISPATCH pDispatch = excelApp.get_Workbooks();
		CWorkbooks excelBooks;
		excelBooks.AttachDispatch(pDispatch);

		pDispatch = excelBooks.Open(fileName, vOptional, vIsReadOnly,
			vOptional, vOptional, vOptional, vOptional, vOptional,
			vOptional, vOptional, vOptional,
			vOptional, vOptional, vOptional, vOptional);

		excelBook.AttachDispatch(pDispatch);
		return true;

	}
	catch (_com_error e) {

		assert(false);
		AfxMessageBox(e.ErrorMessage());
		return false;
	}
}

long COpExcel::GetRowCount(CWorksheet excelSheet)
{
	CRange excelRange;
	excelRange.AttachDispatch(excelSheet.get_UsedRange());

	CRange rowRange;
	rowRange.AttachDispatch(excelRange.get_Rows());

	return rowRange.get_Count();


}

bool COpExcel::SelectFileToOpen(const TCHAR * filedesc, const TCHAR * fileExtName, CString & selectedFile)
{
	
	CString extensionNameHaveDot = TEXT(".");

	extensionNameHaveDot += fileExtName;

	CString filter;
	filter.Format(TEXT("%s(*.%s)|*.%s"), filedesc, fileExtName, fileExtName);
	filter += TEXT("|所有文件(*.*)|*.*||");

	BOOL bOpenFile = TRUE;

	CFileDialog CFileDlg(bOpenFile, extensionNameHaveDot, NULL, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
	CFileDlg.m_ofn.lStructSize = 88;

	if (CFileDlg.DoModal() == IDOK) {

		selectedFile = CFileDlg.GetPathName();

		return true;
	}
	else {
		return false;
	}




}
