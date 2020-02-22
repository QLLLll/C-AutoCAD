#include "StdAfx.h"
#include "Lines.h"
#include <algorithm>

//构建实体邻接表
//void Lines::CreateList(LGraph &G,AcDbObjectIdArray objIds){
//	int length=objIds.length();
//	G.vexnum=length;//顶点数
//	//G.edgenum=length;//边数
//	AcDbLine *pLine=NULL;
//	AcDbLine *pNextLine=NULL;
//	AcGePoint3dArray ArsNodPt; // 交点数组
//	AcDbObjectId IdCurr,IdNext;
//	AcDbCurve* pLineNext = NULL; // 目标line
//	for(int i=0;i<length;i++){
//		G.vexs[i].vex=objIds[i];
//		G.vexs[i].firstedge=NULL;
//	}
//	for(int i=0; i<G.vexnum; i++){
//		IdCurr=objIds[i];
//		acdbOpenObject(pLine, IdCurr, AcDb::kForRead);
//		for(int j=0;j<G.vexnum;j++){
//			IdNext=objIds[j];
//			acdbOpenObject(pNextLine, IdNext, AcDb::kForRead);
//			pLineNext = AcDbCurve::cast(pNextLine);
//			//获取交点
//			ArsNodPt.setLogicalLength(0);
//			pLine->intersectWith(pLineNext, AcDb::kOnBothOperands, ArsNodPt);
//			int iNum = ArsNodPt.length();
//			if(i!=j && iNum>0){
//				EdgeNode *p;
//				p=new EdgeNode;
//				p->adjvex=j;
//				p->nextedge=G.vexs[i].firstedge;
//				G.vexs[i].firstedge=p;
//			}	
//			else 
//				continue;
//		}
//
//	}
//	pLine->close();
//	pLineNext->close();
//	pNextLine->close();
//}

//AcDbObjectIdArray Lines::FindMaxArea(AcDbObjectIdArray objIds,AcGePoint3dArray &q) 
//{  
//	AcGePoint3dArray stack1;
//	//AcGePoint3dArray q;
//	int i,k=0;  //k存最左下角的点在数组的位置
//	int length=q.length();
//	for(i=0;i<length;i++)  
//	{  
//		if(q[i].y<q[k].y || (q[i].y==q[k].y && q[i].x<q[k].x)) //找y最小的点，y相等找x最小的点  
//			k=i;  
//	}  
//	AcGePoint3d tep;  
//	tep=q[0];  
//	q[0]=q[k];  
//	q[k]=tep; 
//	pointMin=q[0];
//	sort(q);  
//
//	stack1.append(q[0]);  
//	stack1.append(q[1]);  
//	stack1.append(q[2]);  
//	int top;  //top为vector的最大值位置
//	for(i=3;i<length;i++)  
//	{  
//		top=stack1.length()-1;
//		if(top>1 && mult(q[i],stack1[top],stack1[top-1])>=0)  //是否左转，是继续判断，不是，就入栈  
//		{
//			stack1[top]=(q[i]);
//		}
//		else 
//			stack1.append(q[i]);  
//	}  
//
//	AcDb3dPolyline *pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, stack1);
//
//	AcDbObjectId pPoly3dId=PostToModelSpace(pPoly3d);
//	AcDbObjectIdArray a;
//	a.append(pPoly3dId);
//	return a;
//}  

//void Lines::BreakLine()
//{
//	// 提示用户选择填充边界
//	ads_name ss;
//	int rt = acedSSGet(L"A", NULL, NULL, NULL, ss); 
//	AcDbObjectIdArray objIds;
//
//	// 初始化填充边界的ID数组
//	if (rt == RTNORM)
//	{
//		long length; acedSSLength(ss, &length); for (int i = 0; i < length; i++)
//		{
//			ads_name ent; acedSSName(ss, i, ent); AcDbObjectId objId; acdbGetObjectId(objId, ent);
//
//			objIds.append(objId);
//		}
//	}
//	acedSSFree(ss);	// 释放选择集
//	acutPrintf(L"\n初始创建了%d条直线",objIds.length());
//
//	AcDbLine *pLine=NULL;
//	AcDbObjectId IdCur;
//	// 主循环
//	for(int i=0;i<objIds.length();i++)
//	{	
//		acutPrintf(L"\n1");
//		IdCur=objIds[i];
//		AcDbEntity* pEnt = NULL; // 目标实体
//		AcDbCurve* pLineNext = NULL; // 目标line
//		AcDbObjectId IdNext; // 目标实体ID
//		AcGePoint3dArray ArsNodPt; // 交点数组
//		// 获取当前线
//		acdbOpenObject(pLine, IdCur, AcDb::kForWrite);
//		int length =objIds.length();
//		// 判断当前线是否有效
//		if(pLine)
//		{// 过滤、求交点、打断处理
//			acutPrintf(L"\n2");
//			for(int j=0;j < length;j++){
//				acutPrintf(L"\n3  %d   %d",objIds.length(),j);
//				if(j == i)
//					continue;
//				acutPrintf(L"\n4  %d   %d",objIds.length(),j);
//				IdNext=objIds[j];
//				acdbOpenObject(pEnt, IdNext, AcDb::kForRead);
//				if(pEnt)
//				{
//					acutPrintf(L"\n5");
//					// 直线属于曲线,直接转换
//					pLineNext = AcDbCurve::cast(pEnt);
//					if(pLineNext)
//					{
//						acutPrintf(L"\n6");
//						//获取交点
//						ArsNodPt.setLogicalLength(0);
//						pLine->intersectWith(pLineNext, AcDb::kOnBothOperands, ArsNodPt);
//						pEnt->close();
//						pLineNext->close();
//						//有交点处理
//						int iNum = ArsNodPt.length();
//						if(iNum > 0)
//						{
//							acutPrintf(L"\niNum=%d",iNum);
//							// 开始点，结束点，打断点
//							AcGePoint3d AptS,AptE, AptB; // 弧线打断始末点
//							double Alen; // 一段线长 
//							//pLine->getDistAtPoint(ArsNodPt[0], Alen); // 获取线起点到交点距离
//							//pLine->getPointAtDist(Alen, AptB); // 获取线打断点
//							AptB=ArsNodPt[0];
//							pLine->getEndPoint(AptE);
//							pLine->getStartPoint(AptS);
//							if(AptE != AptB  && AptS != AptB){
//								acutPrintf(L"\n8");
//								//AcDbLine *pClone=AcDbLine::cast(pLine->clone());
//								AcDbLine *pClone =new AcDbLine(AptB,AptE);
//								pLine->setEndPoint(AptB);
//								//pClone->setStartPoint(AptB);
//								AcDbObjectId pCloneLineId;
//								pCloneLineId = this->PostToModelSpace(pClone);
//								objIds.append(pCloneLineId);
//							}
//						}
//					}
//					else
//						pLineNext->close();
//				}	
//				else
//					pEnt->close();
//			}
//		}	
//		pLine->close();
//	}
//	acutPrintf(L"\n打断后共分为%d条直线",objIds.length());
//}

bool cmpLine(AcGePoint3d q1,AcGePoint3d q2,AcDbObjectIdArray objIds){
	int length=objIds.length();
	AcDbLine *pLine=NULL;
	AcGePoint3d AptS,AptE;
	for(int i=0;i<length;i++){
		AcDbObjectId IdCur=objIds[i];
		acdbOpenObject(pLine, IdCur, AcDb::kForRead);
		pLine->getStartPoint(AptS);
		pLine->getEndPoint(AptE);
		if((q1==AptE && q2==AptS)||(q1==AptS && q2==AptE)){
			pLine->close();
			return true;
		}
	}
	pLine->close();
	return false;
}
void Lines::lineChangeColor(AcDbObjectIdArray objIds){
	int length=objIds.length();
	for(int i=0; i<length; i++){

		// 打开图形数据库中的对象
		AcDbEntity *pEntity;
		acdbOpenObject(pEntity,objIds[i], AcDb::kForWrite);

		// 修改实体的颜色
		//十六无符号数
		Adesk::UInt16 colorIndex=10;
		pEntity->setColorIndex(colorIndex);

		// 用完之后，及时关闭
		pEntity->close();
	}
}


Lines::Lines(void)
{
}



Lines::~Lines(void)
{
}



AcDbObjectIdArray Lines::CreateRegion(const AcDbObjectIdArray& curveIds,double &area)
{
	acutPrintf(L"\n创建面域");
	AcDbObjectIdArray regionIds;	// 生成的面域的ID数组
	AcDbVoidPtrArray curves;	// 指向作为面域边界的曲线的指针的数组
	AcDbVoidPtrArray regions;		// 指向创建的面域对象的指针的数组 
	AcDbEntity *pEnt;		// 临时指针，用来关闭边界曲线 
	AcDbRegion *pRegion;	// 临时对象，用来将面域添加到模型空间

	// 用curveIds初始化curves

	for (int i = 0; i < curveIds.length(); i++)
	{
		acdbOpenAcDbEntity(pEnt, curveIds.at(i), AcDb::kForRead); 
		if (pEnt->isKindOf(AcDbCurve::desc()))
		{
			curves.append(static_cast<void*>(pEnt));
		}
	}

	Acad::ErrorStatus es = AcDbRegion::createFromCurves(curves,regions);

	if (es == Acad::eOk)
	{
		// 将生成的面域添加到模型空间
		for (int i = 0; i < regions.length(); i++)
		{
			// 将空指针（可指向任何类型）转化为指向面域的指针
			pRegion = static_cast<AcDbRegion*>(regions[i]); 
			pRegion->setDatabaseDefaults();
			AcDbObjectId regionId;
			regionId = this->PostToModelSpace(pRegion);//Lines::
			regionIds.append(regionId);
		}
	}
	else	// 如果创建不成功，也要删除已经生成的面域
	{
		for (int i = 0; i < regions.length(); i++)
		{
			delete (AcRxObject*)regions[i];
		}
	}
	pRegion->getArea(area);
	// 关闭作为边界的对象
	for (int i = 0; i < curves.length(); i++)
	{
		pEnt = static_cast<AcDbEntity*>(curves[i]); 
		pEnt->close();
	}
	return regionIds;
}
void Lines::AddRegion(AcDbObjectIdArray objIds){
	acutPrintf(L"\n添加面域");
	acutPrintf(L"\n选择了%d个对象", objIds.length());
	double area=0.0;
	AcDbObjectIdArray regionIds;
	regionIds = Lines::CreateRegion(objIds,area);

	//也可不用选择集来创建AcDbObjectIdArray
	//regionIds=Lines::CreateRegion(this->getObjIds());

	int number = regionIds.length(); 
	
	if (number > 0)
	{
		
		acutPrintf(L"\n已经创建%d个面域,面积为%f", number,area);
	}
	else
	{
		acutPrintf(L"\n创建0个面域！", number);
	}

}
AcDbObjectIdArray Lines::CreateTestHash3(){
	float ptStartX1=500;
	float ptStartY1=500;

	float ptStartX2=1500;
	float ptStartY2=500;

	float ptStartX3=1500;
	float ptStartY3=1500;

	float ptStartX4=500;
	float ptStartY4=1500;

	AcGePoint3d pt1(ptStartX1,ptStartY1,0);
	AcGePoint3d pt2(ptStartX2,ptStartY2,0);
	AcGePoint3d pt3(ptStartX3,ptStartY3,0);	
	AcGePoint3d pt4(ptStartX4,ptStartY4,0);

	AcDbLine *pLine1 =new AcDbLine(pt1,pt2);
	AcDbLine *pLine2=new AcDbLine(pt3,pt2);
	AcDbLine *pLine3=new AcDbLine(pt4,pt3);
	AcDbLine *pLine4=new AcDbLine(pt4,pt1);
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable,AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord,AcDb::kForWrite);

	AcDbObjectId lineId;
	AcDbObjectIdArray objIds;
	pBlockTableRecord->appendAcDbEntity(lineId,pLine1);
		objIds.append(lineId);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine2);
		objIds.append(lineId);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine3);
		objIds.append(lineId);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine4);
	objIds.append(lineId);
	pBlockTable->close();
	pBlockTableRecord->close();
	pLine1->close();
	pLine2->close();
	pLine3->close();
	pLine4->close();
	return objIds;
}
void Lines::CreateTestHash(){
	float ptStartX1=1100;
	float ptStartY1=1000;

	float ptStartX2=600;
	float ptStartY2=2000;

	float ptStartX5=700;
	float ptStartY5=1200;

	float ptStartX6=1200;
	float ptStartY6=1600;

	float ptStartX7=1300;
	float ptStartY7=1800;

	float ptStartX8=2000;
	float ptStartY8=1300;

	float ptStartX9=1400;
	float ptStartY9=1300;

	float ptStartX10=2000;
	float ptStartY10=2000;

	AcGePoint3d pt1(ptStartX1,ptStartY1,0);
	AcGePoint3d pt2(ptStartX2,ptStartY2,0);
	/*AcGePoint3d pt3(ptStartX3,ptStartY3,0);	
	AcGePoint3d pt4(ptStartX4,ptStartY4,0);
	*/AcGePoint3d pt5(ptStartX5,ptStartY5,0);
	AcGePoint3d pt6(ptStartX6,ptStartY6,0);	
	AcGePoint3d pt7(ptStartX7,ptStartY7,0);
	AcGePoint3d pt8(ptStartX8,ptStartY8,0);	
	AcGePoint3d pt9(ptStartX9,ptStartY9,0);
	AcGePoint3d pt10(ptStartX10,ptStartY10,0);	

	AcDbLine *pLine1 =new AcDbLine(pt1,pt2);
	AcDbLine *pLine3 =new AcDbLine(pt5,pt6);
	AcDbLine *pLine4 =new AcDbLine(pt7,pt8);
	AcDbLine *pLine5=new AcDbLine(pt9,pt10);

	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable,AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord,AcDb::kForWrite);
	
	AcDbObjectId lineId;
	
	pBlockTableRecord->appendAcDbEntity(lineId,pLine1);
	//pBlockTableRecord->appendAcDbEntity(lineId,pLine2);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine3);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine4);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine5);
	pBlockTable->close();
	pBlockTableRecord->close();
	pLine1->close();
//	pLine2->close();
	pLine3->close();
	pLine4->close();
	pLine5->close();

}
void Lines::CreateTestHash2(){


	AcGePoint3d pt1(500,500,0);
	AcGePoint3d pt2(2000,500,0);
	AcGePoint3d pt3(2000,2000,0);	
	AcGePoint3d pt4(500,2000,0);
	AcGePoint3d pt5(2500,1000,0);
	AcGePoint3d pt6(2500,500,0);
	AcGePoint3d pt7(1800,2000,0);
	AcGePoint3d pt8(1800,500,0);
	AcGePoint3d pt9(500,600,0);
	AcGePoint3d pt10(2600,600,0);
	AcGePoint3d pt11(500,550,0);
	AcGePoint3d pt12(2600,550,0);
	AcGePoint3d pt13(500,1800,0);
	AcGePoint3d pt14(2500,400,0);
	AcGePoint3d pt15(1700,2000,0);
	AcGePoint3d pt16(1700,500,0);
	AcGePoint3d pt17(1600,2000,0);
	AcGePoint3d pt18(1600,500,0);
	AcDbLine *pLine1 =new AcDbLine(pt1,pt3);
	AcDbLine *pLine2=new AcDbLine(pt2,pt4);
	AcDbLine *pLine3=new AcDbLine(pt5,pt6);
	AcDbLine *pLine4=new AcDbLine(pt7,pt8);
	AcDbLine *pLine5=new AcDbLine(pt1,pt6);
	AcDbLine *pLine6=new AcDbLine(pt9,pt10);
	AcDbLine *pLine7=new AcDbLine(pt11,pt12);
	AcDbLine *pLine8=new AcDbLine(pt13,pt14);
	AcDbLine *pLine9=new AcDbLine(pt15,pt16);
	AcDbLine *pLine10=new AcDbLine(pt17,pt18);
	AcDbBlockTable *pBlockTable;
	acdbHostApplicationServices()->workingDatabase()->getBlockTable(pBlockTable,AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord,AcDb::kForWrite);
	
	AcDbObjectId lineId;
	
	pBlockTableRecord->appendAcDbEntity(lineId,pLine1);

	pBlockTableRecord->appendAcDbEntity(lineId,pLine2);

	pBlockTableRecord->appendAcDbEntity(lineId,pLine3);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine4);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine5);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine6);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine7);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine8);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine9);
	pBlockTableRecord->appendAcDbEntity(lineId,pLine10);

	pBlockTable->close();
	pBlockTableRecord->close();
	pLine1->close();
	pLine2->close();
	pLine3->close();
	pLine4->close();
	pLine5->close();
	pLine6->close();
	pLine7->close();
	pLine8->close();
	pLine9->close();
	pLine10->close();

}


AcDbObjectId Lines::PostToModelSpace(AcDbEntity* pEnt)
{
	AcDbBlockTable *pBlockTable; 
	acdbHostApplicationServices()->workingDatabase()
		->getBlockTable(pBlockTable, AcDb::kForRead);

	AcDbBlockTableRecord *pBlockTableRecord;
	pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord,
		AcDb::kForWrite);

	AcDbObjectId entId;

	pBlockTableRecord->appendAcDbEntity(entId, pEnt);

	pBlockTable->close(); 
	pBlockTableRecord->close();
	pEnt->close();

	return entId;
}


AcDbObjectId Lines::CreateHatch(AcDbObjectIdArray objIds,const char* patName, bool bAssociative)
{
	acutPrintf(L"\n创建填充");
	Acad::ErrorStatus es;
	AcDbHatch *pHatch = new AcDbHatch();

	// 设置填充平面
	AcGeVector3d normal(0, 0, 1); 
	pHatch->setNormal(normal);

	pHatch->setElevation(0);

	// 设置关联性
	pHatch->setAssociative(bAssociative);

	// 设置填充图案
	pHatch->setPattern(AcDbHatch::kPreDefined, _T("solid"/*"ANSI31"*/));

	// 添加填充边界

	es = pHatch->appendLoop(AcDbHatch::kExternal, objIds);

	// 显示填充对象
	es = pHatch->evaluateHatch();

	// 添加到模型空间
	AcDbObjectId hatchId;
	hatchId = this->PostToModelSpace(pHatch);

	

	// 如果是关联性的填充，将填充对象与边界绑定，以便使其能获得边界对象修改的通知
	if (bAssociative)
	{
		AcDbEntity *pEnt;
		for (int i = 0; i < objIds.length(); i++)
		{
			es = acdbOpenAcDbEntity(pEnt, objIds[i],AcDb::kForWrite);
			if (es == Acad::eOk)
			{
				// 添加一个永久反应器
				pEnt->addPersistentReactor(hatchId); 
				pEnt->close();
			}
		}
	}
	double a=666;
	pHatch->getArea(a);
	acutPrintf(L"\n填充面积2为%f",a);
	return hatchId;
}

void Lines::AddHatch(AcDbObjectIdArray objIds)
{
	acutPrintf(L"\n添加填充");
	this->CreateHatch(objIds, "SOLID", true);
}

AcDbObjectIdArray Lines::BreakLine()
{
	ads_name ss;
	int rt = acedSSGet(L"A", NULL, NULL, NULL, ss); 
	AcDbObjectIdArray objIds;
	if (rt == RTNORM)
	{
		long length; 
		acedSSLength(ss, &length); 
		for (int i = 0; i < length; i++)
		{
			ads_name ent; 
			acedSSName(ss, i, ent); 
			AcDbObjectId objId; 
			acdbGetObjectId(objId, ent);
			objIds.append(objId);
		}
	}
	acedSSFree(ss);	// 释放选择集
	acutPrintf(L"\n初始创建了%d条直线",objIds.length());

	AcDbLine *pLine=NULL;
	AcDbObjectId IdCur;
	for(int i=0;i<objIds.length();i++)
	{	
		IdCur=objIds[i];
		AcDbEntity* pEnt = NULL; // 目标实体
		AcDbCurve* pLineNext = NULL; // 目标line
		AcDbObjectId IdNext; // 目标实体ID
		AcGePoint3dArray ArsNodPt; // 交点数组
		acdbOpenObject(pLine, IdCur, AcDb::kForWrite);
		int length =objIds.length();
		if(pLine)
		{
			for(int j=0;j < length;j++){
				if(j == i)
					continue;
				IdNext=objIds[j];
				acdbOpenObject(pEnt, IdNext, AcDb::kForRead);
				if(pEnt)
				{
					// 直线属于曲线,直接转换
					pLineNext = AcDbCurve::cast(pEnt);
					if(pLineNext)
					{
						//获取交点
						ArsNodPt.setLogicalLength(0);
						pLine->intersectWith(pLineNext, AcDb::kOnBothOperands, ArsNodPt);
						pEnt->close();
						pLineNext->close();
						//有交点处理
						int iNum = ArsNodPt.length();
						if(iNum > 0)
						{
							// 开始点，结束点，打断点
							AcGePoint3d AptS,AptE, AptB; // 弧线打断始末点
							AptB=ArsNodPt[0];
							pLine->getEndPoint(AptE);
							pLine->getStartPoint(AptS);
							//currPointsArray.append(AptB);
							if(AptE != AptB  && AptS != AptB){
								AcDbLine *pClone =new AcDbLine(AptB,AptE);
								pLine->setEndPoint(AptB);
								AcDbObjectId pCloneLineId;
								pCloneLineId = this->PostToModelSpace(pClone);
								objIds.append(pCloneLineId);
							}
						}
					}
					else
						pLineNext->close();
				}	
				else
					pEnt->close();
			}
		}	
		pLine->close();
	}
	acutPrintf(L"\n打断后共分为%d条直线",objIds.length());
	return objIds;
}


void Lines::optimizePointsArray(AcGePoint3dArray &currPointsArray){
	//去重交点数组
	int nums=currPointsArray.length();
	for(int i=0;i<nums;i++){
		for(int j=i+1;j<nums;j++){
			if(currPointsArray[i]==currPointsArray[j]){
				currPointsArray.removeAt(j);
				--nums;
				--j;
			}
		}
	}
}
bool cmp1(AcGePoint3d p1,AcGePoint3d p2)   //找到第一个点，返回 true->p1比较小，false->p2比较小
{  
	if(p1.y==p2.y)  
		return p1.x<p2.x;  
	else 
		return p1.y<p2.y; 
} 
void selectMinPoint(AcGePoint3dArray &s){
	int nums=s.length();
	int cur=0;
	AcGePoint3d Min = s[0];
	for(int i=1;i<nums;i++){
		if(cmp1(s[i],Min)){
			Min=s[i];
			cur=i;
		}
	}
	s[cur] = s[0];
	s[0]=Min;
	acutPrintf(L"\n寻找后最小点为x=%f,y=%f",s[0].x,s[0].y);
}

long long dist(AcGePoint3d p1,AcGePoint3d p2) //两点距离的平方  
{  
	return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);  
}  
long long Multiply(AcGePoint3d p1,AcGePoint3d p2,AcGePoint3d p0)  //叉积，是否左转
{  //p0为“原点”
	//二维向量的叉积(x1*y2-x2*y1)
	return (p1.x-p0.x)*(p2.y-p0.y)-(p2.x-p0.x)*(p1.y-p0.y);  
}
bool cmp2(AcGePoint3d p1,AcGePoint3d p2,AcGePoint3d pointMin)   //极角排序的比较函数  
{  
	if(Multiply(p1,p2,pointMin)>0)  
		return true; //证明 p1小 选中p1
	else if(Multiply(p1,p2,pointMin)==0 && dist(p1,pointMin)<dist(p2,pointMin))  //相等的按距离近的  
		return true; //证明 p1小 选中p1
	return false;  
}

void quickSort(AcGePoint3dArray &s, int l, int r){
	if (l< r)
	{
		int i = l, j = r;
		AcGePoint3d x = s[l];
		while (i < j)
		{
			while (i < j && cmp2(x,s[j],s[0])) // 从右向左找第一个小于x的数
				j--;
			if (i < j)
				s[i++] = s[j];
			while (i < j && cmp2(s[i],x,s[0])) // 从左向右找第一个大于等于x的数
				i++;
			if (i < j)
				s[j--] = s[i];
		}
		s[i] = x;
		quickSort(s, l, i - 1); // 递归调用
		quickSort(s, i + 1, r);
	}
}
void Lines::FindTuBao() 
{  
	AcDbObjectIdArray objIds;
	ads_name ss;
	int rt = acedSSGet(L"A", NULL, NULL, NULL, ss); 
	if (rt == RTNORM)
	{
		long length; 
		acedSSLength(ss, &length); 
		for (int i = 0; i < length; i++)
		{
			ads_name ent; 
			acedSSName(ss, i, ent); 
			AcDbObjectId objId; 
			acdbGetObjectId(objId, ent);
			objIds.append(objId);
		}
	}
	acedSSFree(ss);	// 释放选择集
	int nums=objIds.length();

	AcGePoint3dArray pointsArray;
	AcDbLine *pLine=NULL;
	AcDbObjectId IdCur;
	AcGePoint3d AptS,AptE;
	int i;
	for(i=0;i<nums;i++){	
		IdCur=objIds[i];
		acdbOpenObject(pLine, IdCur, AcDb::kForRead);
		pLine->getStartPoint(AptS);
		pLine->getEndPoint(AptE);
		pointsArray.append(AptS);
		pointsArray.append(AptE);
	}
	pLine->close();
	optimizePointsArray(pointsArray);
	nums=pointsArray.length();
	if(nums>3){
		selectMinPoint(pointsArray);//寻找最小的点
		quickSort(pointsArray,1,nums-1);//用来根据极角排序点
		AcGePoint3dArray pArray;
		pArray.append(pointsArray[0]);
		pArray.append(pointsArray[1]);
		pArray.append(pointsArray[2]);
		int top=pArray.length()-1;
		for(i=3;i<nums;i++)  
		{  
			while(Multiply(pointsArray[i],pArray[top],pArray[top-1])>=0)  //是否左转，是继续判断，不是，就入栈  
			{
				pArray.removeLast();
				top=pArray.length()-1;  
			}
			pArray.append(pointsArray[i]);
			
			top=pArray.length()-1;
		} 
		/*AcDb3dPolyline *pPoly3d = new AcDb3dPolyline(AcDb::k3dSimplePoly, pArray);
		PostToModelSpace(pPoly3d);
		
		pPoly3d->close();
		AcDbLine *lastLine=new AcDbLine(pArray[0],pArray[pArray.length()-1]);
		PostToModelSpace(lastLine);
		lastLine->close();*/
		AcDbObjectIdArray actuObjIds;
		AcDbObjectId lineId;
		AcDbLine *lastLine=NULL;
		for(int i=1;i<pArray.length();i++){
			lastLine=new AcDbLine(pArray[i-1],pArray[i]);
			lineId=PostToModelSpace(lastLine);
			actuObjIds.append(lineId);
		}
		lastLine=new AcDbLine(pArray[0],pArray[pArray.length()-1]);
		lineId=PostToModelSpace(lastLine);
		actuObjIds.append(lineId);
		lastLine->close();
		AddHatch(actuObjIds);
	}
}  