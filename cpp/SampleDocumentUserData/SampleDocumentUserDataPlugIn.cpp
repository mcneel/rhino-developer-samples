/////////////////////////////////////////////////////////////////////////////
// SampleDocumentUserDataPlugIn.cpp : defines the initialization routines for the plug-in.
//

#include "StdAfx.h"
#include "SampleDocumentUserDataPlugIn.h"

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
RHINO_PLUG_IN_NAME( L"SampleDocumentUserData" );

// Rhino plug-in id
// Provide a unique uuid for this plug-in
RHINO_PLUG_IN_ID( L"B00B243F-2330-49C1-BB7C-C1EF3277A207" );

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

// The one and only CSampleDocumentUserDataPlugIn object
static CSampleDocumentUserDataPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleDocumentUserDataPlugIn definition

CSampleDocumentUserDataPlugIn& SampleDocumentUserDataPlugIn()
{ 
  // Return a reference to the one and only CSampleDocumentUserDataPlugIn object
  return thePlugIn; 
}

CSampleDocumentUserDataPlugIn::CSampleDocumentUserDataPlugIn()
{
  // Description:
  //   CSampleDocumentUserDataPlugIn constructor. The constructor is called when the
  //   plug-in is loaded and "thePlugIn" is constructed. Once the plug-in
  //   is loaded, CSampleDocumentUserDataPlugIn::OnLoadPlugIn() is called. The 
  //   constructor should be simple and solid. Do anything that might fail in
  //   CSampleDocumentUserDataPlugIn::OnLoadPlugIn().

  // TODO: Add construction code here
  m_plugin_version = RhinoPlugInVersion();
}

CSampleDocumentUserDataPlugIn::~CSampleDocumentUserDataPlugIn()
{
  // Description:
  //   CSampleDocumentUserDataPlugIn destructor. The destructor is called to destroy
  //   "thePlugIn" when the plug-in is unloaded. Immediately before the
  //   DLL is unloaded, CSampleDocumentUserDataPlugIn::OnUnloadPlugin() is called. Do
  //   not do too much here. Be sure to clean up any memory you have allocated
  //   with onmalloc(), onrealloc(), oncalloc(), or onstrdup().

  // TODO: Add destruction code here
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleDocumentUserDataPlugIn::PlugInName() const
{
  // Description:
  //   Plug-in name display string.  This name is displayed by Rhino when
  //   loading the plug-in, in the plug-in help menu, and in the Rhino 
  //   interface for managing plug-ins.

  // TODO: Return a short, friendly name for the plug-in.
  return RhinoPlugInName();
}

const wchar_t* CSampleDocumentUserDataPlugIn::PlugInVersion() const
{
  // Description:
  //   Plug-in version display string. This name is displayed by Rhino 
  //   when loading the plug-in and in the Rhino interface for managing
  //   plug-ins.

  // TODO: Return the version number of the plug-in.
  return m_plugin_version;
}

GUID CSampleDocumentUserDataPlugIn::PlugInID() const
{
  // Description:
  //   Plug-in unique identifier. The identifier is used by Rhino to
  //   manage the plug-ins.

  // TODO: Return a unique identifier for the plug-in.
  // {B00B243F-2330-49C1-BB7C-C1EF3277A207}
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleDocumentUserDataPlugIn::OnLoadPlugIn()
{
  // Description:
  //   Called after the plug-in is loaded and the constructor has been
  //   run. This is a good place to perform any significant initialization,
  //   license checking, and so on.  This function must return TRUE for
  //   the plug-in to continue to load.

  // TODO: Add plug-in initialization code here.
  m_watcher.Register();
  m_watcher.Enable( TRUE );

  return CRhinoUtilityPlugIn::OnLoadPlugIn();
}

void CSampleDocumentUserDataPlugIn::OnUnloadPlugIn()
{
  // Description:
  //   Called when the plug-in is about to be unloaded.  After
  //   this function is called, the destructor will be called.

  // TODO: Add plug-in cleanup code here.

  CRhinoUtilityPlugIn::OnUnloadPlugIn();
}

BOOL CSampleDocumentUserDataPlugIn::CallWriteDocument( const CRhinoFileWriteOptions& options )
{
  // Only write document data if there is something to write
  return ( m_string_table.Count() > 0 ) ? TRUE : FALSE;
}

BOOL CSampleDocumentUserDataPlugIn::WriteDocument( CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileWriteOptions& options )
{
  // Write plug-in document data.  See rhinoSdkPlugIn.h details.

  // If the user is exporting selected geometry, just return
  bool bSelectedMode = options.Mode( CRhinoFileWriteOptions::SelectedMode );
  if( bSelectedMode )
    return TRUE;

  // Write the major and minor version of document as document data
  if( !archive.Write3dmChunkVersion(1, 0) )
    return FALSE;

  // Write the number of string we intend to write as document data
  if( !archive.WriteInt(m_string_table.Count()) )
    return FALSE;
  
  // Write our string table as document data
  for( int i = 0; i < m_string_table.Count(); i++ )
  {
    if( !archive.WriteString(m_string_table[i]) )
      return FALSE;
  }

  return TRUE;
}

BOOL CSampleDocumentUserDataPlugIn::ReadDocument( CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileReadOptions& options )
{
  // Read plug-in document data.  See rhinoSdkPlugIn.h details.

  // If our document data is found in a 3DM file, we are 
  // required to read it. But, we don't necessarily want
  // to use it.

  BOOL bKeepData = FALSE;
  BOOL bNewMode = options.Mode( CRhinoFileReadOptions::NewMode );
  BOOL bOpenMode = options.Mode( CRhinoFileReadOptions::OpenMode );

  // If the user is creating a new model, or opening an existing
  // model, we'll want to keep the document data that we read.
  // Otherwise, we'll read the data, but not use it.  This way,
  // we won't overwrite out data if the user is importing geometry,
  // from another 3DM file.
  if( bNewMode || bOpenMode )
    bKeepData = TRUE;

  if( bKeepData )
    m_string_table.Empty();

  // Read the major and minor version of the document data
  int major = 0, minor = 0;
  if( !archive.Read3dmChunkVersion(&major, &minor) )
    return FALSE;

  // At this point, if we've changed the format of
  // our document data, we'll want to compare the
  // major and minor revision numbers and read our
  // data accordingly.
  if( 1 != major && 0 != minor )
    return FALSE;

  // Read the number of string we intend to read as document data
  int count = 0;
  if( !archive.ReadInt(&count) )
    return FALSE;

  // Read our string table as document data
  ON_wString str;
  for( int i = 0; i < count; i++ )
  {
    if( !archive.ReadString(str) )
      return FALSE;
    
    // Only append the string if we are
    // supposed to
    if( bKeepData )
      m_string_table.Append(str);
  }

  return TRUE;
}

int CSampleDocumentUserDataPlugIn::StringTableCount() const
{
  return m_string_table.Count();
}

ON_wString CSampleDocumentUserDataPlugIn::GetStringTableItem( int index )
{
  ON_wString str;
  if( index >= 0 && index < StringTableCount() )
    str = m_string_table[index];
  return str;
}

int CSampleDocumentUserDataPlugIn::FindStringTableItem( const wchar_t* str )
{
  if( 0 == str || 0 == str[0] )
    return -1;

  for( int i = 0; i < m_string_table.Count(); i++ )
  {
    ON_wString item = m_string_table[i];
    if( 0 == item.CompareNoCase(str) )
      return i;
  }

  return -1;
}

int CSampleDocumentUserDataPlugIn::AddStringTableItem( const wchar_t* str )
{
  if( 0 == str || 0 == str[0] )
    return -1;

  int index = FindStringTableItem( str );
  if( index < 0 )
  {
    m_string_table.Append( str );
    return StringTableCount();
  }

  return -1;
}
  
bool CSampleDocumentUserDataPlugIn::DeleteStringTableItem( const wchar_t* str )
{
  if( 0 == str || 0 == str[0] )
    return false;

  int index = FindStringTableItem( str );
  if( index > 0 && index < StringTableCount() )
  {
    m_string_table.Remove( index );
    return true;
  }

  return false;
}

void CSampleDocumentUserDataPlugIn::ClearStringTable()
{
  m_string_table.Destroy();
}
