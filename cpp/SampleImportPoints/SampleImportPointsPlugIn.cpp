#include "stdafx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleImportPointsPlugIn.h"
#include "Resource.h"

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
RHINO_PLUG_IN_NAME(L"SampleImportPoints");

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID(L"FC3EBED3-87A6-4D6C-9BB3-AF87755638AC");

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
// Provide a description of this plug-in
RHINO_PLUG_IN_DESCRIPTION(L"Rhino SDK Sample - SampleImportPoints");

// Rhino plug-in icon resource id
// Provide the resource id of the plug-in icon.
RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_ICON1);

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"146 North Canal Street, Suite 320\r\nSeattle, WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/rhino-developer-samples");

// The one and only CSampleImportPointsPlugIn object
static CSampleImportPointsPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleImportPointsPlugIn definition

CSampleImportPointsPlugIn& SampleImportPointsPlugIn()
{
  // Return a reference to the one and only CSampleImportPointsPlugIn object
  return thePlugIn;
}

CSampleImportPointsPlugIn::CSampleImportPointsPlugIn()
{
  // Description:
  //   CSampleImportPointsPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleImportPointsPlugIn::OnLoadPlugIn() is called. The
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleImportPointsPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleImportPointsPlugIn::~CSampleImportPointsPlugIn()
{
  // Description:
  //   CSampleImportPointsPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleImportPointsPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleImportPointsPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleImportPointsPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleImportPointsPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {FC3EBED3-87A6-4D6C-9BB3-AF87755638AC}
  return ON_UuidFromString(RhinoPlugInId());
}

int CSampleImportPointsPlugIn::OnLoadPlugIn()
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
  return CRhinoFileImportPlugIn::OnLoadPlugIn();
}

void CSampleImportPointsPlugIn::OnUnloadPlugIn()
{
  // Description:
  //    Called one time when plug-in is about to be unloaded. By this time,
  //    Rhino's mainframe window has been destroyed, and some of the SDK
  //    managers have been deleted. There is also no active document or active
  //    view at this time. Thus, you should only be manipulating your own objects.
  //    or tools here.

  // TODO: Add plug-in cleanup code here.

  CRhinoFileImportPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL32 CSampleImportPointsPlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return false;
}

BOOL32 CSampleImportPointsPlugIn::OnDisplayPlugInHelp(HWND hWnd) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoFileImportPlugIn::OnDisplayPlugInHelp(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// File import overrides

void CSampleImportPointsPlugIn::AddFileType(ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileReadOptions& options)
{
  UNREFERENCED_PARAMETER(options);

  // Description:
  //   When Rhino gets ready to display either the open or import file dialog,
  //   it calls AddFileType() once for each loaded file import plug-in.
  // Parameters:
  //   extensions [in] Append your supported file type extensions to this list.
  //   options [in] File write options.
  // Example:
  //   If your plug-in imports "Geometry Files" that have a ".geo" extension,
  //   then your AddToFileType(....) would look like the following:
  //
  //   CImportPlugIn::AddToFileType(ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileReadOptions& options)
  //   {
  //      CRhinoFileType ft(PlugInID(), L"Geometry Files (*.geo)", L"geo");
  //      extensions.Append(ft);
  //   }

  // Pick an extension no other file import plug-in claims.
  //
  // When two import plug-ins offer the same extension, Rhino resolves the
  // clash in favour of its own built-in importer, and a third-party plug-in
  // cannot win: the deciding test is a base class that is not in the public
  // SDK.  Your ReadFile() is then simply never called, and nothing is printed
  // to tell you why - the import still appears to succeed, because Rhino's
  // importer did the work.
  //
  // This sample used to register ".pts", which Rhino's own Points Import
  // plug-in already reads (along with .asc, .csv, .txt and .xyz), so it never
  // ran on either platform.  ".samplepts" belongs to nobody.
  CRhinoFileType ft(PlugInID(), L"Sample Points File (*.samplepts)", L"samplepts");
  extensions.Append(ft);
}

BOOL32 CSampleImportPointsPlugIn::ReadFile(const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileReadOptions& options)
{
  UNREFERENCED_PARAMETER(index);
  UNREFERENCED_PARAMETER(options);

  // Description:
  //   Rhino calls ReadFile() to create document geometry from an external file.
  // Parameters:
  //   filename [in] The name of file to read.
  //   index [in] The index of file extension added to list in AddToFileType().
  //   doc [in] If importing, then the current Rhino document. Otherwise, an empty Rhino document.
  //   options [in] File read options.
  // Remarks:
  //   The plug-in is responsible for opening the file and writing to it.
  // Return TRUE if successful, otherwise return FALSE.

  FILE* file = ON_FileStream::Open(filename, L"r");
  if (nullptr == file)
    return false;

  ON_3dPointArray point_list;
  const wchar_t* delimiter = L",";
  ON_String buffer;

  for (;;)
  {
    const int ch = fgetc(file);

    if (EOF != ch && '\n' != ch)
    {
      buffer += (char)ch;
      continue;
    }

    ON_wString line(buffer.Array());
    line.Remove('\r');
    line.Remove('"');
    line.TrimLeftAndRight();

    // ParsePointValue() tokenises in place, so hand it the writable buffer
    // rather than letting ON_wString convert to const wchar_t*.
    ON_3dPoint pt;
    if (ParsePointValue(line.Array(), delimiter, pt))
      point_list.Append(pt);

    buffer.Empty();

    if (EOF == ch)
      break;
  }

  ON_FileStream::Close(file);

  for (int i = 0; i < point_list.Count(); i++)
    doc.AddPointObject(point_list[i]);

  doc.Redraw();

  return true;
}

bool CSampleImportPointsPlugIn::ParsePointValue(const wchar_t* string, const wchar_t* delimiter, ON_3dPoint& value)
{
  if (nullptr == string || 0 == string[0] || nullptr == delimiter || 0 == delimiter[0])
    return false;

  ON_3dPoint point = ON_3dPoint::UnsetPoint;
  double d = 0.0;

  wchar_t* context = nullptr;
  wchar_t* token = wcstok_s((wchar_t*)string, delimiter, &context);
  if (token && ParseRealValue(token, d))
    point.x = d;
  else
    return false;

  token = wcstok_s(nullptr, delimiter, &context);
  if (token && ParseRealValue(token, d))
    point.y = d;
  else
    return false;

  token = wcstok_s(nullptr, delimiter, &context);
  if (token)
  {
    if (ParseRealValue(token, d))
      point.z = d;
    else
      return false;
  }
  else
    point.z = 0.0;

  if (!point.IsValid())
    return false;

  value = point;

  return true;
}

bool CSampleImportPointsPlugIn::ParseRealValue(const wchar_t* string, double& value)
{
  if (nullptr == string || 0 == string[0])
    return false;

  return (RhinoParseNumber(string, &value) > 0) ? true : false;
}
