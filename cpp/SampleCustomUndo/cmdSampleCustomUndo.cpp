#include "StdAfx.h"
#include "SampleCustomUndoPlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomUndoEventHandler undo event handler
//

class CSampleCustomUndoEventHandler : public CRhinoUndoEventHandler
{
public:
  CSampleCustomUndoEventHandler();
  ~CSampleCustomUndoEventHandler() {}

  /*
  Description:
    The Rhino Undo command will call this virtual Undo function when
      the event needs to be undone. You should override this function
      to undo changes in your plug-in data.
    WARNING: Never change any setting in the Rhino document or application.
      Rhino handles ALL changes to the application and document and you will
      break the Undo/Redo commands if you make any changes to the application
      or document.
  Parameters:
    cmd                - [in] Command being undone. Can be NULL.
    action_description - [in] Description of what is being undone.  Can be NULL.
    bCreatedByRedo     - [in] True if a "Redo" was responsible for creating this record.
    undo_event_sn      - [in] Runtime serial number Rhino assigned to the event.
  */
  void Undo(const CRhinoCommand* cmd, const wchar_t* action_description, bool bCreatedByRedo, unsigned int undo_event_sn);

public:
  double m_amount;
};

CSampleCustomUndoEventHandler::CSampleCustomUndoEventHandler()
{
  m_amount = 0.0;
}

void CSampleCustomUndoEventHandler::Undo(const CRhinoCommand* cmd, const wchar_t* action_description, bool bCreatedByRedo, unsigned int undo_event_sn)
{
  UNREFERENCED_PARAMETER(cmd);
  UNREFERENCED_PARAMETER(action_description);
  UNREFERENCED_PARAMETER(bCreatedByRedo);
  UNREFERENCED_PARAMETER(undo_event_sn);

  // Undo the change to CSampleCustomUndoPlugIn::m_balance
  double balance = SampleCustomUndoPlugIn().Balance();
  balance -= m_amount;
  SampleCustomUndoPlugIn().SetBalance(balance);
  RhinoApp().Print(L"New balance: %g\n", balance);
}

//
// END SampleCustomUndoEventHandler undo event handler
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomUndoEarnTen command
//

#pragma region SampleCustomUndoEarnTen command

class CCommandSampleCustomUndoEarnTen : public CRhinoCommand
{
public:
  CCommandSampleCustomUndoEarnTen() {}
  ~CCommandSampleCustomUndoEarnTen() {}
  UUID CommandUUID()
  {
    // {B2A558B6-D81-4FAF-B828-8A6A91DA7018}
    static const GUID SampleCustomUndoEarnTenCommand_UUID =
    { 0xB2A558B6, 0xD81, 0x4FAF, { 0xB8, 0x28, 0x8A, 0x6A, 0x91, 0xDA, 0x70, 0x18 } };
    return SampleCustomUndoEarnTenCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCustomUndoEarnTen"; }
  const wchar_t* LocalCommandName() const { return L"SampleCustomUndoEarnTen"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleCustomUndoEarnTen object
static class CCommandSampleCustomUndoEarnTen theSampleCustomUndoEarnTenCommand;

CRhinoCommand::result CCommandSampleCustomUndoEarnTen::RunCommand(const CRhinoCommandContext& context)
{
  const double amount = 10.0;

  CSampleCustomUndoEventHandler* pUndoHandler = new CSampleCustomUndoEventHandler();
  pUndoHandler->m_amount = amount;
  context.m_doc.AddCustomUndoEvent(pUndoHandler);

  double balance = SampleCustomUndoPlugIn().Balance();
  balance += amount;
  SampleCustomUndoPlugIn().SetBalance(balance);

  RhinoApp().Print(L"New balance: %g\n", balance);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCustomUndoEarnTen command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomUndoSpendFive command
//

#pragma region SampleCustomUndoSpendFive command

class CCommandSampleCustomUndoSpendFive : public CRhinoCommand
{
public:
  CCommandSampleCustomUndoSpendFive() {}
  ~CCommandSampleCustomUndoSpendFive() {}
  UUID CommandUUID()
  {
    // {AEB24D28-3A28-483F-B999-0867597E2C55}
    static const GUID SampleCustomUndoSpendFiveCommand_UUID =
    { 0xAEB24D28, 0x3A28, 0x483F, { 0xB9, 0x99, 0x08, 0x67, 0x59, 0x7E, 0x2C, 0x55 } };
    return SampleCustomUndoSpendFiveCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCustomUndoSpendFive"; }
  const wchar_t* LocalCommandName() const { return L"SampleCustomUndoSpendFive"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleCustomUndoSpendFive object
static class CCommandSampleCustomUndoSpendFive theSampleCustomUndoSpendFiveCommand;

CRhinoCommand::result CCommandSampleCustomUndoSpendFive::RunCommand(const CRhinoCommandContext& context)
{
  const double amount = -5.0;

  CSampleCustomUndoEventHandler* pUndoHandler = new CSampleCustomUndoEventHandler();
  pUndoHandler->m_amount = amount;
  context.m_doc.AddCustomUndoEvent(pUndoHandler);

  double balance = SampleCustomUndoPlugIn().Balance();
  balance += amount;
  SampleCustomUndoPlugIn().SetBalance(balance);

  RhinoApp().Print(L"New balance: %g\n", balance);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCustomUndoSpendFive command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCustomUndo command
//

#pragma region SampleCustomUndo command

class CCommandSampleCustomUndo : public CRhinoCommand
{
public:
  CCommandSampleCustomUndo() {}
  ~CCommandSampleCustomUndo() {}
  UUID CommandUUID()
  {
    // {913C87E9-9FF4-4C7E-A852-8D665F5FA6EC}
    static const GUID SampleCustomUndoCommand_UUID =
    { 0x913C87E9, 0x9FF4, 0x4C7E, { 0xA8, 0x52, 0x8D, 0x66, 0x5F, 0x5F, 0xA6, 0xEC } };
    return SampleCustomUndoCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleCustomUndo"; }
  const wchar_t* LocalCommandName() const { return L"SampleCustomUndo"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleCustomUndo object
static class CCommandSampleCustomUndo theSampleCustomUndoCommand;

CRhinoCommand::result CCommandSampleCustomUndo::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);
  RhinoApp().Print(L"%s plug-in loaded.\n", SampleCustomUndoPlugIn().PlugInName());
  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleCustomUndo command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
