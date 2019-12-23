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
	static AcDbObjectId CopyBlockDefFromOtherDwg(const TCHAR *fileName, const TCHAR *blkDefName);
	static AcDbObjectId GetBlkDefId(const TCHAR *blkName, AcDbDatabase * pDb = acdbHostApplicationServices()->workingDatabase());
	static AcDbObjectId InsertDwgBlockDef(const TCHAR * dwgFileName, const
		TCHAR *blkName, bool bOverwriteIfExist, AcDbDatabase *pDb = acdbHostApplicationServices()->workingDatabase());

	
};

