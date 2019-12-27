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
//----- ModelessDialog.h : Declaration of the CModelessDialog
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include"resource.h"
#include "C:\Autodesk\Autodesk_ObjectARX_2018_Win_64_and_32_Bit\inc\acuiButton.h"


//-----------------------------------------------------------------------------
class CModelessDialog : public CAcUiDialog {
	DECLARE_DYNAMIC (CModelessDialog)
	
public:
	CModelessDialog (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_DIALOG_MODELESS} ;
	
protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;
	virtual void PostNcDestory();
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CAcUiPickButton m_btnPickEd;
	CAcUiPickButton m_btnPickSt;
	afx_msg void OnClickedButtonClose();
	afx_msg void OnClickedButtonLine();
	afx_msg void OnClickedButtonPickEn();
	afx_msg void OnClickedButtonPickSt();
	CString m_strEdX;
	CString m_strEdY;
	CString m_strEdZ;
	CString m_strStX;
	CString m_strStY;
	CString m_strStZ;
	afx_msg void OnClose();

	
} ;
