#include "stdafx.h"
#include "RegMsg.h"

static bool isReged = FALSE;
extern BOOL func(MSG *msg);
RegMsg::RegMsg()
{
}


RegMsg::~RegMsg()
{
}

void RegMsg::reg()
{
	if (!isReged) {
		acutPrintf(L"\n reging...\n");
		if (!acedRegisterFilterWinMsg(func)) {

			acutPrintf(L"\n reg  failed\n");

		}
		else{

			acutPrintf(L"\n reg success\n");
			isReged = TRUE;

		}
	}
}

void RegMsg::unReg()
{
	if (isReged) {

		if (!acedRemoveFilterWinMsg(func)) {

			acutPrintf(L"\n unreg  failed\n");

		}
		else {

			acutPrintf(L"\n unreg success\n");

			isReged = FALSE;
		}
	}
}
