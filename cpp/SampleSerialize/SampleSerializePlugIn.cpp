#include "StdAfx.h"
#include "C:\Program Files\Rhino 6.0 SDK\Inc\rhinoSdkPlugInDeclare.h"
#include "SampleSerializePlugIn.h"

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
RHINO_PLUG_IN_NAME(L"SampleSerialize");

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID(L"9FF95264-C2CB-4899-B8A6-C701927370BA");

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

// The one and only CSampleSerializePlugIn object
static CSampleSerializePlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleSerializePlugIn definition

CSampleSerializePlugIn& SampleSerializePlugIn()
{
  // Return a reference to the one and only CSampleSerializePlugIn object
  return thePlugIn;
}

CSampleSerializePlugIn::CSampleSerializePlugIn()
{
  // Description:
  //   CSampleSerializePlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleSerializePlugIn::OnLoadPlugIn() is called. The
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleSerializePlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleSerializePlugIn::~CSampleSerializePlugIn()
{
  // Description:
  //   CSampleSerializePlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleSerializePlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleSerializePlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleSerializePlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleSerializePlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {9FF95264-C2CB-4899-B8A6-C701927370BA}
  return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleSerializePlugIn::OnLoadPlugIn()
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

  m_plugin_watcher.Register();
  m_plugin_watcher.Enable(TRUE);

  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleSerializePlugIn::OnUnloadPlugIn()
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

BOOL CSampleSerializePlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return FALSE;
}

BOOL CSampleSerializePlugIn::OnDisplayPlugInHelp(HWND hWnd) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoUtilityPlugIn::OnDisplayPlugInHelp(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// Document user data overrides

BOOL CSampleSerializePlugIn::CallWriteDocument(const CRhinoFileWriteOptions& options)
{
  // Don't save document data if only selected objects are saved
  return options.SelectedObjectFilter() ? FALSE : TRUE;
}

BOOL CSampleSerializePlugIn::WriteDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileWriteOptions& options)
{
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(options);

  // Our serialization data class handles all writing
  bool rc = m_plugin_data.Write(archive);
  return rc ? TRUE : FALSE;
}

BOOL CSampleSerializePlugIn::ReadDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileReadOptions& options)
{
  UNREFERENCED_PARAMETER(doc);

  // If we are asked to read our document data, then we need to read our document data.
  // But if Rhino is importing another 3dm file, we may or many not want this document's
  // data. Or, we might want to merge this document's data with ours. In this example,
  // if Rhino is importing another 3dm file, we will go ahead and read our data, just
  // like we should. But we will just throw it away...
  BOOL bImporting = options.Mode(CRhinoFileReadOptions::ImportMode) || options.Mode(CRhinoFileReadOptions::ImportReferenceMode);
  if (bImporting)
  {
    CSampleSerializeData data;
    bool rc = data.Read(archive);
    return rc ? TRUE : FALSE;
  }

  bool rc = m_plugin_data.Read(archive);
  return rc ? TRUE : FALSE;
}

CSampleSerializeData& CSampleSerializePlugIn::Data()
{
  return m_plugin_data;
}