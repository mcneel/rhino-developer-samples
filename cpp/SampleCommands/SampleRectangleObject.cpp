#include "stdafx.h"
#include "SampleRectangleObject.h"

/////////////////////////////////////////////////////////////////////////////
// CSampleRectangleObject

ON_OBJECT_IMPLEMENT(CSampleRectangleObject, CRhinoCurveObject, "975BC780-2D30-4543-85A0-9D14E06F7F6C")

CSampleRectangleObject::CSampleRectangleObject()
  : CRhinoCurveObject()
{
}

CSampleRectangleObject::CSampleRectangleObject(const ON_3dmObjectAttributes& att)
  : CRhinoCurveObject(att)
{
}

CSampleRectangleObject::CSampleRectangleObject(const CSampleRectangleObject& src)
  : CRhinoCurveObject(src)
{
}

CSampleRectangleObject& CSampleRectangleObject::operator=(const CSampleRectangleObject& src)
{
  if (&src != this)
    CRhinoCurveObject::operator=(src);
  return *this;
}

const wchar_t* CSampleRectangleObject::ShortDescription(bool bPlural) const
{
  return bPlural ? L"rectangle objects" : L"rectangle object";
}


/////////////////////////////////////////////////////////////////////////////
// CSampleRectangleGrip

class CSampleRectangleGrip : public CRhinoGripObject
{
  ON_OBJECT_DECLARE(CSampleRectangleGrip);

public:
  CSampleRectangleGrip();
  ~CSampleRectangleGrip() = default;

  // virtual CRhinoObject::ShortDescription override
  const wchar_t* ShortDescription(bool bPlural) const override;

public:
  bool m_bActive; // true if grip motion can change dimension.
};

ON_OBJECT_IMPLEMENT(CSampleRectangleGrip, CRhinoGripObject, "49C9CE2B-364D-4733-84D0-25EF4F285D96");

CSampleRectangleGrip::CSampleRectangleGrip()
  : m_bActive(true)
{
}

const wchar_t* CSampleRectangleGrip::ShortDescription(bool bPlural) const
{
  return bPlural ? L"rectangle points" : L"rectangle point";
}


/////////////////////////////////////////////////////////////////////////////
// CSampleRectangleGrips

class CSampleRectangleGrips : public CRhinoObjectGrips
{
public:
  static ON_UUID Id();
  static class CSampleRectangleGrips* RectangleGrips(CRhinoObjectGrips* grips);

public:
  CSampleRectangleGrips();
  ~CSampleRectangleGrips();

  // virtual CRhinoObjectGrips::Reset override
  void Reset() override;

  // virtual CRhinoObjectGrips override
  CRhinoObject* NewObject() override;

  // virtual CRhinoObjectGrips override
  void Draw(CRhinoDrawGripsSettings& dgs) override;

  /////////////////////////////////////////////////
  //
  // Implementation
  //
  bool CreateGrips(const ON_PolylineCurve& rectangle);

  // Uses current grip locations to update m_rectangle.
  void UpdateRectangle();

  // temporary rectangle being edited.
  ON_Polyline m_rectangle;

  // original rectangle 
  ON_Polyline m_rectangle0;

  // Grip layout
  //
  // 6-----5------4
  // |            |
  // 7            3
  // |            |
  // 0-----1------2
  //
  CSampleRectangleGrip m_rectangle_grips[8];

  // true if grips are being dynamically dragged and
  // m_rectangle needs to be drawn
  bool m_bDrawRectangle;

  // Rhino object the grips belong to
  CSampleRectangleObject* RectangleObject() const;
};

ON_UUID CSampleRectangleGrips::Id()
{
  // {5FD31CCF-4E57-4911-90E8-DBF04494ED05}
  static const ON_UUID CSampleRectangleGrips_UUID =
  { 0x5fd31ccf, 0x4e57, 0x4911,{ 0x90, 0xe8, 0xdb, 0xf0, 0x44, 0x94, 0xed, 0x5 } };
  return CSampleRectangleGrips_UUID;
}

CSampleRectangleGrips* CSampleRectangleGrips::RectangleGrips(CRhinoObjectGrips* grips)
{
  if (nullptr == grips)
    return nullptr;
  if (CRhinoGripObject::custom_grip != grips->m__grips_type)
    return nullptr;
  if (ON_UuidCompare(CSampleRectangleGrips::Id(), grips->m_grips_id))
    return nullptr;
  return static_cast<CSampleRectangleGrips*>(grips);
}

CSampleRectangleGrips::CSampleRectangleGrips()
  : CRhinoObjectGrips(CRhinoGripObject::custom_grip)
{
  m_grips_id = CSampleRectangleGrips::Id();
  m_bDrawRectangle = false;
}

CSampleRectangleGrips::~CSampleRectangleGrips()
{
  // The grips pointed to in CRhinoObjectGrips::m_grip_list[]
  // are in CSampleRectangleGrips::m_rectangle_grips[].
  // The destructors for member variables like m_rectangle_grips[]
  // are called before the base class destructor
  // ~CRhinoObjectGrips so m_grip_list[] needs to be empty.
  // (otherwise it has pointers to classes whose
  // destructors have already been called). 
  m_grip_list.SetCount(0);
}

void CSampleRectangleGrips::Reset()
{
  m_bDrawRectangle = false;
  m_rectangle = m_rectangle0;
  CRhinoObjectGrips::Reset();
}

CRhinoObject* CSampleRectangleGrips::NewObject()
{
  UpdateRectangle();

  CSampleRectangleObject* pNewObject = nullptr;

  if (m_bGripsMoved && m_bDrawRectangle)
  {
    CSampleRectangleObject* pOldObject = RectangleObject();
    if (pOldObject)
      pNewObject = new CSampleRectangleObject(*pOldObject);
    else
      pNewObject = new CSampleRectangleObject();

    ON_PolylineCurve rectangle_curve(m_rectangle);

    // Copy any user data from the original curve to the new curve
    if (pOldObject && pOldObject->Curve() && pOldObject->Curve()->FirstUserData())
      rectangle_curve.CopyUserData(*pOldObject->Curve());

    pNewObject->SetCurve(rectangle_curve);
  }

  return pNewObject;
}

void CSampleRectangleGrips::Draw(CRhinoDrawGripsSettings& dgs)
{
  UpdateRectangle();
  if (m_bDrawRectangle && dgs.m_bDrawDynamicStuff)
  {
    const int count = m_rectangle.Count();
    const ON_3dPoint* P = m_rectangle.Array();
    for (int i = 1; i < count; i++)
      dgs.m_dp.DrawLine(P[i - 1], P[i]);
  }
  CRhinoObjectGrips::Draw(dgs);
}

bool CSampleRectangleGrips::CreateGrips(const ON_PolylineCurve& rectangle)
{
  if (rectangle.PointCount() < 4)
    return false;

  m_bDrawRectangle = false;

  if (m_grip_list.Count() > 0)
    return false;

  m_rectangle = rectangle.m_pline;
  m_rectangle0 = m_rectangle;

  ON_Line line;
  for (int i = 0; i < 4; i++)
  {
    int gi = 2 * i;
    line.from = m_rectangle[i];
    line.to = m_rectangle[i + 1];
    m_rectangle_grips[gi].m_base_point = line.from;
    m_rectangle_grips[gi + 1].m_base_point = 0.5 * line.from + 0.5 * line.to;
    m_rectangle_grips[gi].m_bActive = true;
    m_rectangle_grips[gi + 1].m_bActive = true;
  }

  m_grip_list.Reserve(8);
  for (int i = 0; i < 8; i++)
    m_grip_list.Append(&m_rectangle_grips[i]);

  return true;
}

void CSampleRectangleGrips::UpdateRectangle()
{
  if (m_bNewLocation)
  {
    // Update rectangle from grip locations
    m_rectangle.Reserve(5);
    m_rectangle.SetCount(5);

    // If anything moved this time, the ones that didn't move will
    // be inactive for the rest of the drag
    for (int i = 0; i < 8; i++)
    {
      if (m_rectangle_grips[i].m_bActive && m_rectangle_grips[i].GripMoved())
      {
        for (i = 0; i < 8; i++)
        {
          if (!m_rectangle_grips[i].GripMoved())
            m_rectangle_grips[i].m_bActive = false;
        }
        break;
      }
    }

    // first check corners
    for (int i = 0; i < 8; i += 2)
    {
      if (m_rectangle_grips[i].m_bActive && m_rectangle_grips[i].GripMoved())
      {
        // if corner moves, middles are inactive for this drag
        m_rectangle_grips[(i + 1) % 8].m_bActive = false;
        m_rectangle_grips[(i + 7) % 8].m_bActive = false;
      }
    }

    // second check middles
    for (int i = 1; i < 8; i += 2)
    {
      if (m_rectangle_grips[i].m_bActive && m_rectangle_grips[i].GripMoved())
      {
        // if middle moves, corners are inactive for this drag
        m_rectangle_grips[(i + 1) % 8].m_bActive = false;
        m_rectangle_grips[(i + 7) % 8].m_bActive = false;
      }
    }

    double x0 = m_rectangle[0].x;
    if (m_rectangle_grips[0].m_bActive && m_rectangle_grips[0].GripMoved())
      x0 = m_rectangle_grips[0].GripLocation().x;
    else if (m_rectangle_grips[6].m_bActive && m_rectangle_grips[6].GripMoved())
      x0 = m_rectangle_grips[6].GripLocation().x;
    else if (m_rectangle_grips[7].m_bActive && m_rectangle_grips[7].GripMoved())
      x0 = m_rectangle_grips[7].GripLocation().x;

    double x1 = m_rectangle[2].x;
    if (m_rectangle_grips[4].m_bActive && m_rectangle_grips[4].GripMoved())
      x1 = m_rectangle_grips[4].GripLocation().x;
    else if (m_rectangle_grips[2].m_bActive && m_rectangle_grips[2].GripMoved())
      x1 = m_rectangle_grips[2].GripLocation().x;
    else if (m_rectangle_grips[3].m_bActive && m_rectangle_grips[3].GripMoved())
      x1 = m_rectangle_grips[3].GripLocation().x;

    double y0 = m_rectangle[0].y;
    if (m_rectangle_grips[0].m_bActive && m_rectangle_grips[0].GripMoved())
      y0 = m_rectangle_grips[0].GripLocation().y;
    else if (m_rectangle_grips[2].m_bActive && m_rectangle_grips[2].GripMoved())
      y0 = m_rectangle_grips[2].GripLocation().y;
    else if (m_rectangle_grips[1].m_bActive && m_rectangle_grips[1].GripMoved())
      y0 = m_rectangle_grips[1].GripLocation().y;

    double y1 = m_rectangle[2].y;
    if (m_rectangle_grips[4].m_bActive && m_rectangle_grips[4].GripMoved())
      y1 = m_rectangle_grips[4].GripLocation().y;
    else if (m_rectangle_grips[6].m_bActive && m_rectangle_grips[6].GripMoved())
      y1 = m_rectangle_grips[6].GripLocation().y;
    else if (m_rectangle_grips[5].m_bActive && m_rectangle_grips[5].GripMoved())
      y1 = m_rectangle_grips[5].GripLocation().y;

    m_rectangle[0].x = m_rectangle[3].x = x0;
    m_rectangle[1].x = m_rectangle[2].x = x1;
    m_rectangle[0].y = m_rectangle[1].y = y0;
    m_rectangle[2].y = m_rectangle[3].y = y1;

    m_rectangle[4] = m_rectangle[0];

    // apply rectangular constraints to grip locations
    ON_Line line;
    for (int i = 0; i < 4; i++)
    {
      int gi = 2 * i;
      line.from = m_rectangle[i];
      line.to = m_rectangle[i + 1];
      m_rectangle_grips[gi].SetPoint(line.from);
      m_rectangle_grips[gi + 1].SetPoint(0.5*line.from + 0.5*line.to);
    }

    m_bDrawRectangle = true;
    m_bNewLocation = false;
  }
}

CSampleRectangleObject* CSampleRectangleGrips::RectangleObject() const
{
  return CSampleRectangleObject::Cast(m_owner_object);
}


/////////////////////////////////////////////////////////////////////////////
// CSampleRectangleGripsEnabler

class CSampleRectangleGripsEnabler : public CRhinoGripsEnabler
{
public:
  CSampleRectangleGripsEnabler();
  void TurnOnGrips(CRhinoObject* object) const;
};

CSampleRectangleGripsEnabler::CSampleRectangleGripsEnabler()
{
  m_grips_id = CSampleRectangleGrips::Id();
}

void CSampleRectangleGripsEnabler::TurnOnGrips(CRhinoObject* object) const
{
  CSampleRectangleObject* rectangle_object = CSampleRectangleObject::Cast(object);
  if (nullptr != rectangle_object)
    rectangle_object->EnableGrips(true);
}


/////////////////////////////////////////////////////////////////////////////
// CSampleRectangleGripsRegistration

class CSampleRectangleGripsRegistration
{
public:
  CSampleRectangleGripsRegistration();
  ~CSampleRectangleGripsRegistration();

  void RegisterGrips();

private:
  CSampleRectangleGripsEnabler * m_pGripsEnabler;
};

CSampleRectangleGripsRegistration::CSampleRectangleGripsRegistration()
  : m_pGripsEnabler(nullptr)
{
}

CSampleRectangleGripsRegistration::~CSampleRectangleGripsRegistration()
{
  if (nullptr != m_pGripsEnabler)
  {
    delete m_pGripsEnabler;
    m_pGripsEnabler = nullptr;
  }
}

void CSampleRectangleGripsRegistration::RegisterGrips()
{
  if (nullptr == m_pGripsEnabler)
  {
    // Register once (and only once)
    m_pGripsEnabler = new CSampleRectangleGripsEnabler();
    RhinoApp().RegisterGripsEnabler(m_pGripsEnabler);
  }
}

// The one and only CSampleRectangleGripsRegistration object
static class CSampleRectangleGripsRegistration theRectangleGripsRegistrar;

void CSampleRectangleObject::EnableGrips(bool bGripsOn)
{
  if (bGripsOn)
    theRectangleGripsRegistrar.RegisterGrips();

  if (!bGripsOn || (m_grips && CRhinoGripObject::custom_grip != m_grips->m__grips_type))
  {
    // Turn off wrong kind of grips
    CRhinoObject::EnableGrips(false);
  }

  if (bGripsOn && !m_grips)
  {
    const ON_PolylineCurve* rectangle_curve = ON_PolylineCurve::Cast(Curve());
    if (nullptr != rectangle_curve)
    {
      // Turn on rectangle grips
      CSampleRectangleGrips* rectangle_grips = new CSampleRectangleGrips();
      if (nullptr != rectangle_grips)
      {
        if (rectangle_grips->CreateGrips(*rectangle_curve))
          CRhinoObject::EnableCustomGrips(rectangle_grips);
        else
          delete rectangle_grips;
      }
    }
  }
}