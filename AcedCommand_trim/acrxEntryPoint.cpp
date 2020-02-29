//-----------------------------------------------------------------------------
//----- acrxEntryPoint.cpp
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include"DwgDataBaseUtil.h"
#include"acedCmdNF.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("ECD")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CAcedCommand_trimApp : public AcRxArxApp {

public:
	CAcedCommand_trimApp() : AcRxArxApp() {}

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
	static void ECDMyGroupEcdTrim() {

		//ads_point pt1 = { 2.0,5.0,0.0 };
		////ads_name first, last;
		//acedCommandS(RTSTR, L"CIRCLE", RTSTR, L"5,5", RTSTR, L"2", 0);

		//AcDbLine *line = new AcDbLine(AcGePoint3d(1, 5, 0), AcGePoint3d(8, 5, 0));
		//CDwgDataBaseUtil::PostToModelSpace(line);
		//line->close();

		/*acdbEntNext(NULL,first);
		acdbEntLast(last);*/


		ads_name sName;


		if (acedSSGet(NULL, NULL, NULL, NULL, sName) == RTNORM) {
			
			acedCommandS(RTSTR, _T("TRIM"), RTPICKS, sName, RTSTR, _T(""), RTSTR, _T("C")/* RTPOINT, pt1, RTSTR, _T(""),*/ );
				
		
		}		//acdbGetAdsName()
		//asDblArray(ptFrom)
		//acedEntSel(TEXT(""))

		/*ads_name pl1, pl2;

		ads_point pt11, pt22;

		if (acedEntSel(TEXT("选择:\n"), pl1, pt11) != RTNORM) {

			return;
		}
		if (acedEntSel(TEXT("选择:\n"), pl2, pt22) != RTNORM) {

			return;
		}
		ads_point ptInput;
		if (acedGetPoint(NULL, TEXT("输入一个点:\n"), ptInput) != RTNORM) {

			return;

		}*/

		//acedCommandS(RTSTR, L"TRIM", RTENAME, pl1,  RTSTR, L"C", RTLB, RTENAME, pl2, RTPOINT, ptInput, RTPOINT, ptInput, RTLE, RTSTR, "", 0);
		//acedCommandS(RTSTR, "trim", RTSTR, "", RTSTR, "c", RTPOINT, ptInput , RTSTR, "");
		//acedCommandS(RTSTR, L"TRIM", RTENAME, pl1, RTENAME, pl2,RTSTR, L"", RTSTR, L"C", RTLB, RTPOINT, ptInput, RTLE, RTSTR, "", 0);
		/*acedCommandS(RTSTR, "TRIM", RTENAME, pl1, RTENAME, pl2, RTSTR, "", RTSTR, "E", RTSTR, "", RTSTR, "E", RTSTR, "", 0);*/

		acutPrintf(L"OK\n");

	}
};

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CAcedCommand_trimApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CAcedCommand_trimApp, ECDMyGroup, EcdTrim, EcdTrim, ACRX_CMD_MODAL, NULL)


