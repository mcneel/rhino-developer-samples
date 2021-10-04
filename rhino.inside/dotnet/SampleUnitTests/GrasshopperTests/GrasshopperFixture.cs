using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Rhino.Test
{
  public class GrasshopperFixture : IDisposable
  {
    private object _Core = null;
    private object _GHPlugin = null;
    private object _DocIO { get; set; }
    private object _Doc { get; set; }
    private bool _isDisposed;
    protected string FilePath { get; private set; }

    static GrasshopperFixture()
    {
      // This MUST be included in a static constructor to ensure that no Rhino DLLs
      // are loaded before the resolver is set up. Avoid creating other static functions
      // and members which may reference Rhino assemblies, as that may cause those
      // assemblies to be loaded before this is called.
      RhinoInside.Resolver.Initialize();
    }
    public GrasshopperFixture(string GHFilePath)
    {
      FilePath = GHFilePath;
      InitializeCore();
    }

    protected virtual void Dispose(bool disposing)
    {
      if (_isDisposed) return;
      if (disposing)
      {
        _Doc = null;
        _DocIO = null;
        GHPlugin.CloseAllDocuments();
        _GHPlugin = null;
        Core.Dispose();
      }

      // TODO: free unmanaged resources (unmanaged objects) and override finalizer
      // TODO: set large fields to null
      _isDisposed = true;
    }

    // // TODO: override finalizer only if 'Dispose(bool disposing)' has code to free unmanaged resources
    // ~GrasshopperFixture()
    // {
    //     // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
    //     Dispose(disposing: false);
    // }

    public void Dispose()
    {
      // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
      Dispose(disposing: true);
      GC.SuppressFinalize(this);
    }

    public Rhino.Runtime.InProcess.RhinoCore Core
    {
      get
      {
        if (null == _Core) InitializeCore();
        return _Core as Rhino.Runtime.InProcess.RhinoCore;
      }
    }
    public Grasshopper.Plugin.GH_RhinoScriptInterface GHPlugin
    {
      get
      {
        if (null == _GHPlugin) InitializeGrasshopperPlugin();
        return _GHPlugin as Grasshopper.Plugin.GH_RhinoScriptInterface;
      }
    }
    public Grasshopper.Kernel.GH_DocumentIO DocIO
    {
      get
      {
        if (null == _DocIO) InitializeDocIO();
        return _DocIO as Grasshopper.Kernel.GH_DocumentIO;
      }
    }

    public Grasshopper.Kernel.GH_Document Doc
    {
      get
      {
        if (null == _Doc && null != FilePath)
          _Doc = LoadGrasshopperDoc(FilePath);
        return _Doc as Grasshopper.Kernel.GH_Document;
      }
    }

    public Grasshopper.Kernel.GH_Document LoadGrasshopperDoc(string filePath)
    {
      if (null != _Doc)
        return Doc;
      if (!DocIO.Open(filePath))
        throw new InvalidOperationException("File Loading Failed");
      else
      {
        var doc = DocIO.Document;
        _Doc = doc;

        // Documents are typically only enabled when they are loaded
        // into the Grasshopper canvas. In this case we -may- want to
        // make sure our document is enabled before using it.
        doc.Enabled = true;
      }
      return Doc;
    }

    void InitializeCore()
    {
      _Core = new Rhino.Runtime.InProcess.RhinoCore();
    }
    void InitializeGrasshopperPlugin()
    {
      if (null == _Core) InitializeCore();
      // we do this in a seperate function to absolutely ensure that the core is initialized before we load the GH plugin,
      // which will happen automatically when we enter the function containing GH references
      InitializeGrasshopperPlugin2();
    }
    void InitializeGrasshopperPlugin2()
    {
      _GHPlugin = Rhino.RhinoApp.GetPlugInObject("Grasshopper");
      var ghp = _GHPlugin as Grasshopper.Plugin.GH_RhinoScriptInterface;
      ghp.RunHeadless();
    }
    void InitializeDocIO()
    {
      // we do this in a seperate function to absolutely ensure that the core is initialized before we load the GH plugin,
      // which will happen automatically when we enter the function containing GH references
      if (null == _GHPlugin) InitializeGrasshopperPlugin();
      InitializeDocIO2();
    }
    void InitializeDocIO2()
    {
      var docIO = new Grasshopper.Kernel.GH_DocumentIO();
      _DocIO = docIO;
    }
  }

}


