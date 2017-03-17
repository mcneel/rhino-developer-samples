#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDumpMaterialUserData command
//

#pragma region SampleDumpMaterialUserData command

class CCommandSampleDumpMaterialUserData : public CRhinoCommand
{
public:
  CCommandSampleDumpMaterialUserData() {}
  ~CCommandSampleDumpMaterialUserData() {}
  UUID CommandUUID()
  {
    // {F8D47A6-2252-4FE7-BC99-46564E1F751B}
    static const GUID SampleDumpMaterialUserDataCommand_UUID =
    { 0xF8D47A6, 0x2252, 0x4FE7, { 0xBC, 0x99, 0x46, 0x56, 0x4E, 0x1F, 0x75, 0x1B } };
    return SampleDumpMaterialUserDataCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleDumpMaterialUserData"; }
  const wchar_t* LocalCommandName() const { return L"SampleDumpMaterialUserData"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext&);
};

// The one and only CCommandSampleDumpMaterialUserData object
static class CCommandSampleDumpMaterialUserData theSampleDumpMaterialUserDataCommand;

CRhinoCommand::result CCommandSampleDumpMaterialUserData::RunCommand(const CRhinoCommandContext& context)
{
  CRhinoApp& theApp = RhinoApp();

  const int material_count = context.m_doc.m_material_table.MaterialCount();
  if (0 == material_count)
  {
    theApp.Print(L"No materials to dump\n");
    return CRhinoCommand::success;
  }

  for (int i = 0; i < material_count; i++)
  {
    const CRhinoMaterial& material = context.m_doc.m_material_table[i];
    if (material.IsDeleted())
      continue;

    theApp.Print(L"Material = %d\n", i);

    ON_wString str;
    ON_UuidToString(material.Id(), str);
    theApp.Print(L"  Material ID = %s\n", (const wchar_t*)str);

    str = L"<none>";
    if (!material.Name().IsEmpty())
      str = material.Name();
    theApp.Print(L"  Material Name = %s\n", (const wchar_t*)str);

    str = L"<none>";
    if (ON_UuidIsNotNil(material.MaterialPlugInId()))
      ON_UuidToString(material.MaterialPlugInId(), str);
    theApp.Print(L"  PlugIn ID = %s\n", (const wchar_t*)str);

    int user_data_count = 0;
    ON_UserData* user_data = material.FirstUserData();
    while (0 != user_data)
    {
      theApp.Print(L"  UserData = %d\n", user_data_count++);

      str.Empty();
      ON_UuidToString(user_data->m_userdata_uuid, str);
      theApp.Print(L"    UserData ID = %s\n", (const wchar_t*)str);

      str.Empty();
      ON_UuidToString(user_data->m_application_uuid, str);
      theApp.Print(L"    UserData PlugIn ID = %s\n", (const wchar_t*)str);

      str.Empty();
      if (!user_data->GetDescription(str))
        str = L"<none>";
      theApp.Print(L"    UserData Description = %s\n", (const wchar_t*)str);

      user_data = user_data->Next();
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDumpMaterialUserData command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
