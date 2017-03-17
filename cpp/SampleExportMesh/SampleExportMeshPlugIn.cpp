#include "StdAfx.h"
#include "C:\Program Files\Rhino 6.0 SDK\Inc\rhinoSdkPlugInDeclare.h"
#include "SampleExportMeshPlugIn.h"

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
RHINO_PLUG_IN_NAME( L"SampleExportMesh" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"F2F3C62B-31F9-4A6F-BEAA-7F1EE7AB6709" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__ "  " __TIME__ )

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/Rhino6Samples_CPP");

// The one and only CSampleExportMeshPlugIn object
static CSampleExportMeshPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleExportMeshPlugIn definition

CSampleExportMeshPlugIn& SampleExportMeshPlugIn()
{
  // Return a reference to the one and only CSampleExportMeshPlugIn object
  return thePlugIn;
}

CSampleExportMeshPlugIn::CSampleExportMeshPlugIn()
  : m_mesh_ui_style(0)
{
  // Description:
  //   CSampleExportMeshPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleExportMeshPlugIn::OnLoadPlugIn() is called. The
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleExportMeshPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleExportMeshPlugIn::~CSampleExportMeshPlugIn()
{
  // Description:
  //   CSampleExportMeshPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleExportMeshPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleExportMeshPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleExportMeshPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleExportMeshPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {F2F3C62B-31F9-4A6F-BEAA-7F1EE7AB6709}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleExportMeshPlugIn::OnLoadPlugIn()
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
  return CRhinoFileExportPlugIn::OnLoadPlugIn();
}

void CSampleExportMeshPlugIn::OnUnloadPlugIn()
{
  // Description:
  //    Called one time when plug-in is about to be unloaded. By this time,
  //    Rhino's mainframe window has been destroyed, and some of the SDK
  //    managers have been deleted. There is also no active document or active
  //    view at this time. Thus, you should only be manipulating your own objects.
  //    or tools here.

  // TODO: Add plug-in cleanup code here.

  CRhinoFileExportPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// Online help overrides

BOOL CSampleExportMeshPlugIn::AddToPlugInHelpMenu() const
{
  // Description:
  //   Return true to have your plug-in name added to the Rhino help menu.
  //   OnDisplayPlugInHelp will be called when to activate your plug-in help.

  return FALSE;
}

BOOL CSampleExportMeshPlugIn::OnDisplayPlugInHelp( HWND hWnd ) const
{
  // Description:
  //   Called when the user requests help about your plug-in.
  //   It should display a standard Windows Help file (.hlp or .chm).

  // TODO: Add support for online help here.
  return CRhinoFileExportPlugIn::OnDisplayPlugInHelp( hWnd );
}

/////////////////////////////////////////////////////////////////////////////
// File export overrides

void CSampleExportMeshPlugIn::AddFileType(ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileWriteOptions& options)
{
  UNREFERENCED_PARAMETER(options);

  // Description:
  //   When Rhino gets ready to display either the save or export file dialog,
  //   it calls AddFileType() once for each loaded file export plug-in.
  // Parameters:
  //   extensions [in] Append your supported file type extensions to this list.
  //   options [in] File write options.
  // Example:
  //   If your plug-in exports "Geometry Files" that have a ".geo" extension,
  //   then your AddToFileType(....) would look like the following:
  //
  //   CExportPlugIn::AddToFileType(ON_ClassArray<CRhinoFileType>&  extensions, const CRhinoFileWriteOptions& options)
  //   {
  //      CRhinoFileType ft(PlugInID(), L"Geometry Files (*.geo)", L"geo");
  //      extensions.Append(ft);
  //   }

  // TODO: Add supported file extensions here

  extensions.Append(CRhinoFileType(PlugInID(), L"Sample Mesh (*.mesh)", L"mesh"));
}

BOOL CSampleExportMeshPlugIn::WriteFile(const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileWriteOptions& options)
{
  UNREFERENCED_PARAMETER(filename);
  UNREFERENCED_PARAMETER(index);
  UNREFERENCED_PARAMETER(doc);
  UNREFERENCED_PARAMETER(options);

  // Description:
  //   Rhino calls WriteFile() to write document geometry to an external file.
  // Parameters:
  //   filename [in] The name of file to write.
  //   index [in] The index of file extension added to list in AddToFileType().
  //   doc [in] The current Rhino document.
  //   options [in] File write options.
  // Remarks:
  //   The plug-in is responsible for opening the file and writing to it.
  // Return TRUE if successful, otherwise return FALSE.

  // TODO: Add file export code here

  // Are we saving or exporting?
  bool bExport = options.SelectedObjectFilter();

  // Are we in interactive or scripted mode?
  bool bScript = options.UseBatchMode();

  ON_SimpleArray<const CRhinoObject*> objects(256);

  // Get objects to save/export
  CRhinoObjectIterator it(doc, CRhinoObjectIterator::undeleted_objects);
  if (bExport)
  {
    it.EnableSelectedFilter();
    it.EnableVisibleFilter();
  }

  // Do the iteration...
  const CRhinoObject* obj = 0;
  for (obj = it.First(); obj; obj = it.Next())
    objects.Append(obj);

  ON_ClassArray<CRhinoObjectMesh> meshes(objects.Count());
  ON_MeshParameters mesh_parameters = m_mesh_parameters;
  int mesh_ui_style = (bScript) ? 2 : m_mesh_ui_style;

  // Get the meshes to save/export
  CRhinoCommand::result res = RhinoMeshObjects(objects, mesh_parameters, mesh_ui_style, meshes);
  if (res == CRhinoCommand::success)
  {
    if (mesh_ui_style >= 0 && mesh_ui_style <= 1)
      m_mesh_ui_style = mesh_ui_style;
    m_mesh_parameters = mesh_parameters;
  }
  else
  {
    if (bExport)
      RhinoApp().Print(L"\nNo meshes to export.\n");
    else
      RhinoApp().Print(L"\nNo meshes to save.\n");
    return FALSE;
  }

  // Write the mesh file
  FILE* fp = 0;
  errno_t err = _wfopen_s(&fp, filename, L"w");
  bool rc = (0 == err || 0 != fp);
  if (!rc)
  {
    RhinoApp().Print(L"\nUnable to open \"%s\" for writing.\n", filename);
    return FALSE;
  }

  // Write mesh count
  ON_wString s;
  s.Format(L"meshcount=%d\n", meshes.Count());
  rc = (fputws(s, fp) >= 0);

  int i, j;
  for (i = 0; rc && i < meshes.Count(); i++)
  {
    const CRhinoObjectMesh& obj_mesh = meshes[i];
    const ON_Mesh* mesh = obj_mesh.m_mesh;
    rc = (0 != mesh);

    // Write mesh number
    if (rc)
    {
      s.Format(L"mesh=%d\n", i);
      rc = (fputws(s, fp) >= 0);
    }

    // Write mesh vertex count
    if (rc)
    {
      s.Format(L"vertexcount=%d\n", mesh->m_V.Count());
      rc = (fputws(s, fp) >= 0);
    }

    // Write mesh face count
    if (rc)
    {
      s.Format(L"facecount=%d\n", mesh->m_F.Count());
      rc = (fputws(s, fp) >= 0);
    }

    // Write mesh vertices
    if (rc)
    {
      for (j = 0; rc && j < mesh->m_V.Count(); j++)
      {
        const ON_3fPoint& p = mesh->m_V[j];
        s.Format(L"vertex=(%.16f,%.16f,%.16f)\n", p.x, p.y, p.z);
        rc = (fputws(s, fp) >= 0);
      }
    }

    // Write mesh faces
    if (rc)
    {
      for (j = 0; rc && j < mesh->m_F.Count(); j++)
      {
        const ON_MeshFace& f = mesh->m_F[j];
        s.Format(L"face=(%d,%d,%d,%d)\n", f.vi[0], f.vi[1], f.vi[2], f.vi[3]);
        rc = (fputws(s, fp) >= 0);
      }
    }
  }

  fclose(fp);

  return (rc) ? TRUE : FALSE;
}

