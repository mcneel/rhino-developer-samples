#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGetMultipleSpecial command
//

class CCurveObjectRef
{
public:
  CCurveObjectRef();
  CCurveObjectRef(const CRhinoObjRef& object_ref, int value);
  CCurveObjectRef(const CCurveObjectRef& src);
  CCurveObjectRef& operator=(const CCurveObjectRef& src);

  const ON_Curve* Curve() const;
  int Value() const;

  bool IsValid() const;

private:
  CRhinoObjRef m_object_ref;
  int m_value;
};

CCurveObjectRef::CCurveObjectRef()
  : m_value(0)
{
}

CCurveObjectRef::CCurveObjectRef(const CRhinoObjRef& object_ref, int value)
{
  m_object_ref = object_ref;
  m_value = value;
}

CCurveObjectRef::CCurveObjectRef(const CCurveObjectRef& src)
{
  m_object_ref = src.m_object_ref;
  m_value = src.m_value;
}

CCurveObjectRef& CCurveObjectRef::operator=(const CCurveObjectRef& src)
{
  if (this != &src)
  {
    m_object_ref = src.m_object_ref;
    m_value = src.m_value;
  }
  return *this;
}

const ON_Curve* CCurveObjectRef::Curve() const
{
  return m_object_ref.Curve();
}

int CCurveObjectRef::Value() const
{
  return m_value;
}

bool CCurveObjectRef::IsValid() const
{
  return (nullptr != m_object_ref.Curve() && m_value >= 1 && m_value <= 99);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleGetMultipleSpecial : public CRhinoCommand
{
public:
  CCommandSampleGetMultipleSpecial() = default;
  UUID CommandUUID() override
  {
    // {1923F5AA-11D9-4613-9EC9-7B079CBEB20B}
    static const GUID SampleGetMultipleSpecialCommand_UUID =
    { 0x1923F5AA, 0x11D9, 0x4613, { 0x9E, 0xC9, 0x7B, 0x07, 0x9C, 0xBE, 0xB2, 0x0B } };
    return SampleGetMultipleSpecialCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleGetMultipleSpecial"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleGetMultipleSpecial object
static class CCommandSampleGetMultipleSpecial theSampleGetMultipleSpecialCommand;

CRhinoCommand::result CCommandSampleGetMultipleSpecial::RunCommand(const CRhinoCommandContext& context)
{
  ON_ClassArray<CCurveObjectRef> curve_refs;

  int value = 2;

  for (;;)
  {
    CRhinoGetObject go;
    go.SetGeometryFilter(CRhinoGetObject::curve_object);
    go.AddCommandOptionInteger(RHCMDOPTNAME(L"Value"), &value, L"Curve value", 1, 99);
    go.EnablePreSelect(false);
    if (0 == curve_refs.Count())
    {
      go.SetCommandPrompt(L"Select first curve");
    }
    else
    {
      go.SetCommandPrompt(L"Select next curve. Press Enter when done");
      go.EnableDeselectAllBeforePostSelect(false);
      go.AcceptNothing();
    }

    CRhinoGet::result res = go.GetObjects(1, 1);
    if (res == CRhinoGet::option)
      continue;

    if (res == CRhinoGet::object)
    {
      CCurveObjectRef obj_ref(go.Object(0), value);
      curve_refs.Append(obj_ref);
      continue;
    }

    if (res != CRhinoGet::nothing)
      return CRhinoCommand::cancel;

    break;
  }

  for (int i = 0; i < curve_refs.Count(); i++)
  {
    const CCurveObjectRef& obj_ref = curve_refs[i];
    const ON_Curve* curve = obj_ref.Curve();
    if (nullptr != curve)
    {
      RhinoApp().Print(L"Curve %d value = %d\n", i, obj_ref.Value());
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleGetMultipleSpecial command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
