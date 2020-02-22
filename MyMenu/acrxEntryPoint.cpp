// (C) Copyright 2002-2012 by Autodesk, Inc. 
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
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "CAcadApplication.h"
#include "CAcadDocument.h"
#include "CAcadMenuBar.h"
#include "CAcadMenuGroup.h"
#include "CAcadMenuGroups.h"
#include "CAcadPopupMenu.h"
#include "CAcadPopupMenus.h"
#include"CAcadModelSpace.h"
#include"CAcadToolbars.h"
#include"CAcadToolbar.h"
#include"CAcadToolbarItem.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

void addMenuThroughMfcCom()
{
	TRY
	{
	CAcadApplication IAcad(acedGetAcadWinApp()->GetIDispatch(TRUE));

	CAcadMenuBar IMenuBar(IAcad.get_MenuBar());

	long numberOfMenus;
	numberOfMenus = IMenuBar.get_Count();

	CAcadMenuGroups IMenuGroups(IAcad.get_MenuGroups());

	VARIANT index;
	VariantInit(&index);
	V_VT(&index) = VT_I4;
	V_I4(&index) = 0;

	CAcadMenuGroup IMenuGroup(IMenuGroups.Item(index));

	CAcadPopupMenus IPopUpMenus(IMenuGroup.get_Menus());

	CString cstrMenuName = _T("AsdkComAccess");

	VariantInit(&index);
	V_VT(&index) = VT_BSTR;
	V_BSTR(&index) = cstrMenuName.AllocSysString();

	IDispatch* pDisp = NULL;

	//see if the menu is already there
	TRY{ pDisp = IPopUpMenus.Item(index); pDisp->AddRef(); } CATCH(COleDispatchException,e) {}END_CATCH;

	if (pDisp == NULL) {
		//create it
		CAcadPopupMenu IPopUpMenu(IPopUpMenus.Add(cstrMenuName));

		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = 0;
		IPopUpMenu.AddMenuItem(index, _T("&Add A ComCircle"), _T("AddCircle "));

		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = 1;
		IPopUpMenu.AddSeparator(index);

		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = 2;
		IPopUpMenu.AddMenuItem(index, _T("Auto&LISP Example"), _T("(prin1 \"Hello\") "));

		pDisp = IPopUpMenu.m_lpDispatch;
		pDisp->AddRef();
	}

	CAcadPopupMenu IPopUpMenu(pDisp);
	if (!IPopUpMenu.get_OnMenuBar())
	{
		VariantInit(&index);
		V_VT(&index) = VT_I4;
		V_I4(&index) = numberOfMenus - 2;;
		IPopUpMenu.InsertInMenuBar(index);
	}
	else
	{
		VariantInit(&index);
		V_VT(&index) = VT_BSTR;
		V_BSTR(&index) = cstrMenuName.AllocSysString();
		IPopUpMenus.RemoveMenuFromMenuBar(index);
		VariantClear(&index);
	}
	pDisp->Release();
	}
		CATCH(COleDispatchException, e)
	{
		e->ReportError();
		e->Delete();
	}
	END_CATCH;
}

void addCircleThroughMfcCom()
{
	TRY
	{
		CAcadApplication IApp;
	CAcadDocument IDoc;
	CAcadModelSpace IMSpace;

	IDispatch *pDisp = acedGetAcadWinApp()->
		GetIDispatch(TRUE); //AddRef is called on the pointer
	IApp.AttachDispatch(pDisp); // does not call AddRef()
	IApp.put_Visible(true);
	pDisp = IApp.get_ActiveDocument(); //AddRef is called
	IDoc.AttachDispatch(pDisp);
	pDisp = IDoc.get_ModelSpace(); //AddRef is called
	IMSpace.AttachDispatch(pDisp);

	SAFEARRAYBOUND rgsaBound;
	rgsaBound.lLbound = 0L;
	rgsaBound.cElements = 3;
	SAFEARRAY* pStartPoint = NULL;
	pStartPoint = SafeArrayCreate(VT_R8, 1, &rgsaBound);
	// X value
	long i = 0;
	double value = 4.0;
	SafeArrayPutElement(pStartPoint, &i, &value);
	// Y value
	i++;
	value = 2.0;
	SafeArrayPutElement(pStartPoint, &i, &value);
	// Z value
	i++;
	value = 0.0;
	SafeArrayPutElement(pStartPoint, &i, &value);

	VARIANT pt1;
	VariantInit(&pt1);
	V_VT(&pt1) = VT_ARRAY | VT_R8;
	V_ARRAY(&pt1) = pStartPoint;
	IMSpace.AddCircle(pt1, 2.0);

	VariantClear(&pt1);
	// Release() is called implicitly on the local objects
	}
		CATCH(COleDispatchException, e)
	{
		e->ReportError();
		e->Delete();
	}
	END_CATCH;


}

void addMenuAndBarMfcCom() {
	// 添加菜单
	CAcadApplication acad(acedGetAcadWinApp()->GetIDispatch(TRUE));
	CAcadMenuBar menuBar(acad.get_MenuBar());
	long menuCount = menuBar.get_Count();

	CAcadMenuGroups menuGroups(acad.get_MenuGroups());
	CAcadMenuGroup menuGroup(menuGroups.Item(COleVariant((long)0)));

	CAcadPopupMenus popUpMenus(menuGroup.get_Menus());
	CAcadPopupMenu popUpMenu(popUpMenus.Add(TEXT("Test")));

	popUpMenu.AddMenuItem(COleVariant((long)0), TEXT("线"), TEXT("Line "));

	CAcadPopupMenu subMenu(popUpMenu.AddSubMenu(COleVariant((long)1), TEXT("画图")));
	subMenu.AddMenuItem(COleVariant((long)0), TEXT("圆"), TEXT("Circle "));

	popUpMenu.InsertInMenuBar(COleVariant(menuCount));

	// 添加工具栏
	CAcadToolbars toolbars(menuGroup.get_Toolbars());
	long toolbarCount = toolbars.get_Count();
	CAcadToolbar toolbar(toolbars.Add(TEXT("NewToolbar")));
	toolbar.AddToolbarButton(COleVariant((long)0), TEXT("线"), TEXT("画线"), TEXT("Line "), COleVariant());

}



//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CMyMenuApp : public AcRxArxApp {

public:
	CMyMenuApp() : AcRxArxApp() {}

	virtual AcRx::AppRetCode On_kInitAppMsg(void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kInitAppMsg(pkt);

		// TODO: Add your initialization code here

		return (retCode);
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg(void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode = AcRxArxApp::On_kUnloadAppMsg(pkt);

		// TODO: Unload dependencies here

		return (retCode);
	}

	virtual void RegisterServerComponents() {
	}

	static void ECDMyGroupMyCommand() {
		//addMenuThroughMfcCom();
		addMenuAndBarMfcCom();
	}

	
	static void ECDMyGroupAddCircle() {
		addCircleThroughMfcCom();

	}


};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CMyMenuApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CMyMenuApp, ECDMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CMyMenuApp, ECDMyGroup, AddCircle, AddCircle, ACRX_CMD_MODAL, NULL)
