#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleObjectIterator command
//

#pragma region SampleObjectIterator command

class CCommandSampleObjectIterator : public CRhinoCommand
{
public:
  CCommandSampleObjectIterator() = default;
  UUID CommandUUID() override
  {
    // {98FC1690-C782-46E5-AFF2-688EF0EFCC80}
    static const GUID SampleObjectIteratorCommand_UUID =
    { 0x98FC1690, 0xC782, 0x46E5, { 0xAF, 0xF2, 0x68, 0x8E, 0xF0, 0xEF, 0xCC, 0x80 } };
    return SampleObjectIteratorCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleObjectIterator"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  const wchar_t* m_pszNormal = L"Normal";
  const wchar_t* m_pszLocked = L"Locked";
  const wchar_t* m_pszHidden = L"Hidden";
  const wchar_t* m_pszActive = L"Active";
  const wchar_t* m_pszReference = L"Reference";
  const wchar_t* m_pszNo = L"No";
  const wchar_t* m_pszYes = L"Yes";
};

// The one and only CCommandSampleObjectIterator object
static class CCommandSampleObjectIterator theSampleObjectIteratorCommand;

CRhinoCommand::result CCommandSampleObjectIterator::RunCommand(const CRhinoCommandContext& context)
{
  bool bNormal = true;
  bool bLocked = true;
  bool bHidden = true;
  bool bActive = true;
  bool bReference = true;;

  Settings().GetBool(m_pszNormal, bNormal, bNormal);
  Settings().GetBool(m_pszLocked, bLocked, bLocked);
  Settings().GetBool(m_pszHidden, bHidden, bHidden);
  Settings().GetBool(m_pszActive, bActive, bActive);
  Settings().GetBool(m_pszReference, bReference, bReference);

  CRhinoGetOption go;
  go.SetCommandPrompt(L"Object iterator options");
  go.AcceptNothing();
  go.AddCommandOptionToggle(RHCMDOPTNAME(m_pszNormal), RHCMDOPTVALUE(m_pszNo), RHCMDOPTVALUE(m_pszYes), bNormal, &bNormal);
  go.AddCommandOptionToggle(RHCMDOPTNAME(m_pszLocked), RHCMDOPTVALUE(m_pszNo), RHCMDOPTVALUE(m_pszYes), bLocked, &bLocked);
  go.AddCommandOptionToggle(RHCMDOPTNAME(m_pszHidden), RHCMDOPTVALUE(m_pszNo), RHCMDOPTVALUE(m_pszYes), bHidden, &bHidden);
  go.AddCommandOptionToggle(RHCMDOPTNAME(m_pszActive), RHCMDOPTVALUE(m_pszNo), RHCMDOPTVALUE(m_pszYes), bActive, &bActive);
  go.AddCommandOptionToggle(RHCMDOPTNAME(m_pszReference), RHCMDOPTVALUE(m_pszNo), RHCMDOPTVALUE(m_pszYes), bReference, &bReference);

  while (true)
  {
    CRhinoGet::result res = go.GetOption();

    if (res == CRhinoGet::option)
      continue;

    if (res != CRhinoGet::nothing)
      return CRhinoCommand::cancel;

    if (!bNormal && !bLocked && !bHidden)
    {
      RhinoApp().Print(L"Either \"%ls\", \"%ls\" or \"%ls\" must be \"%ls\"\n",
        m_pszNormal, m_pszLocked, m_pszHidden, m_pszYes
      );
      continue;
    }

    if (!bActive && !bReference)
    {
      RhinoApp().Print(L"Either \"%ls\" or \"%ls\" must be \"%ls\"\n",
        m_pszActive, m_pszReference, m_pszYes
      );
      continue;
    }

    break;
  }

  unsigned int state = 0;
  if (bNormal)
    state |= CRhinoObjectIterator::normal_objects;
  if (bLocked)
    state |= CRhinoObjectIterator::locked_objects;
  if (bHidden)
    state |= CRhinoObjectIterator::hidden_objects;

  unsigned int category = 0;
  if (bActive)
    category |= CRhinoObjectIterator::active_objects;
  if (bReference)
    category |= CRhinoObjectIterator::reference_objects;

  CRhinoObjectIterator iterator(
    context.m_doc,
    (CRhinoObjectIterator::object_state)state,
    (CRhinoObjectIterator::object_category)category
  );

  CRhinoObject* pObject = nullptr;
  for (pObject = iterator.First(); nullptr != pObject; pObject = iterator.Next())
  {
    ON_wString str;
    ON_UuidToString(pObject->ModelObjectId(), str);

    RhinoApp().Print(L"%ls, %ls, %ls, %ls\n",
      pObject->ShortDescription(false),
      static_cast<const wchar_t*>(str),
      pObject->IsNormal() ? m_pszNormal : pObject->IsLocked() ? m_pszLocked : m_pszHidden,
      pObject->IsReference() ? m_pszReference : m_pszActive
    );
  }

  Settings().SetBool(m_pszNormal, bNormal);
  Settings().SetBool(m_pszLocked, bLocked);
  Settings().SetBool(m_pszHidden, bHidden);
  Settings().SetBool(m_pszActive, bActive);
  Settings().SetBool(m_pszReference, bReference);

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleObjectIterator command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
