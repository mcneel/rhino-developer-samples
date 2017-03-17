#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleGuilloche command
//

class CSampleGuillocheArgs
{
public:
  CSampleGuillocheArgs();
  ~CSampleGuillocheArgs();

  void Defaults();
  void Destroy();

  // Divides a circle this many times
  int Steps() const;
  void SetSteps(int steps, bool bCalculate = false);

  // The major ripple
  double MajorRipple() const;
  void SetMajorRipple(double major_ripple, bool bCalculate = false);

  // The minor ripple
  double MinorRipple() const;
  void SetMinorRipple(double minor_ripple, bool bCalculate = false);

  // Radius type effect
  double Radius() const;
  void SetRadius(double radius, bool bCalculate = false);

  // Angle multiplier
  double Multiplier() const;
  void SetMultiplier(double multiplier, bool bCalculate = false);

  // Scale of output
  double Amplitude() const;
  void SetAmplitude(double amplitude, bool bCalculate = false);

  // Output curve type
  bool CreatePolyline() const;
  void SetCreatePolyline(bool bPline, bool bCalculate = false);

  // Calculates the Guilloché pattern points
  bool Calculate();

  // Returns the Guilloché pattern as a curve
  const ON_Curve* Curve() const;

  // Take ownership of the curve. 
  // Memory for the curve is allocated and becomes the responsibility of the caller.
  ON_Curve* DetachCurve();

  // Loads persistenct command settings from the Rhino profile
  void LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);

  // Save persistent command settings to the Rhino profile
  void SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);

public:
  static int MinSteps();
  static int MaxSteps();

  static double MinMajorRipple();
  static double MaxMajorRipple();

  static double MinMinorRipple();
  static double MaxMinorRipple();

  static double MinRadius();
  static double MaxRadius();

  static double MinMultiplier();
  static double MaxMultiplier();

  static double MinAmplitude();
  static double MaxAmplitude();

public:
  int m_steps;            // Divides a circle this many times
  double m_major_ripple;  // The major ripple
  double m_minor_ripple;  // The minor ripple
  double m_radius;        // Radius type effect
  double m_multiplier;    // Angle multiplier
  double m_amplitude;     // Scale of output
  bool m_bPline;          // Output curve type

private:
  ON_Curve* m_curve;
};

////////////////////////////////////////////////////////////////

CSampleGuillocheArgs::CSampleGuillocheArgs()
  : m_curve(0)
{
  Defaults();
}

CSampleGuillocheArgs::~CSampleGuillocheArgs()
{
  Destroy();
}

void CSampleGuillocheArgs::Defaults()
{
  m_steps = 391;
  m_major_ripple = 21;
  m_minor_ripple = 0.28;
  m_radius = 47;
  m_multiplier = 1.7;
  m_amplitude = 3.5;
  m_curve = 0;
  m_bPline = false;
}

void CSampleGuillocheArgs::Destroy()
{
  if (0 != m_curve)
  {
    delete m_curve;
    m_curve = 0;
  }
}

bool CSampleGuillocheArgs::Calculate()
{
  if (0 != m_curve)
  {
    delete m_curve;
    m_curve = 0;
  }

  ON_3dPointArray points;
  points.SetCapacity(m_steps + 1);
  points.SetCount(m_steps + 1);

  double theta = 0;
  double theta_step = 2 * ON_PI / m_steps;

  double s = (m_major_ripple + m_minor_ripple) / m_minor_ripple;
  double rR = m_minor_ripple + m_major_ripple;
  double rp = m_minor_ripple + m_radius;

  for (int t = 0; t < m_steps + 1; t++)
  {
    double x = rR * cos(m_multiplier * theta) + rp * cos(s * m_multiplier * theta);
    double y = rR * sin(m_multiplier * theta) + rp * sin(s * m_multiplier * theta);

    x *= m_amplitude;
    y *= m_amplitude;

    points[t] = ON_3dPoint(x, y, 0.0);

    theta += theta_step;
  }

  if (m_bPline)
    m_curve = new ON_PolylineCurve(points);
  else
    m_curve = RhinoInterpCurve(3, points, 0, 0, 0);

  return true;
}

const ON_Curve* CSampleGuillocheArgs::Curve() const
{
  return m_curve;
}

ON_Curve* CSampleGuillocheArgs::DetachCurve()
{
  if (0 != m_curve)
  {
    ON_Curve* curve = m_curve;
    m_curve = 0;
    return curve;
  }
  return 0;
}

int CSampleGuillocheArgs::Steps() const
{
  return m_steps;
}

void CSampleGuillocheArgs::SetSteps(int steps, bool bCalculate)
{
  m_steps = RHINO_CLAMP(steps, CSampleGuillocheArgs::MinSteps(), CSampleGuillocheArgs::MaxSteps());
  if (bCalculate)
    Calculate();
}

int CSampleGuillocheArgs::MinSteps()
{
  return 10;
}

int CSampleGuillocheArgs::MaxSteps()
{
  return 3000;
}

double CSampleGuillocheArgs::MajorRipple() const
{
  return m_major_ripple;
}

void CSampleGuillocheArgs::SetMajorRipple(double major_ripple, bool bCalculate)
{
  m_major_ripple = RHINO_CLAMP(major_ripple, CSampleGuillocheArgs::MinMajorRipple(), CSampleGuillocheArgs::MaxMajorRipple());
  if (bCalculate)
    Calculate();
}

double CSampleGuillocheArgs::MinMajorRipple()
{
  return 0.0;
}

double CSampleGuillocheArgs::MaxMajorRipple()
{
  return 100.0;
}

double CSampleGuillocheArgs::MinorRipple() const
{
  return m_minor_ripple;
}

void CSampleGuillocheArgs::SetMinorRipple(double minor_ripple, bool bCalculate)
{
  m_minor_ripple = RHINO_CLAMP(minor_ripple, CSampleGuillocheArgs::MinMinorRipple(), CSampleGuillocheArgs::MaxMinorRipple());
  if (bCalculate)
    Calculate();
}

double CSampleGuillocheArgs::MinMinorRipple()
{
  return 0.0;
}

double CSampleGuillocheArgs::MaxMinorRipple()
{
  return 0.5;
}

double CSampleGuillocheArgs::Radius() const
{
  return m_radius;
}

void CSampleGuillocheArgs::SetRadius(double radius, bool bCalculate)
{
  m_radius = RHINO_CLAMP(radius, CSampleGuillocheArgs::MinRadius(), CSampleGuillocheArgs::MaxRadius());
  if (bCalculate)
    Calculate();
}

double CSampleGuillocheArgs::MinRadius()
{
  return -100.0;
}

double CSampleGuillocheArgs::MaxRadius()
{
  return 200.0;
}

double CSampleGuillocheArgs::Multiplier() const
{
  return m_multiplier;
}

void CSampleGuillocheArgs::SetMultiplier(double multiplier, bool bCalculate)
{
  m_multiplier = RHINO_CLAMP(multiplier, CSampleGuillocheArgs::MinMultiplier(), CSampleGuillocheArgs::MaxMultiplier());
  if (bCalculate)
    Calculate();
}

double CSampleGuillocheArgs::MinMultiplier()
{
  return -0.1;
}

double CSampleGuillocheArgs::MaxMultiplier()
{
  return 100.0;
}

double CSampleGuillocheArgs::Amplitude() const
{
  return m_amplitude;
}

void CSampleGuillocheArgs::SetAmplitude(double amplitude, bool bCalculate)
{
  m_amplitude = RHINO_CLAMP(amplitude, CSampleGuillocheArgs::MinAmplitude(), CSampleGuillocheArgs::MaxAmplitude());
  if (bCalculate)
    Calculate();
}

double CSampleGuillocheArgs::MinAmplitude()
{
  return 1.0;
}

double CSampleGuillocheArgs::MaxAmplitude()
{
  return 20.0;
}

bool CSampleGuillocheArgs::CreatePolyline() const
{
  return m_bPline;
}

void CSampleGuillocheArgs::SetCreatePolyline(bool bPline, bool bCalculate)
{
  m_bPline = bPline;
  if (bCalculate)
    Calculate();
}

void CSampleGuillocheArgs::LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  int n = 0;
  double d = 0.0;

  if (pc.LoadProfileInt(lpszSection, L"steps", &n))
    SetSteps(n);

  if (pc.LoadProfileDouble(lpszSection, L"major_ripple", &d))
    SetMajorRipple(d);

  if (pc.LoadProfileDouble(lpszSection, L"minor_ripple", &d))
    SetMinorRipple(d);

  if (pc.LoadProfileDouble(lpszSection, L"radius", &d))
    SetRadius(d);

  if (pc.LoadProfileDouble(lpszSection, L"multiplier", &d))
    SetMultiplier(d);

  if (pc.LoadProfileDouble(lpszSection, L"amplitude", &d))
    SetAmplitude(d);
}

void CSampleGuillocheArgs::SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  pc.SaveProfileInt(lpszSection, L"steps", m_steps);
  pc.SaveProfileDouble(lpszSection, L"major_ripple", m_major_ripple);
  pc.SaveProfileDouble(lpszSection, L"minor_ripple", m_minor_ripple);
  pc.SaveProfileDouble(lpszSection, L"radius", m_radius);
  pc.SaveProfileDouble(lpszSection, L"multiplier", m_multiplier);
  pc.SaveProfileDouble(lpszSection, L"amplitude", m_amplitude);
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CSampleGuillocheConduit : public CRhinoDisplayConduit
{
public:
  CSampleGuillocheConduit();
  bool ExecConduit(CRhinoDisplayPipeline& dp, UINT nChannel, bool& bTerminate);

public:
  CSampleGuillocheArgs m_args;
};

CSampleGuillocheConduit::CSampleGuillocheConduit()
  : CRhinoDisplayConduit(CSupportChannels::SC_CALCBOUNDINGBOX | CSupportChannels::SC_DRAWOVERLAY)
{
}

////////////////////////////////////////////////////////////////

bool CSampleGuillocheConduit::ExecConduit(CRhinoDisplayPipeline& dp, UINT nActiveChannel, bool& bTerminateChannel)
{
  UNREFERENCED_PARAMETER(bTerminateChannel);

  if (nActiveChannel == CSupportChannels::SC_CALCBOUNDINGBOX)
  {
    const ON_Curve* crv = m_args.Curve();
    if (crv)
      m_pChannelAttrs->m_BoundingBox.Union(crv->BoundingBox());
  }
  else if (nActiveChannel == CSupportChannels::SC_DRAWOVERLAY)
  {
    const ON_Curve* crv = m_args.Curve();
    if (crv)
      dp.DrawCurve(*crv, RhinoApp().AppSettings().ActiveLayerColor());
  }

  return true;
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////


class CCommandSampleGuilloche : public CRhinoCommand
{
public:
  CCommandSampleGuilloche() {}
  ~CCommandSampleGuilloche() {}
  UUID CommandUUID()
  {
    // {C2B28A4E-D89C-4B57-AFF4-71F15FB0DABA}
    static const GUID SampleGuillocheCommand_UUID =
    { 0xC2B28A4E, 0xD89C, 0x4B57,{ 0xAF, 0xF4, 0x71, 0xF1, 0x5F, 0xB0, 0xDA, 0xBA } };
    return SampleGuillocheCommand_UUID;
  }
  const wchar_t* EnglishCommandName() { return L"SampleGuilloche"; }
  const wchar_t* LocalCommandName() const { return L"SampleGuilloche"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context);

  void LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);
  void SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc);

private:
  CSampleGuillocheArgs m_args;
};

// The one and only CCommandSampleGuilloche object
static class CCommandSampleGuilloche theSampleGuillocheCommand;

void CCommandSampleGuilloche::LoadProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  m_args.LoadProfile(lpszSection, pc);
}

void CCommandSampleGuilloche::SaveProfile(LPCTSTR lpszSection, CRhinoProfileContext& pc)
{
  m_args.SaveProfile(lpszSection, pc);
}

CRhinoCommand::result CCommandSampleGuilloche::RunCommand(const CRhinoCommandContext& context)
{
  CSampleGuillocheConduit conduit;
  conduit.m_args = m_args;
  conduit.m_args.Calculate();
  conduit.Enable();
  context.m_doc.Redraw();

  CSampleGuillocheArgs& args = conduit.m_args;

  CRhinoGetOption go;
  go.SetCommandPrompt(L"Guilloché pattern options");
  go.AddCommandOptionNumber(RHCMDOPTNAME(L"Amplitude"), &args.m_amplitude, L"Amplitude", false, CSampleGuillocheArgs::MinAmplitude(), CSampleGuillocheArgs::MaxAmplitude());
  go.AddCommandOptionNumber(RHCMDOPTNAME(L"MajorRipple"), &args.m_major_ripple, L"MajorRipple", false, CSampleGuillocheArgs::MinMajorRipple(), CSampleGuillocheArgs::MaxMajorRipple());
  go.AddCommandOptionNumber(RHCMDOPTNAME(L"MinorRipple"), &args.m_minor_ripple, L"MinorRipple", false, CSampleGuillocheArgs::MinMinorRipple(), CSampleGuillocheArgs::MaxMinorRipple());
  go.AddCommandOptionNumber(RHCMDOPTNAME(L"Multiplier"), &args.m_multiplier, L"Multiplier", false, CSampleGuillocheArgs::MinMultiplier(), CSampleGuillocheArgs::MaxMultiplier());
  go.AddCommandOptionNumber(RHCMDOPTNAME(L"Radius"), &args.m_radius, L"Radius", false, CSampleGuillocheArgs::MinRadius(), CSampleGuillocheArgs::MaxRadius());
  go.AddCommandOptionInteger(RHCMDOPTNAME(L"Steps"), &args.m_steps, L"Steps", CSampleGuillocheArgs::MinSteps(), CSampleGuillocheArgs::MaxSteps());
  go.AddCommandOptionToggle(RHCMDOPTNAME(L"Output"), RHCMDOPTVALUE(L"Curve"), RHCMDOPTVALUE(L"Polyline"), args.m_bPline, &args.m_bPline);
  go.AcceptNothing();
  for (;;)
  {
    CRhinoGet::result res = go.GetOption();
    if (res == CRhinoGet::option)
    {
      conduit.m_args.Calculate();
      context.m_doc.Redraw();
      continue;
    }

    break;
  }

  conduit.Disable();
  context.m_doc.Redraw();

  CRhinoCommand::result rc = go.CommandResult();
  if (rc == CRhinoCommand::success)
  {
    CRhinoCurveObject* crv_obj = new CRhinoCurveObject();
    crv_obj->SetCurve(conduit.m_args.DetachCurve());
    context.m_doc.AddObject(crv_obj);
    context.m_doc.Redraw();

    m_args = conduit.m_args;
  }

  return CRhinoCommand::success;
}

//
// END SampleGuilloche command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
