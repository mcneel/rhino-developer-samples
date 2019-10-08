#include "stdafx.h"
#include "ON_XformArray.h"

// ON_XformArray class declaration

ON_XformArray::ON_XformArray()
  : m_serialno(1)
{
  m_xforms.SetCapacity(256);
}

ON_XformArray::ON_XformArray(const ON_SimpleArray<ON_Xform>& src)
  : m_serialno(1)
{
  m_xforms = src;
}

ON_XformArray& ON_XformArray::operator=(const ON_SimpleArray<ON_Xform>& src)
{
  m_xforms = src;
  m_serialno++;
  return *this;
}

const ON_Xform& ON_XformArray::operator[](int i) const
{
  if (0 <= i && i < m_xforms.Count())
    return m_xforms[i];
  return ON_Xform::Unset;
}

const ON_Xform& ON_XformArray::GetAt(int i) const
{
  return this->operator[](i);
}

bool ON_XformArray::SetAt(int i, const ON_Xform& src)
{
  if (0 <= i && i < m_xforms.Count())
  {
    m_xforms[i] = src;
    m_serialno++;
    return true;
  }
  return false;
}

unsigned int ON_XformArray::RuntimeSerialNumber() const
{
  return m_serialno;
}

int ON_XformArray::Count() const
{
  return m_xforms.Count();
}

void ON_XformArray::Append(const ON_Xform& src)
{
  m_xforms.Append(src);
  m_serialno++;
}

void ON_XformArray::Append(int count, const ON_Xform* src)
{
  if (count > 0 && nullptr != src)
  {
    m_xforms.Append(count, src);
    m_serialno++;
  }
}

void ON_XformArray::Remove()
{
  if (m_xforms.Count() > 0)
  {
    m_xforms.Remove();
    m_serialno++;
  }
}

void ON_XformArray::RemoveAt(int i)
{
  if (0 <= i && i < m_xforms.Count())
  {
    m_xforms.Remove(i);
    m_serialno++;
  }
}

const ON_Xform* ON_XformArray::ToArray() const
{
  return m_xforms.Array();
}

void ON_XformArray::Empty()
{
  m_xforms.Empty();
  m_serialno++;
}