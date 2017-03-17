/////////////////////////////////////////////////////////////////////////////
// RhinoRectangleObject.h
//

#pragma once

////////////////////////////////////////////////////////////////////////////////////////
// CRhinoRectangleObject
//
class CRhinoRectangleObject : public CRhinoCurveObject
{
  ON_OBJECT_DECLARE( CRhinoRectangleObject );

public:
  CRhinoRectangleObject();                                        // assigns a new object UUID
  CRhinoRectangleObject(const ON_3dmObjectAttributes&);           // assigns a new object UUID
  CRhinoRectangleObject( const CRhinoRectangleObject& );          // copies source object UUID
  CRhinoRectangleObject& operator=(const CRhinoRectangleObject&); // copies source object UUID
  ~CRhinoRectangleObject();

  //Description:
  //  virtual CRhinoObject::ShortDescription override
  //Returns:
  //  "rectangle objects" or "rectangle object"
  const wchar_t* ShortDescription( bool bPlural ) const;

  // virtual CRhinoObject::EnableGrips override
  void EnableGrips( bool bGripsOn );

  // Description:
  //   Specify the rectangle geometry.
  // Parameters:
  //   curve - [in] curve geometry copied to object
  // Returns:
  //   Pointer to curve geometry on CRhinoRectangleObject.
  ON_Curve* SetCurve( const ON_PolylineCurve& curve );

  // Description:
  //   Specify the rectangle geometry.
  // Parameters:
  //   pCurve - [in] ~CRhinoRectangleObject() will delete this curve
  void SetCurve( ON_PolylineCurve* pCurve );

  const ON_PolylineCurve* Curve() const;
};
