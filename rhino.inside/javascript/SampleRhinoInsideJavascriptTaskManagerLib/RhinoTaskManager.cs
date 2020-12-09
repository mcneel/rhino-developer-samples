using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Grasshopper.Kernel;
using Rhino;
using Rhino.PlugIns;
using Rhino.Runtime.InProcess;

namespace SampleRhinoInsideJavascriptTaskManagerLib
{
  /// <summary>
  /// Custom task manager inspired by https://www.infoworld.com/article/3063560/building-your-own-task-scheduler-in-c.html.
  /// </summary>
  /// 
  public sealed class RhinoTaskManager : TaskScheduler, IDisposable
  {
    private ConcurrentQueue<Task> tasksCollection = new ConcurrentQueue<Task>();
    private readonly Thread mainThread = null;
    RhinoCore rhinoCore;

    Func<object, Task<object>> cb;

    GH_Document definition;

    static readonly Guid GrasshopperGuid = new Guid(0xB45A29B1, 0x4343, 0x4035, 0x98, 0x9E, 0x04, 0x4E, 0x85, 0x80, 0xD9, 0xCF);

    public RhinoTaskManager()
    {

      RhinoInside.Resolver.Initialize();

      mainThread = new Thread(new ThreadStart(Execute));
      mainThread.TrySetApartmentState(ApartmentState.STA);
      if (!mainThread.IsAlive)
        mainThread.Start();

      mainThread.Name = "RhinoTaskManagerThread";

    }

    public async Task<object> StartRhino(dynamic input)
    {
      try
      {
        
        rhinoCore = new RhinoCore(new string[] { "/NOSPLASH" }, WindowStyle.Hidden);

        // Subscribe to events
        RhinoApp.Idle += RhinoApp_Idle;
        RhinoApp.Initialized += RhinoApp_Initialized;

        return "Rhino Started";
      }
      catch (Exception ex)
      {
        return ex;
      }
    }

    public async Task<object> StartGrasshopper(dynamic input)
    {
        if (!PlugIn.LoadPlugIn(GrasshopperGuid))
          return null;

        var ghInit = Rhino.RhinoApp.RunScript("!_-Grasshopper _W _T ENTER", false) ? true : false;

        // Subscribe to events
        Grasshopper.Instances.ActiveCanvas.Document_ObjectsAdded += Document_ObjectsAdded;
        Grasshopper.Instances.ActiveCanvas.DocumentChanged += ActiveCanvas_DocumentChanged;

        // input.cb is a callback function in JavaScript.
        // This is to be called whenever there is an added object in the GH canvas.
        cb = (Func<object, Task<object>>) input.cb;

        return null;

    }

    /// <summary>
    /// Task to start Rhino.
    /// </summary>
    /// <param name="input"></param>
    /// <returns>TODO: Add more meaningful return object.</returns>
    public async Task<object> StartRhinoTask(dynamic input)
    {
      var result = await Task.Factory.StartNew(() => StartRhino(null), CancellationToken.None, TaskCreationOptions.None, this);
      return result;
    }

    /// <summary>
    /// Task to start Grasshopper.
    /// </summary>
    /// <param name="input"></param>
    /// <returns>TODO: Add more meaningful return object.</returns>
    public async Task<object> StartGrasshopperTask(dynamic input)
    {
      var result = await Task.Factory.StartNew(() => StartGrasshopper(input), CancellationToken.None, TaskCreationOptions.None, this);
      return "Grasshopper Initialized.";
    }

    public async Task<object> DoSomethingTask(dynamic input)
    {
      await Task.Factory.StartNew(() => DoSomething(input), CancellationToken.None, TaskCreationOptions.None, this);
      return null;
    }

    async Task<object> DoSomething(dynamic input)
    {
      var sphere = new Rhino.Geometry.Sphere(Rhino.Geometry.Point3d.Origin, 2.00);
      var sphereMesh = Rhino.Geometry.Mesh.CreateFromBrep(sphere.ToBrep(), Rhino.Geometry.MeshingParameters.Default)[0];
      cb?.Invoke(Newtonsoft.Json.JsonConvert.SerializeObject(sphereMesh));
      return null;
    }

    // Controls the main Rhino message loop.
    private void Execute()
    {
      if (rhinoCore == null)
        StartRhino(null);

      rhinoCore.Run();
    }

    private void ExecuteTasks()
    {
      while (tasksCollection.TryDequeue(out var t))
      {
        TryExecuteTask(t);
      }
    }

    // Event Handlers

    private void ActiveCanvas_DocumentChanged(Grasshopper.GUI.Canvas.GH_Canvas sender, Grasshopper.GUI.Canvas.GH_CanvasDocumentChangedEventArgs e)
    {
      // Console.WriteLine("GH: Doc Changed");
      definition = e.NewDocument;
      definition.SolutionEnd += Definition_SolutionEnd;
    }

    private void Definition_SolutionEnd(object sender, GH_SolutionEventArgs e)
    {
      // Process Meshes
      var mesh = GetDocumentPreview(e.Document);
      cb?.Invoke(Newtonsoft.Json.JsonConvert.SerializeObject(mesh));
    }

    Rhino.Geometry.Mesh GetDocumentPreview(GH_Document document)
    {
      var meshPreview = new Rhino.Geometry.Mesh();

      foreach (var obj in document.Objects.OfType<IGH_ActiveObject>())
      {
        if (obj.Locked)
          continue;

        if (obj is IGH_PreviewObject previewObject)
        {
          if (previewObject.IsPreviewCapable)
          {
            //primitivesBoundingBox = Rhino.Geometry.BoundingBox.Union(primitivesBoundingBox, previewObject.ClippingBox);

            if (obj is IGH_Component component)
            {
              if (!component.Hidden)
                foreach (var param in component.Params.Output)
                  foreach (var value in param.VolatileData.AllData(true))
                  {
                    if (value is IGH_PreviewData)
                    {
                      switch (value.ScriptVariable())
                      {
                        case Rhino.Geometry.Mesh mesh:
                          meshPreview.Append(mesh);
                          break;
                        case Rhino.Geometry.Brep brep:
                          var previewMesh = new Rhino.Geometry.Mesh();
                          previewMesh.Append(Rhino.Geometry.Mesh.CreateFromBrep(brep, Rhino.Geometry.MeshingParameters.Default));
                          meshPreview.Append(previewMesh);
                          break;
                      }
                    }
                  }
            }
            else if (obj is IGH_Param param)
            {
              foreach (var value in param.VolatileData.AllData(true))
              {
                if (value is IGH_PreviewData)
                {
                  switch (value.ScriptVariable())
                  {
                    case Rhino.Geometry.Mesh mesh:
                      meshPreview.Append(mesh);
                      break;
                    case Rhino.Geometry.Brep brep:
                      var previewMesh = new Rhino.Geometry.Mesh();
                      previewMesh.Append(Rhino.Geometry.Mesh.CreateFromBrep(brep, Rhino.Geometry.MeshingParameters.Default));
                      meshPreview.Append(previewMesh);
                      break;
                  }
                }
              }
            }
          }
        }
      }

      return meshPreview;
    }

    private void Document_ObjectsAdded(object sender, Grasshopper.Kernel.GH_DocObjectEventArgs e)
    {
      Console.WriteLine("GH: Added object to document.");
    }

    private void RhinoApp_Idle(object sender, EventArgs e)
    {
      ExecuteTasks();
    }

    private void RhinoApp_Initialized(object sender, EventArgs e)
    {
      Console.WriteLine("Rhino Initialized.");
    }

    //Other methods

    protected override IEnumerable<Task> GetScheduledTasks()
    {
      return tasksCollection.ToArray();
    }

    protected override void QueueTask(Task task)
    {
      if (task != null)
      {
        tasksCollection.Enqueue(task);
      }
    }

    protected override bool TryExecuteTaskInline(Task task, bool taskWasPreviouslyQueued)
    {
      return false;
    }

    private void Dispose(bool disposing)
    {
      if (!disposing) return;
      //tasksCollection.CompleteAdding();
      //tasksCollection.Dispose();
    }

    public void Dispose()
    {
      Dispose(true);
      GC.SuppressFinalize(this);
    }
  }
    
}
