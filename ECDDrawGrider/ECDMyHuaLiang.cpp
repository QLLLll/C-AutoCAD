#include "stdafx.h"
#include "ECDMyHuaLiang.h"
#include<vector>
#include"DwgDataBaseUtil.h"
#include"SelectUtil.h"
#include"GePointUtil.h"
#include"GetInputUtil.h"
#include"MathUtil.h"
ECDMyHuaLiang::ECDMyHuaLiang()
{
}


ECDMyHuaLiang::~ECDMyHuaLiang()
{
}
AcDbEntity *ECDMyHuaLiang::GetEntity(const ACHAR *str) {

	AcRxClass *cls = AcDbEntity::desc();

	AcDbEntity *pEnt = NULL;

	AcGePoint3d pt;

	if (CSelectUtil::PromptSelectEntity(str, cls, pEnt, pt, false))
	{

		return pEnt;


	}
	return NULL;

}

AcDbObjectId ECDMyHuaLiang::AddLayer(AcDbDatabase * db, const ACHAR * layerName, AcCmColor color)
{
	AcDbObjectId layerId;

	AcDbLayerTable *lyerTbl = NULL;

	if (db->getSymbolTable(lyerTbl, AcDb::kForWrite) == ErrorStatus::eOk) {
		ErrorStatus es = lyerTbl->getAt(layerName, layerId);
		if (!layerId.isNull())
		{
			lyerTbl->close();
			return layerId;

		}


		AcDbLayerTableRecord *lyerTblRec = new AcDbLayerTableRecord();

		lyerTblRec->setName(layerName);

		

		lyerTblRec->setColor(color);

		lyerTbl->add(layerId,lyerTblRec);

		lyerTblRec->close();
		lyerTbl->close();	
	}
	return layerId;
}

void ECDMyHuaLiang::CutLine(AcDbEntity * ent1, AcDbEntity * ent2, std::vector<AcDbLine*> listLines)
{

	AcGePoint3dArray pt3dColl;

	AcGePoint3d ptArr1[3] = {};
	AcGePoint3d ptArr2[3] = {};


	listLines.at(0)->intersectWith(ent1, AcDb::kOnBothOperands, pt3dColl, 0, 0);
	ptArr1[1] = pt3dColl[0];
	pt3dColl.setLogicalLength(0);

	listLines.at(1)->intersectWith(ent1, AcDb::kOnBothOperands, pt3dColl, 0, 0);
	ptArr1[2] = pt3dColl[0];
	pt3dColl.setLogicalLength(0);


	listLines.at(0)->intersectWith(ent2, AcDb::kOnBothOperands, pt3dColl, 0, 0);
	ptArr2[1] = pt3dColl[0];
	pt3dColl.setLogicalLength(0);

	listLines.at(1)->intersectWith(ent2, AcDb::kOnBothOperands, pt3dColl, 0, 0);
	ptArr2[2] = pt3dColl[0];
	pt3dColl.setLogicalLength(0);


	AcDbLine *line2 = new AcDbLine(ptArr1[1], ptArr2[1]);
	AcDbLine *line3 = new AcDbLine(ptArr1[2], ptArr2[2]);

	AcCmColor colorDeepPink = AcCmColor();//深粉色

	colorDeepPink.setRGB(255, 20, 147);

	AcDbObjectId layer1 = AddLayer(acdbHostApplicationServices()->workingDatabase(), TEXT("ECD斜梁1"), colorDeepPink);
	AcDbObjectId layer2 = AddLayer(acdbHostApplicationServices()->workingDatabase(), TEXT("ECD斜梁2"), colorDeepPink);
	
	line2->setLayer(layer1);
	line3->setLayer(layer2);

	CDwgDataBaseUtil::PostToModelSpace(line2);
	CDwgDataBaseUtil::PostToModelSpace(line3);
	
	ent1->close();
	ent2->close();
	listLines.at(0)->close();
	listLines.at(1)->close();
	line2->close();
	line3->close();
}

AcGePoint3d ECDMyHuaLiang::GetCenterPt(AcDbEntity * ent)
{
	AcDbExtents extends;
	ent->getGeomExtents(extends);

	AcGePoint3d minPt = extends.minPoint();
	AcGePoint3d maxPt = extends.maxPoint();

	return CGePointUtil::GetMiddlePoint(minPt, maxPt);


}

void ECDMyHuaLiang::Command()
{
	AcDbEntity * ent1 = GetEntity(L"\n请选择第一个实体：\n");
	AcDbEntity * ent2 = GetEntity(L"\n请选择另一个实体：\n");
	if (ent1 == NULL || ent2 == NULL) {

		AfxMessageBox(TEXT("选择的实体为空，请重新执行命令"));
		return;

	}
	AcGePoint3d pt1 = GetCenterPt(ent1);
	AcGePoint3d pt2 = GetCenterPt(ent2);

	

	AcDbLine *line = new AcDbLine(pt1, pt2);

	double lKuan = 0.0;

	if (!CGetInputUtil::GetReal(L"\n请输入梁宽\n", 5, 4, lKuan)) {

		return;

	}

	double width = 1.0*lKuan / 2;


	AcGeVector3d v = (pt2 - pt1).normal().rotateBy(CMathUtil::PI() / 2, AcGeVector3d::kZAxis);

	AcDbLine *line1 = NULL;
		line->getTransformedCopy(AcGeMatrix3d::translation(v*width), (AcDbEntity*&)line1);
		AcDbLine *line2 = NULL;
		line->getTransformedCopy(AcGeMatrix3d::translation(v*width*-1), (AcDbEntity*&)line2);

	

	std::vector<AcDbLine*> listLines;

	listLines.push_back(line1);
	listLines.push_back(line2);

	CutLine(ent1, ent2, listLines);

}
