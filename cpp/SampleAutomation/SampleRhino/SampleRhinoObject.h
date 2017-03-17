/////////////////////////////////////////////////////////////////////////////
// CSampleRhinoObject.h : COM object definition
//

#pragma once

class CSampleRhinoObject : public CCmdTarget
{
	DECLARE_DYNAMIC(CSampleRhinoObject)

public:
	CSampleRhinoObject();
	virtual ~CSampleRhinoObject();
	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

  BSTR Hello(void);
  DOUBLE Add(DOUBLE a, DOUBLE b);
  SHORT Command(LPCTSTR bstrScript, SHORT nEchoMode);

  enum 
  {
    dispidHello = 1L,
    dispidAdd = 2L,
    dispidCommand = 3L
  };
};
