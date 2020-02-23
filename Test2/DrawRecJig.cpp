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

Adesk::Boolean DrawRecJig::update()
{
	AcGePoint2d ptCenter2d=CConvertUtil::ToPoint2d(ptCenter);
	AcGePoint2d ptCurrent2d=CConvertUtil::ToPoint2d(ptCurrent);
	AcGeVector2d vecCur=ptCurrent2d-ptCenter2d;
	
	double dist = CConvertUtil::ToPoint2d(ptCenter).distanceTo(CConvertUtil::ToPoint2d(ptCurrent));

	double angle=vecCur.angleTo(recVec);

	if(angle/CMathUtil::PI()*180>90){

		recVec*=-1;

	}
	angle=vecCur.angleTo(recVec);

	double len1=dist*cos(angle);
	AcGePoint2d pt2=ptCenter2d+recVec.normal()*len1;

	AcGePoint2d pt4=ptCurrent2d+recVec.normal()*-len1;

	

	// 提取两个角点的坐标值
	//double x1 = ptCenter.x, x2 = ptCurrent.x;
	//double y1 = ptCenter.y, y2 = ptCurrent.y;

	//// 计算矩形的角点
	//AcGePoint2d ptLeftBottom(min(x1, x2), min(y1, y2));
	//AcGePoint2d ptRightBottom(max(x1, x2), min(y1, y2));
	//AcGePoint2d ptRightTop(max(x1, x2), max(y1, y2));
	//AcGePoint2d ptLeftTop(min(x1, x2), max(y1, y2));
	/*pl->setPointAt(0, ptLeftBottom);
	pl->setPointAt(1, ptLeftTop);
	pl->setPointAt(2, ptRightTop);
	pl->setPointAt(3, ptRightBottom);*/

	pl->setPointAt(0, ptCenter2d);
	pl->setPointAt(1, pt2);
	pl->setPointAt(2, ptCurrent2d);
	pl->setPointAt(3, pt4);


	

	return Adesk::kTrue;
}

AcDbEntity * DrawRecJig::entity() const
{
	return pl;
}

bool DrawRecJig::DoIt(AcGePoint3d & ptCenter,AcGeVector2d recVec2d, AcDbObjectId &plId)
{
	this->recVec=recVec2d;
	this->ptCenter = ptCenter;

	pl = new AcDbPolyline();

	for (int i = 0; i < 4; i++)
	{
		pl->addVertexAt(i, CConvertUtil::ToPoint2d(ptCenter));
	}
	pl->setClosed(true);

	setDispPrompt(L"请拖动鼠标：");

	if (drag()==kNormal) {

		//加入到模型空间的封装
		plId=CDwgDataBaseUtil::PostToModelSpace(pl);
		pl->close();
		return true;
	}
	else {
		delete pl;
		return false;

	}

}
