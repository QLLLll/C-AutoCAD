#pragma once
class BlockUtil
{
public:
	BlockUtil();
	~BlockUtil();
	static AcDbObjectId CreateBlk(AcDbDatabase * db=acdbHostApplicationServices()->workingDatabase());
	static AcDbObjectId InsertBlk(const ACHAR* blkName, AcDbDatabase * db = acdbHostApplicationServices()->workingDatabase());
	static AcDbObjectId InsertBlk(AcDbObjectId blkId, AcDbDatabase * db = acdbHostApplicationServices()->workingDatabase());
	static AcDbObjectId InsertBlkWidthAttr(AcDbObjectId blkRecId,AcGePoint3d &ptInsert, AcDbDatabase * db = acdbHostApplicationServices()->workingDatabase());
	static void AppendAttrDef2Br(AcDbBlockReference *br, AcDbAttributeDefinition * def);
	static void SetAttribute2Br(AcDbBlockReference *br, const TCHAR *tag, const TCHAR *val);
};

