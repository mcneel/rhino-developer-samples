#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleDrawPoints command
//

class CSampleDrawPointsConduit : public CRhinoDisplayConduit
{
public:
  CSampleDrawPointsConduit();
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate) override;

public:
  ON_3dPointArray m_points;
};

CSampleDrawPointsConduit::CSampleDrawPointsConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_PREDRAWOBJECTS)
{
}

bool CSampleDrawPointsConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate)
{
  UNREFERENCED_PARAMETER(bTerminate);

  if (nChannel == CSupportChannels::SC_CALCBOUNDINGBOX)
  {
    m_pChannelAttrs->m_BoundingBox.Union(m_points.BoundingBox());
  }
  else if (nChannel == CSupportChannels::SC_PREDRAWOBJECTS)
  {
    for (int i = 0; i < m_points.Count(); i++)
    {
      dp.DrawPoint(m_points[i], 3, RPS_CONTROL_POINT, i & 2 ? RGB(255,0, 0): RGB(0, 0, 255));
    }
  }

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#pragma region SampleDrawPoints command

class CCommandSampleDrawPoints : public CRhinoCommand
{
public:
  CCommandSampleDrawPoints();
  UUID CommandUUID() override
  {
    // {34F03E07-2524-4562-91C3-C01487C036B9}
    static const GUID SampleDrawPointsCommand_UUID =
    { 0x34F03E07, 0x2524, 0x4562,{ 0x91, 0xC3, 0xC0, 0x14, 0x87, 0xC0, 0x36, 0xB9 } };
    return SampleDrawPointsCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleDrawPoints"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  ON_RandomNumberGenerator m_rand;
};

// The one and only CCommandSampleDrawPoints object
static class CCommandSampleDrawPoints theSampleDrawPointsCommand;

CCommandSampleDrawPoints::CCommandSampleDrawPoints()
{
  m_rand.Seed();
}

CRhinoCommand::result CCommandSampleDrawPoints::RunCommand(const CRhinoCommandContext& context)
{
  const int count = 100;
  const double min = -20.0;
  const double max = 20.0;

  CSampleDrawPointsConduit conduit;

  for (int i = 0; i < count; i++)
  {
    double x = m_rand.RandomDouble(min, max);
    double y = m_rand.RandomDouble(min, max);
    double z = m_rand.RandomDouble(min, max);
    conduit.m_points.Append(ON_3dPoint(x, y, z));
  }

  conduit.Enable(context.m_doc.RuntimeSerialNumber());
  context.m_doc.Regen();

  CRhinoGetString gs;
  gs.SetCommandPrompt(L"Press <Enter> to continue");
  gs.AcceptNothing();
  gs.GetString();

  conduit.Disable();
  context.m_doc.Regen();

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleDrawPoints command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
