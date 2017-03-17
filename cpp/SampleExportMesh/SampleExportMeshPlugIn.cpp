/////////////////////////////////////////////////////////////////////////////
// SampleExportMeshPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleExportMeshPlugIn.h"

// The plug-in object must be constructed before any plug-in classes
// derived from CRhinoCommand. The #pragma init_seg(lib) ensures that
// this happens.

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
RHINO_PLUG_IN_ID( L"46B72A24-955D-431A-9E6C-75B8741E7728" );

// Rhino plug-in version
// Provide a version number string for this plug-in
RHINO_PLUG_IN_VERSION( __DATE__"  "__TIME__ )

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION( L"Robert McNeel & Associates" );
RHINO_PLUG_IN_DEVELOPER_ADDRESS( L"3670 Woodland Park Avenue North\015\012Seattle WA 98103" );
RHINO_PLUG_IN_DEVELOPER_COUNTRY( L"United States" );
RHINO_PLUG_IN_DEVELOPER_PHONE( L"206-545-6877" );
RHINO_PLUG_IN_DEVELOPER_FAX( L"206-545-7321" );
RHINO_PLUG_IN_DEVELOPER_EMAIL( L"tech@mcneel.com" );
RHINO_PLUG_IN_DEVELOPER_WEBSITE( L"http://www.rhino3d.com" );
RHINO_PLUG_IN_UPDATE_URL( L"https://github.com/mcneel/" );

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
{
  // Description:
  //   CSampleExportMeshPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleExportMeshPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleExportMeshPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();

  m_mesh_ui_style = 0; // 0 = simple dialog 
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
  // {46B72A24-955D-431A-9E6C-75B8741E7728}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleExportMeshPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  return CRhinoFileExportPlugIn::OnLoadPlugIn();
}

void CSampleExportMeshPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.

  CRhinoFileExportPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// File export overrides

void CSampleExportMeshPlugIn::AddFileType( ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileWriteOptions& options )
{
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

  // TODO: Add supported file extensions here.
  extensions.Append( CRhinoFileType(PlugInID(), L"Sample Mesh (*.mesh)", L"mesh") );
}

BOOL CSampleExportMeshPlugIn::WriteFile( const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileWriteOptions& options )
{
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

  // TODO: Add file export code here.

  // Are we saving or exporting?
  bool bExport = options.Mode( CRhinoFileWriteOptions::SelectedMode );
  // Are we in interactive or scripted mode?
  bool bScript = options.Mode( CRhinoFileWriteOptions::BatchMode );

  ON_SimpleArray<const CRhinoObject*> objects( 256 );

  // Get objects to save/export
  CRhinoObjectIterator it( doc, CRhinoObjectIterator::undeleted_objects );
  if( bExport )
  {
    it.EnableSelectedFilter();
    it.EnableVisibleFilter();
  }

  // Do the iteration...
  const CRhinoObject* obj = 0;
  for( obj = it.First(); obj; obj = it.Next() )
    objects.Append( obj );

  ON_ClassArray<CRhinoObjectMesh> meshes( objects.Count() );
  ON_MeshParameters mesh_parameters = m_mesh_parameters;
  int mesh_ui_style = ( bScript ) ? 2 : m_mesh_ui_style;

  // Get the meshes to save/export
  CRhinoCommand::result res = RhinoMeshObjects( objects, mesh_parameters, mesh_ui_style, meshes );
  if( res == CRhinoCommand::success )
  {
    if( mesh_ui_style >= 0 && mesh_ui_style <= 1 )
      m_mesh_ui_style = mesh_ui_style;
    m_mesh_parameters = mesh_parameters;
  }
  else 
  {
    if( bExport )
      RhinoApp().Print( L"\nNo meshes to export.\n" );
    else
      RhinoApp().Print( L"\nNo meshes to save.\n" );
    return FALSE;
  }

  // Write the mesh file
  FILE* fp = 0;
  errno_t err = _wfopen_s( &fp, filename, L"w" );
  bool rc = ( 0 == err || 0 != fp );
  if( !rc )
  {
    RhinoApp().Print( L"\nUnable to open \"%s\" for writing.\n", filename );
    return FALSE;
  }

  // Write mesh count
  ON_wString s;
  s.Format( L"meshcount=%d\n", meshes.Count() );
  rc = ( fputws(s, fp) >= 0 );

  int i, j;
  for( i = 0; rc && i < meshes.Count(); i++ )
  {
    const CRhinoObjectMesh& obj_mesh = meshes[i];
    const ON_Mesh* mesh = obj_mesh.m_mesh;
    rc = ( 0 != mesh );

    // Write mesh number
    if( rc )
    {
      s.Format( L"mesh=%d\n", i );
      rc = ( fputws(s, fp) >= 0 );
    }

    // Write mesh vertex count
    if( rc )
    {
      s.Format( L"vertexcount=%d\n", mesh->m_V.Count() );
      rc = ( fputws(s, fp) >= 0 );
    }

    // Write mesh face count
    if( rc )
    {
      s.Format( L"facecount=%d\n", mesh->m_F.Count() );
      rc = ( fputws(s, fp) >= 0 );
    }

    // Write mesh vertices
    if( rc )
    {
      for( j = 0; rc && j < mesh->m_V.Count(); j++ )
      {
        const ON_3fPoint& p = mesh->m_V[j];
        s.Format( L"vertex=(%.16f,%.16f,%.16f)\n", p.x, p.y, p.z );
        rc = ( fputws(s, fp) >= 0 );
      }
    }

    // Write mesh faces
    if( rc )
    {
      for( j = 0; rc && j < mesh->m_F.Count(); j++ )
      {
        const ON_MeshFace& f = mesh->m_F[j];
        s.Format( L"face=(%d,%d,%d,%d)\n", f.vi[0], f.vi[1], f.vi[2], f.vi[3] );
        rc = ( fputws(s, fp) >= 0 );
      }
    }
  }

  fclose( fp );
  
  return ( rc ) ? TRUE : FALSE;
}

