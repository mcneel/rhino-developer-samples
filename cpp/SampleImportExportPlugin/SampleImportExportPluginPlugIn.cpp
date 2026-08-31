#include "StdAfx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleImportExportPluginPlugIn.h"
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
RHINO_PLUG_IN_NAME(L"SampleImportExportPlugin");

// Rhino plug-in id
RHINO_PLUG_IN_ID(L"6A8F7B21-5C4E-4A93-B0D7-2E1F9C63A845");

// Rhino plug-in version
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)

// Rhino plug-in description
RHINO_PLUG_IN_DESCRIPTION(L"Rhino SDK Sample - SampleImportExportPlugin");

// Rhino plug-in icon resource id
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

// The one and only CSampleImportExportPluginPlugIn object
static CSampleImportExportPluginPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleImportExportPluginPlugIn definition

CSampleImportExportPluginPlugIn& SampleImportExportPluginPlugIn()
{
  return thePlugIn;
}

CSampleImportExportPluginPlugIn::CSampleImportExportPluginPlugIn()
{
  m_plugin_version = RhinoPlugInVersion();
}

CSampleImportExportPluginPlugIn::~CSampleImportExportPluginPlugIn()
{
}

const wchar_t* CSampleImportExportPluginPlugIn::PlugInName() const
{
  return RhinoPlugInName();
}

const wchar_t* CSampleImportExportPluginPlugIn::PlugInVersion() const
{
  return m_plugin_version;
}

GUID CSampleImportExportPluginPlugIn::PlugInID() const
{
  return ON_UuidFromString(RhinoPlugInId());
}

BOOL CSampleImportExportPluginPlugIn::OnLoadPlugIn()
{
  return TRUE;
}

void CSampleImportExportPluginPlugIn::OnUnloadPlugIn()
{
}

BOOL CSampleImportExportPluginPlugIn::AddToPlugInHelpMenu() const
{
  return TRUE;
}

BOOL CSampleImportExportPluginPlugIn::OnDisplayPlugInHelp(HWND hWnd) const
{
  ON_wString url = RhinoPlugInUpdateURL();
  if (url.IsEmpty())
    return FALSE;
  ::ShellExecute(hWnd, L"open", url, nullptr, nullptr, SW_SHOWNORMAL);
  return TRUE;
}

const wchar_t* CSampleImportExportPluginPlugIn::FileHeader()
{
  return L"# Rhino SampleImportExportPlugin point file";
}

/////////////////////////////////////////////////////////////////////////////
// File types

void CSampleImportExportPluginPlugIn::AddFileType(ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileReadOptions& options)
{
  UNREFERENCED_PARAMETER(options);

  // The last argument adds an Options button to the file dialog, which is what
  // makes DisplayOptionsDialog reachable.
  CRhinoFileType ft(PlugInID(), L"Sample Point Files (*.samplepts)", L"samplepts", true);
  extensions.Append(ft);
}

void CSampleImportExportPluginPlugIn::AddFileType(ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileWriteOptions& options)
{
  UNREFERENCED_PARAMETER(options);

  // Deliberately the same description and extension as the read list. A format the
  // plug-in both reads and writes normally looks identical in both dialogs, which
  // is why DisplayOptionsDialog cannot tell them apart by description.
  CRhinoFileType ft(PlugInID(), L"Sample Point Files (*.samplepts)", L"samplepts", true);
  extensions.Append(ft);
}

/////////////////////////////////////////////////////////////////////////////
// Reading

BOOL CSampleImportExportPluginPlugIn::ReadFile(const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileReadOptions& options)
{
  UNREFERENCED_PARAMETER(index);
  UNREFERENCED_PARAMETER(options);

  if (nullptr == filename || 0 == filename[0])
    return FALSE;

  FILE* fp = nullptr;
  if (0 != _wfopen_s(&fp, filename, L"r"))
    fp = nullptr;
  if (nullptr == fp)
    return FALSE;

  int point_count = 0;
  char buffer[1024];
  while (nullptr != fgets(buffer, (int)sizeof(buffer), fp))
  {
    double x = 0.0, y = 0.0, z = 0.0;
    if (3 == sscanf_s(buffer, "%lf,%lf,%lf", &x, &y, &z))
    {
      doc.AddPointObject(ON_3dPoint(x, y, z));
      point_count++;
    }
  }

  fclose(fp);

  if (point_count > 0)
    doc.Redraw();

  RhinoApp().Print(L"Read %d points from %s\n", point_count, filename);

  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Writing

int CSampleImportExportPluginPlugIn::WriteFile(const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileWriteOptions& options)
{
  UNREFERENCED_PARAMETER(index);

  if (nullptr == filename || 0 == filename[0])
    return 0;

  // Export writes only what is selected; Save writes everything.
  const bool bSelectedOnly = options.SelectedObjectFilter() ? true : false;

  ON_SimpleArray<ON_3dPoint> points;
  CRhinoObjectIterator it(doc, CRhinoObjectIterator::normal_objects, CRhinoObjectIterator::active_objects);
  it.SetObjectFilter(ON::point_object);
  for (const CRhinoObject* obj = it.First(); nullptr != obj; obj = it.Next())
  {
    if (bSelectedOnly && !obj->IsSelected())
      continue;
    const CRhinoPointObject* point_obj = CRhinoPointObject::Cast(obj);
    if (nullptr != point_obj)
      points.Append(point_obj->Point().point);
  }

  FILE* fp = nullptr;
  if (0 != _wfopen_s(&fp, filename, L"w"))
    fp = nullptr;
  if (nullptr == fp)
    return 0;

  fprintf(fp, "%ls\n", FileHeader());
  for (int i = 0; i < points.Count(); i++)
    fprintf(fp, "%.17g,%.17g,%.17g\n", points[i].x, points[i].y, points[i].z);

  fclose(fp);

  RhinoApp().Print(L"Wrote %d points to %s\n", points.Count(), filename);

  return 1;
}

/////////////////////////////////////////////////////////////////////////////
// Options

void CSampleImportExportPluginPlugIn::DisplayOptionsDialog(HWND parent, const CRhinoFileType& fileType)
{
  // One override serves both dialogs. Ask the file type which one is calling -
  // the description is the same in both directions.
  const wchar_t* direction = L"unknown";
  if (fileType.IsForImport())
    direction = L"import";
  else if (fileType.IsForExport())
    direction = L"export";

  ON_wString message;
  message.Format(L"SampleImportExportPlugin options for %s.", direction);

  ::RhinoMessageBox(parent, message, RhinoPlugInName(), MB_OK | MB_ICONINFORMATION);
}
