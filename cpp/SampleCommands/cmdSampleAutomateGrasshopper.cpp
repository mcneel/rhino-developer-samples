#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleAutomateGrasshopper command
//

#pragma region SampleAutomateGrasshopper command

/*
Description:
  Generic function to call an OLE method or to set/get an OLE property.
Parameters:
  dwFlag     - [in] Type of call to make, which can be any of the following values:
                      DISPATCH_PROPERTYPUT - Set property value
                      DISPATCH_PROPERTYGET - Get property value
                      DISPATCH_METHOD - Call a method
  pvResult   – [in] Return value for the call made, which can be another IDispatch object,
                    or an integer value, or a boolean, or so on...
  lpDispatch – [in] IDispatch interface object for which the call is to be made.
  lpName     – [in] Property or method name.
  cArgs      – [in] Number of arguments followed after this parameter.
  ...        - [in] Parameters in reverse order for the call (it can be values of a property,
                    or parameters of a method for the IDispatch object).
Returns:
  The result of the the call as an HRESULT.
Remarks:
  The call is reposible for any memory allocated and returned in pvResult.
*/
HRESULT OLEMethod(WORD dwFlag, VARIANT* pvResult, LPDISPATCH lpDispatch, LPOLESTR lpName, int cArgs...)
{
  if (0 == lpDispatch)
    return E_FAIL;

  va_list marker;
  va_start(marker, cArgs);

  DISPPARAMS dp = { NULL, NULL, 0, 0 };
  DISPID dispidNamed = DISPID_PROPERTYPUT;
  DISPID dispID = 0;

  // Get DISPID for name passed
  HRESULT hr = lpDispatch->GetIDsOfNames(IID_NULL, &lpName, 1, LOCALE_USER_DEFAULT, &dispID);
  if (FAILED(hr))
    return hr;

  // Allocate memory for arguments
  VARIANT* pArgs = new VARIANT[cArgs + 1];

  // Extract arguments
  for (int i = 0; i < cArgs; i++)
    pArgs[i] = va_arg(marker, VARIANT);

  // Build DISPPARAMS
  dp.cArgs = cArgs;
  dp.rgvarg = pArgs;

  // Handle special-case for property-puts
  if (dwFlag & DISPATCH_PROPERTYPUT)
  {
    dp.cNamedArgs = 1;
    dp.rgdispidNamedArgs = &dispidNamed;
  }

  // Make the call
  hr = lpDispatch->Invoke(dispID, IID_NULL, LOCALE_SYSTEM_DEFAULT, dwFlag, &dp, pvResult, NULL, NULL);

  // End variable-argument section
  va_end(marker);

  // Don't leak...
  delete[] pArgs;

  return hr;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleAutomateGrasshopper : public CRhinoCommand
{
public:
  CCommandSampleAutomateGrasshopper() {}
  ~CCommandSampleAutomateGrasshopper() {}
  UUID CommandUUID()
  {
    // {AAA95B6-8F9E-4F81-9E3E-10F786382C11}
    static const GUID SampleAutomateGrasshopperCommand_UUID =
    { 0xAAA95B6, 0x8F9E, 0x4F81, { 0x9E, 0x3E, 0x10, 0xF7, 0x86, 0x38, 0x2C, 0x11 } };
    return SampleAutomateGrasshopperCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleAutomateGrasshopper"; }
  const wchar_t* LocalCommandName() const { return L"SampleAutomateGrasshopper"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleAutomateGrasshopper object
static class CCommandSampleAutomateGrasshopper theSampleAutomateGrasshopperCommand;

CRhinoCommand::result CCommandSampleAutomateGrasshopper::RunCommand(const CRhinoCommandContext& context)
{
  // Grasshopper's plug-in UUID
  static const wchar_t* uuid_str = L"b45a29b1-4343-4035-989e-044e8580d9cf";
  ON_UUID uuid = ON_UuidFromString(uuid_str);

  // Verify that Grasshopper is installed
  bool bGrasshopperInstalled = false;
  ON_ClassArray<CRhinoPlugInRecord> plugin_records(CRhinoPlugIn::PlugInCount());
  if (CRhinoPlugIn::GetPlugInList(plugin_records, (int)CRhinoPlugIn::utility_plugin))
  {
    for (int i = 0; i < plugin_records.Count(); i++)
    {
      if (uuid == plugin_records[i].PlugInUUID())
      {
        bGrasshopperInstalled = true;
        break;
      }
    }
  }

  if (!bGrasshopperInstalled)
  {
    RhinoApp().Print(L"Grasshopper not installed.\n");
    return CRhinoCommand::cancel;
  }


  // Prompt the user for a Grasshopper solution to open
  CString filename;

  if (context.IsInteractive())
  {
    DWORD dwFlags = OFN_ENABLESIZING | OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
    const wchar_t* szFilter = L"Grasshopper Files (*.gh;*.ghx)|*.gh; *.ghx||";
    CWnd* pParentWnd = CWnd::FromHandle(RhinoApp().MainWnd());

    CFileDialog dialog(TRUE, L"gh", 0, dwFlags, szFilter, pParentWnd, 0, TRUE);
    INT_PTR rc = dialog.DoModal();
    if (rc != IDOK)
      return CRhinoCommand::cancel;

    filename = dialog.GetPathName();
  }
  else
  {
    CRhinoGetString gs;
    gs.SetCommandPrompt(L"Grasshopper file to open");
    gs.GetString();
    if (gs.CommandResult() != CRhinoCommand::success)
      return gs.CommandResult();

    filename = gs.String();
    filename.TrimLeft();
    filename.TrimRight();
    if (filename.IsEmpty())
      return CRhinoCommand::nothing;
  }

  if (!CRhinoFileUtilities::FileExists(filename))
  {
    RhinoApp().Print(L"File \"%s\" not found.\n", filename);
    return CRhinoCommand::failure;
  }


  // This should probably go in CSampleCommandsPlugIn::OnLoadPlugIn...
  CoInitializeEx(0, COINIT_APARTMENTTHREADED);


  // Get Grasshopper's IDispatch object
  CRhinoCommand::result rc = CRhinoCommand::failure;
  LPDISPATCH lpDispatch = (LPDISPATCH)RhinoApp().GetPlugInObjectInterface(uuid, IID_IDispatch);
  if (0 != lpDispatch)
  {
    // Invoke the "OpenDocument" method
    COleVariant vaResult;
    COleVariant vaFileName(filename);
    HRESULT hr = OLEMethod(DISPATCH_METHOD, &vaResult, lpDispatch, L"OpenDocument", 1, vaFileName);

    // Interpret results
    if (SUCCEEDED(hr))
    {
      if (vaResult.vt == VT_BOOL && vaResult.boolVal == VARIANT_TRUE)
        rc = CRhinoCommand::success;
    }

    // Release the object
    lpDispatch->Release();
  }


  // This should probably go inCSampleCommandsPlugIn::OnUnloadPlugIn...
  CoUninitialize();

  return rc;
}

#pragma endregion

//
// END SampleAutomateGrasshopper command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
