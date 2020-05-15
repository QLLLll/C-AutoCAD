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

//-----------------------------------------------------------------------------
#define szRDS _RXST("WY")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CTongJiKuaiApp : public AcRxArxApp {

public:
	CTongJiKuaiApp () : AcRxArxApp () {}

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

	virtual void RegisterServerComponents() {
	}

	
	//统计块
	struct ListACHAR
	{
		CString BlockName;
		int Nubmer;
	};
	typedef AcArray<ListACHAR*> ListACHARArray;
	static void WYMyGroupMyCommand() {
		ads_name ss;
		CStringArray mmm1;
		resbuf *rbList = acutBuildList(RTDXF0, _T("INSERT"), RTNONE);
		acutPrintf(_T("\n请选择需要统计的图块: "));
		if (RTNORM != acedSSGet(NULL, NULL, NULL, rbList, ss))
		{
			acutRelRb(rbList);
			acutPrintf(_T("\n未选择到有效对象!"));
			return;
		}
		acutRelRb(rbList);
		Adesk::Int32 nSSLength = 0;
		acedSSLength(ss, &nSSLength);
		ads_name ent;
		AcDbObjectId objId;
		
		ListACHARArray listEnts;
		for (Adesk::Int32 i = 0; i < nSSLength; i++)
		{
			acedSSName(ss, i, ent);
			acdbGetObjectId(objId, ent);
			AcDbEntity *pEnt_B = NULL;
			if (acdbOpenObject(pEnt_B, objId, AcDb::kForWrite) != Acad::eOk)continue;
			AcDbBlockReference *pBlkRef1 = AcDbBlockReference::cast(pEnt_B);
			AcDbObjectId pBlkTblRecId1;
			pBlkTblRecId1 = pBlkRef1->blockTableRecord();
			AcDbBlockTableRecord *pBlkTblRec1;
			if (acdbOpenObject(pBlkTblRec1, pBlkTblRecId1, AcDb::kForRead) != Acad::eOk)continue;
			ACHAR *BName = new ACHAR[MAX_PATH];
			pBlkTblRec1->getName(BName);
			pBlkRef1->close();
			pBlkTblRec1->close();
			pEnt_B->close();
			CString T3;
			T3.Format(_T("%s"), BName);
			mmm1.Add(T3);
		}
		acedSSFree(ss);
		for (int i = 0; i < mmm1.GetSize(); i++)
		{
			CString TT = mmm1.GetAt(i);
			int ii = 0;
			acutPrintf(_T("\n第一个块名TT为：%s		"), TT);
			ListACHAR *listEnt=NULL;
			

				for (int j = 0; j < listEnts.length();j++)
				{
					if (listEnts[j]->BlockName == TT) {
						listEnt = listEnts[j];
						break;
					}


				}

				if (listEnt==NULL)
				{
					listEnt = new ListACHAR();
					listEnt->BlockName = TT;
					listEnt->Nubmer = 1;
					listEnts.append(listEnt);
				}
				else {
					listEnt->Nubmer++;
				}
				
			

		}

		for (int i = 0; i < listEnts.length(); i++)
		{
			acutPrintf(_T("\n找到块名为：%s		共有%d个"), listEnts.at(i)->BlockName, listEnts.at(i)->Nubmer);
		}
		for (int i = 0; i < listEnts.length(); i++) {

			delete listEnts.at(i);
			listEnts.at(i) = NULL;

		}

	}

	

	

	
	
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CTongJiKuaiApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CTongJiKuaiApp, WYMyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)


