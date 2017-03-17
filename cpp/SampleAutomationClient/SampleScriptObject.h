// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

//#import "C:\\dev\\github\\mcneel\\Rhino6Samples_CPP\\SampleAutomation\\x64\\Debug\\SampleAutomation.tlb" no_namespace
#import "D:\\dev\\github\\mcneel\\Rhino6Samples_CPP\\SampleAutomation\\x64\\Debug\\SampleAutomation.tlb" no_namespace
// CSampleScriptObject wrapper class

class CSampleScriptObject : public COleDispatchDriver
{
public:
  CSampleScriptObject() {} // Calls COleDispatchDriver default constructor
  CSampleScriptObject(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
  CSampleScriptObject(const CSampleScriptObject& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

  // Attributes
public:

  // Operations
public:


  // ISampleScriptObject methods
public:
  CString Hello()
  {
    CString result;
    InvokeHelper(0x1, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
    return result;
  }
  double Add(double a, double b)
  {
    double result;
    static BYTE parms[] = VTS_R8 VTS_R8;
    InvokeHelper(0x2, DISPATCH_METHOD, VT_R8, (void*)&result, parms, a, b);
    return result;
  }
  short Command(LPCTSTR bstrScript, short nEchoMode)
  {
    short result;
    static BYTE parms[] = VTS_BSTR VTS_I2;
    InvokeHelper(0x3, DISPATCH_METHOD, VT_I2, (void*)&result, parms, bstrScript, nEchoMode);
    return result;
  }

  // ISampleScriptObject properties
public:
};
