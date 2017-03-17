#include "stdafx.h"
#include "SampleScriptObject.h"

IMPLEMENT_DYNAMIC(CSampleScriptObject, CCmdTarget)

CSampleScriptObject::CSampleScriptObject()
{
  EnableAutomation();
}

CSampleScriptObject::~CSampleScriptObject()
{
}

void CSampleScriptObject::OnFinalRelease()
{
  // When the last reference for an automation object is released
  // OnFinalRelease is called.  The base class will automatically
  // deletes the object.  Add additional cleanup required for your
  // object before calling the base class.
  CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSampleScriptObject, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSampleScriptObject, CCmdTarget)
  DISP_FUNCTION_ID(CSampleScriptObject, "Hello", dispidHello, Hello, VT_BSTR, VTS_NONE)
  DISP_FUNCTION_ID(CSampleScriptObject, "Add", dispidAdd, Add, VT_R8, VTS_R8 VTS_R8)
  DISP_FUNCTION_ID(CSampleScriptObject, "Command", dispidCommand, Command, VT_I2, VTS_BSTR VTS_I2)
END_DISPATCH_MAP()

// Note: we add support for IID_ICSampleScriptObject to support typesafe binding
// from VBA.  This IID must match the GUID that is attached to the 
// dispinterface in the .IDL file.

// {52D524B4-C92F-412E-AAD4-56B879EBAF68}
static const IID IID_ISampleScriptObject =
{ 0x52D524B4, 0xC92F, 0x412E, { 0xAA, 0xD4, 0x56, 0xB8, 0x79, 0xEB, 0xAF, 0x68 } };

BEGIN_INTERFACE_MAP(CSampleScriptObject, CCmdTarget)
  INTERFACE_PART(CSampleScriptObject, IID_ISampleScriptObject, Dispatch)
END_INTERFACE_MAP()

BSTR CSampleScriptObject::Hello()
{
  CString strResult = L"Hello from SampleRhino!";
  return strResult.AllocSysString();
}

DOUBLE CSampleScriptObject::Add(DOUBLE a, DOUBLE b)
{
  return a + b;
}

SHORT CSampleScriptObject::Command(LPCTSTR bstrScript, SHORT nEchoMode)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  ON_wString strScript = bstrScript;
  if (strScript.IsEmpty())
    return 0;

  bool rc = RhinoApp().RunScript(strScript, nEchoMode ? 1 : 0);

  return (rc ? 1 : 0);
}
