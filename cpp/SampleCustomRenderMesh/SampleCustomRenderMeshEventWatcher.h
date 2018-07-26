#pragma once

class CSampleCustomRenderMeshEventWatcher : public CRhinoEventWatcher
{
public:
  void OnReplaceObject(CRhinoDoc& doc, CRhinoObject& old_object, CRhinoObject& new_object) override;
};