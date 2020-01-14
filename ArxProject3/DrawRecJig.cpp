#include "stdafx.h"
#include "DrawRecJig.h"
#include"GePointUtil.h"
#include"MathUtil.h"
#include"ConvertUtil.h"
#include"DwgDataBaseUtil.h"
DrawRecJig::DrawRecJig()
{
	pl = NULL;
}


DrawRecJig::~DrawRecJig()
{
}

AcEdJig::DragStatus DrawRecJig::sampler()
{
	setUserInputControls((AcEdJig::UserInputControls)(

		AcEdJig::kAccept3dCoordinates |
		AcEdJig::kNoNegativeResponseAccepted |
		AcEdJig::kNullResponseAccepted
		));
	AcGePoint3d ptTemp;

	AcEdJig::DragStatus status = acquirePoint(ptTemp);

	if (ptTemp != ptCurrent) {

		ptCurrent= ptTemp;

	}
	else if (status == DragStatus::kNormal) {

		return AcEdJig::kNoChange;
	}
	return status;


}

bool DrawRecJig::update()
{
	double dist = CConvertUtil::ToPoint2d(ptCenter).distanceTo(CConvertUtil::ToPoint2d(ptCurrent));
	/*for (int i = 0; i < 4; i++)
	{
		double angle = i*CMathUtil::PI()*0.5 + CMathUtil::PI()*0.25;
		AcGePoint2d pt = CGePointUtil::PolarPoint(CConvertUtil::ToPoint2d(ptCenter), angle, dist);
		pl->setPointAt(i, pt);
	}*/

	// 提取两个角点的坐标值
	double x1 = ptCenter.x, x2 = ptCurrent.x;
	double y1 = ptCenter.y, y2 = ptCurrent.y;

	// 计算矩形的角点
	AcGePoint2d ptLeftBottom(min(x1, x2), min(y1, y2));
	AcGePoint2d ptRightBottom(max(x1, x2), min(y1, y2));
	AcGePoint2d ptRightTop(max(x1, x2), max(y1, y2));
	AcGePoint2d ptLeftTop(min(x1, x2), max(y1, y2));

	pl->setPointAt(0, ptLeftBottom);
	pl->setPointAt(1, ptLeftTop);
	pl->setPointAt(2, ptRightTop);
	pl->setPointAt(3, ptRightBottom);



	return true;

}

AcDbEntity * DrawRecJig::entity() const
{
	return pl;
}

bool DrawRecJig::DoIt(AcGePoint3d & ptCenter, AcDbObjectId &plId)
{
	
	this->ptCenter = ptCenter;

	pl = new AcDbPolyline();

	for (int i = 0; i < 4; i++)
	{
		pl->addVertexAt(i, CConvertUtil::ToPoint2d(ptCenter));
	}
	pl->setClosed(true);

	setDispPrompt(L"请拖动鼠标：");

	if (drag()==kNormal) {

		plId=CDwgDataBaseUtil::PostToModelSpace(pl);

		return true;
	}
	else {
		delete pl;
		return false;

	}

}
