// TjAreaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TjAreaDlg.h"
#include "afxdialogex.h"
#include"resource.h"
#include"SelectUtil.h"
#include"TextFileUtil.h"
#include"StringUtil.h"
#include"ConvertUtil.h"
#include"GePointUtil.h"
#include"DwgDataBaseUtil.h"
#include"GetInputUtil.h"
#include"vector"
#include<iostream>
#include<iomanip>
#include <fstream>
#include <comdef.h>
using namespace std;



// CTjAreaDlg 对话框

IMPLEMENT_DYNAMIC(CTjAreaDlg, CDialog)

CTjAreaDlg::CTjAreaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DIALOG1, pParent)
{

}

CTjAreaDlg::~CTjAreaDlg()
{
}

void CTjAreaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTjAreaDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &CTjAreaDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CTjAreaDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CTjAreaDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON6, &CTjAreaDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON5, &CTjAreaDlg::OnBnClickedButton5)
END_MESSAGE_MAP()


// CTjAreaDlg 消息处理程序

//标注面积
void CTjAreaDlg::OnBnClickedButton2()
{
	acDocManager->lockDocument(acDocManager->curDocument());
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);

	AcDbObjectIdArray oIdArr;

	ErrorStatus es;
	double textHeight = 0.0;

	if (CSelectUtil::PromptSelectEnts(TEXT("请选择要求面积的封闭曲线:\n"),
		NULL, oIdArr))
	{
		for each (AcDbObjectId  oId in oIdArr)
		{

			AcDbEntity * pEnt = NULL;

			es = acdbOpenAcDbEntity(pEnt, oId, AcDb::kForWrite);

			if (es == ErrorStatus::eOk) {

				if (pEnt->isKindOf(AcDbPolyline::desc())) {

					AcDbPolyline * pl = (AcDbPolyline*)pEnt;

					pl->setClosed(true);


					double area = 0.0;
					pl->getArea(area);

					if (textHeight <= 0.0) {
						textHeight = (int)sqrt(area) / 7;
					}
					/*if (textHeight < 2) {
						textHeight = 2;
					}*/
					area /= 1e6;
					AcDbExtents extents;

					pl->getGeomExtents(extents);

					AcGePoint3d zxPoint =
						CGePointUtil::GetMiddlePoint(
							extents.minPoint(), extents.maxPoint());

					CString mj;

					mj.Format(TEXT("%f平方米"), area);

					AcDbText *text = new AcDbText(zxPoint, mj);

					text->setColorIndex(1);
					text->setTextString(mj);
					text->setHeight(textHeight);

					CDwgDataBaseUtil::PostToModelSpace(text);
					pEnt->close();
					text->close();
				}

				else if (pEnt->isKindOf(AcDbCircle::desc())) {

					AcDbCircle * cir = (AcDbCircle*)pEnt;

					double area = 0.0;
					cir->getArea(area);
					area /= 1e6;
					AcDbExtents extents;

					cir->getGeomExtents(extents);

					AcGePoint3d zxPoint =
						CGePointUtil::GetMiddlePoint(
							extents.minPoint(), extents.maxPoint());

					CString mj;

					mj.Format(TEXT("%f平方米"), area);

					AcDbText *text = new AcDbText(zxPoint, mj);

					text->setColorIndex(1);

					text->setTextString(mj);
					text->setHeight(textHeight);

					CDwgDataBaseUtil::PostToModelSpace(text);
					pEnt->close();
					text->close();
				}
				else {

					CString str;
					str.Format(TEXT("不是多段线%d"), es);

					MessageBox(str);
					pEnt->close();
					continue;
				}

			}
			else {
				CString str;
				str.Format(TEXT("错误%d"), es);


				MessageBox(str);
				continue;

			}
		}
	}
	acDocManager->unlockDocument(acDocManager->curDocument());

	this->CloseWindow();
}

//面积求和
void CTjAreaDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	acDocManager->lockDocument(acDocManager->curDocument());
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);

	vector<AcDbEntity*> vecEnts;

	ads_name ssName;
	if (acedSSGet(NULL, NULL, NULL, NULL, ssName) != RTNORM) {
		return;
	}

	int length;
	acedSSLength(ssName, &length);

	for (int i = 0; i < length; i++)
	{
		ads_name ent;
		acedSSName(ssName, i, ent);
		AcDbObjectId oId;
		if (acdbGetObjectId(oId, ent) == ErrorStatus::eOk) {
			AcDbEntity * pEnt = NULL;
			ErrorStatus es;

			es = acdbOpenObject(pEnt, oId, AcDb::OpenMode::kForWrite);

			if (es == ErrorStatus::eOk)
			{
				vecEnts.push_back(pEnt);

			}
		}
	}


	double sumArea = 0.0;
	double textHeight = 0.0;
	for (int i = 0; i < (int)vecEnts.size(); i++)
	{

		AcDbEntity * pEnt = vecEnts.at(i);

		if (pEnt->isKindOf(AcDbPolyline::desc())) {

			AcDbPolyline * pl = (AcDbPolyline*)pEnt;

			pl->setClosed(true);


			double area = 0.0;
			pl->getArea(area);

			if (textHeight ==0.0) {

				textHeight = (int)sqrt(area) / 7;

			}
			sumArea += area;

			pEnt->close();

		}

		else if (pEnt->isKindOf(AcDbCircle::desc())) {


			AcDbCircle * cir = (AcDbCircle*)pEnt;

			double area = 0.0;
			cir->getArea(area);
			sumArea += area;

			pEnt->close();
		}
	}

	sumArea /= 1e6;

	CString mj;

	mj.Format(TEXT("%f平方米"), sumArea);

	AcGePoint3d ptInsert;

	CGetInputUtil::GetPoint(TEXT("插入总面积文字：\n"), ptInsert);

	AcDbText *text = new AcDbText(ptInsert, mj);

	text->setColorIndex(1);
	text->setTextString(mj);
	text->setHeight(textHeight);

	CDwgDataBaseUtil::PostToModelSpace(text);
	text->close();

	acDocManager->unlockDocument(acDocManager->curDocument());

	this->CloseWindow();
}

//数值求和
void CTjAreaDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	acDocManager->lockDocument(acDocManager->curDocument());
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);

	vector<AcDbEntity*> vecEnts;


	ads_name ssName;
	if (acedSSGet(NULL, NULL, NULL, NULL, ssName) != RTNORM) {
		return;
	}

	int length;

	acedSSLength(ssName, &length);

	for (int i = 0; i < length; i++)
	{

		ads_name ent;
		acedSSName(ssName, i, ent);

		AcDbObjectId oId;
		if (acdbGetObjectId(oId, ent) == ErrorStatus::eOk) {
			AcDbEntity * pEnt = NULL;
			ErrorStatus es;

			es = acdbOpenObject(pEnt, oId, AcDb::OpenMode::kForRead);

			if (es == ErrorStatus::eOk)
			{
				vecEnts.push_back(pEnt);

			}
			else {

				CString  ss;
				ss.Format(TEXT("错误%d"), es);
				MessageBox(ss);
			}
		}
	}

	double sumArea = 0.0;
	double textHeight = 0.0;
	for (int i = 0; i < (int)vecEnts.size(); i++) {

		AcDbEntity * tempEnt = vecEnts.at(i);

		if (tempEnt->isKindOf(AcDbText::desc())) {

			AcDbText* text = (AcDbText*)tempEnt;

			CString str = text->textString();

			if (i == 0) {
				textHeight = text->height();
			}

			text->close();

			CString strTemp;
			for (int i = 0; i < str.GetLength(); i++)
			{
				if (str[i] >= '0' && str[i] <= '9' || str[i] == '.')
					strTemp += str[i];
			}
			sumArea += CConvertUtil::ToDouble(strTemp);

		}
	}

	CString mj;

	mj.Format(TEXT("%f平方米"), sumArea);

	AcGePoint3d ptInsert;

	CGetInputUtil::GetPoint(TEXT("插入总面积文字：\n"), ptInsert);

	AcDbText *text = new AcDbText(ptInsert, mj);

	text->setColorIndex(1);
	text->setTextString(mj);
	text->setHeight(textHeight);

	CDwgDataBaseUtil::PostToModelSpace(text);
	text->close();


	acDocManager->unlockDocument(acDocManager->curDocument());

	this->CloseWindow();
}
//统计面积到cad制表
void CTjAreaDlg::OnBnClickedButton6()
{
	acDocManager->lockDocument(acDocManager->curDocument());
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);
	AcDbTable *table = new AcDbTable();
	AcGePoint3d insertPt;

	table->insertRows(0, 50, 1);
	table->insertColumns(0, 200, 2);
	table->setWidth(600);


	table->setValue(0, 0, L"面积统计");

	AcCmColor *colorRed = new AcCmColor();
	colorRed->setColorIndex(1);

	AcCmColor *colorYellow = new AcCmColor();
	colorYellow->setColorIndex(2);

	table->setContentColor(0, 0, *colorRed);
	table->setAlignment(0, 0, AcDb::CellAlignment::kMiddleCenter);
	table->setTextHeight(0, 0, 40);


	table->setValue(1, 0, L"编号");
	table->setValue(1, 1, L"面积（m2）");


	table->setContentColor(1, 0, *colorRed);
	table->setContentColor(1, 1, *colorRed);


	table->setAlignment(1, 0, AcDb::CellAlignment::kMiddleCenter);
	table->setAlignment(1, 1, AcDb::CellAlignment::kMiddleCenter);


	table->setTextHeight(1, 0, 25);
	table->setTextHeight(1, 1, 25);

	AcDbObjectId tableId;
	if (CGetInputUtil::GetPoint(L"\n插入表格：", insertPt)) {


		table->setPosition(insertPt);

		tableId = CDwgDataBaseUtil::PostToModelSpace(table);

		table->close();
	}
	int index = 1;
	AcGePoint3d selectPt;
	double a = 0.0;
	int textHeight = 5;
	while (CGetInputUtil::GetPoint(TEXT("在封闭区域点选【esc 结束】:"), selectPt)) {

		AcDbVoidPtrArray ptrArr;
		ErrorStatus es = acedTraceBoundary(selectPt, false, ptrArr);

		if (es != Acad::eOk) {
			acutPrintf(L"\nboundary=%d", es);
			continue;
		}

		AcDbPolyline * pl = NULL;
		pl = (AcDbPolyline*)ptrArr[0];

		pl->getArea(a);



		if (index == 1) {

			textHeight = (int)sqrt(a) / 7;

		}

		a /= 1e6;
		pl->erase();
		pl->close();
		pl = NULL;

		AcDbTable* openTable = NULL;

		es = acdbOpenObject(openTable, tableId, AcDb::kForWrite);

		if (es != ErrorStatus::eOk) {

			return;

		}
		openTable->insertRows(index + 1, 25);

		table->setContentColor(index + 1, 0, *colorRed);
		table->setContentColor(index + 1, 1, *colorRed);

		openTable->setTextHeight(index + 1, 0, 25);
		openTable->setTextHeight(index + 1,1, 25);

		openTable->setAlignment(index + 1, 0, AcDb::CellAlignment::kMiddleCenter);
		openTable->setAlignment(index + 1,1, AcDb::CellAlignment::kMiddleCenter);
		
		CString strIndex,strArea;

		strIndex.Format(TEXT("%d"),index);
		strArea.Format(TEXT("%f"), a);

		AcDbText *text = new AcDbText(selectPt, strIndex);
		text->setHeight(textHeight);
		CDwgDataBaseUtil::PostToModelSpace(text);
		text->close();


		ACHAR* ach1=strIndex.AllocSysString();
		ACHAR* ach2 = strArea.AllocSysString();

		openTable->setValue(index + 1, 0, ach1);
		openTable->setValue(index + 1,1, ach2);
		
		index++;

		openTable->close();
	}



	delete colorRed;
	delete colorYellow;
	acDocManager->unlockDocument(acDocManager->curDocument());

	this->CloseWindow();
}

//输出面积到txt
void CTjAreaDlg::OnBnClickedButton5()
{
	acDocManager->lockDocument(acDocManager->curDocument());
	// TODO: 在此添加控件通知处理程序代码
	this->ShowWindow(SW_HIDE);
	// TODO: 在此添加控件通知处理程序代码
	vector<char*>vecLines;

	vecLines.push_back("面积统计列表\r\n");
	vecLines.push_back("序号\t\t面积(m2)\r\n");

	int index = 1;
	AcGePoint3d selectPt;
	double a = 0.0;
	int textHeight = 5;
	while (CGetInputUtil::GetPoint(TEXT("在封闭区域点选【esc 结束】:"), selectPt)) {

		AcDbVoidPtrArray ptrArr;
		ErrorStatus es = acedTraceBoundary(selectPt, false, ptrArr);

		if (es != Acad::eOk) {
			acutPrintf(L"\nboundary=%d", es);
			continue;
		}

		AcDbPolyline * pl = NULL;
		pl = (AcDbPolyline*)ptrArr[0];

		pl->getArea(a);

		if (index == 1) {

			textHeight = (int)sqrt(a) / 7;

		}

		a /= 1e6;
		pl->erase();
		pl->close();
		pl = NULL;
		CString strIndex;

		strIndex.Format(TEXT("%d"), index);
		
		AcDbText *text = new AcDbText(selectPt, strIndex);
		text->setHeight(textHeight);
		CDwgDataBaseUtil::PostToModelSpace(text);
		text->close(); 

		CString strLine;
		strLine.Format(TEXT("%d\t\t%f\r\n"), index, a);
		
		USES_CONVERSION;		
		char* chLine = T2A(strLine);
		
		vecLines.push_back(chLine);
		index++;
	}


	resbuf * fileNameBuf=NULL;

	acedGetFileNavDialog(TEXT("保存文件"), 
		TEXT("area.txt"),
		TEXT("txt"),
		TEXT("保存文件"),
		1,
		&fileNameBuf);

	if (fileNameBuf->resval.rstring != NULL) {

		CString fpath = fileNameBuf->resval.rstring;
		
		USES_CONVERSION;
		char *chPath= T2A(fpath);

		ofstream  myfile(chPath, ofstream::trunc);
		
		if (!myfile)
		{
			acutPrintf(TEXT("打开文件出错!"));
		}
		else
		{
			for (int i=0;i<(int)vecLines.size();i++)
			{
				myfile.write(vecLines[i], strlen(vecLines[i]));

			}
			myfile.flush();
			myfile.close();
		}

	}

	acutRelRb(fileNameBuf);
	acDocManager->unlockDocument(acDocManager->curDocument());

	this->CloseWindow();
}


