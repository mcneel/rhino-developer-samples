#include "StdAfx.h"
#include "SampleUserInterfacePlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMenuCommand1 command
//

class CCommandSampleMenuCommand1 : public CRhinoCommand
{
public:
  CCommandSampleMenuCommand1() {}
  ~CCommandSampleMenuCommand1() {}
  UUID CommandUUID()
  {
    // {281EE457-82E8-4053-ADDF-685878B87DC5}
    static const GUID SampleMenuCommand1Command_UUID =
    { 0x281EE457, 0x82E8, 0x4053, { 0xAD, 0xDF, 0x68, 0x58, 0x78, 0xB8, 0x7D, 0xC5 } };
    return SampleMenuCommand1Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMenuCommand1"; }
  const wchar_t* LocalCommandName() { return L"SampleMenuCommand1"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMenuCommand1 object
static class CCommandSampleMenuCommand1 theSampleMenuCommand1Command;

CRhinoCommand::result CCommandSampleMenuCommand1::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString wStr;
  wStr.Format(L"The \"%s\" command is under construction.\n", EnglishCommandName());
  if (context.IsInteractive())
    RhinoMessageBox(wStr, EnglishCommandName(), MB_OK);
  else
    RhinoApp().Print(wStr);
  return CRhinoCommand::success;
}

//
// END SampleMenuCommand1 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMenuCommand2 command
//

class CCommandSampleMenuCommand2 : public CRhinoCommand
{
public:
  CCommandSampleMenuCommand2() {}
  ~CCommandSampleMenuCommand2() {}
  UUID CommandUUID()
  {
    // {B96253EF-B140-495E-B127-2D8DA7AECE10}
    static const GUID SampleMenuCommand2Command_UUID =
    { 0xB96253EF, 0xB140, 0x495E, { 0xB1, 0x27, 0x2D, 0x8D, 0xA7, 0xAE, 0xCE, 0x10 } };
    return SampleMenuCommand2Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMenuCommand2"; }
  const wchar_t* LocalCommandName() { return L"SampleMenuCommand2"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMenuCommand2 object
static class CCommandSampleMenuCommand2 theSampleMenuCommand2Command;

CRhinoCommand::result CCommandSampleMenuCommand2::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString wStr;
  wStr.Format(L"The \"%s\" command is under construction.\n", EnglishCommandName());
  if (context.IsInteractive())
    RhinoMessageBox(wStr, EnglishCommandName(), MB_OK);
  else
    RhinoApp().Print(wStr);
  return CRhinoCommand::success;
}

//
// END SampleMenuCommand2 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMenuCommand3 command
//

class CCommandSampleMenuCommand3 : public CRhinoCommand
{
public:
  CCommandSampleMenuCommand3() {}
  ~CCommandSampleMenuCommand3() {}
  UUID CommandUUID()
  {
    // {15F1279E-EBCF-44D1-B91A-898FEBDE5A6B}
    static const GUID SampleMenuCommand3Command_UUID =
    { 0x15F1279E, 0xEBCF, 0x44D1, { 0xB9, 0x1A, 0x89, 0x8F, 0xEB, 0xDE, 0x5A, 0x6B } };
    return SampleMenuCommand3Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMenuCommand3"; }
  const wchar_t* LocalCommandName() { return L"SampleMenuCommand3"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMenuCommand3 object
static class CCommandSampleMenuCommand3 theSampleMenuCommand3Command;

CRhinoCommand::result CCommandSampleMenuCommand3::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString wStr;
  wStr.Format(L"The \"%s\" command is under construction.\n", EnglishCommandName());
  if (context.IsInteractive())
    RhinoMessageBox(wStr, EnglishCommandName(), MB_OK);
  else
    RhinoApp().Print(wStr);
  return CRhinoCommand::success;
}

//
// END SampleMenuCommand3 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMenuCommand4 command
//

class CCommandSampleMenuCommand4 : public CRhinoCommand
{
public:
  CCommandSampleMenuCommand4() {}
  ~CCommandSampleMenuCommand4() {}
  UUID CommandUUID()
  {
    // {25E02B0B-EEEC-425A-8E5E-52F5382A80B2}
    static const GUID SampleMenuCommand4Command_UUID =
    { 0x25E02B0B, 0xEEEC, 0x425A, { 0x8E, 0x5E, 0x52, 0xF5, 0x38, 0x2A, 0x80, 0xB2 } };
    return SampleMenuCommand4Command_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleMenuCommand4"; }
  const wchar_t* LocalCommandName() { return L"SampleMenuCommand4"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleMenuCommand4 object
static class CCommandSampleMenuCommand4 theSampleMenuCommand4Command;

CRhinoCommand::result CCommandSampleMenuCommand4::RunCommand(const CRhinoCommandContext& context)
{
  ON_wString wStr;
  wStr.Format(L"The \"%s\" command is under construction.\n", EnglishCommandName());
  if (context.IsInteractive())
    RhinoMessageBox(wStr, EnglishCommandName(), MB_OK);
  else
    RhinoApp().Print(wStr);
  return CRhinoCommand::success;
}

//
// END SampleMenuCommand4 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
