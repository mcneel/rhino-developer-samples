#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleCommandName command
//

/// <summary>
/// Rhino object, command, and option name helpers.
/// </summary>
class CRhinoNameHelper
{
public:
  /// <summary>
  /// Tests a string to see if it can be used as a name for an object,
  /// layer, material, group, font, etc.
  /// </summary>
  static bool IsValidName(const wchar_t* name);

  /// <summary>
  /// Tests a string to see if it can be used as a command name.
  /// </summary>
  static bool IsValidCommandName(const wchar_t* name);

  /// <summary>
  /// Tests a string to see if it can be used as a command option name.
  /// </summary>
  static bool IsValidOptionName(const wchar_t* option_name);

  /// <summary>
  /// Tests a string to see if it can be used as a command option value.
  /// </summary>
  static bool IsValidOptionValueName(const wchar_t* option_value);

private:
  static bool IsValidCommandNameLetter(wchar_t c);
  static bool IsValidCommandNameFirstChar(wchar_t c);
  static bool IsValidCommandNameSecondChar(wchar_t c);
  static bool IsValidOptionValueFirstChar(wchar_t c);
  static bool IsValidOptionValueSecondChar(wchar_t c);
};

bool CRhinoNameHelper::IsValidName(const wchar_t* name)
{
  return ON_ModelComponent::IsValidComponentName(name);
}

bool CRhinoNameHelper::IsValidCommandName(const wchar_t* name)
{
  bool rc = false;
  bool bHaveLetter = false;
  if (name)
  {
    bHaveLetter = IsValidCommandNameLetter(*name);
    rc = IsValidCommandNameFirstChar(*name);
    for (name++; *name && rc; name++)
    {
      if (!bHaveLetter)
        bHaveLetter = IsValidCommandNameLetter(*name);
      rc = IsValidCommandNameSecondChar(*name);
    }
    if (false == rc && bHaveLetter && '[' == *name && '[' == name[-1] && '0' <= name[1] && name[1] <= '9')
    {
      name++;
      while (*name && *name >= '0' && *name <= '9')
        name++;
      if (']' == name[0] && ']' == name[1] && 0 == name[2])
      {
        rc = true;
      }
    }
  }
  return (rc && bHaveLetter) ? true : false;
}

bool CRhinoNameHelper::IsValidOptionName(const wchar_t* option_name)
{
  if (option_name && option_name[0] && option_name[0] == '.' && option_name[1] != '.')
    option_name++;
  return IsValidCommandName(option_name);
}

bool CRhinoNameHelper::IsValidOptionValueName(const wchar_t* option_value)
{
  bool rc = false;
  if (option_value)
  {
    rc = IsValidOptionValueFirstChar(*option_value);
    for (option_value++; *option_value && rc; option_value++)
      rc = IsValidOptionValueSecondChar(*option_value);
  }
  return rc;
}

bool CRhinoNameHelper::IsValidCommandNameLetter(wchar_t c)
{
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c >= 128);
}

bool CRhinoNameHelper::IsValidCommandNameFirstChar(wchar_t c)
{
  return (IsValidCommandNameLetter(c) || (c >= '0' && c <= '9'));
}

bool CRhinoNameHelper::IsValidCommandNameSecondChar(wchar_t c)
{
  return (IsValidCommandNameFirstChar(c) || c == '_');
}

bool CRhinoNameHelper::IsValidOptionValueFirstChar(wchar_t c)
{
  return (IsValidCommandNameFirstChar(c) || c == '.' || c == '+' || c == '-');
}

bool CRhinoNameHelper::IsValidOptionValueSecondChar(wchar_t c)
{
  return (c > 32 && c != 127 && c != '(' && c != ')');
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleCommandName : public CRhinoCommand
{
public:
  CCommandSampleCommandName() = default;
  UUID CommandUUID() override
  {
    // {B7131391-14B5-4774-8494-8CFC9E8CEB55}
    static const GUID SampleCommandNameCommand_UUID =
    { 0xB7131391, 0x14B5, 0x4774, { 0x84, 0x94, 0x8C, 0xFC, 0x9E, 0x8C, 0xEB, 0x55 } };
    return SampleCommandNameCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleCommandName"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleCommandName object
static class CCommandSampleCommandName theSampleCommandNameCommand;

CRhinoCommand::result CCommandSampleCommandName::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Command name to test for validity");
  gs.GetString();
  if (CRhinoCommand::success != gs.CommandResult())
    return gs.CommandResult();

  ON_wString name = gs.String();
  name.TrimLeftAndRight();
  if (name.IsEmpty())
    return CRhinoCommand::nothing;

  const wchar_t* psz = static_cast<const wchar_t*>(name);
  bool rc = CRhinoNameHelper::IsValidCommandName(psz);
  RhinoApp().Print(L"\"%ls\" is %ls Rhino command name.\n", psz, rc ? L"a valid" : L"an invalid");

  return CRhinoCommand::success;
}

//
// END SampleCommandName command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
