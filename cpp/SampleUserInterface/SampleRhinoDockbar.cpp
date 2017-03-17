#include "stdafx.h"
#include "SampleRhinoDockbar.h"

IMPLEMENT_SERIAL(CSampleRhinoDockbar, CRhinoUiDockBar, 1)

CSampleRhinoDockbar::CSampleRhinoDockbar()
{
}

CSampleRhinoDockbar::~CSampleRhinoDockbar()
{
}

BEGIN_MESSAGE_MAP(CSampleRhinoDockbar, CRhinoUiDockBar)
END_MESSAGE_MAP()

UUID CSampleRhinoDockbar::DockBarID() const
{
  // {308C1F0B-998E-450E-AE7B-3425AE529B9C}
  static const GUID SampleRhinoDockbar_UUID =
  { 0x308c1f0b, 0x998e, 0x450e, { 0xae, 0x7b, 0x34, 0x25, 0xae, 0x52, 0x9b, 0x9c } };
  return SampleRhinoDockbar_UUID;
}

const wchar_t* CSampleRhinoDockbar::DockBarName(int language_id)  const
{
  UNREFERENCED_PARAMETER(language_id);
  return L"Sample";
}

bool CSampleRhinoDockbar::CreateDockBarControls()
{
  // Create the dialog to display in our dockbar
  bool rc = CreateDockBarClientDialog(
    CSampleRhinoDockbarDialog::IDD,
    RUNTIME_CLASS(CSampleRhinoDockbarDialog),
    AfxGetStaticModuleState()
  );

  return rc;
}