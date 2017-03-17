/////////////////////////////////////////////////////////////////////////////
// SampleSerializePlugIn.cpp
//

#include "StdAfx.h"
#include "SampleSerializePlugIn.h"

#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#pragma warning(pop)

// Rhino plug-in declaration
RHINO_PLUG_IN_DECLARE

// Rhino plug-in name
RHINO_PLUG_IN_NAME(L"SampleSerialize");

// Rhino plug-in id
RHINO_PLUG_IN_ID(L"868C462C-6342-4F6C-97AE-2945AF19CB70");

// Rhino plug-in version
RHINO_PLUG_IN_VERSION(__DATE__"  "__TIME__)

// Rhino plug-in developer declarations
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\015\012Seattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/");

// The one and only CSampleSerializePlugIn object
static CSampleSerializePlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleSerializePlugIn definition

CSampleSerializePlugIn& SampleSerializePlugIn()
{ 
  return thePlugIn; 
}

CSampleSerializePlugIn::CSampleSerializePlugIn()
{
  m_plugin_version = RhinoPlugInVersion();
}

CSampleSerializePlugIn::~CSampleSerializePlugIn()
{
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleSerializePlugIn::PlugInName() const
{
  return RhinoPlugInName();
}

const wchar_t* CSampleSerializePlugIn::PlugInVersion() const
{
  return m_plugin_version;
}

GUID CSampleSerializePlugIn::PlugInID() const
{
  return ON_UuidFromString( RhinoPlugInId() );
}

BOOL CSampleSerializePlugIn::OnLoadPlugIn()
{
  m_plugin_watcher.Register();
  m_plugin_watcher.Enable(TRUE);
  return TRUE;
}

void CSampleSerializePlugIn::OnUnloadPlugIn()
{
}

/////////////////////////////////////////////////////////////////////////////
// Document user data overrides

BOOL CSampleSerializePlugIn::CallWriteDocument(const CRhinoFileWriteOptions& options)
{
  // Don't save document data if only selected objects are saved
  BOOL bSelected = options.Mode(CRhinoFileWriteOptions::SelectedMode);
  return bSelected ? FALSE : TRUE;
}

BOOL CSampleSerializePlugIn::WriteDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileWriteOptions& options)
{
  // Our serialization data class handles all writing
  bool rc = m_plugin_data.Write(archive);
  return rc ? TRUE : FALSE;
}

BOOL CSampleSerializePlugIn::ReadDocument(CRhinoDoc& doc, ON_BinaryArchive& archive, const CRhinoFileReadOptions& options)
{
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