#pragma once

class CSampleScriptObject : public CCmdTarget
{
  DECLARE_DYNAMIC(CSampleScriptObject)

public:
  CSampleScriptObject();
  virtual ~CSampleScriptObject();
  virtual void OnFinalRelease();

protected:
  DECLARE_MESSAGE_MAP()
  DECLARE_DISPATCH_MAP()
  DECLARE_INTERFACE_MAP()

  BSTR Hello();
  DOUBLE Add(DOUBLE a, DOUBLE b);
  SHORT Command(LPCTSTR bstrScript, SHORT nEchoMode);

  enum
  {
    dispidHello = 1L,
    dispidAdd = 2L,
    dispidCommand = 3L
  };
};


