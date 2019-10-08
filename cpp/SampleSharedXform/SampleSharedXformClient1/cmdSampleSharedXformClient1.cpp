#include "StdAfx.h"
#include "SampleSharedXformClient1PlugIn.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSharedXformClient1 command
//

#pragma region SampleSharedXformClient1 command

class CCommandSampleSharedXformClient1 : public CRhinoCommand
{
public:
  CCommandSampleSharedXformClient1() = default;
  ~CCommandSampleSharedXformClient1() = default;
  UUID CommandUUID() override
  {
    // {F39D28C3-2C6E-45D3-BD6A-18D59378D7B8}
    static const GUID SampleSharedXformClient1Command_UUID =
    { 0xF39D28C3, 0x2C6E, 0x45D3, { 0xBD, 0x6A, 0x18, 0xD5, 0x93, 0x78, 0xD7, 0xB8 } };
    return SampleSharedXformClient1Command_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleSharedXformClient1"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleSharedXformClient1 object
static class CCommandSampleSharedXformClient1 theSampleSharedXformClient1Command;

CRhinoCommand::result CCommandSampleSharedXformClient1::RunCommand(const CRhinoCommandContext& context)
{
  RhinoApp().Print(L"%s:\n", EnglishCommandName());

  RhinoApp().Print(L"  TransformTable.Count: %d\n", CSampleSharedXformTable::Count());

  ON_SimpleArray<ON_UUID> uuids;
  const int uuid_count = CSampleSharedXformTable::Ids(uuids);
  if (uuid_count > 0)
  {
    for (int i = 0; i < uuid_count; i++)
    {
      ON_SimpleArray<ON_Xform> xforms;
      const int xform_count = CSampleSharedXformTable::Transforms(uuids[i], xforms);

      ON_wString str;
      ON_UuidToString(uuids[i], str);
      const wchar_t* psz = static_cast<const wchar_t*>(str);

      RhinoApp().Print(L"  Object id: %s, transform count: %d\n", psz, xform_count);
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleSharedXformClient1 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
