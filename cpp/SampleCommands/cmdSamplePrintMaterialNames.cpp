
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
  virtual UUID CommandUUID() override { static const UUID uuid = { 0x8B3D01CD, 0x56F7, 0x49DC, { 0xB9, 0xD2, 0x02, 0x97, 0xD8, 0x0D, 0x29, 0xFF } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SamplePrintMaterialNames"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  void PrintMaterialTable(CRhinoDoc& doc);
  void PrintRdkMaterialTable(CRhinoDoc& doc);
};

// The one and only CCommandSamplePrintMaterialNames object
static class CCommandSamplePrintMaterialNames theSamplePrintMaterialNamesCommand;

CRhinoCommand::result CCommandSamplePrintMaterialNames::RunCommand(const CRhinoCommandContext& context)
{
  auto* pDoc = context.Document();
  if (nullptr == pDoc)
    return failure;

  PrintMaterialTable(*pDoc);
  PrintRdkMaterialTable(*pDoc);

  return CRhinoCommand::success;
}

void CCommandSamplePrintMaterialNames::PrintMaterialTable(CRhinoDoc& doc)
{
  int index = 0;
  for (int i = 0; i < doc.m_material_table.MaterialCount(); i++)
  {
    const auto& material = doc.m_material_table[i];
    if (!material.IsDeleted())
    {
      ON_wString name = material.Name();
      RhinoApp().Print(L"Material(%d) = %s\n", index++, static_cast<const wchar_t*>(name));
    }
  }
}

void CCommandSamplePrintMaterialNames::PrintRdkMaterialTable(CRhinoDoc& doc)
{
  auto* pIt = doc.Contents().NewIterator(CRhRdkContent::Kinds::Material, CRhRdkDocument::it_Normal);

  int index = 0;
  const CRhRdkContent* pContent = nullptr;
  while (nullptr != (pContent = pIt->Next()))
  {
    RhinoApp().Print(L"RDK Material(%d) = %s\n", index++, static_cast<const wchar_t*>(pContent->DisplayName()));
  }

  delete pIt;
}

#pragma endregion

//
// END SamplePrintMaterialNames command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
