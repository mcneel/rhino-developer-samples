#pragma once

#include "SampleRhinoDockbarDialog.h"

class CSampleRhinoDockbar : public CRhinoUiDockBar
{
  DECLARE_SERIAL(CSampleRhinoDockbar)

public:
  CSampleRhinoDockbar();
  virtual ~CSampleRhinoDockbar();

  // Required overrides
  UUID DockBarID() const override;
  const wchar_t* DockBarName(int language_id = 1033) const override;

protected:
  bool CreateDockBarControls();

protected:
  DECLARE_MESSAGE_MAP()
};


