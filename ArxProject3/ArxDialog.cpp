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
//----- ArxDialog.cpp : Implementation of CArxDialog
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "ArxDialog.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CArxDialog, CAcUiDialog)

BEGIN_MESSAGE_MAP(CArxDialog, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
	ON_BN_CLICKED(IDC_BUTTON_ANGLE, &CArxDialog::OnClickedButtonAngle)
	ON_BN_CLICKED(IDC_BUTTON_POINT, &CArxDialog::OnClickedButtonPoint)
	ON_EN_KILLFOCUS(IDC_EDIT_ANGLE, &CArxDialog::OnKillfocusEditAngle)
	ON_EN_KILLFOCUS(IDC_EDIT_XPT, &CArxDialog::OnKillfocusEditXpt)
	ON_EN_KILLFOCUS(IDC_EDIT_YPT, &CArxDialog::OnKillfocusEditYpt)
	ON_EN_KILLFOCUS(IDC_EDIT_ZPT, &CArxDialog::OnKillfocusEditZpt)
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CArxDialog::CArxDialog (CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CArxDialog::IDD, pParent, hInstance) {
}

//-----------------------------------------------------------------------------
void CArxDialog::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ANGLE, m_btnAngle);
	DDX_Control(pDX, IDC_BUTTON_POINT, m_btnPick);
	DDX_Control(pDX, IDC_EDIT_ANGLE, m_editAngle);
	DDX_Control(pDX, IDC_EDIT_XPT, m_editXPt);
	DDX_Control(pDX, IDC_EDIT_YPT, m_editYPt);
	DDX_Control(pDX, IDC_EDIT_ZPT, m_editZPt);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CArxDialog::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}


void CArxDialog::DisplayPoint()
{
	m_editXPt.SetWindowText(m_strXPt);
	m_editXPt.Convert();
	m_editYPt.SetWindowText(m_strYPt);
	m_editYPt.Convert();
	m_editZPt.SetWindowText(m_strZPt);
	m_editZPt.Convert();

}

void CArxDialog::DisplayAngle()
{
	m_editAngle.SetWindowTextW(m_strAngle);
	m_editAngle.Convert();


}

BOOL CArxDialog::OnInitDialog()
{

	CAcUiDialog::OnInitDialog();

	m_editXPt.SetRange(-100.0, 100.0);
	m_editYPt.SetRange(-100.0, 100.0);
	m_editZPt.SetRange(-100.0, 100.0);

	m_editAngle.SetRange(0.0, 90.0);

	m_btnPick.AutoLoad();
	m_btnAngle.AutoLoad();

	m_strAngle = "0.0";
	m_strXPt = "0.0";
	m_strYPt = "0.0";
	m_strZPt = "0.0";

	DisplayPoint();
	DisplayAngle();

	return TRUE;
}

void CArxDialog::OnClose()
{
	
	double x = _wtof(m_strXPt);
	double y = _wtof(m_strYPt);
	double z = _wtof(m_strZPt);

	acutPrintf(L"\n选择的点坐标为（%.2f,%.2f,%.2f)", x, y, z);

	CAcUiDialog::OnClose();
}

void CArxDialog::OnClickedButtonAngle()
{
	
	BeginEditorCommand();

	ads_point pt;

	//字符串转换为数字
	acdbDisToF(m_strXPt, -1, &pt[X]);
	acdbDisToF(m_strYPt, -1, &pt[Y]);
	acdbDisToF(m_strZPt, -1, &pt[Z]);

	double angle;
	const double PI = 4 * atan(1);

	if (acedGetAngle(pt, L"\n输入角度：", &angle) == RTNORM) {

		CompleteEditorCommand();

		m_strAngle.Format(L"%.2f", angle*(180.0 / PI));

		DisplayAngle();

	}
	else {

		CancelEditorCommand();

	}
}


void CArxDialog::OnClickedButtonPoint()
{
	BeginEditorCommand();

	ads_point pt;

	if (acedGetPoint(NULL, L"\n请选择一个点：", pt) == RTNORM) {
		
		CompleteEditorCommand();
		m_strXPt.Format(L"%.2f", pt[X]);
		m_strYPt.Format(L"%.2f", pt[Y]);
		m_strZPt.Format(L"%.2f", pt[Z]);

		DisplayPoint();

	}
	else {

		CancelEditorCommand();

	}
}


void CArxDialog::OnKillfocusEditAngle()
{	
	m_editAngle.Convert();
	m_editAngle.GetWindowTextW(m_strAngle);
}


void CArxDialog::OnKillfocusEditXpt()
{
	m_editXPt.Convert();
	m_editXPt.GetWindowTextW(m_strXPt);
}


void CArxDialog::OnKillfocusEditYpt()
{
	m_editYPt.Convert();
	m_editYPt.GetWindowTextW(m_strYPt);
}


void CArxDialog::OnKillfocusEditZpt()
{
	m_editZPt.Convert();
	m_editZPt.GetWindowTextW(m_strZPt);
}
