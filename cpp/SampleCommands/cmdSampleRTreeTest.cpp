#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleRTreeTest command
//

#pragma region SampleRTreeTest command

class CCommandSampleRTreeTest : public CRhinoCommand
{
public:
  CCommandSampleRTreeTest() = default;
  UUID CommandUUID() override
  {
    // {5BBB8E53-4534-4758-8DC4-ECC76B0692A9}
    static const GUID SampleRTreeTestCommand_UUID =
    { 0x5BBB8E53, 0x4534, 0x4758, { 0x8D, 0xC4, 0xEC, 0xC7, 0x6B, 0x06, 0x92, 0xA9 } };
    return SampleRTreeTestCommand_UUID;
  }
  const wchar_t* EnglishCommandName() override { return L"SampleRTreeTest"; }
  CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;
};

// The one and only CCommandSampleRTreeTest object
static class CCommandSampleRTreeTest theSampleRTreeTestCommand;

typedef struct tagFACEID
{
  ON_RTreeBBox m_bbox;
  ON_RTreeBBox m_P; // middle of face
  int m_face_id;
  int m_rtree_node_count;
  bool m_bInTree;
  bool m_bInIterator;
} FACEID, *PFACEID;

static void SampleGetFaceId(const ON_Mesh* mesh, ON_SimpleArray<FACEID>& fid)
{
  if (nullptr == mesh)
    return;

  ON_BoundingBox fbbox;
  ON_3dPoint P[5];
  const int fcount = mesh->m_F.Count();
  fid.SetCount(0);
  fid.Reserve(fcount);

  for (int fi = 0; fi < fcount; fi++)
  {
    const int* fvi = mesh->m_F[fi].vi;
    int fvi_count = (fvi[2] != fvi[3]) ? 4 : 3;
    P[3] = ON_3dPoint::Origin;
    for (int k = 0; k < fvi_count; k++)
      P[k] = mesh->m_V[fvi[k]];

    fbbox.Set(3, 0, fvi_count, 3, &P[0].x, false);
    FACEID& f = fid.AppendNew();
    memcpy(&f.m_bbox.m_min[0], &fbbox.m_min.x, sizeof(f.m_bbox));
    f.m_face_id = fi;
    P[4] = (P[0] + P[1] + P[2] + P[3]) / ((double)fvi_count);
    memcpy(&f.m_P.m_min[0], &P[4].x, sizeof(f.m_P.m_min));
    memcpy(&f.m_P.m_max[0], &P[4].x, sizeof(f.m_P.m_max));
    f.m_bInTree = true;
  }
}

static bool SampleFindFaceId(const ON_RTree& rtree, const FACEID& f, ON_SimpleArray<int>& result)
{
  result.SetCount(0);
  bool bFound = rtree.Search(f.m_P.m_min, f.m_P.m_max, result);
  if (!bFound)
  {
    ON_ERROR("rtree.Search failed");
    return false;
  }
  if (result.Count() <= 0 && f.m_bInTree)
  {
    ON_ERROR("rtree.Search failed");
    return false;
  }

  bFound = false;
  for (int k = 0; k < result.Count(); k++)
  {
    if (result[k] == f.m_face_id)
    {
      bFound = true;
      break;
    }
  }
  if (!bFound && f.m_bInTree)
  {
    ON_ERROR("rtree search failed to find correct fid");
    return false;
  }
  if (bFound && !f.m_bInTree)
  {
    ON_ERROR("rtree search found removed fid");
    return false;
  }
  return true;
}

static bool SampleIterator(const ON_RTree& rtree, ON_SimpleArray<FACEID>& fid)
{
  bool rc = false;
  int leafcount = 0;
  int itcount = 0;
  for (int fi = 0; fi < fid.Count(); fi++)
  {
    fid[fi].m_bInIterator = false;
    if (fid[fi].m_bInTree)
      leafcount++;
  }

  ON_SimpleArray<int> result(256);
  ON_RTreeIterator rit(rtree);
  const ON_RTreeBranch* leaf = nullptr;
  bool bFirst = false, bNext = true;
  for (bFirst = rit.First(); 0 != (leaf = rit.Value()); bNext = rit.Next())
  {
    FACEID& f = fid[(int)leaf->m_id];
    if (f.m_bInIterator)
    {
      ON_ERROR("iterator hit leaf twice");
      break;
    }
    else
    {
      itcount++;
      f.m_bInIterator = true;
    }
    if (!SampleFindFaceId(rtree, f, result))
    {
      SampleFindFaceId(rtree, f, result);
    }
  }

  for (;;)
  {
    if (!bFirst)
    {
      ON_ERROR("rit.First() failed");
      break;
    }
    if (0 != leaf)
    {
      ON_ERROR("rit.Value() failed");
      break;
    }
    if (bNext)
    {
      ON_ERROR("rit.Next() failed");
      break;
    }
    if (itcount != leafcount)
    {
      ON_ERROR("rit goof up");
      break;
    }
    rc = true;
    break;
  }

  return rc;
}

static int SampleSearch(const ON_RTree& rtree, const ON_SimpleArray<FACEID>& fid)
{
  int total_found = 0;
  ON_SimpleArray<int> result(256);
  const int fcount = fid.Count();
  int fi;
  for (fi = 0; fi < fcount; fi++)
  {
    const FACEID& f = fid[fi];
    if (!SampleFindFaceId(rtree, f, result))
      return -1;
    const_cast<FACEID&>(f).m_rtree_node_count = result.Count();
    total_found += result.Count();
  }
  return total_found;
}

CRhinoCommand::result CCommandSampleRTreeTest::RunCommand(const CRhinoCommandContext& context)
{
  ON_RTree rtree;
  ON_SimpleArray<FACEID> fid;

  {
    CRhinoGetObject go;
    go.SetCommandPrompt(L"Pick mesh");
    go.SetGeometryFilter(ON::mesh_object);
    go.GetObjects(1, 1);
    if (CRhinoCommand::success != go.CommandResult())
      return go.CommandResult();

    const ON_Mesh* mesh = go.Object(0).Mesh();
    if (0 == mesh)
      return CRhinoCommand::failure;
    SampleGetFaceId(mesh, fid);
    rtree.CreateMeshFaceTree(mesh);
  }

  const int fcount = fid.Count();
  int fi = 0;

  if (!SampleIterator(rtree, fid))
    return CRhinoCommand::failure;

  int total_found = SampleSearch(rtree, fid);

  RhinoApp().Print("Search results\n");
  RhinoApp().Print("  ON_RTree: %d\n", total_found);

  const size_t sizeof_rtree = rtree.SizeOf();
  size_t rtree_leaf_count = rtree.ElementCount();

  RhinoApp().Print("Memory Usage MAX_NODE_COUNT = %d\n", (int)ON_RTree_MAX_NODE_COUNT);
  RhinoApp().Print("  ON_RTree: %4d KB (%d)\n", sizeof_rtree / 1024, sizeof_rtree);
  RhinoApp().Print("  ON_RTree: %d elements\n", rtree_leaf_count);

  int remove_count = 0;
  srand(0);
  for (int i = 0; i < fcount / 8; i++)
  {
    fi = rand() % fcount;
    const FACEID& f = fid[fi];
    if (f.m_bInTree)
    {
      const_cast<FACEID&>(f).m_bInTree = false;
      if (!rtree.Remove(f.m_P.m_min, f.m_P.m_max, f.m_face_id))
      {
        ON_ERROR("rtree.Remove() failed\n");
        break;
      }
      remove_count++;
    }
  }

 SampleSearch(rtree, fid);

  // Test iterator
  const ON_RTreeBranch* element = nullptr;
  ON_RTreeIterator rit(rtree);
  if (!rit.First())
  {
    ON_ERROR("Iterator is bad");
  }
  else
  {
    ON_SimpleArray<bool> mark_array(fcount);
    ON_SimpleArray<INT_PTR> id_list(fcount);
    mark_array.SetCount(fcount);
    mark_array.Zero();
    bool* mark = mark_array.Array();
    int idx = 0;
    for (;;)
    {
      element = rit.Value();
      if (0 == element)
      {
        ON_ERROR("rit is bad");
        break;
      }
      if (mark[element->m_id])
      {
        ON_ERROR("rit is bad");
        break;
      }
      id_list.Append(element->m_id);
      mark[element->m_id] = true;
      idx++;
      if (!rit.Next())
      {
        if (0 != rit.Value())
        {
          ON_ERROR("rit is bad");
        }
        break;
      }
    }
    if (idx != fcount - remove_count)
    {
      ON_ERROR("rit is bad");
    }

    ON_RTreeIterator bit;
    if (!bit.Initialize(rtree) || !bit.Last() || 0 == bit.Value())
    {
      ON_ERROR("bit is bad");
    }
    else
    {
      idx = fcount - remove_count;
      idx--;
      for (;;)
      {
        element = bit.Value();
        if (0 == element)
        {
          ON_ERROR("bit bad");
          break;
        }
        if (element->m_id != id_list[idx])
        {
          ON_ERROR("bit bad");
          break;
        }
        idx--;
        if (!bit.Prev())
        {
          if (idx != -1)
          {
            ON_ERROR("bit bad");
          }
          break;
        }
        else if (idx < 0)
        {
          ON_ERROR("bit bad");
          break;
        }
      }
    }
  }

  return CRhinoCommand::success;
}

#pragma endregion

//
// END SampleRTreeTest command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
