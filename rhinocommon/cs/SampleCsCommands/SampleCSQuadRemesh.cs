using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using Rhino.Input;
using Rhino.Input.Custom;

namespace SampleCsCommands
{
  public class SampleCsQuadRemesh : Command
  {
    static SampleCsQuadRemesh _instance;
    public SampleCsQuadRemesh()
    {
      _instance = this;
    }

    ///<summary>The only instance of the SampleCSQuadRemesh command.</summary>
    public static SampleCsQuadRemesh Instance
    {
      get { return _instance; }
    }

    public override string EnglishName
    {
      get { return "SampleCsQuadRemesh"; }
    }

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      //Select some object
      var go = new GetObject();
      go.SetCommandPrompt("Select object to remesh");
      go.GeometryFilter = ObjectType.Brep | ObjectType.Mesh;
      go.Get();

      if (go.Result() != GetResult.Object)
        return Result.Cancel;

      //Grab the results of the object picker.
      var rhino_object = go.Object(0).Object();


      //Set our desired quad remesh parameters
      var quad_remesh_parameters = new QuadRemeshParameters()
      {
        TargetQuadCount = 2000,
        AdaptiveQuadCount = true,
        AdaptiveSize = .5,
        DetectHardEdges = true,
        SymmetryAxis = QuadRemeshSymmetryAxis.X | QuadRemeshSymmetryAxis.Y | QuadRemeshSymmetryAxis.Z, //Set symmetry on all 3 axis
        GuideCurveInfluence = 0, //Guide Curve Influence 0 Approx ; 1 = Edge Ring ; 2 = Edge Loop (ignored if no curve array input)
      };

      //Remesh a selected BRep
      if (rhino_object.ObjectType == ObjectType.Brep)
      {
        if (!(rhino_object.Geometry is Brep brep))
          return Result.Cancel;

        //Remesh the brep
        //Optionally this method also accepts an IEnumerable<Curve> overload.
        //Pass it your curve object array/list if you want guide curves)
        //If guide curves, lastly change your quad_remesh_parameters.GuideCurveInfluence if using curves
        var remeshed = Mesh.QuadRemeshBrep(brep, quad_remesh_parameters);

        if (remeshed == null)
          return Result.Cancel;

        //Add the result to the doc
        doc.Objects.AddMesh(remeshed);
        doc.Views.Redraw();
        RhinoApp.WriteLine("1 Brep successfully remeshed");
      }
      //Remesh a selected Mesh
      else if(rhino_object.ObjectType == ObjectType.Mesh)
      {
        if (!(rhino_object is MeshObject mesh_object))
          return Result.Cancel;

        //Make sure this is a valid selected mesh object (it should be)
        if (!mesh_object.MeshGeometry.IsValid)
          return Result.Cancel;

        //Do the remeshing of the Mesh Objects Mesh Geometry
        var remeshed = mesh_object.MeshGeometry.QuadRemesh(quad_remesh_parameters);

        if (remeshed == null)
          return Result.Cancel;

        //Add the result to the doc
        doc.Objects.AddMesh(remeshed);
        doc.Views.Redraw();
        RhinoApp.WriteLine("1 Mesh successfully remeshed");
      }








      return Result.Success;
    }
  }
}