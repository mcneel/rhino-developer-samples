#pragma once

class CSampleRectangleObject : public CRhinoCurveObject
{
  ON_OBJECT_DECLARE(CSampleRectangleObject);

public:
  CSampleRectangleObject();                                             // assigns a new object UUID
  CSampleRectangleObject(const ON_3dmObjectAttributes& att);            // assigns a new object UUID
  CSampleRectangleObject(const CSampleRectangleObject& src);            // copies source object UUID
  CSampleRectangleObject& operator=(const CSampleRectangleObject& src); // copies source object UUID
  ~CSampleRectangleObject() = default;

  // Description:
  //   Virtual CRhinoObject::ShortDescription override
  const wchar_t* ShortDescription(bool bPlural) const override;
 
  // Description:
  //   Virtual CRhinoObject::EnableGrips override
  void EnableGrips(bool bGripsOn) override;
};
