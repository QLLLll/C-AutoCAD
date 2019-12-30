// 从类型库向导中用“添加类”创建的计算机生成的 IDispatch 包装器类

#import "C:\\Program Files\\Common Files\\Autodesk Shared\\acax22enu.tlb" no_namespace
// CAcadPopupMenu 包装器类

class CAcadPopupMenu : public COleDispatchDriver
{
public:
	CAcadPopupMenu() {} // 调用 COleDispatchDriver 默认构造函数
	CAcadPopupMenu(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CAcadPopupMenu(const CAcadPopupMenu& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// 特性
public:

	// 操作
public:


	// IAcadPopupMenu 方法
public:
	LPDISPATCH Item(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT;
		InvokeHelper(0x0, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	LPUNKNOWN get__NewEnum()
	{
		LPUNKNOWN result;
		InvokeHelper(0xfffffffc, DISPATCH_PROPERTYGET, VT_UNKNOWN, (void*)&result, NULL);
		return result;
	}
	long get_Count()
	{
		long result;
		InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Application()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020003, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH get_Parent()
	{
		LPDISPATCH result;
		InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	CString get_Name()
	{
		CString result;
		InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Name(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0x60020005, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_NameNoMnemonic()
	{
		CString result;
		InvokeHelper(0x60020007, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	BOOL get_ShortcutMenu()
	{
		BOOL result;
		InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL get_OnMenuBar()
	{
		BOOL result;
		InvokeHelper(0x60020009, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	LPDISPATCH AddMenuItem(VARIANT& Index, LPCTSTR Label, LPCTSTR Macro)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_BSTR VTS_BSTR;
		InvokeHelper(0x6002000a, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index, Label, Macro);
		return result;
	}
	LPDISPATCH AddSubMenu(VARIANT& Index, LPCTSTR Label)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT VTS_BSTR;
		InvokeHelper(0x6002000b, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index, Label);
		return result;
	}
	LPDISPATCH AddSeparator(VARIANT& Index)
	{
		LPDISPATCH result;
		static BYTE parms[] = VTS_VARIANT;
		InvokeHelper(0x6002000c, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, parms, &Index);
		return result;
	}
	void InsertInMenuBar(VARIANT& Index)
	{
		static BYTE parms[] = VTS_VARIANT;
		InvokeHelper(0x6002000d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, &Index);
	}
	void RemoveFromMenuBar()
	{
		InvokeHelper(0x6002000e, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	CString get_TagString()
	{
		CString result;
		InvokeHelper(0x6002000f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}

	// IAcadPopupMenu 属性
public:

};
