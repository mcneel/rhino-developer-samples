#pragma once

class CSampleRectangleObject : public CRhinoCurveObject
{
  ON_OBJECT_DECLARE(CSampleRectangleObject);

public:
  CSampleRectangleObject();                                         // assigns a new object UUID
  CSampleRectangleObject(const ON_3dmObjectAttributes&);            // assigns a new object UUID
  CSampleRectangleObject(const CSampleRectangleObject&);            // copies source object UUID
  CSampleRectangleObject& operator=(const CSampleRectangleObject&); // copies source object UUID
  ~CSampleRectangleObject();

  //Description:
  //  virtual CRhinoObject::ShortDescription override
  //Returns:
  //  "rectangle objects" or "rectangle object"
  const wchar_t* ShortDescription(bool bPlural) const;

  // virtual CRhinoObject::EnableGrips override
  void EnableGrips(bool bGripsOn);

  // Description:
  //   Specify the rectangle geometry.
  // Parameters:
  //   curve - [in] curve geometry copied to object
  // Returns:
  //   Pointer to curve geometry on CSampleRectangleObject.
  ON_Curve* SetCurve(const ON_PolylineCurve& curve);

  // Description:
  //   Specify the rectangle geometry.
  // Parameters:
  //   pCurve - [in] ~CSampleRectangleObject() will delete this curve
  void SetCurve(ON_PolylineCurve* pCurve);

  const ON_PolylineCurve* Curve() const;
};
