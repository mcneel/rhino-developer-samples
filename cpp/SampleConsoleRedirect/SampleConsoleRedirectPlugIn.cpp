#include "StdAfx.h"
#include "rhinoSdkPlugInDeclare.h"
#include "SampleConsoleRedirectPlugIn.h"
#include "Resource.h"

#pragma warning(push)
#pragma warning(disable : 4073)
#pragma init_seg(lib)
#pragma warning(pop)

// Rhino plug-in declarations
RHINO_PLUG_IN_DECLARE
RHINO_PLUG_IN_NAME(L"SampleConsoleRedirect");
RHINO_PLUG_IN_ID(L"E59AA20A-7E9B-4B6F-B899-D4A7299ABE35");
RHINO_PLUG_IN_VERSION(__DATE__ "  " __TIME__)
RHINO_PLUG_IN_DESCRIPTION(L"SampleConsoleRedirect plug-in for Rhinoceros®");
RHINO_PLUG_IN_ICON_RESOURCE_ID(IDI_ICON);
RHINO_PLUG_IN_DEVELOPER_ORGANIZATION(L"Robert McNeel & Associates");
RHINO_PLUG_IN_DEVELOPER_ADDRESS(L"3670 Woodland Park Avenue North\r\nSeattle WA 98103");
RHINO_PLUG_IN_DEVELOPER_COUNTRY(L"United States");
RHINO_PLUG_IN_DEVELOPER_PHONE(L"206-545-6877");
RHINO_PLUG_IN_DEVELOPER_FAX(L"206-545-7321");
RHINO_PLUG_IN_DEVELOPER_EMAIL(L"devsupport@mcneel.com");
RHINO_PLUG_IN_DEVELOPER_WEBSITE(L"http://www.rhino3d.com");
RHINO_PLUG_IN_UPDATE_URL(L"https://github.com/mcneel/rhino-developer-samples");

// The one and only CSampleConsoleRedirectPlugIn object
static class CSampleConsoleRedirectPlugIn thePlugIn;

/////////////////////////////////////////////////////////////////////////////
// CSampleConsoleRedirectPlugIn definition

CSampleConsoleRedirectPlugIn& SampleConsoleRedirectPlugIn()
{
	return thePlugIn;
}

CSampleConsoleRedirectPlugIn::CSampleConsoleRedirectPlugIn()
{
	m_plugin_version = RhinoPlugInVersion();
}

/////////////////////////////////////////////////////////////////////////////
// Required overrides

const wchar_t* CSampleConsoleRedirectPlugIn::PlugInName() const
{
	return RhinoPlugInName();
}

const wchar_t* CSampleConsoleRedirectPlugIn::PlugInVersion() const
{
	return m_plugin_version;
}

GUID CSampleConsoleRedirectPlugIn::PlugInID() const
{
	// {E59AA20A-7E9B-4B6F-B899-D4A7299ABE35}
	return ON_UuidFromString(RhinoPlugInId());
}

/////////////////////////////////////////////////////////////////////////////
// Additional overrides

BOOL CSampleConsoleRedirectPlugIn::OnLoadPlugIn()
{
#if RHINO_DEBUG_PLUGIN
  CreateOutputConsole();
#endif

	// TODO: Add plug-in initialization code here.
	return TRUE;
}

void CSampleConsoleRedirectPlugIn::OnUnloadPlugIn()
{
	// TODO: Add plug-in cleanup code here.
}

bool CSampleConsoleRedirectPlugIn::CreateOutputConsole()
{
  bool rc = AllocConsole();
  if (rc)
  {
    SetConsoleTitle(PlugInName());

    // Redirect the CRT standard input, output, and error handles to the console
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stderr);
    freopen("CONOUT$", "w", stdout);

    DWORD dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    DWORD dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
    DWORD dwCreationDisposition = OPEN_EXISTING;
    DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;

    HANDLE hStdout = CreateFile(L"CONOUT$", dwDesiredAccess, dwShareMode, nullptr, dwCreationDisposition, dwFlagsAndAttributes, nullptr);
    HANDLE hStdin = CreateFile(L"CONIN$", dwDesiredAccess, dwShareMode, nullptr, dwCreationDisposition, dwFlagsAndAttributes, nullptr);

    SetStdHandle(STD_OUTPUT_HANDLE, hStdout);
    SetStdHandle(STD_ERROR_HANDLE, hStdout);
    SetStdHandle(STD_INPUT_HANDLE, hStdin);

    // Clear the error state for each of the C++ standard stream objects. 
    std::clog.clear();
    std::wclog.clear();
    std::cout.clear();
    std::wcout.clear();
    std::cerr.clear();
    std::wcerr.clear();
    std::cin.clear();
    std::wcin.clear();

    std::wcout << PlugInVersion() << std::endl;
  }
  return rc;
}
