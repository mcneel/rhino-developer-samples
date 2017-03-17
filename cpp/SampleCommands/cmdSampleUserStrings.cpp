#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSetUserStrings command
//

#pragma region SampleSetUserStrings command

class CCommandSampleSetUserStrings : public CRhinoCommand
{
public:
  CCommandSampleSetUserStrings();
  ~CCommandSampleSetUserStrings() {}
  UUID CommandUUID()
  {
    // {98455B14-BF0D-4DC1-BE9C-BB59723602BC}
    static const GUID SampleSetUserStringsCommand_UUID =
    { 0x98455B14, 0xBF0D, 0x4DC1, { 0xBE, 0x9C, 0xBB, 0x59, 0x72, 0x36, 0x02, 0xBC } };
    return SampleSetUserStringsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleSetUserStrings"; }
  const wchar_t* LocalCommandName() const { return L"SampleSetUserStrings"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);

private:
  ON_wString ToEnglish(const int number);

private:
  ON_ClassArray<ON_wString> m_numbers;
  ON_ClassArray<ON_wString> m_prefixes;
};

// The one and only CCommandSampleSetUserStrings object
static class CCommandSampleSetUserStrings theSampleSetUserStringsCommand;

CCommandSampleSetUserStrings::CCommandSampleSetUserStrings()
{
  m_numbers.Append(L"zero");
  m_numbers.Append(L"one");
  m_numbers.Append(L"two");
  m_numbers.Append(L"three");
  m_numbers.Append(L"four");
  m_numbers.Append(L"five");
  m_numbers.Append(L"six");
  m_numbers.Append(L"seven");
  m_numbers.Append(L"eight");
  m_numbers.Append(L"nine");
  m_numbers.Append(L"ten");
  m_numbers.Append(L"eleven");
  m_numbers.Append(L"twelve");
  m_numbers.Append(L"thirteen");
  m_numbers.Append(L"fourteen");

  m_prefixes.Append(L"twen");
  m_prefixes.Append(L"thir");
  m_prefixes.Append(L"for");
  m_prefixes.Append(L"fif");
  m_prefixes.Append(L"six");
  m_prefixes.Append(L"seven");
  m_prefixes.Append(L"eigh");
  m_prefixes.Append(L"nine");
}

CRhinoCommand::result CCommandSampleSetUserStrings::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object to set user strings");
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& object_ref = go.Object(0);
  const CRhinoObject* rhino_object = object_ref.Object();
  if (0 == rhino_object)
    return CRhinoCommand::failure;

  const wchar_t* key0 = L"Integer";
  const wchar_t* key1 = L"English";

  //ON_Object* obj = const_cast<ON_Geometry*>(rhino_object->Geometry());
  ON_Object* obj = const_cast<CRhinoObjectAttributes*>(&rhino_object->Attributes());

  ON_ClassArray<ON_UserString> user_strings;
  const int count = obj->GetUserStrings(user_strings);

  if (0 == count)
  {
    ON_UserString us[2];
    us[0].m_key = key0;
    us[0].m_string_value = L"0";
    us[1].m_key = key1;
    us[1].m_string_value = ToEnglish(0);
    obj->SetUserStrings(2, us, true);
  }
  else
  {
    int value = 0;
    int key0_index = -1;
    int key1_index = -1;
    for (int i = 0; i < count; i++)
    {
      if (-1 == key0_index && 0 == user_strings[i].m_key.CompareNoCase(key0))
        key0_index = i;
      else if (-1 == key1_index && 0 == user_strings[i].m_key.CompareNoCase(key1))
        key1_index = i;
      if (0 <= key0_index && 0 <= key1_index)
        break;
    }

    if (0 <= key0_index)
    {
      value = _wtoi(user_strings[key0_index].m_string_value);
      ON_wString str;
      str.Format(L"%d", ++value);
      user_strings[key0_index].m_string_value = str;
    }
    else
    {
      ON_UserString us;
      us.m_key = key0;
      us.m_string_value = L"0";
      user_strings.Append(us);
    }

    if (0 <= key1_index)
    {
      user_strings[key1_index].m_string_value = ToEnglish(value);
    }
    else
    {
      ON_UserString us;
      us.m_key = key1;
      us.m_string_value = ToEnglish(value);
      user_strings.Append(us);
    }

    obj->SetUserStrings(user_strings.Count(), user_strings.Array(), true);
  }

  return CRhinoCommand::success;
}

ON_wString CCommandSampleSetUserStrings::ToEnglish(const int number)
{
  if (number < 0)
    return ON_wString(L"minus ") + ToEnglish(-number);

  if (number <= 14)
    return m_numbers[number];

  if (number < 20)
    return m_prefixes[number - 12] + "teen";

  if (number < 100)
  {
    unsigned int remainder = number - (static_cast<int>(number / 10) * 10);
    return m_prefixes[number / 10 - 2] + (0 != remainder ? ON_wString(L"ty ") + ToEnglish(remainder) : ON_wString(L"ty"));
  }

  if (number < 1000)
  {
    unsigned int remainder = number - (static_cast<int>(number / 100) * 100);
    return m_numbers[number / 100] + (0 != remainder ? ON_wString(L" hundred ") + ToEnglish(remainder) : ON_wString(" hundred"));
  }

  if (number < 1000000)
  {
    unsigned int thousands = static_cast<int>(number / 1000);
    unsigned int remainder = number - (thousands * 1000);
    return ToEnglish(thousands) + (0 != remainder ? ON_wString(L" thousand ") + ToEnglish(remainder) : ON_wString(L" thousand"));
  }

  if (number < 1000000000)
  {
    unsigned int millions = static_cast<int>(number / 1000000);
    unsigned int remainder = number - (millions * 1000000);
    return ToEnglish(millions) + (0 != remainder ? ON_wString(L" million ") + ToEnglish(remainder) : ON_wString(L" million"));
  }

  return ON_wString(L"Number too large");
}

#pragma endregion

//
// END SampleSetUserStrings command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGetUserStrings command
//

#pragma region SampleGetUserStrings command

class CCommandSampleGetUserStrings : public CRhinoCommand
{
public:
  CCommandSampleGetUserStrings() {}
  ~CCommandSampleGetUserStrings() {}
  UUID CommandUUID()
  {
    // {AC1A1733-27DE-437B-B3F3-A8CB80683BFC}
    static const GUID SampleGetUserStringsCommand_UUID =
    { 0xAC1A1733, 0x27DE, 0x437B, { 0xB3, 0xF3, 0xA8, 0xCB, 0x80, 0x68, 0x3B, 0xFC } };
    return SampleGetUserStringsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGetUserStrings"; }
  const wchar_t* LocalCommandName() const { return L"SampleGetUserStrings"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleGetUserStrings object
static class CCommandSampleGetUserStrings theSampleGetUserStringsCommand;

CRhinoCommand::result CCommandSampleGetUserStrings::RunCommand(const CRhinoCommandContext& context)
{
  UNREFERENCED_PARAMETER(context);

  CRhinoGetObject go;
  go.SetCommandPrompt(L"Select object to get user strings");
  go.EnableSubObjectSelect(FALSE);
  go.GetObjects(1, 1);
  if (go.CommandResult() != CRhinoCommand::success)
    return go.CommandResult();

  const CRhinoObjRef& obj_ref = go.Object(0);
  const CRhinoObject* obj = obj_ref.Object();
  if (0 == obj)
    return CRhinoCommand::failure;

  ON_ClassArray<ON_UserString> user_strings;
  const int count = obj->Attributes().GetUserStrings(user_strings);

  if (0 == count)
  {
    RhinoApp().Print(L"Object has not attribute user strings.\n");
  }
  else
  {
    for (int i = 0; i < count; i++)
    {
      RhinoApp().Print(L"Key: %s, Value: %s\n", (const wchar_t*)user_strings[i].m_key, (const wchar_t*)user_strings[i].m_string_value);
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleGetUserStrings command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
