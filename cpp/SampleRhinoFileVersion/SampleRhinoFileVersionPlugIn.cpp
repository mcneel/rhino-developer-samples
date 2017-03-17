#include "StdAfx.h"
#include "C:\Program Files\Rhino 6.0 SDK\Inc\rhinoSdkPlugInDeclare.h"
#include "SampleRhinoFileVersionPlugIn.h"

// The plug-in object must be constructed before any plug-in classes derived
// from CRhinoCommand. The #pragma init_seg(lib) ensures that this happens.
#pragma warning( push )
#pragma warning( disable : 4073 )
#pragma init_seg( lib )
#pragma warning( pop )

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME(L"Rhino 3DM File Version");

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID(L"8753EE21-1ADE-46C9-9768-9C08BFFB891B");

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/Rhino6Samples_CPP");

// For calculating the size of arrays for safe string functions
#ifndef cch
#define ccht(Array, EltType) (sizeof(Array) / sizeof(EltType))
#define cch(Array) ccht(Array, (Array)[0])
#endif

// The one and only CSampleRhinoFileVersionPlugIn object
static CSampleRhinoFileVersionPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleRhinoFileVersionPlugIn definition

CSampleRhinoFileVersionPlugIn& SampleRhinoFileVersionPlugIn()
{
  // Return a reference to the one and only CSampleRhinoFileVersionPlugIn object
  return thePlugIn;
}

CSampleRhinoFileVersionPlugIn::CSampleRhinoFileVersionPlugIn()
{
  // Description:
  //   CSampleRhinoFileVersionPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleRhinoFileVersionPlugIn::OnLoadPlugIn() is called. The
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleRhinoFileVersionPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleRhinoFileVersionPlugIn::~CSampleRhinoFileVersionPlugIn()
{
  // Description:
  //   CSampleRhinoFileVersionPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleRhinoFileVersionPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleRhinoFileVersionPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleRhinoFileVersionPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleRhinoFileVersionPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {8753EE21-1ADE-46C9-9768-9C08BFFB891B}
  return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleRhinoFileVersionPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // Remarks:
  //    Plug-ins are not loaded until after Rhino is started and a default document
  //    is created.  Because the default document already exists
  //    CRhinoEventWatcher::On????Document() functions are not called for the default
  //    document.  If you need to do any document initialization/synchronization then
  //    override this function and do it here.  It is not necessary to call
  //    CPlugIn::OnLoadPlugIn() from your derived class.

  // TODO: Add plug-in initialization code here.
  
  m_watcher.Register();
  m_watcher.Enable();

  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleRhinoFileVersionPlugIn::OnUnloadPlugIn()
{
  // Description:
  //    Called one time when plug-in is about to be unloaded. By this time,
  //    Rhino's mainframe window has been destroyed, and some of the SDK
  //    managers have been deleted. There is also no active document or active
  //    view at this time. Thus, you should only be manipulating your own objects.
  //    or tools here.

  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CSampleRhinoFileVersionPlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return FALSE;
}

BOOL CSampleRhinoFileVersionPlugIn::OnDisplayPlugInHelp(HWND hWnd) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoUtilityPlugIn::OnDisplayPlugInHelp(hWnd);
}

CRhinoPlugIn::plugin_load_time CSampleRhinoFileVersionPlugIn::PlugInLoadTime()
{
  return CRhinoPlugIn::load_plugin_at_startup;
}

void CSampleRhinoFileVersionWatcher::OnBeginOpenDocument(CRhinoDoc& doc, const wchar_t* filename, BOOL bMerge, BOOL bReference)
{
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(bMerge);
  UNREFERENCED_PARAMETER(bReference);

  if (0 == filename || 0 == filename[0] || !CRhinoFileUtilities::FileExists(filename))
    return;

  wchar_t ext[_MAX_EXT];
  wmemset(ext, 0, cch(ext));
  _wsplitpath_s(filename, 0, 0, 0, 0, 0, 0, ext, cch(ext));
  if (0 == ext[0] || (0 != _wcsicmp(L".3dm", ext) && 0 != _wcsicmp(L".3dmbak", ext)))
    return;

  int rhino_file_version = 0;

  FILE* archive_fp = ON::OpenFile(filename, L"rb");
  if (archive_fp)
  {
    ON_BinaryFile archive(ON::archive_mode::read3dm, archive_fp);

    int file_version = 0;
    ON_String start_section_comments;
    if (archive.Read3dmStartSection(&file_version, start_section_comments))
      rhino_file_version = file_version;

    ON::CloseFile(archive_fp);
  }

  switch (rhino_file_version)
  {
  case 1:
  {
    if (bMerge)
      RhinoApp().Print(L"\nImporting a Rhino 1.0 format file.\n");
    else
      RhinoApp().Print(L"\nOpening a Rhino 1.0 format file.\n");
  }
  break;
  case 2:
    if (bMerge)
      RhinoApp().Print(L"\nImporting a Rhino 2.0 format file.\n");
    else
      RhinoApp().Print(L"\nOpening a Rhino 2.0 format file.\n");
    break;
  case 3:
    if (bMerge)
      RhinoApp().Print(L"\nImporting a Rhino 3.0 format file.\n");
    else
      RhinoApp().Print(L"\nOpening a Rhino 3.0 format file.\n");
    break;
  case 4:
    if (bMerge)
      RhinoApp().Print(L"\nImporting a Rhino 4.0 format file.\n");
    else
      RhinoApp().Print(L"\nOpening a Rhino 4.0 format file.\n");
    break;
  case 5:
  case 50:
    if (bMerge)
      RhinoApp().Print(L"\nImporting a Rhino 5 format file.\n");
    else
      RhinoApp().Print(L"\nOpening a Rhino 5 format file.\n");
    break;
  case 6:
  case 60:
    if (bMerge)
      RhinoApp().Print(L"\nImporting a Rhino 6 format file.\n");
    else
      RhinoApp().Print(L"\nOpening a Rhino 6 format file.\n");
    break;
  }
}