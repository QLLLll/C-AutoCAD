#pragma once
#import "c:\Program Files\Common Files\System\ado\msado15.dll" no_namespace \
	rename("EOF", "adoEOF") rename("BOF", "adoBOF")
class ADOConn
{
private:
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
public:
	ADOConn(void);
	~ADOConn(void);
	void OnInitADOConn(void);
	void ExitConnect(void);
	_RecordsetPtr GetRecordSet(_bstr_t bstrSql);
	bool ExecuteSQL(_bstr_t bstrSql);

};
