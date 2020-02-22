#include "stdafx.h"
#include "EcdZhuangZhu.h"
#include<vector>
#include"DwgDataBaseUtil.h"
#include"SelectUtil.h"
#include"GePointUtil.h"
#include"GetInputUtil.h"
#include"MathUtil.h"
#include"ConvertUtil.h"

EcdZhuangZhu::EcdZhuangZhu()
{
}


EcdZhuangZhu::~EcdZhuangZhu()
{
}

void EcdZhuangZhu::Command()
{
	double radius = 0.0;

	if (!CGetInputUtil::GetReal(L"\nqÇëÊäÈë°ë¾¶:", 5, 4, radius))
		return;

	while (true) {

		AcGePoint3d centerPt;

		if (!CGetInputUtil::GetPoint(L"\nÇëÊäÈë×®Öù²åÈëµã£º", centerPt))
			return;

		AcDbCircle *c = new AcDbCircle(centerPt, AcGeVector3d::kZAxis, radius);

		AcCmColor colorYellow = AcCmColor::AcCmColor();

		colorYellow.setRGB(255, 255, 0);//»ÆÉ«

		c->setColor(colorYellow);

		AcGePoint3d ptHStart = centerPt - AcGeVector3d::kXAxis * 200;
		AcGePoint3d ptHEnd = centerPt + AcGeVector3d::kXAxis * 200;

		AcDbLine * hLine = new AcDbLine(ptHStart, ptHEnd);


		AcGePoint3d ptZStart = centerPt - AcGeVector3d::kYAxis * 200;
		AcGePoint3d ptZEnd = centerPt + AcGeVector3d::kYAxis * 200;

		AcDbLine *zLine = new AcDbLine(ptZStart, ptZEnd);

		hLine->setLineWeight(AcDb::LineWeight::kLnWt030);
		zLine->setLineWeight(AcDb::LineWeight::kLnWt030);

		AcGePoint3d ptYx1 = centerPt + AcGeVector3d::kXAxis*(radius + 800);

		ptYx1 = ptYx1.rotateBy(CMathUtil::PI()*(70.0 / 180),
			AcGeVector3d::kZAxis, centerPt);

		AcGePoint3d ptYx2 = ptYx1 + AcGeVector3d::kXAxis * 700;

		AcDbPolyline *plYx = new AcDbPolyline();

		AcCmColor color1 = AcCmColor::AcCmColor();

		color1.setRGB(0, 128, 0);//ÂÌÉ«

		plYx->setColor(color1);

		plYx->addVertexAt(plYx->numVerts(), CConvertUtil::ToPoint2d(centerPt), 0, 0, 0);
		plYx->addVertexAt(plYx->numVerts(), CConvertUtil::ToPoint2d(ptYx1), 0, 0, 0);
		plYx->addVertexAt(plYx->numVerts(), CConvertUtil::ToPoint2d(ptYx2), 0, 0, 0);

		AcDbText *dbText = new AcDbText();

		dbText->setTextString(L"ZH1");

		AcDbObjectId styleId;

		dbText->setHorizontalMode(AcDb::TextHorzMode::kTextMid);

		dbText->setHeight(135);

		AcCmColor colorRed = AcCmColor::AcCmColor();

		colorRed.setRGB(255, 0, 0);//ºìÉ«

		dbText->setColor(colorRed);

		dbText->setAlignmentPoint(AcGePoint3d((ptYx2.x + ptYx1.x) / 2, (ptYx2.y + ptYx1.y) / 2 + 100, 0));

		AcDbDatabase *db = acdbHostApplicationServices()->workingDatabase();

		AcCmColor colorWhite = AcCmColor::AcCmColor();

		colorWhite.setRGB(255, 255, 255);//°×É«

		AcDbObjectId layer1 = AddLayer(db, L"ECD×®Öù01", colorWhite);
		AcDbObjectId layer2 = AddLayer(db, L"ECD×®Öù02", colorYellow);
		AcDbObjectId layer3 = AddLayer(db, L"ECD×®Öù03", colorWhite);

		ErrorStatus es=hLine->setLayer(layer1);

		acutPrintf(L"\n%d", es);

		zLine->setLayer(layer1);
		c->setLayer(layer2);
		plYx->setLayer(layer3);
		dbText->setLayer(layer3);

		CDwgDataBaseUtil::PostToModelSpace(hLine);
		CDwgDataBaseUtil::PostToModelSpace(zLine);
		CDwgDataBaseUtil::PostToModelSpace(c);
		CDwgDataBaseUtil::PostToModelSpace(plYx);
		CDwgDataBaseUtil::PostToModelSpace(dbText);


		hLine->close();
		zLine->close();
		c->close();
		plYx->close();
		dbText->close();

	}


}

AcDbObjectId EcdZhuangZhu::AddLayer(AcDbDatabase * db, const ACHAR * layerName, AcCmColor color)
{
	AcDbObjectId layerId;

	AcDbLayerTable *lyerTbl = NULL;

	if (db->getSymbolTable(lyerTbl, AcDb::kForWrite) == ErrorStatus::eOk) {

		ErrorStatus es = lyerTbl->getAt(layerName, layerId);
			acutPrintf(L"\n layer=%d", es);
		if (!layerId.isNull())
		{
			lyerTbl->close();
			return layerId;

		}


		AcDbLayerTableRecord *lyerTblRec = new AcDbLayerTableRecord();

		

		lyerTblRec->setName(layerName);

		lyerTblRec->setColor(color);

		lyerTbl->add(layerId, lyerTblRec);

		lyerTblRec->close();
		lyerTbl->close();
	}
	return layerId;
}
