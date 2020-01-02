// MySqlCon.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MySqlCon.h"
#include"ADOConn.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

CString Convert(_variant_t var) {
	CString str;

	if (var.vt == VT_I4)
	{
		long lNum;
		char szCh[21];
		str = var.bstrVal;
		WideCharToMultiByte
			(CP_ACP, 0, var.bstrVal, -1,
				szCh, sizeof(szCh), NULL, NULL);
	}

	switch (var.vt)
	{
	case VT_BOOL:
		str = var.boolVal == 0 ? "0" : "1";
		break;
	case VT_BSTR:         //var is BSTR type
		str = var.bstrVal;
		break;

	case VT_I2:           //var is short int type 
		str.Format(L"%d", (int)var.iVal);
		break;

	case VT_I4:          //var is long int type
		str.Format(L"%d", var.lVal);
		break;

	case VT_R4:         //var is float type
		str.Format(L"%10.6f", (double)var.fltVal);
		break;

	case VT_R8:         //var is double type
		str.Format(L"%10.6f", var.dblVal);
		break;

	case VT_CY:        //var is CY type
		str = COleCurrency(var).Format();
		break;

	case VT_DATE:     //var is DATE type
		str = COleDateTime(var).Format();
		break;

	default:
		str.Format(L"Unk type %d\n", var.vt);
		TRACE(L"Unknown type %d\n", var.vt);
	}
	return str;
}

void MySql() {
	ADOConn m_adoConn;
	m_adoConn.OnInitADOConn();
	CString sql("select * from Tbl_UserKq");
	_RecordsetPtr m_pRecordset;
	m_pRecordset = m_adoConn.GetRecordSet((_bstr_t)sql);




	while (m_pRecordset->adoEOF == 0)
	{
		//strcpy_s(node.nodeName, (char*)(_bstr_t)m_pRecordset->GetCollect("nodeName"));
		//node.nodeLeft = m_pRecordset->GetCollect("nodeLeft");
		//node.nodeRight = m_pRecordset->GetCollect("nodeRight");

		_variant_t name = m_pRecordset->GetCollect("UserName");

		CString uName = Convert(name);

		AfxMessageBox(uName);

		m_pRecordset->MoveNext();
	}
	//断开数据库
	m_adoConn.ExitConnect();

}

int main()
{
    int nRetCode = 0;

   /* HMODULE hModule = ::GetModuleHandle(nullptr);

    if (hModule != nullptr)
    {
        // 初始化 MFC 并在失败时显示错误
        if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
        {
            // TODO: 更改错误代码以符合您的需要
            wprintf(L"错误: MFC 初始化失败\n");
            nRetCode = 1;
        }
        else
        {
            // TODO: 在此处为应用程序的行为编写代码。
        }
    }
    else
    {
        // TODO: 更改错误代码以符合您的需要
        wprintf(L"错误: GetModuleHandle 失败\n");
        nRetCode = 1;
    }*/

	MySql();

    return nRetCode;
}



