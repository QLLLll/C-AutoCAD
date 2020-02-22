// MFCDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCDialog.h"
#include "afxdialogex.h"


// MFCDialog 对话框

IMPLEMENT_DYNAMIC(MFCDialog, CDialogEx)

MFCDialog::MFCDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_MODAL, pParent)
{

}

MFCDialog::~MFCDialog()
{
}

void MFCDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MFCDialog, CDialogEx)
END_MESSAGE_MAP()


// MFCDialog 消息处理程序
