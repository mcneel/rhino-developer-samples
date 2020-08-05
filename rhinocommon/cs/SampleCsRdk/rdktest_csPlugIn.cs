using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.Remoting.Activation;
using Rhino;
using Rhino.Display;
using Rhino.Render;
using System.Threading;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Render.ChangeQueue;
using Rhino.PlugIns;

namespace SampleCsRdk
{

  public class NewSizeArgs : EventArgs
  {
    public ViewInfo ViewInfo { get; private set; }
    public NewSizeArgs(ViewInfo viewInfo)
    {
      ViewInfo = viewInfo;
    }
  }
  public class RdkTestChangeQueue : ChangeQueue
  {
    internal RdkTestChangeQueue(Guid pluginId, uint doc, ViewInfo view) : base(pluginId, doc, view, null, true, true)
    {

    }

    public event EventHandler<NewSizeArgs> ViewChanged;
    protected override void ApplyViewChange(ViewInfo viewInfo)
    {
      ViewChanged?.Invoke(this, new NewSizeArgs(viewInfo));
    }

    public bool HasChanges
    {
      get; private set;
    }

    protected override void NotifyEndUpdates()
    {
      HasChanges = true;
    }

  }

  public class RdkTestViewportClassInfo : RealtimeDisplayModeClassInfo
  {

    public override string Name
    {
      get
      {
        return "RdkTest";
      }
    }

    public override Guid GUID
    {
      get
      {
        return new Guid("758E5239-C2E4-49C7-A0BB-545C5FCD24EB");
      }
    }

    public override Type RealtimeDisplayModeType
    {
      get
      {
        return typeof(RdkTestViewport);
      }
    }

    public override bool DrawOpenGl
    {
      get
      {
        return false;
      }
    }
  }

  public class RdkTestViewport : RealtimeDisplayMode
  {
    private RenderWindow rw;
    private RdkTestChangeQueue cq;
    private Thread RenderThread;
    private bool quit;
    private bool available;
    private Size rs;
    private Random rnd = new Random();
    private const int m_maxsamples = 50;
    private int m_samples = 0;
    public bool StartRender(int w, int h, RhinoDoc doc, ViewInfo view, ViewportInfo viewportInfo, bool forCapture, RenderWindow renderWindow)
    {
      available = true;
      rw = renderWindow;
      rs = new Size((int)w, (int)h);
      rw.SetSize(rs);
      cq = new RdkTestChangeQueue(PlugIn.IdFromName("RdkTest"), doc.RuntimeSerialNumber, view);
      cq.ViewChanged += Cq_ViewChanged;
      cq.CreateWorld();
      cq.Flush();
      quit = false;
      RenderThread = new Thread(this.RenderIt)
      {
        Name = "A RdkTest Render Thread"
      };
      RenderThread.Start();
      return true;
    }

    private readonly object uselock = new object();

    private void Cq_ViewChanged(object sender, NewSizeArgs e)
    {
      lock (uselock)
      {
        var sp = e.ViewInfo.Viewport.ScreenPort;
        rs = new Size(sp.Width, sp.Height);
        if (rw.Size() != rs)
        {
          rw.SetSize(rs);
        }
        SetView(cq.GetQueueView());
        m_samples = 0;
      }
    }

    public void RenderIt()
    {
      while (!quit)
      {
        if (m_samples < m_maxsamples)
        {
          lock (uselock)
          {
            using (var ch = rw.OpenChannel(RenderWindow.StandardChannels.RGBA))
            {
              for (var x = 0; x < rs.Width; x++)
              {
                for (var y = 0; y < rs.Height; y++)
                {
                  ch.SetValue(x, y, new Color4f((float)rnd.NextDouble(), (float)rnd.NextDouble(), (float)rnd.NextDouble(), 1.0f));
                }
              }
            }
          }
          rw.Invalidate();
          m_samples++;
          SignalRedraw();
        }
        Thread.Sleep(10);
        if (cq.HasChanges) cq.Flush();
      }
    }

    public bool IsRenderframeAvailable()
    {
      return true;
    }

    public override bool IsRendererStarted()
    {
      return available;
    }
    public override bool IsFrameBufferAvailable(ViewInfo view)
    {
      var yes = cq.AreViewsEqual(GetView(), view);
      return yes;
    }

    public void ShutdownRender()
    {
      lock (uselock)
      {
        available = false;
        quit = true;
        RenderThread.Join();
      }
    }
    public override string HudProductName()
    {
      return "RdkTest";
    }

    public override int HudMaximumPasses()
    {
      return m_maxsamples;
    }

    public override int LastRenderedPass()
    {
      return m_samples;
    }
    public override int HudLastRenderedPass()
    {
      return m_samples;
    }

    public override bool HudRendererPaused()
    {
      return false;
    }

    public override bool HudRendererLocked()
    {
      return false;
    }

    public override bool HudShowMaxPasses()
    {
      return true;
    }

    public override bool HudShowPasses()
    {
      return true;
    }

    public override void GetRenderSize(out int width, out int height)
    {
      throw new NotImplementedException();
    }

    public override bool StartRenderer(int w, int h, RhinoDoc doc, ViewInfo view, ViewportInfo viewportInfo, bool forCapture, RenderWindow renderWindow)
    {
      throw new NotImplementedException();
    }

    public override void ShutdownRenderer()
    {
      throw new NotImplementedException();
    }

    public override bool IsCompleted()
    {
      throw new NotImplementedException();
    }
  }

  class MyAsyncRenderContext : AsyncRenderContext
  {
    public uint session_id = uint.MaxValue;
    public bool CancelRender { get; set; }

    public MyAsyncRenderContext()
    {
    }

    override public void StopRendering()
    {
      if (RenderThread != null)
      {
        CancelRender = true;
        RenderThread.Join();
      }
    }

    protected override void DeleteThis()
    {
      base.DeleteThis();
    }

    public void Renderer()
    {
      MyAsyncRenderContext aRC = this;
      RenderWindow rw = aRC.RenderWindow;

      if (null != rw)
      {
        Size size = rw.Size();
        using (RenderWindow.Channel channel = rw.OpenChannel(Rhino.Render.RenderWindow.StandardChannels.RGBA))
        {
          for (int x = 0; x < size.Width; x++)
          {
            for (int y = 0; y < size.Height; y++)
            {
              Rhino.Display.Color4f color = new Rhino.Display.Color4f(y / (float)size.Width, x / (float)size.Height, 1.0f, 1.0f);
              channel.SetValue(x, y, color);
              rw.Invalidate();
              Thread.Sleep(0);
            }
            float progress = x / (float)size.Width;
            rw.SetProgress(String.Format("Rendering C# rendering {0:d}% complete", (int)(progress * 100.0f)), progress);
            Thread.Sleep(1);

            if (aRC.CancelRender)
              break;
          }
          rw.SetProgress("Rendering complete", 1.0f);
          Thread.Sleep(10);
        }

        rw.EndAsyncRender(RenderWindow.RenderSuccessCode.Completed);
      }
    }
  }

  class MyRenderPipeline : RenderPipeline
  {
    private bool m_bStopFlag;
    private MyAsyncRenderContext m_asyncRenderContext;

    public MyRenderPipeline(RhinoDoc doc, Rhino.Commands.RunMode mode, Rhino.PlugIns.RenderPlugIn plugin, ref AsyncRenderContext aRC)
      : base(doc, mode, plugin, RenderPipeline.RenderSize(doc),
             "RdkTest", Rhino.Render.RenderWindow.StandardChannels.RGBA, false, false, ref aRC)
    {
     m_asyncRenderContext = (MyAsyncRenderContext)aRC;
    }

    public bool Cancel() { return m_bStopFlag; }

    protected override bool OnRenderBegin()
    {
      m_bStopFlag = false;
      m_asyncRenderContext.StartRenderThread(m_asyncRenderContext.Renderer, "rdktest_cs render thread");
      return true;
    }

    protected override bool OnRenderBeginQuiet(Size imageSize)
    {
      return false;
    }

    protected override void OnRenderEnd(RenderEndEventArgs e)
    {
      // unused for async rendering
    }

    protected override bool ContinueModal()
    {
      return true; // always return true when async rendering
    }

    protected override bool OnRenderWindowBegin(Rhino.Display.RhinoView view, System.Drawing.Rectangle rect) { return false; }
  }
  public class rdktest_csPlugIn : Rhino.PlugIns.RenderPlugIn
  {
    protected override Rhino.PlugIns.LoadReturnCode OnLoad(ref string errorMessage)
    {
      // Tell Rhino about our custom render content
      RenderContent.RegisterContent(this);
      RealtimeDisplayMode.RegisterDisplayModes(this);
      //Rhino.Render.IOPlugIn.RegisterContentIo(this.Assembly, this.Id);

      // Tell Rhino about our custom mesh provider
      CustomRenderMeshProvider2.RegisterProviders(GetType().Assembly, Id);

      RhinoDoc.NewDocument += g_rhino_doc_new_doc;

      return Rhino.PlugIns.LoadReturnCode.Success;
    }

    private static EventHandler<DocumentEventArgs> g_rhino_doc_new_doc = RhinoDocNewDocument;
    static void RhinoDocNewDocument(object sender, DocumentEventArgs e)
    {
      //AddDefaultCustomEnvironment(e.Document);
    }

    static void AddDefaultCustomEnvironment(RhinoDoc rhinoDoc)
    {
      // There is no default environment so add one now
      var type = typeof(CustomEnvironment);
      var content = RenderContent.Create(type, RenderContent.ShowContentChooserFlags.None, rhinoDoc) as CustomEnvironment;
      if (content == null) return;
      rhinoDoc.CurrentEnvironment.ForBackground = content;
      rhinoDoc.RenderSettings.BackgroundStyle = BackgroundStyle.Environment;
    }

    protected override void RegisterRenderPanels(RenderPanels panels)
    {
      var type = typeof (CustomRenderPanel);
      panels.RegisterPanel(this, RenderPanelType.RenderWindow, type, this.Id, "Custom Panel", true, true);
    }

    protected override void RegisterRenderTabs(RenderTabs tabs)
    {
      var type = typeof(CustomRenderPanel);
      tabs.RegisterTab(this, type, this.Id, "Custom Panel", SystemIcons.Exclamation);
    }

    protected override Rhino.Commands.Result RenderWindow(RhinoDoc doc, Rhino.Commands.RunMode modes, bool fastPreview, Rhino.Display.RhinoView view, System.Drawing.Rectangle rect, bool inWindow)
    {
      return Rhino.Commands.Result.Failure;
    }

    protected override void CreatePreview(CreatePreviewEventArgs scene)
    {
      if (scene.Quality == PreviewSceneQuality.Low)
      {
        // Use built in image for quick preview so we don't slow the UI down.
        scene.PreviewImage = new Bitmap(Properties.Resources.AtoZ, scene.PreviewImageSize); 
        return;
      }
      // New preview bitmap
      Bitmap bitmap = null;
      // If previewing a custom environment
      if (scene.ContentKind == RenderContentKind.Environment)
      {
        Color4f color;
        if (scene.Environment.Fields.TryGetValue("background", out color) ||
            scene.Environment.Fields.TryGetValue("color", out color) ||
            scene.Environment.Fields.TryGetValue("background-color", out color))
          bitmap = BitmapForScene(scene, color);
      }
      else if (scene.ContentKind == RenderContentKind.Material)
      {
        // Make sure the content is a material
        RenderMaterial render_material = null;
        foreach (var obj in scene.Objects)
        {
          render_material = obj.Material;
          if (render_material != null) break;
        }
        if (null != render_material)
        {
          Color4f color;
          // First try to get the value of the custom color field, if it is not found
          // then look for the RDK "diffuse" color field
          if (!render_material.Fields.TryGetValue("color", out color) &&
              !render_material.Fields.TryGetValue(RenderMaterial.BasicMaterialParameterNames.Diffuse, out color))
            color = Color4f.Black; // Could not find either of the color fields
          // Create preview bitmap of requested size
          bitmap = BitmapForScene(scene, color);
        }
      }
      scene.PreviewImage = bitmap ?? new Bitmap(Properties.Resources.AtoZ, scene.PreviewImageSize);
    }

    Bitmap BitmapForScene(CreatePreviewEventArgs scene, Color4f color)
    {
      var bitmap = new Bitmap(scene.PreviewImageSize.Width, scene.PreviewImageSize.Height, PixelFormat.Format24bppRgb);
      // Fill the bitmap using the computed color
      using (var g = Graphics.FromImage(bitmap))
      {
        g.Clear(Color.FromArgb(255, color.AsSystemColor()));
        g.DrawRectangle(Pens.Black, 0, 0, bitmap.Width - 1, bitmap.Height - 1);
      }
      return bitmap;
    }

    protected override Result Render(RhinoDoc doc, RunMode mode, bool fastPreview)
    {
      AsyncRenderContext aRC = new MyAsyncRenderContext();

      RenderPipeline pipe = new MyRenderPipeline(doc, mode, this, ref aRC);
      ((MyAsyncRenderContext)aRC).RenderWindow = pipe.GetRenderWindow();
      ((MyAsyncRenderContext)aRC).RenderWindow.SetSize(RenderPipeline.RenderSize(doc));

      RenderPipeline.RenderReturnCode rc = pipe.Render();

      if (RenderPipeline.RenderReturnCode.Ok != rc)
        RhinoApp.WriteLine("Render start failed:" + rc.ToString());
      else
        RhinoApp.WriteLine("Rendering started!", (int)rc);

      return Rhino.Commands.Result.Success;
    }

    //public RenderSceneWithNoMeshes

    /*
    protected override void InitializeDecalProperties(ref List<NamedValue> properties)
    {
      properties.Add(new NamedValue("Test", 1.0));
    }

    protected override bool ShowDecalProperties(ref List<NamedValue> properties)
    {
      System.Windows.Forms.MessageBox.Show("Custom Decal Properties");
      return true;
    }
    */
  }
}
