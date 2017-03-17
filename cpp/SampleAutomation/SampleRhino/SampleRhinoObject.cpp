/////////////////////////////////////////////////////////////////////////////
// CSampleRhinoObject.cpp : implementation file
//

#include "stdafx.h"
#include "SampleRhinoObject.h"

IMPLEMENT_DYNAMIC(CSampleRhinoObject, CCmdTarget)

CSampleRhinoObject::CSampleRhinoObject()
{
	EnableAutomation();
}

CSampleRhinoObject::~CSampleRhinoObject()
{
}

void CSampleRhinoObject::OnFinalRelease()
{
	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSampleRhinoObject, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSampleRhinoObject, CCmdTarget)
  DISP_FUNCTION_ID(CSampleRhinoObject, "Hello", dispidHello, Hello, VT_BSTR, VTS_NONE)
  DISP_FUNCTION_ID(CSampleRhinoObject, "Add", dispidAdd, Add, VT_R8, VTS_R8 VTS_R8)
  DISP_FUNCTION_ID(CSampleRhinoObject, "Command", dispidCommand, Command, VT_I2, VTS_BSTR VTS_I2)
END_DISPATCH_MAP()

// Note: we add support for IID_ISampleRhinoObject to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {8A843D40-7CB2-4567-BC70-F7743FA5BBCE}
static const IID IID_ISampleRhinoObject =
{ 0x8A843D40, 0x7CB2, 0x4567, { 0xBC, 0x70, 0xF7, 0x74, 0x3F, 0xA5, 0xBB, 0xCE } };

BEGIN_INTERFACE_MAP(CSampleRhinoObject, CCmdTarget)
	INTERFACE_PART(CSampleRhinoObject, IID_ISampleRhinoObject, Dispatch)
END_INTERFACE_MAP()

BSTR CSampleRhinoObject::Hello(void)
{
  CString strResult = L"Hello from SampleRhino!";
  return strResult.AllocSysString();
}

DOUBLE CSampleRhinoObject::Add(DOUBLE a, DOUBLE b)
{
  return a + b;
}

SHORT CSampleRhinoObject::Command(LPCTSTR bstrScript, SHORT nEchoMode)
{
  AFX_MANAGE_STATE( AfxGetStaticModuleState() );

  ON_wString strScript = bstrScript;
  if( strScript.IsEmpty() )
    return 0;

  bool rc = RhinoApp().RunScript( strScript, nEchoMode ? 1 : 0 );

  return ( rc ? 1 : 0 );
}
