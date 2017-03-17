#include "StdAfx.h"
#include "C:\Program Files\Rhino 6.0 SDK\Inc\rhinoSdkPlugInDeclare.h"
#include "SampleCommandsPlugIn.h"
#include "resource.h"

// The plug-in object must be constructed before any plug-in classes derived
// from CRhinoCommand. The #pragma init_seg(lib) ensures that this happens.
#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#pragma warning(pop)

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
// Provide a short, friendly name for this plug-in.
RHINO_PLUG_IN_NAME(L"SampleCommands");

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID(L"05455F22-B196-4457-9657-273269AB8ACE");

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

// The one and only CSampleCommandsPlugIn object
static CSampleCommandsPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleCommandsPlugIn definition

CSampleCommandsPlugIn& SampleCommandsPlugIn()
{
  // Return a reference to the one and only CSampleCommandsPlugIn object
  return thePlugIn;
}

CSampleCommandsPlugIn::CSampleCommandsPlugIn()
  : m_hCursor(0)
{
  // Description:
  //   CSampleCommandsPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleCommandsPlugIn::OnLoadPlugIn() is called. The
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleCommandsPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleCommandsPlugIn::~CSampleCommandsPlugIn()
{
  // Description:
  //   CSampleCommandsPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleCommandsPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  if (0 != m_hCursor)
  {
    DestroyCursor(m_hCursor);
    m_hCursor = 0;
  }
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleCommandsPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleCommandsPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleCommandsPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {05455F22-B196-4457-9657-273269AB8ACE}
  return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleCommandsPlugIn::OnLoadPlugIn()
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
  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleCommandsPlugIn::OnUnloadPlugIn()
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

BOOL CSampleCommandsPlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return FALSE;
}

BOOL CSampleCommandsPlugIn::OnDisplayPlugInHelp(HWND hWnd) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoUtilityPlugIn::OnDisplayPlugInHelp(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// Document user data overrides

BOOL CSampleCommandsPlugIn::CallWriteDocument(const CRhinoFileWriteOptions& options)
{
  // If the user is exporting selected geometry, return FALSE
  if (options.SelectedObjectFilter())
    return FALSE;

  // Only write document data if there is something to write
  return (m_string_table.Count() > 0) ? TRUE : FALSE;
}

BOOL CSampleCommandsPlugIn::WriteDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileWriteOptions& options)
{
  UNREFERENCED_PARAMETER(doc);

  // Write plug-in document data.  See rhinoSdkPlugIn.h details.

  // If the user is exporting selected geometry, just return
  if (options.SelectedObjectFilter())
    return TRUE;

  // Write the major and minor version of document as document data
  if (!archive.Write3dmChunkVersion(1, 0))
    return FALSE;

  // Write the number of string we intend to write as document data
  if (!archive.WriteInt(m_string_table.Count()))
    return FALSE;

  // Write our string table as document data
  for (int i = 0; i < m_string_table.Count(); i++)
  {
    if (!archive.WriteString(m_string_table[i]))
      return FALSE;
  }

  return TRUE;
}

BOOL CSampleCommandsPlugIn::ReadDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileReadOptions& options)
{
  UNREFERENCED_PARAMETER(doc);

  // Read plug-in document data.  See rhinoSdkPlugIn.h details.

  // If our document data is found in a 3DM file, we are 
  // required to read it. But, we don't necessarily want
  // to use it.

  BOOL bKeepData = FALSE;
  BOOL bNewMode = options.Mode(CRhinoFileReadOptions::NewMode);
  BOOL bOpenMode = options.Mode(CRhinoFileReadOptions::OpenMode);

  // If the user is creating a new model, or opening an existing
  // model, we'll want to keep the document data that we read.
  // Otherwise, we'll read the data, but not use it.  This way,
  // we won't overwrite out data if the user is importing geometry,
  // from another 3DM file.
  if (bNewMode || bOpenMode)
    bKeepData = TRUE;

  if (bKeepData)
    m_string_table.Empty();

  // Read the major and minor version of the document data
  int major = 0, minor = 0;
  if (!archive.Read3dmChunkVersion(&major, &minor))
    return FALSE;

  // At this point, if we've changed the format of
  // our document data, we'll want to compare the
  // major and minor revision numbers and read our
  // data accordingly.
  if (1 != major && 0 != minor)
    return FALSE;

  // Read the number of string we intend to read as document data
  int count = 0;
  if (!archive.ReadInt(&count))
    return FALSE;

  // Read our string table as document data
  ON_wString str;
  for (int i = 0; i < count; i++)
  {
    if (!archive.ReadString(str))
      return FALSE;

    // Only append the string if we are
    // supposed to
    if (bKeepData)
      m_string_table.Append(str);
  }

  return TRUE;
}

int CSampleCommandsPlugIn::StringTableCount() const
{
  return m_string_table.Count();
}

ON_wString CSampleCommandsPlugIn::GetStringTableItem(int index)
{
  ON_wString str;
  if (index >= 0 && index < StringTableCount())
    str = m_string_table[index];
  return str;
}

int CSampleCommandsPlugIn::FindStringTableItem(const wchar_t* str)
{
  if (0 == str || 0 == str[0])
    return -1;

  for (int i = 0; i < m_string_table.Count(); i++)
  {
    ON_wString item = m_string_table[i];
    if (0 == item.CompareNoCase(str))
      return i;
  }

  return -1;
}

int CSampleCommandsPlugIn::AddStringTableItem(const wchar_t* str)
{
  if (0 == str || 0 == str[0])
    return -1;

  int index = FindStringTableItem(str);
  if (index < 0)
  {
    m_string_table.Append(str);
    return StringTableCount();
  }

  return -1;
}

bool CSampleCommandsPlugIn::DeleteStringTableItem(const wchar_t* str)
{
  if (0 == str || 0 == str[0])
    return false;

  int index = FindStringTableItem(str);
  if (index > 0 && index < StringTableCount())
  {
    m_string_table.Remove(index);
    return true;
  }

  return false;
}

void CSampleCommandsPlugIn::ClearStringTable()
{
  m_string_table.Destroy();
}

HCURSOR CSampleCommandsPlugIn::SampleCursor()
{
  // By default, MFC uses the resource handle of the main application to load
  // the resource template. If you have an exported function in a DLL, such as
  // one that launches a dialog box in the DLL, this template is actually stored
  // in the DLL module. You need to switch the module state for the correct handle
  // to be used. You can do this by adding the following code to the beginning of 
  // the function. This swaps the current module state with the state returned from
  // AfxGetStaticModuleState until the end of the current scope.
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if (0 == m_hCursor)
    m_hCursor = (HCURSOR)::LoadImage(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_SAMPLE_CURSOR), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
  return m_hCursor;
}