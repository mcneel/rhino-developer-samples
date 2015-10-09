using System;
using Rhino;

namespace examples_cs
{
  [System.Runtime.InteropServices.Guid("d31dc306-358e-4e26-a453-620e0f2f9116")]
  public class ex_custompython : Rhino.Commands.Command
  {
    public override string EnglishName { get { return "examples_custompython"; } }

    protected override Rhino.Commands.Result RunCommand(RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      if (null == m_python)
      {
        m_python = Rhino.Runtime.PythonScript.Create();
        if (null == m_python)
        {
          RhinoApp.WriteLine("Error: Unable to create an instance of the python engine");
          return Rhino.Commands.Result.Failure;
        }
      }
      m_python.ScriptContextDoc = new CustomPythonDoc(doc);

      const string script = @"
import rhinoscriptsyntax as rs
rs.AddLine((0,0,0), (10,10,10))
";
      m_python.ExecuteScript(script);
      return Rhino.Commands.Result.Success;
    }

    Rhino.Runtime.PythonScript m_python;
  }

  // our fake RhinoDoc
  public class CustomPythonDoc
  {
    readonly RhinoDoc m_doc;
    public CustomPythonDoc(RhinoDoc doc)
    {
      m_doc = doc;
    }

    readonly CustomObjectTable m_table = new CustomObjectTable();
    public CustomObjectTable Objects
    {
      get { return m_table; }
    }

    public Rhino.DocObjects.Tables.ViewTable Views
    {
      get
      {
        return m_doc.Views;
      }
    }

  }

  public class CustomObjectTable
  {
    public Guid AddLine(Rhino.Geometry.Point3d p1, Rhino.Geometry.Point3d p2)
    {
      Rhino.Geometry.Line l = new Rhino.Geometry.Line(p1, p2);
      if (l.IsValid)
      {
        Guid id = Guid.NewGuid();
        m_lines_dict.Add(id, l);
        return id;
      }
      return Guid.Empty;
    }

    readonly System.Collections.Generic.Dictionary<Guid, Rhino.Geometry.Line> m_lines_dict = new System.Collections.Generic.Dictionary<Guid, Rhino.Geometry.Line>();
  }

}

