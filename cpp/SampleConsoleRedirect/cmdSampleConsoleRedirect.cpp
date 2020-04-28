#include "StdAfx.h"
#include "SampleConsoleRedirectPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleConsoleRedirect command
//

#pragma region SampleConsoleRedirect command

class CCommandSampleConsoleRedirect : public CRhinoCommand
{
public:
  CCommandSampleConsoleRedirect() = default;
  ~CCommandSampleConsoleRedirect() = default;
  UUID CommandUUID() override
  {
    // {A0071476-C6AB-4D83-86BE-DB6CAFDD8DC6}
    static const GUID SampleConsoleRedirectCommand_UUID =
    { 0xA0071476, 0xC6AB, 0x4D83, { 0x86, 0xBE, 0xDB, 0x6C, 0xAF, 0xDD, 0x8D, 0xC6 } };
    return SampleConsoleRedirectCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleConsoleRedirect"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleConsoleRedirect object
static class CCommandSampleConsoleRedirect theSampleConsoleRedirectCommand;

CRhinoCommand::result CCommandSampleConsoleRedirect::RunCommand(const CRhinoCommandContext& context)
{
  // Test stdio
  printf("Test printf output to stdout\n");
  fprintf(stdout, "Test fprintf output to stdout\n");
  fprintf(stderr, "Test fprintf output to stderr\n");

  // Test wide stdio
  wprintf(L"Test wprintf output to stdout\n");
  fwprintf(stdout, L"Test fwprintf output to stdout\n");
  fwprintf(stderr, L"Test fwprintf output to stderr\n");

  // Test iostreams
  std::cout << "Test output to cout" << std::endl;
  std::cerr << "Test output to cerr" << std::endl;
  std::clog << "Test output to clog" << std::endl;

  // Test wide iostreams
  std::wcout << L"Test output to wcout" << std::endl;
  std::wcerr << L"Test output to wcerr" << std::endl;
  std::wclog << L"Test output to wclog" << std::endl;

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleConsoleRedirect command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
