#pragma once
class CDatabaseJigEntity:public AcDbEntity
{
public:
	CDatabaseJigEntity(const AcDbObjectIdArray & ids) : m_Ids(ids) {}
	~CDatabaseJigEntity();
	CDatabaseJigEntity();
	virtual Adesk::Boolean subWorldDraw(AcGiWorldDraw *mode);//实时显示改变
	void setXform(const AcGeMatrix3d & xform) { m_Xform = xform; }
	bool CDatabaseJigEntity::move();
	BOOL transform();
	BOOL transformedCopy();
private:
	AcDbObjectIdArray	m_Ids;	//保存所有拖动对象的ID
	AcGeMatrix3d		m_Xform;//变换矩阵
};

