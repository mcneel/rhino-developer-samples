/////////////////////////////////////////////////////////////////////////////
// RhinoRectangleObject.cpp
//

#include "stdafx.h"
#include "RhinoRectangleObject.h"

ON_OBJECT_IMPLEMENT( CRhinoRectangleObject, CRhinoCurveObject, "DD761B37-17AE-4E5A-B47C-60CFE1AF88F4")

CRhinoRectangleObject::CRhinoRectangleObject()
: CRhinoCurveObject()
{
}

CRhinoRectangleObject::CRhinoRectangleObject(const ON_3dmObjectAttributes& a)
: CRhinoCurveObject(a)
{
}

CRhinoRectangleObject::CRhinoRectangleObject( const CRhinoRectangleObject& src ) 
: CRhinoCurveObject(src)
{
}

CRhinoRectangleObject& CRhinoRectangleObject::operator=(const CRhinoRectangleObject& src)
{
  if( &src != this ) 
  {
    CRhinoCurveObject::operator=(src);
  }

  return *this;
}

CRhinoRectangleObject::~CRhinoRectangleObject()
{
}

const wchar_t* CRhinoRectangleObject::ShortDescription( bool bPlural ) const
{
  return bPlural ? L"rectangle objects" : L"rectangle object";
}

ON_Curve* CRhinoRectangleObject::SetCurve( const ON_PolylineCurve& curve )
{
  return CRhinoCurveObject::SetCurve( curve );
}

void CRhinoRectangleObject::SetCurve( ON_PolylineCurve* pCurve )
{
  CRhinoCurveObject::SetCurve( pCurve );
}

const ON_PolylineCurve* CRhinoRectangleObject::Curve() const
{
  return ON_PolylineCurve::Cast( CRhinoCurveObject::Curve() );
}


/////////////////////////////////////////////////////////////////////////////
// CRhinoRectangleGrip

class CRhinoRectangleGrip : public CRhinoGripObject
{
  ON_OBJECT_DECLARE( CRhinoRectangleGrip );

public:
  CRhinoRectangleGrip();
  ~CRhinoRectangleGrip();

  // virtual CRhinoObject::ShortDescription override
  const wchar_t* ShortDescription( bool bPlural ) const;

  bool m_bActive; // true if grip motion can change dimension.
};

ON_OBJECT_IMPLEMENT( CRhinoRectangleGrip, CRhinoGripObject, "19E44BCF-EF51-47F0-A4D7-E7CFC864BE08" );

CRhinoRectangleGrip::CRhinoRectangleGrip()
{
  m_bActive = true;
}

CRhinoRectangleGrip::~CRhinoRectangleGrip()
{
}

const wchar_t* CRhinoRectangleGrip::ShortDescription( bool bPlural ) const
{
  return bPlural ? L"rectangle points" : L"rectangle point";
}


/////////////////////////////////////////////////////////////////////////////
// CRhinoRectangleGrips

class CRhinoRectangleGrips : public CRhinoObjectGrips
{
public:
  static
  const ON_UUID m_rectangle_grips_id;

  static
  class CRhinoRectangleGrips* RectangleGrips( CRhinoObjectGrips* grips );

public:
  CRhinoRectangleGrips();
  ~CRhinoRectangleGrips();

  // virtual CRhinoObjectGrips::Reset override
  void Reset();

  // virtual CRhinoObjectGrips override
  CRhinoObject* NewObject();

  // virtual CRhinoObjectGrips override
  void Draw( CRhinoDrawGripsSettings& dgs );

  /////////////////////////////////////////////////
  //
  // Implementation
  //
  bool CreateGrips( const ON_PolylineCurve& rectangle );

  // Uses current grip locations to update m_nurbs_curve.
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
  CRhinoRectangleGrip m_rectangle_grips[8];

  // true if grips are being dynamically dragged and
  // m_rectangle needs to be drawn
  bool m_bDrawRectangle;

  // Rhino object the grips belong to
  CRhinoRectangleObject* RectangleObject() const;
};

// {21A75B67-485F-47B2-8E71-FC594BEBAF89}
const ON_UUID CRhinoRectangleGrips::m_rectangle_grips_id = 
{ 0x21A75B67, 0x485F, 0x47B2, { 0x8E, 0x71, 0xFC, 0x59, 0x4B, 0xEB, 0xAF, 0x89 } };

CRhinoRectangleGrips* CRhinoRectangleGrips::RectangleGrips( CRhinoObjectGrips* grips )
{
  if( !grips )
    return 0;
  if( CRhinoGripObject::custom_grip != grips->m__grips_type )
    return 0;
  if( ON_UuidCompare(CRhinoRectangleGrips::m_rectangle_grips_id, grips->m_grips_id) )
    return 0;
  return static_cast<CRhinoRectangleGrips*>(grips);
}

CRhinoRectangleGrips::CRhinoRectangleGrips()
: CRhinoObjectGrips( CRhinoGripObject::custom_grip )
{
  m_grips_id = CRhinoRectangleGrips::m_rectangle_grips_id;
  m_bDrawRectangle = false;
}

CRhinoRectangleGrips::~CRhinoRectangleGrips()
{
  // The grips pointed to in CRhinoObjectGrips::m_grip_list[]
  // are in CRhinoRectangleGrips::m_rectangle_grips[].
  // The destructors for member variables like m_rectangle_grips[]
  // are called before the base class destructor
  // ~CRhinoObjectGrips so m_grip_list[] needs to be empty.
  // (otherwise it has pointers to classes whose
  // destructors have already been called). 
  m_grip_list.SetCount(0);
}

void CRhinoRectangleGrips::Reset()
{
  m_bDrawRectangle = false;
  m_rectangle = m_rectangle0;
  CRhinoObjectGrips::Reset();
}

CRhinoObject* CRhinoRectangleGrips::NewObject()
{
  UpdateRectangle();

  CRhinoRectangleObject* pNewObject = 0;
  
  if (m_bGripsMoved && m_bDrawRectangle)
  {
    CRhinoRectangleObject* pOldObject = RectangleObject();
    if (pOldObject)
      pNewObject = new CRhinoRectangleObject(*pOldObject);
    else
      pNewObject = new CRhinoRectangleObject();

    ON_PolylineCurve rectangle_curve(m_rectangle);

    // Copy any user data from the original curve to the new curve
    if (pOldObject && pOldObject->Curve() && pOldObject->Curve()->FirstUserData())
      rectangle_curve.CopyUserData(*pOldObject->Curve());

    pNewObject->SetCurve(rectangle_curve);
  }

  return pNewObject;
}

void CRhinoRectangleGrips::Draw( CRhinoDrawGripsSettings& dgs )
{
  UpdateRectangle();
  if ( m_bDrawRectangle && dgs.m_bDrawDynamicStuff )
  {
    const int count = m_rectangle.Count();
    const ON_3dPoint* P = m_rectangle.Array();
    int i;
    for( i = 1; i < count; i++ )
      dgs.m_vp.DrawLine( P[i-1],P[i] );
  }
  CRhinoObjectGrips::Draw(dgs);
}

bool CRhinoRectangleGrips::CreateGrips( const ON_PolylineCurve& rectangle )
{
  if( rectangle.PointCount() < 4 )
    return false;

  m_bDrawRectangle = false;

  if( m_grip_list.Count() > 0 )
    return false;

  m_rectangle = rectangle.m_pline;
  m_rectangle0 = m_rectangle;

  ON_Line L;
  int i;
  for( i = 0; i < 4; i++ )
  {
    int gi = 2*i;
    L.from = m_rectangle[i];
    L.to = m_rectangle[i+1];
    m_rectangle_grips[gi].m_base_point = L.from;
    m_rectangle_grips[gi+1].m_base_point = 0.5*L.from + 0.5*L.to;    
    m_rectangle_grips[gi].m_bActive = true;
    m_rectangle_grips[gi+1].m_bActive = true;
  }

  m_grip_list.Reserve(8);
  for( i = 0; i < 8; i++ )
  {
    m_grip_list.Append( &m_rectangle_grips[i] );
  }
  return true;
}

void CRhinoRectangleGrips::UpdateRectangle()
{
  if ( m_bNewLocation )
  {
    // Update rectangle from grip locations
    int i;
    m_rectangle.Reserve(5);
    m_rectangle.SetCount(5);

    // If anything moved this time, the ones that didn't move will
    // be inactive for the rest of the drag
    for ( i = 0; i < 8; i++ )
    {
      if ( m_rectangle_grips[i].m_bActive && m_rectangle_grips[i].GripMoved() )
      {
        for ( i = 0; i < 8; i++ )
        {
          if ( !m_rectangle_grips[i].GripMoved() )
            m_rectangle_grips[i].m_bActive = false;
        }        
        break;
      }
    }

    // first check corners
    for ( i = 0; i < 8; i += 2 )
    {
      if ( m_rectangle_grips[i].m_bActive && m_rectangle_grips[i].GripMoved() )
      {
        // if corner moves, middles are inactive for this drag
        m_rectangle_grips[(i+1)%8].m_bActive = false;
        m_rectangle_grips[(i+7)%8].m_bActive = false;
      }
    }

    // second check middles
    for ( i = 1; i < 8; i += 2 )
    {
      if ( m_rectangle_grips[i].m_bActive && m_rectangle_grips[i].GripMoved() )
      {
        // if middle moves, corners are inactive for this drag
        m_rectangle_grips[(i+1)%8].m_bActive = false;
        m_rectangle_grips[(i+7)%8].m_bActive = false;
      }
    }

    // 
    double x0 = m_rectangle[0].x;
    if ( m_rectangle_grips[0].m_bActive && m_rectangle_grips[0].GripMoved() )
      x0 = m_rectangle_grips[0].GripLocation().x;
    else if ( m_rectangle_grips[6].m_bActive && m_rectangle_grips[6].GripMoved() )
      x0 = m_rectangle_grips[6].GripLocation().x;
    else if ( m_rectangle_grips[7].m_bActive && m_rectangle_grips[7].GripMoved() )
      x0 = m_rectangle_grips[7].GripLocation().x;

    double x1 = m_rectangle[2].x;
    if ( m_rectangle_grips[4].m_bActive && m_rectangle_grips[4].GripMoved() )
      x1 = m_rectangle_grips[4].GripLocation().x;
    else if ( m_rectangle_grips[2].m_bActive && m_rectangle_grips[2].GripMoved() )
      x1 = m_rectangle_grips[2].GripLocation().x;
    else if ( m_rectangle_grips[3].m_bActive && m_rectangle_grips[3].GripMoved() )
      x1 = m_rectangle_grips[3].GripLocation().x;

    double y0 = m_rectangle[0].y;
    if ( m_rectangle_grips[0].m_bActive && m_rectangle_grips[0].GripMoved() )
      y0 = m_rectangle_grips[0].GripLocation().y;
    else if ( m_rectangle_grips[2].m_bActive && m_rectangle_grips[2].GripMoved() )
      y0 = m_rectangle_grips[2].GripLocation().y;
    else if ( m_rectangle_grips[1].m_bActive && m_rectangle_grips[1].GripMoved() )
      y0 = m_rectangle_grips[1].GripLocation().y;

    double y1 = m_rectangle[2].y;
    if ( m_rectangle_grips[4].m_bActive && m_rectangle_grips[4].GripMoved() )
      y1 = m_rectangle_grips[4].GripLocation().y;
    else if ( m_rectangle_grips[6].m_bActive && m_rectangle_grips[6].GripMoved() )
      y1 = m_rectangle_grips[6].GripLocation().y;
    else if ( m_rectangle_grips[5].m_bActive && m_rectangle_grips[5].GripMoved() )
      y1 = m_rectangle_grips[5].GripLocation().y;

    m_rectangle[0].x = m_rectangle[3].x = x0;
    m_rectangle[1].x = m_rectangle[2].x = x1;
    m_rectangle[0].y = m_rectangle[1].y = y0;
    m_rectangle[2].y = m_rectangle[3].y = y1;

    m_rectangle[4] = m_rectangle[0];

    // apply rectangular constraints to grip locations
    ON_Line L;
    for ( i = 0; i < 4; i++ )
    {
      int gi = 2*i;
      L.from = m_rectangle[i];
      L.to = m_rectangle[i+1];
      m_rectangle_grips[gi].SetPoint( L.from );
      m_rectangle_grips[gi+1].SetPoint( 0.5*L.from + 0.5*L.to );
    }

    m_bDrawRectangle = true;
    m_bNewLocation = false;
  }
}

CRhinoRectangleObject* CRhinoRectangleGrips::RectangleObject() const
{
  return CRhinoRectangleObject::Cast( m_owner_object );
}


/////////////////////////////////////////////////////////////////////////////
// CRhinoRectangleGripsEnabler

class CRhinoRectangleGripsEnabler : public CRhinoGripsEnabler
{
public:
  CRhinoRectangleGripsEnabler();
  void TurnOnGrips( CRhinoObject* object ) const;
};

CRhinoRectangleGripsEnabler::CRhinoRectangleGripsEnabler()
{
  m_grips_id = CRhinoRectangleGrips::m_rectangle_grips_id;
}

void CRhinoRectangleGripsEnabler::TurnOnGrips( CRhinoObject* object ) const
{
  CRhinoRectangleObject* obj = CRhinoRectangleObject::Cast( object );
  if( obj )
  {
    obj->EnableGrips( true );
  }
}


/////////////////////////////////////////////////////////////////////////////
// CRhinoRectangleGripsRegistration

class CRhinoRectangleGripsRegistration
{
public:
  CRhinoRectangleGripsRegistration();
  ~CRhinoRectangleGripsRegistration();

  void RegisterGrips();

private:
  CRhinoRectangleGripsEnabler* m_pGripsEnabler;
};

CRhinoRectangleGripsRegistration::CRhinoRectangleGripsRegistration()
: m_pGripsEnabler(0)
{
}

CRhinoRectangleGripsRegistration::~CRhinoRectangleGripsRegistration()
{
  if( m_pGripsEnabler )
  {
    delete m_pGripsEnabler;
    m_pGripsEnabler = 0;
  }
}

void CRhinoRectangleGripsRegistration::RegisterGrips()
{
  if ( !m_pGripsEnabler )
  {
    // register once and only once
    m_pGripsEnabler = new CRhinoRectangleGripsEnabler();
    RhinoApp().RegisterGripsEnabler( m_pGripsEnabler );
  }
}

// The one and only CRhinoRectangleGripsRegistration object
static class CRhinoRectangleGripsRegistration theRectangleGripsRegistrar;


void CRhinoRectangleObject::EnableGrips( bool bGripsOn )
{
  if( bGripsOn )
    theRectangleGripsRegistrar.RegisterGrips();

  if( !bGripsOn || (m_grips && 0 == CRhinoRectangleGrips::RectangleGrips(m_grips)) )
  {
    // turn off wrong kind of grips
    CRhinoObject::EnableGrips( false );
  }

  CRhinoDoc* doc = RhinoApp().ActiveDoc();

  if( bGripsOn && !m_grips )
  {
    const ON_PolylineCurve* pline = Curve();
    if( pline )
    {
      // turn on rectangle grips
      CRhinoRectangleGrips* rectangle_grips = new CRhinoRectangleGrips();
      if( rectangle_grips->CreateGrips( *pline) )
        CRhinoObject::EnableCustomGrips( rectangle_grips );
      else
        delete rectangle_grips;
    }
  }
}