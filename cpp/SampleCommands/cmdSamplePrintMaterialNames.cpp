#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SamplePrintMaterialNames command
//

#pragma region SamplePrintMaterialNames command

class CCommandSamplePrintMaterialNames : public CRhinoCommand
{
public:
  CCommandSamplePrintMaterialNames() = default;
  UUID CommandUUID() override
  {
    // {8B3D01CD-56F7-49DC-B9D2-0297D80D29FF}
    static const GUID SamplePrintMaterialNamesCommand_UUID =
    { 0x8B3D01CD, 0x56F7, 0x49DC, { 0xB9, 0xD2, 0x02, 0x97, 0xD8, 0x0D, 0x29, 0xFF } };
    return SamplePrintMaterialNamesCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SamplePrintMaterialNames"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  void PrintMaterialTable(CRhinoDoc& doc);
  void PrintRdkMaterialTable(CRhinoDoc& doc);
};

// The one and only CCommandSamplePrintMaterialNames object
static class CCommandSamplePrintMaterialNames theSamplePrintMaterialNamesCommand;

CRhinoCommand::result CCommandSamplePrintMaterialNames::RunCommand(const CRhinoCommandContext& context)
{
  PrintMaterialTable(context.m_doc);
  PrintRdkMaterialTable(context.m_doc);
  return CRhinoCommand::success;
}

void CCommandSamplePrintMaterialNames::PrintMaterialTable(CRhinoDoc& doc)
{
  const CRhinoMaterialTable& material_table = doc.m_material_table;
  int index = 0;
  for (int i = 0; i < material_table.MaterialCount(); i++)
  {
    const CRhinoMaterial& material = material_table[i];
    if (!material.IsDeleted())
    {
      ON_wString name = material.Name();
      RhinoApp().Print(L"Material(%d) = %s\n", index++, static_cast<const wchar_t*>(name));
    }
  }
}

void CCommandSamplePrintMaterialNames::PrintRdkMaterialTable(CRhinoDoc& doc)
{
  const CRhRdkDocument& rdk_doc = CRhRdkDocument::Get(doc);
  const IRhRdkContentList& material_list = rdk_doc.MaterialList();

  IRhRdkContentList::Iterator* iterator = material_list.NewIterator();
  if (nullptr == iterator)
    return;

  int index = 0;
  const CRhRdkContent* content = nullptr;
  while (nullptr != (content = iterator->Next()))
  {
    if (content->IsKind(CRhRdkContent::Kinds::Material))
    {
      const CRhRdkMaterial* material = static_cast<const CRhRdkMaterial*>(content);
      if (nullptr != material)
      {
        ON_wString name = material->InstanceName();
        RhinoApp().Print(L"RDK Material(%d) = %s\n", index++, static_cast<const wchar_t*>(name));
      }
    }
  }
}

#pragma endregion

//
// END SamplePrintMaterialNames command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
