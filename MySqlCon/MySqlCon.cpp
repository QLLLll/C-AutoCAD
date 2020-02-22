// MySqlCon.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "MySqlCon.h"
#include"ADOConn.h"
#include"time.h"
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
string getTime()
{
	time_t timep;
	tm t;
	time(&timep);
	char tmp[64];
	localtime_s(&t, &timep);
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", &t);
	return tmp;
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
		_variant_t time1 = m_pRecordset->GetCollect("STime");
		_variant_t time2 = m_pRecordset->GetCollect("ETime");

		CString uName = Convert(name);
		CString t1 = Convert(time1);
		CString t2 = Convert(time2);


		//AfxMessageBox(uName);
		//PrintfCStr(uName);
		/*char *sName= uName.GetBuffer(uName.GetLength());
		
		uName.ReleaseBuffer();*/


		int len = WideCharToMultiByte(CP_ACP, 0, uName, -1, NULL, 0, NULL, NULL);
		char *ptxtTemp = new char[len + 1];
		WideCharToMultiByte(CP_ACP, 0, uName, -1, ptxtTemp, len, NULL, NULL);

		printf("%s: %S ---- %S\n",ptxtTemp, t1.GetBuffer(), t2.GetBuffer());
		
		t1.ReleaseBuffer();
		t2.ReleaseBuffer();
		
		delete[] ptxtTemp;

		m_pRecordset->MoveNext();
	}
	//m_pRecordset->Close();
	string t = getTime();
	char sdate[30];
	strcpy_s(sdate, t.c_str());

	const char * tt = sdate;
	try {

		//插入		
		/*ATL::COleDateTime st =  ATL::COleDateTime(2020, 1, 2, 9, 0, 0);

				m_pRecordset->AddNew();
				m_pRecordset->PutCollect("UserName", _variant_t("JKJK"));
				m_pRecordset->PutCollect("ChuQing", _variant_t(1));
				m_pRecordset->PutCollect("STime", _variant_t(st));
				m_pRecordset->PutCollect("ETime", _variant_t(st));
				m_pRecordset->PutCollect("JbLeiJi", _variant_t(0));

				m_pRecordset->Update();*/
				//修改
				/*
						CString sql2("select UserName from Tbl_UserKq where (UserName='JKJK5')");
						_RecordsetPtr m_pRecordset2;
						m_pRecordset2 = m_adoConn.GetRecordSet((_bstr_t)sql2);


						while (!m_pRecordset2->adoEOF) {
							m_pRecordset2->PutCollect("UserName", _variant_t("JKJK6"));
							m_pRecordset2->Update();
							m_pRecordset2->MoveNext();

						}
						*/
						//刪除
						/*
								CString sql3("delete from Tbl_UserKq where (UserName='JKJK6')");
								if (m_adoConn.ExecuteSQL((_bstr_t)sql3)) {
									AfxMessageBox(L"刪除成功");

								}
								*/

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.ErrorMessage());
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



