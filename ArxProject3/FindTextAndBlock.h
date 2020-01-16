#pragma once
#include<vector>
using namespace std;
class CFindTextAndBlock
{
public:
	CFindTextAndBlock(ACHAR *tName, ACHAR *lName) :textName(tName), layerName(lName) { type = 1; };
	CFindTextAndBlock(ACHAR *bName, ACHAR *lName,int type) :blockName(bName), layerName(lName) { type = 2; };
	~CFindTextAndBlock();
	void Command();


private:
	void getLayer();
	bool isContainsLayer(ACHAR*lName);

private:

	ACHAR *layerName;
	vector<ACHAR*> vecLayer;
	vector<CString> vecLayer1;
	AcDbObjectId blockId;
	ACHAR *blockName;
	ACHAR *textName;
	int type = 0;//1:²é×Ö·û2£º²é¿é
};

