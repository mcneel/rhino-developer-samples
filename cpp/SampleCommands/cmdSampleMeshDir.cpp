#include "stdafx.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//
// BEGIN SampleMeshDir command
//

class CMeshDir
{
public:
  CMeshDir();
  CMeshDir( const ON_Mesh* );
  void SetMesh( const ON_Mesh* );

  ON_SimpleArray<ON_3dPoint> m_face_center;
  ON_SimpleArray<ON_3dVector> m_face_normal;
  const ON_Mesh* m_mesh;
};

CMeshDir::CMeshDir()
{
  m_mesh = NULL;
}

CMeshDir::CMeshDir( const ON_Mesh* mesh )
{
  SetMesh( mesh );
}

void CMeshDir::SetMesh( const ON_Mesh* mesh )
{
  m_mesh = mesh;
  m_face_center.Destroy();
  m_face_normal.Destroy();
  if( m_mesh )
  {
    if( !m_mesh->HasFaceNormals() )
    {
      const_cast<ON_Mesh*>(m_mesh)->ComputeFaceNormals();
    }
    if( m_mesh->HasFaceNormals() )
    {
      int fi, fcnt = m_mesh->m_F.Count();
      m_face_center.Reserve(fcnt);
      m_face_normal.Reserve(fcnt);
      ON_3dPoint p;
      ON_3dVector n;
      for( fi = 0; fi < fcnt; fi++ )
      {
        const int* fvi = m_mesh->m_F[fi].vi;
        if( m_mesh->m_F[fi].IsQuad() )
        {
          p = 0.25*(m_mesh->m_V[fvi[0]] + m_mesh->m_V[fvi[1]] + m_mesh->m_V[fvi[2]] + m_mesh->m_V[fvi[3]]);
        }
        else if( m_mesh->m_F[fi].IsTriangle() )
        {
          p = (1.0f/3.0f)*(m_mesh->m_V[fvi[0]] + m_mesh->m_V[fvi[1]] + m_mesh->m_V[fvi[2]]);
        }
        else
          continue;
        n = m_mesh->m_FN[fi];
        m_face_center.Append(p);
        m_face_normal.Append(n);
      }
    }
  }
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CMeshDirDrawCallback : public CRhinoDrawCallback
{
public:
  CMeshDirDrawCallback();
  void DrawMiddleground( CRhinoViewport&, CRhinoDoc& );

  ON_ClassArray<CMeshDir> m_mesh_list;
  ON_Color m_face_normal_color;
  ON_Color m_vertex_normal_color;
  bool m_draw_face_normals;
  bool m_draw_vertex_normals;
};

CMeshDirDrawCallback::CMeshDirDrawCallback()
{
  m_face_normal_color = RGB(0,0,0);
  m_vertex_normal_color = RGB(255,255,255);
  m_draw_face_normals = true;
  m_draw_vertex_normals = true;
}

void CMeshDirDrawCallback::DrawMiddleground( CRhinoViewport& vp, CRhinoDoc& )
{
  int mi, mcnt, fi, fcnt, vi, vcnt;
  mcnt = m_mesh_list.Count();
  for( mi = 0; mi < mcnt; mi++ )
  {
    ON_Color saved_color = vp.DrawColor();
    CMeshDir& md = m_mesh_list[mi];

    if( m_draw_face_normals )
    {
      fcnt = md.m_face_center.Count();
      vp.SetDrawColor( m_face_normal_color );
      for( fi = 0; fi < fcnt; fi++ )
      {
        vp.DrawDirectionArrow( md.m_face_center[fi], md.m_face_normal[fi] );
      }
    }

    if( m_draw_vertex_normals )
    {
      if ( md.m_mesh->HasVertexNormals() )
      {
        vp.SetDrawColor( m_vertex_normal_color );
        vcnt = md.m_mesh->m_V.Count();
        for( vi = 0; vi < vcnt; vi++ )
        {
          vp.DrawDirectionArrow( ON_3dPoint(md.m_mesh->m_V[vi]), ON_3dVector(md.m_mesh->m_N[vi]) );
        }
      }
    }

    vp.SetDrawColor( saved_color );
  }
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

class CCommandSampleMeshDir : public CRhinoCommand
{
public:
	CCommandSampleMeshDir();
	~CCommandSampleMeshDir() {}
	UUID CommandUUID()
	{
		// {E35209D2-AC0D-43D1-A79C-2B432F04F9BA}
		static const GUID SampleMeshDirCommand_UUID =
		{ 0xE35209D2, 0xAC0D, 0x43D1, { 0xA7, 0x9C, 0x2B, 0x43, 0x2F, 0x04, 0xF9, 0xBA } };
		return SampleMeshDirCommand_UUID;
	}
	const wchar_t* EnglishCommandName() { return L"SampleMeshDir"; }
	const wchar_t* LocalCommandName() { return L"SampleMeshDir"; }
	CRhinoCommand::result RunCommand( const CRhinoCommandContext& );
  
  void LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );
  void SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc );

private:
  bool m_draw_face_normals;
  bool m_draw_vertex_normals;
};

// The one and only CCommandSampleMeshDir object
static class CCommandSampleMeshDir theSampleMeshDirCommand;

CCommandSampleMeshDir::CCommandSampleMeshDir()
{
  m_draw_face_normals = true;
  m_draw_vertex_normals = true;
}

CRhinoCommand::result CCommandSampleMeshDir::RunCommand( const CRhinoCommandContext& context )
{
  CRhinoGetObject go;
  go.SetGeometryFilter( CRhinoObject::mesh_object | CRhinoObject::surface_object | CRhinoObject::polysrf_object | CRhinoObject::instance_reference );
  go.SetCommandPrompt( L"Select mesh, surface, or polysurface objects" );
  go.GetObjects(1,0);
  if( go.Result() != CRhinoGet::object )
    return CRhinoCommand::cancel;

  int i = 0;
  ON_SimpleArray<const CRhinoObject*> objects( go.ObjectCount() );
  for( i = 0; i < go.ObjectCount(); i++ )
    objects.Append( go.Object(i).Object() );

  ON_ClassArray<CRhinoObjRef> render_meshes;
  int count = RhinoGetRenderMeshes( objects, render_meshes, true, false );
  if( count > 0 )
  {
    CMeshDirDrawCallback callback;
    callback.m_mesh_list.Reserve( count );
    for( i = 0; i < render_meshes.Count(); i++ )
    {
      const ON_Mesh* mesh = render_meshes[i].Mesh();
      if( mesh )
        callback.m_mesh_list.AppendNew().SetMesh( mesh );
    }

    CRhinoGetOption go;
    go.SetCommandPrompt( L"Press Enter when done" );
    go.AcceptNothing();
    go.AddCommandOptionToggle( RHCMDOPTNAME(L"FaceNormals"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), m_draw_face_normals, &m_draw_face_normals );
    go.AddCommandOptionToggle( RHCMDOPTNAME(L"VertexNormals"), RHCMDOPTVALUE(L"No"), RHCMDOPTVALUE(L"Yes"), m_draw_vertex_normals, &m_draw_vertex_normals );

    CRhinoGet::result res = CRhinoGet::nothing;
    for( ;; )
    {
      callback.m_draw_face_normals = m_draw_face_normals;
      callback.m_draw_vertex_normals = m_draw_vertex_normals;

      CRhinoView::AddDrawCallback( &callback );
      context.m_doc.Redraw( CRhinoView::regenerate_display_hint );

      res = go.GetOption();
      CRhinoView::RemoveDrawCallback( &callback );

      if( res == CRhinoGet::option )
        continue;
      break;
    }
  }

  context.m_doc.Redraw( CRhinoView::regenerate_display_hint );
  
  return CRhinoCommand::success;
}

void CCommandSampleMeshDir::LoadProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc )
{
  pc.LoadProfileBool( lpszSection, L"DrawFaceNormals", &m_draw_face_normals );
  pc.LoadProfileBool( lpszSection, L"DrawVertexNormals", &m_draw_vertex_normals );
}

void CCommandSampleMeshDir::SaveProfile( LPCTSTR lpszSection, CRhinoProfileContext& pc )
{
  pc.SaveProfileBool( lpszSection, L"DrawFaceNormals", m_draw_face_normals );
  pc.SaveProfileBool( lpszSection, L"DrawVertexNormals", m_draw_vertex_normals );
}
//
// END SampleMeshDir command
//
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
