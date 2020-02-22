#pragma once
#include"resource.h"

// MFCDialog 对话框

class MFCDialog : public CDialogEx
{
	DECLARE_DYNAMIC(MFCDialog)

public:
	MFCDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MFCDialog();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_MODAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
