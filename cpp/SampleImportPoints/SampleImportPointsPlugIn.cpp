/////////////////////////////////////////////////////////////////////////////
// SampleImportPointsPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleImportPointsPlugIn.h"

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
RHINO_PLUG_IN_NAME( L"SampleImportPoints" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"5DFB5F6B-156C-4C08-B816-AD6E1A3A6444" );

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
  // {5DFB5F6B-156C-4C08-B816-AD6E1A3A6444}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleImportPointsPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  return CRhinoFileImportPlugIn::OnLoadPlugIn();
}

void CSampleImportPointsPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.

  CRhinoFileImportPlugIn::OnUnloadPlugIn();
}

/////////////////////////////////////////////////////////////////////////////
// File import overrides

void CSampleImportPointsPlugIn::AddFileType( ON_ClassArray<CRhinoFileType>& extensions, const CRhinoFileReadOptions& options )
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

  CRhinoFileType ft( PlugInID(), L"Test Points File (*.pts)", L"pts" );
  extensions.Append( ft );
}

BOOL CSampleImportPointsPlugIn::ReadFile( const wchar_t* filename, int index, CRhinoDoc& doc, const CRhinoFileReadOptions& options )
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
	CStdioFile* file = 0;

  TRY
  {
		file = new CStdioFile( filename, CFile::modeRead | CFile::shareDenyNone | CFile::typeText );
    if( !file )
      return FALSE;

    ON_3dPointArray point_list;
    CString string;
    wchar_t delimiter = ',';

    while( file->ReadString(string) )
    {
      string.TrimLeft();
      string.TrimRight();
      string.Remove( '\r' );
      string.Remove( '\n' );
      string.Remove( '"' );

      ON_3dPoint pt;
      if( ParsePointValue(string, delimiter, pt) )
        point_list.Append( pt );
    }

    if( point_list.Count() > 0 )
    {
      int i;
      for( i = 0; i < point_list.Count(); i++ )
        doc.AddPointObject( point_list[i] );
    }

    file->Close();
    delete file;
    file = 0;

    doc.Redraw();

    return TRUE;
  }

	CATCH( CFileException, e )
	{
		e->ReportError();
		if( file )
		{
			file->Close();
			delete file;
      file = 0;
		}
		return FALSE;
	}

	AND_CATCH( CMemoryException, pEx )
	{
		AfxAbort();
	}

	END_CATCH

	return FALSE;
}

BOOL CSampleImportPointsPlugIn::ParsePointValue( const wchar_t* string, wchar_t delimiter, ON_3dPoint& pt )
{
  if( 0 == string || 0 == string[0] )
    return FALSE;

  ON_wString tag( delimiter );
  ON_3dPoint point;
  double d = 0.0;

  wchar_t* token = wcstok( (wchar_t*)string, tag );
  if( token && ParseRealValue(token, d) )
    point.x = d;
  else
    return FALSE;

  token = wcstok( 0, tag );
  if( token && ParseRealValue(token, d) )
    point.y = d;
  else
    return FALSE;

  token = wcstok( 0, tag );
  if( token )
  {
    if( ParseRealValue(token, d) )
      point.z = d;
    else
      return FALSE;
  }
  else
    point.z = 0.0;

  pt = point;

  return TRUE;
}

BOOL CSampleImportPointsPlugIn::ParseRealValue( const wchar_t* string, double& value )
{
  if( 0 == string || 0 == string[0] )
    return FALSE;

	while( *string == ' ' || *string == '\t' )
		string++;

	wchar_t first = string[0];
	double d = wcstod( string, (wchar_t**)&string );
	if( d == 0.0 && first != '0' )
    return FALSE;
	
  while( *string == ' ' || *string == '\t' )
		string++;

	if( *string != '\0' )
    return FALSE;

  value = d;

  return TRUE;
}