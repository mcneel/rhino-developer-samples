using System;
using Rhino;

namespace rdktest_cs
{
  [System.Runtime.InteropServices.Guid("68d3c5e9-2f92-42f3-aeca-c60a77cd59c3")]
  public class rdktest_csCommand : Rhino.Commands.Command
  {
    static rdktest_csCommand m_thecommand;
    public rdktest_csCommand()
    {
      // Rhino only creates one instance of each command class defined in a plug-in, so it is
      // safe to hold on to a static reference.
      m_thecommand = this;
    }

    ///<summary>The one and only instance of this command</summary>
    public static rdktest_csCommand TheCommand
    {
      get { return m_thecommand; }
    }

    ///<returns>The command name as it appears on the Rhino command line</returns>
    public override string EnglishName
    {
      get { return "rdktest_cs"; }
    }
    /*
    private void ContentAdded(object src, Rhino.Render.RenderContentEventArgs args)
    {
      RhinoApp.WriteLine("Content added - " + args.Content.Name);
    }

    private void ContentChanged(object src, Rhino.Render.RenderContentEventArgs args)
    {
      RhinoApp.WriteLine("Content changed - " + args.Content.Name);
    }
    
    private void ContentListCleared(object src, Rhino.Render.ContentList.ContentListEventArgs args)
    {
      RhinoApp.WriteLine("Content list cleared - " + args.ContentList.ToString());
    }

    private void CRMChanged(object src, System.EventArgs args)
    {
      RhinoApp.WriteLine("Custom render meshes changed");
    }

    private void RendererChanged(object src, System.EventArgs args)
    {
      RhinoApp.WriteLine("Current renderer changed");
    }

    private void DocSettingsChanged(object src, Rhino.RhinoDoc.RdkDocumentSettingsChangedArgs args)
    {
      RhinoApp.WriteLine("Doc settings changed - " + args.Flags + "   " + args.RenderingInfo);
    }
    */

    protected override Rhino.Commands.Result RunCommand(RhinoDoc doc, Rhino.Commands.RunMode mode)
    {
      /*
      Rhino.Render.RenderContent.RenderContentAdded += ContentAdded;

      Rhino.Render.RenderContent.RenderContentChanged += new EventHandler<Rhino.Render.RenderContent.ContentChangedEventArgs>(ContentChanged);
      Rhino.Render.ContentList.ContentListCleared += new EventHandler<Rhino.Render.ContentList.ContentListEventArgs>(ContentListCleared);

      Rhino.Render.CustomRenderMesh.Manager.CustomRenderMeshesChanged += new EventHandler(CRMChanged);

      Rhino.RhinoApp.RendererChanged += new EventHandler(RendererChanged);

      Rhino.RhinoDoc.RdkSettingsChanged += new EventHandler<Rhino.RhinoDoc.RdkDocumentSettingsChangedArgs>(DocSettingsChanged);
      */

      return Rhino.Commands.Result.Success;
    }
  }
}

