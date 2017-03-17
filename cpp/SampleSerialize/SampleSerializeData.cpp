/////////////////////////////////////////////////////////////////////////////
// SampleSerializeData.cpp
//

#include "StdAfx.h"
#include "SampleSerializeData.h"

CSampleSerializeData::CSampleSerializeData()
  : m_integer(0)
  , m_double(0.0)
  , m_mesh(0)
  , m_curve(0)
  , m_brep(0)
{
}

CSampleSerializeData::~CSampleSerializeData()
{
  Destroy();
}

void CSampleSerializeData::Destroy()
{
  m_integer = 0;
  m_double = 0.0;
  m_string.Destroy();
  m_point_list.Destroy();

  if (0 != m_mesh)
  {
    delete m_mesh;
    m_mesh = 0;
  }

  if (0 != m_brep)
  {
    delete m_brep;
    m_brep = 0;
  }

  if (0 != m_curve)
  {
    delete m_curve;
    m_curve = 0;
  }

  for (int i = 0; i < m_curve_list.Count(); i++)
  {
    if (0 != m_curve_list[i])
    {
      delete m_curve_list[i];
      m_curve_list[i] = 0;
    }
  }
  m_curve_list.Destroy();
}

int CSampleSerializeData::Integer() const
{
  return m_integer;
}

void CSampleSerializeData::SetInteger(int value)
{
  m_integer = value;
}

double CSampleSerializeData::Double() const
{
  return m_double;
}

void CSampleSerializeData::SetDouble(double value)
{
  m_double = value;
}

const wchar_t* CSampleSerializeData::String() const
{
  return m_string;
}

void CSampleSerializeData::SetString(const wchar_t* value)
{
  if (0 != value && 0 != value[0])
    m_string = value;
  else
    m_string.Destroy();
}

int CSampleSerializeData::Points(ON_SimpleArray<ON_3dPoint>& points)
{
  const int points_Count = points.Count();
  points.Append(m_point_list.Count(), m_point_list.Array());
  return points.Count() - points_Count;
}

void CSampleSerializeData::SetPoints(const ON_SimpleArray<ON_3dPoint>& points)
{
  m_point_list = points;
}

const ON_Mesh* CSampleSerializeData::Mesh() const
{
  return m_mesh;
}

void CSampleSerializeData::SetMesh(ON_Mesh* mesh)
{
  if (0 != m_mesh && m_mesh != mesh)
  {
    delete m_mesh;
    m_mesh = 0;
  }
  if (0 != mesh ) 
  {
    m_mesh = mesh;
  }
}

const ON_Brep* CSampleSerializeData::Brep() const
{
  return m_brep;
}

void CSampleSerializeData::SetBrep(ON_Brep* brep)
{
  if (0 != m_brep && m_brep != brep)
  {
    delete m_brep;
    m_brep = 0;
  }
  if (0 != brep ) 
  {
    m_brep = brep;
  }
}

const ON_Curve* CSampleSerializeData::Curve() const
{
  return m_curve;
}

void CSampleSerializeData::SetCurve(ON_Curve* curve)
{
  if (0 != m_curve && m_curve != curve)
  {
    delete m_curve;
    m_curve = 0;
  }
  if (0 != curve ) 
  {
    m_curve = curve;
  }
}

int CSampleSerializeData::Curves(ON_SimpleArray<const ON_Curve*>& curves)
{
  const int curves_Count = curves.Count();
  curves.Append(m_curve_list.Count(), m_curve_list.Array());
  return curves.Count() - curves_Count;
}

void CSampleSerializeData::SetCurves(const ON_SimpleArray<ON_Curve*>& curves)
{
  for (int i = 0; i < m_curve_list.Count(); i++)
  {
    if (0 != m_curve_list[i])
    {
      delete m_curve_list[i];
      m_curve_list[i] = 0;
    }
  }

  const int curves_Count = curves.Count();
  m_curve_list.SetCapacity(curves_Count);

  for (int i = 0; i < curves_Count; i++)
  {
    if (0 != curves[i])
      m_curve_list.Append(curves[i]);
  }
}

bool CSampleSerializeData::Write(ON_BinaryArchive& archive)
{
  // Begin write
  bool rc = archive.BeginWrite3dmChunk(TCODE_ANONYMOUS_CHUNK, 0);
  if (rc)
  {
    // Write version chunk
    rc = archive.Write3dmChunkVersion(1, 0);

    // Write integer
    if (rc) rc = archive.WriteInt(m_integer);

    // Write double
    if (rc) rc = archive.WriteDouble(m_double);

    // Write string
    if (rc) rc = archive.WriteString(m_string);

    // Write point list
    if (rc) rc = archive.WriteArray(m_point_list);

    // Write mesh
    if (rc)
    {
      if (0 != m_mesh)
      {
        rc = archive.WriteInt(1);
        if (rc)
          rc = archive.WriteObject(*m_mesh);
      }
      else
        rc = archive.WriteInt(0); // NULL mesh
    }

    // Write brep
    if (rc)
    {
      if (0 != m_brep)
      {
        rc = archive.WriteInt(1);
        if (rc)
          rc = archive.WriteObject(*m_brep);
      }
      else
        rc = archive.WriteInt(0); // NULL brep
    }

    // Write polymorphic curve
    if (rc)
    {
      if (0 != m_curve)
      {
        rc = archive.WriteInt(1);
        if (rc)
          rc = archive.WriteObject(*m_curve);
      }
      else
        rc = archive.WriteInt(0); // NULL curve
    }

    // Write curve list
    if (rc) rc = m_curve_list.Write(archive);

    // End write
    if (!archive.EndWrite3dmChunk())
      rc = false;
  }

  return rc;
}

bool CSampleSerializeData::Read(ON_BinaryArchive& archive)
{
  int major_version = 0;
  int minor_version = 0;
  ON__UINT32 tcode = 0;
  ON__INT64 big_value = 0;
  Destroy();

  // Begin read
  bool rc = archive.BeginRead3dmBigChunk(&tcode, &big_value);
  if (rc)
  {
    // Validate tcode
    if (rc) rc = (tcode == TCODE_ANONYMOUS_CHUNK);

    // Read version chunk
    if (rc) rc = archive.Read3dmChunkVersion(&major_version, &minor_version);

    // Validate version
    if (rc) rc = (major_version == 1);
    if (rc)
    {
      // Read integer
      rc = archive.ReadInt(&m_integer);

      // Read double
      if (rc) rc = archive.ReadDouble(&m_double);

      // Read string
      if (rc) rc = archive.ReadString(m_string);

      // Read point list
      if (rc) rc = archive.ReadArray(m_point_list);

      // Read mesh
      if (rc)
      {
        int flag = 0;
        rc = archive.ReadInt(&flag);
        if (rc && 1 == flag)
        {
          ON_Object* p = 0;
          rc = archive.ReadObject(&p) ? true : false;
          m_mesh = ON_Mesh::Cast(p);
          if (0 == m_mesh)
            delete p;
        }
      }

      // Read brep
      if (rc)
      {
        int flag = 0;
        rc = archive.ReadInt(&flag);
        if (rc && 1 == flag)
        {
          ON_Object* p = 0;
          rc = archive.ReadObject(&p) ? true : false;
          m_brep = ON_Brep::Cast(p);
          if (0 == m_brep)
            delete p;
        }
      }

      // Read polymorphic curve
      if (rc)
      {
        int flag = 0;
        rc = archive.ReadInt(&flag);
        if (rc && 1 == flag)
        {
          ON_Object* p = 0;
          rc = archive.ReadObject(&p) ? true : false;
          m_curve = ON_Curve::Cast(p);
          if (0 == m_curve)
            delete p;
        }
      }

      // Read curve list
      if (rc) rc = m_curve_list.Read(archive);
    }

    // End read
    if (!archive.EndRead3dmChunk())
    {
      rc = false;
    }
  }

  return rc;
}
