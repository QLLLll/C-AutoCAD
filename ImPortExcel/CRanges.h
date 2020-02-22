//为MSO.DLL文件所在路径
#import "C:\Program Files\Common Files\microsoft shared\OFFICE16\MSO.DLL" \
rename("RGB", "MSORGB") \
rename("DocumentProperties", "MSODocumentProperties")
using namespace Office;

//为VBE6EXT.OLB文件所在路径
#import "C:\Program Files (x86)\Common Files\microsoft shared\VBA\VBA6\VBE6EXT.OLB"
using namespace VBIDE;

//为EXCEL.EXE文件所在路径
#import "C:\Program Files\Microsoft Office\Office16\EXCEL.EXE" \
rename("DialogBox", "ExcelDialogBox") \
rename("RGB", "ExcelRGB") \
rename("CopyFile", "ExcelCopyFile") \
rename("ReplaceText", "ExcelReplaceText") \
no_auto_exclude
using namespace Excel;

class CRanges : public COleDispatchDriver
{
public:
	CRanges() {} // 调用 COleDispatchDriver 默认构造函数
	CRanges(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CRanges(const CRanges& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// Ranges 方法
public:
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x94, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	long get_Creator()
	{
		long result;
		InvokeHelper(0x95, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x96, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get__Default(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT;
		InvokeHelper(0x0, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Item(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT;
		InvokeHelper(0xaa, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x76, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}

	// Ranges 属性
public:

};
