/////////////////////////////////////////////////////////////////////////////
// SampleImportGeomviewPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleImportGeomviewPlugIn.h"

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
RHINO_PLUG_IN_NAME( L"SampleImportGeomview" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"7AC5A609-B985-45D9-9B85-F04DB28FEA9D" );

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

// The one and only CSampleImportGeomviewPlugIn object
static CSampleImportGeomviewPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleImportGeomviewPlugIn definition

CSampleImportGeomviewPlugIn& SampleImportGeomviewPlugIn()
{ 
  // Return a reference to the one and only CSampleImportGeomviewPlugIn object
  return thePlugIn; 
}

CSampleImportGeomviewPlugIn::CSampleImportGeomviewPlugIn()
{
  // Description:
  //   CSampleImportGeomviewPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleImportGeomviewPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleImportGeomviewPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleImportGeomviewPlugIn::~CSampleImportGeomviewPlugIn()
{
  // Description:
  //   CSampleImportGeomviewPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleImportGeomviewPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleImportGeomviewPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino 
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleImportGeomviewPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino 
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleImportGeomviewPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {7AC5A609-B985-45D9-9B85-F04DB28FEA9D}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleImportGeomviewPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  return CRhinoFileImportPlugIn::OnLoadPlugIn();
}

void CSampleImportGeomviewPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.

  CRhinoFileImportPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// File import overrides

void CSampleImportGeomviewPlugIn::AddFileType( ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileReadOptions& options )
{
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

  // TODO: Add supported file extensions here.
  CRhinoFileType ft;
  ft.SetFileTypePlugInID( PlugInID() );
  ft.FileTypeDescription( L"Geomview OFF (*.off)" );
  ft.AddFileTypeExtension( L"off" );
  extensions.Append( ft );
}

BOOL CSampleImportGeomviewPlugIn::ReadFile( const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileReadOptions& options )
{
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

  // TODO: Add file import code here.
  if( index == 0 )
    return ReadOffFile( filename, doc );

  RhinoApp().Print( L"%s has encountered an unsupported file type index: %d\n", PlugInName(), index );
  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// Read the OFF file

BOOL CSampleImportGeomviewPlugIn::ReadOffFile( const wchar_t* filename, CRhinoDoc& doc )
{
	std::string buf;
	ON_String str( filename );
	
	std::fstream OFFfile( str );
	OFFfile >> buf;

	if( buf != "OFF" )
		return FALSE;

	int i, j, numvert, numface, numedge;

	OFFfile >> numvert >> numface >> numedge;
	ON_Mesh mesh( numface, numvert, FALSE, FALSE );
	for( int i = 0; i < numvert; i++ )
  {
		ON_3dPoint p;
		OFFfile >> p.x >> p.y >> p.z;
		mesh.SetVertex( i, p );
	}

	int find = 0;
	int vind = numvert - 1;
	for( i = 0; i < numface; i++ )
  {
		int ngon;
		OFFfile >> ngon;
  	ON_SimpleArray<int> vert(32);
    vert.SetCount( ngon );
		for( j = 0; j < ngon; j++ )
			OFFfile >> vert[j];
		if( ngon == 3 )
    {
			mesh.SetTriangle( find++, vert[0], vert[1],vert[2] ); 
		}
		else if( ngon == 4 )
    {
			mesh.SetQuad( find++, vert[0], vert[1],vert[2], vert[3] ); 
		}
		else if( ngon > 4 )
    {
			ON_3fPoint center(0,0,0);
			for( j = 0; j < ngon; j++ )
				center += mesh.m_V[vert[j]];
			center *= (float)(1.0 / ngon);
			mesh.SetVertex( ++vind, center );
			
			for( j = 0; j < ngon; j++ )
      {
				mesh.SetTriangle( find++, vert[j], vert[(j+1)%ngon], vind ); 			
			}
		}
	}
	
  mesh.ComputeVertexNormals();
  mesh.Compact();

	doc.AddMeshObject( mesh );

  RhinoApp().RunScript( L"_Zoom _All _Extents", 0 );

  return TRUE;
}