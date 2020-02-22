// (C) Copyright 2002-2007 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- ModelessDialog.cpp : Implementation of CModelessDialog
//-----------------------------------------------------------------------------

#include "StdAfx.h"
#include "resource.h"
#include "ModelessDialog.h"
#include"DwgDataBaseUtil.h"
#include"GetInputUtil.h"
extern CModelessDialog *pDialog;
//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CModelessDialog, CAcUiDialog)

BEGIN_MESSAGE_MAP(CModelessDialog, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CModelessDialog::OnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_LINE, &CModelessDialog::OnClickedButtonLine)
	ON_BN_CLICKED(IDC_BUTTON_PICK_EN, &CModelessDialog::OnClickedButtonPickEn)
	ON_BN_CLICKED(IDC_BUTTON_PICK_ST, &CModelessDialog::OnClickedButtonPickSt)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CModelessDialog::CModelessDialog (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CModelessDialog::IDD, pParent, hInstance), m_strEdX(_T(""))
, m_strEdY(_T(""))
, m_strEdZ(_T(""))
, m_strStX(_T(""))
, m_strStY(_T(""))
, m_strStZ(_T(""))
{
}

//-----------------------------------------------------------------------------
void CModelessDialog::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_PICK_EN, m_btnPickEd);
	DDX_Control(pDX, IDC_BUTTON_PICK_ST, m_btnPickSt);

	DDX_Text(pDX, IDC_EDIT_ENX, m_strEdX);
	DDX_Text(pDX, IDC_EDIT_ENY, m_strEdY);
	DDX_Text(pDX, IDC_EDIT_ENZ, m_strEdZ);
	DDX_Text(pDX, IDC_EDIT_STX, m_strStX);
	DDX_Text(pDX, IDC_EDIT_STY, m_strStY);
	DDX_Text(pDX, IDC_EDIT_STZ, m_strStZ);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CModelessDialog::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}

void CModelessDialog::PostNcDestory()
{
	delete this;

	if (pDialog != NULL)
	{

		pDialog = NULL;

	}
	CAcUiDialog::PostNcDestroy();

}

BOOL CModelessDialog::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	m_btnPickEd.AutoLoad();
	m_btnPickSt.AutoLoad();

	return TRUE;
}


void CModelessDialog::OnClickedButtonClose()
{
	CAcUiDialog::OnClose();

	DestroyWindow();
}


void CModelessDialog::OnClickedButtonLine()
{
	AcGePoint3d ptStart = AcGePoint3d(_wtof(m_strStX), _wtof(m_strStY),
		_wtof(m_strStZ));
	AcGePoint3d ptEnd = AcGePoint3d(_wtof(m_strEdX), _wtof(m_strEdY),
		_wtof(m_strEdZ));

	acDocManager->lockDocument(curDoc());

	AcDbLine * line = new AcDbLine(ptStart, ptEnd);

	CDwgDataBaseUtil::PostToModelSpace(line);

	line->close();

	acDocManager->unlockDocument(curDoc());
}


void CModelessDialog::OnClickedButtonPickEn()
{
	BeginEditorCommand();

	AcGePoint3d ptEt;

	if (CGetInputUtil::GetPoint(L"请输入终点：", ptEt)) {

		CompleteEditorCommand();

		m_strEdX.Format(L"%.2f", ptEt[X]);
		m_strEdY.Format(L"%.2f", ptEt[Y]);
		m_strEdZ.Format(L"%.2f", ptEt[Z]);

	}
	else {

		CancelEditorCommand();

	}

	UpdateData(FALSE);
}


void CModelessDialog::OnClickedButtonPickSt()
{
	BeginEditorCommand();

	AcGePoint3d ptSt;

	if (CGetInputUtil::GetPoint(L"请输入起点：", ptSt)) {

		CompleteEditorCommand();

		m_strStX.Format(L"%.2f", ptSt[X]);
		m_strStY.Format(L"%.2f", ptSt[Y]);
		m_strStZ.Format(L"%.2f", ptSt[Z]);

	}
	else {

		CancelEditorCommand();

	}
	
	UpdateData(FALSE);
}


void CModelessDialog::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CAcUiDialog::OnClose();
	DestroyWindow();
}
