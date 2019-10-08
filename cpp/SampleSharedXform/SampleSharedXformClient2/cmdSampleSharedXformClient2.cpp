#include "StdAfx.h"
#include "SampleSharedXformClient2PlugIn.h"
#include "SampleSharedXformSdk.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleSharedXformClient2 command
//

#pragma region SampleSharedXformClient2 command

class CCommandSampleSharedXformClient2 : public CRhinoCommand
{
public:
  CCommandSampleSharedXformClient2() = default;
  ~CCommandSampleSharedXformClient2() = default;
  UUID CommandUUID() override
  {
    // {E6DA23F9-5E68-4555-BA32-37527F47DBB1}
    static const GUID SampleSharedXformClient2Command_UUID =
    { 0xE6DA23F9, 0x5E68, 0x4555, { 0xBA, 0x32, 0x37, 0x52, 0x7F, 0x47, 0xDB, 0xB1 } };
    return SampleSharedXformClient2Command_UUID;
  }

  const wchar_t* EnglishCommandName() override { return L"SampleSharedXformClient2"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  const wchar_t* SampleSharedXformLibrary() { return L"SampleSharedXformLib.dll"; };
};

// The one and only CCommandSampleSharedXformClient2 object
static class CCommandSampleSharedXformClient2 theSampleSharedXformClient2Command;

CRhinoCommand::result CCommandSampleSharedXformClient2::RunCommand(const CRhinoCommandContext& context)
{
  CSampleSharedXformTable xform_table;
  xform_table.Load(SampleSharedXformLibrary());
  if (!xform_table.IsLoaded())
  {
    RhinoApp().Print(L"Unable to load %s\n", SampleSharedXformLibrary());
    return CRhinoCommand::failure;
  }

  RhinoApp().Print(L"%s:\n", EnglishCommandName());

  RhinoApp().Print(L"  TransformTable.Count: %d\n", xform_table.Count());

  ON_SimpleArray<ON_UUID> uuids;
  const int uuid_count = xform_table.Ids(uuids);
  if (uuid_count > 0)
  {
    for (int i = 0; i < uuid_count; i++)
    {
      ON_SimpleArray<ON_Xform> xforms;
      const int xform_count = xform_table.Transforms(uuids[i], xforms);

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
// END SampleSharedXformClient2 command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
