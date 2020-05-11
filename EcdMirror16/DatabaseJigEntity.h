#pragma once
class CDatabaseJigEntity :public AcDbEntity
{
public:
	CDatabaseJigEntity(const AcDbObjectIdArray & ids) : m_Ids(ids) {}
	~CDatabaseJigEntity();
	CDatabaseJigEntity();
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);//实时显示改变
	void setXform(const AcGeMatrix3d & xform, AcGePoint3d & pt1, AcGePoint3d &pt2)
	{
		m_Xform = xform;
		m_pt1 = pt1;
		m_pt2 = pt2;

	}
	bool CDatabaseJigEntity::move();
	BOOL transform();
	BOOL transformedCopy();
	void MyMirror(const AcDbObjectId & oId, AcDbObjectId &getId);


	AcDbObjectIdArray m_copyIds;
private:
	AcDbObjectIdArray	m_Ids;	//保存所有拖动对象的ID
	AcGeMatrix3d		m_Xform;//变换矩阵
	AcGePoint3d m_pt1;
	AcGePoint3d m_pt2;
	void ChangeLine(AcDbLine* newLine, AcDbLine*oLine);
};

