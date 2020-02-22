#include "stdafx.h"
#include "LineUtil.h"
#include"DwgDataBaseUtil.h"

CLineUtil::CLineUtil()
{
}


CLineUtil::~CLineUtil()
{
}

 AcDbObjectId CLineUtil::Add(const AcGePoint3d &startPoint, const AcGePoint3d &endPoint, AcDbDatabase *pDb)
{
	// 创建直线实体
	AcDbLine *pLine = new AcDbLine(startPoint, endPoint);

	// 将直线实体添加到模型空间
	return CDwgDataBaseUtil::PostToModelSpace(pLine, pDb);
}
