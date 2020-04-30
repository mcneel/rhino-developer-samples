#include "StdAfx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleModelessCancelPlugIn.h"
#include "Resource.h"

#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#pragma warning(pop)

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
RHINO_PLUG_IN_NAME(L"SampleModelessCancel");

// Rhino plug-in id
RHINO_PLUG_IN_ID(L"43667D36-3425-43DA-833E-635688510A9B");

// Rhino plug-in version
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
RHINO_PLUG_IN_DESCRIPTION(L"SampleModelessCancel plug-in for Rhinoceros®");

// Rhino plug-in icon resource id
RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_ICON);

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/rhino-developer-samples");

// The one and only CSampleModelessCancelPlugIn object
static class CSampleModelessCancelPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleModelessCancelPlugIn definition

CSampleModelessCancelPlugIn& SampleModelessCancelPlugIn()
{
  return thePlugIn;
}

CSampleModelessCancelPlugIn::CSampleModelessCancelPlugIn()
  : m_window_message(0)
{
  m_plugin_version = RhinoPlugInVersion();
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleModelessCancelPlugIn::PlugInName() const
{
  return RhinoPlugInName();
}

const wchar_t* CSampleModelessCancelPlugIn::PlugInVersion() const
{
  return m_plugin_version;
}

GUID CSampleModelessCancelPlugIn::PlugInID() const
{
  // {43667D36-3425-43DA-833E-635688510A9B}
  return ON_UuidFromString(RhinoPlugInId());
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides

BOOL CSampleModelessCancelPlugIn::OnLoadPlugIn()
{
  return TRUE;
}

void CSampleModelessCancelPlugIn::OnUnloadPlugIn()
{
}

unsigned int CSampleModelessCancelPlugIn::WindowMessage()
{
  if (m_window_message == 0)
    m_window_message = ::RegisterWindowMessage(L"F45A62B6-1A75-42B5-ABEB-2A5D09A546F1");
  return m_window_message;
}

/////////////////////////////////////////////////////////////////////////////
// CRhinoModelessDialogCommand definition

CRhinoModelessDialogCommand::CRhinoModelessDialogCommand(
  bool bTransparent,
  bool bDoNotRepeat,
  const class CRhinoPlugIn* pPlugIn,
  bool bTestCommand
)
  : CRhinoCommand(bTransparent, bDoNotRepeat, pPlugIn, bTestCommand)
{
}

CRhinoModelessDialogCommand::~CRhinoModelessDialogCommand()
{
}

