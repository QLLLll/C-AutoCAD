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
//----- ArxDialog.h : Declaration of the CArxDialog
//-----------------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------
#include "acui.h"
#include"resource.h"
#include "C:\Autodesk\Autodesk_ObjectARX_2018_Win_64_and_32_Bit\inc\acuiButton.h"
#include "C:\Autodesk\Autodesk_ObjectARX_2018_Win_64_and_32_Bit\inc\acuiEdit.h"

//-----------------------------------------------------------------------------
class CArxDialog : public CAcUiDialog {
	DECLARE_DYNAMIC (CArxDialog)

public:
	CArxDialog (CWnd *pParent =NULL, HINSTANCE hInstance =NULL) ;

	enum { IDD = IDD_ARX_MODAL} ;

protected:
	virtual void DoDataExchange (CDataExchange *pDX) ;
	afx_msg LRESULT OnAcadKeepFocus (WPARAM, LPARAM) ;

	DECLARE_MESSAGE_MAP()
public:
	CAcUiPickButton m_btnAngle;
	CAcUiPickButton m_btnPick;
	CAcUiAngleEdit m_editAngle;
	CAcUiNumericEdit m_editXPt;
	CAcUiNumericEdit m_editYPt;
	CAcUiNumericEdit m_editZPt;

	CString m_strAngle;
	CString m_strZPt;
	CString m_strYPt;
	CString m_strXPt;

	void DisplayPoint();
	void DisplayAngle();
	virtual BOOL OnInitDialog();
	virtual void OnClose();

	afx_msg void OnClickedButtonAngle();
	afx_msg void OnClickedButtonPoint();
	afx_msg void OnKillfocusEditAngle();
	afx_msg void OnKillfocusEditXpt();
	afx_msg void OnKillfocusEditYpt();
	afx_msg void OnKillfocusEditZpt();
} ;
