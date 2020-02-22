
#include "StdAfx.h"
#include "ADOConn.h"

ADOConn::ADOConn(void)
{
}


ADOConn::~ADOConn(void)
{
}


void ADOConn::OnInitADOConn(void)
{
	//初始化ole/com库环境
	::CoInitialize(NULL);
	try
	{
		//创建connection对象
		m_pConnection.CreateInstance("ADODB.Connection");
		//设置连接字符串
		_bstr_t strConnect = "driver={SQL Server};uid=sa;pwd=940619.lq;Server=.;Database=Learnning";
		m_pConnection->Open(strConnect, "sa", "940619.lq", adModeUnknown);
	}
	//捕捉异常
	catch (_com_error e)
	{
		//显示错误信息
		AfxMessageBox(e.Description());
	}
}


void ADOConn::ExitConnect(void)
{
	//关闭记录集和连接
	if (m_pRecordset != NULL&&m_pRecordset->State)
		m_pRecordset->Close();
	m_pConnection->Close();
	//释放环境
	::CoUninitialize();
}


_RecordsetPtr ADOConn::GetRecordSet(_bstr_t bstrSql)
{
	try
	{
		//连接数据库，如果connection是空，则重新连接数据库
		if (m_pConnection == NULL)
			OnInitADOConn();
		//创建记录对象
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		//取得表中的记录
		m_pRecordset->Open(bstrSql, m_pConnection.GetInterfacePtr(), adOpenDynamic,
			adLockOptimistic, adCmdText);
		
		/*m_pRecordset.CreateInstance("ADODB.Recordset");
		m_pRecordset->Open(bstrSql,

			_variant_t((IDispatch*)m_pConnection, true),

			adOpenStatic,

			adLockOptimistic,

			adCmdText);*/
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
	//返回记录集
	return m_pRecordset;
}


bool ADOConn::ExecuteSQL(_bstr_t bstrSql)
{
	_variant_t recordsAffected;
	try
	{
		//数据库是否已连接
		if (m_pConnection == NULL)
			OnInitADOConn();
		m_pConnection->Execute(bstrSql, NULL, adCmdText);
		return true;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
	return false;
}



