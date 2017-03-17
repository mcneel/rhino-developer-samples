#pragma once

#include "SampleMarkerUserData.h"

class CSampleMarkerObject : public CRhinoPointObject
{
  ON_OBJECT_DECLARE(CSampleMarkerObject);

public:
  CSampleMarkerObject();                                      // assigns a new object UUID
  CSampleMarkerObject(const ON_3dmObjectAttributes&);         // assigns a new object UUID
  ~CSampleMarkerObject();
  CSampleMarkerObject(const CSampleMarkerObject&);            // copies source object UUID
  CSampleMarkerObject(const CRhinoPointObject&);              // copies source object UUID
  CSampleMarkerObject& operator=(const CSampleMarkerObject&); // copies source object UUID
  CSampleMarkerObject& operator=(const CRhinoPointObject&);   // copies source object UUID
  CSampleMarkerObject& operator=(const ON_3dPoint&);          // leave attributes unchanged.

  int Pick(const CRhinoPickContext& pick_context, CRhinoObjRefArray& pick_list) const;

  // virtual CRhinoObject::Draw override
  void Draw(CRhinoDisplayPipeline&) const;

  // virtual CRhinoObject::Draw override
  void Draw(CRhinoViewport&) const;

  // virtual CRhinoObject::ShortDescription override
  const wchar_t* ShortDescription(bool bPlural) const;
};