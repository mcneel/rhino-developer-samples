using System;
using Rhino;
using Rhino.FileIO;
using Rhino.PlugIns;
using Rhino.UI;
using SampleCsCommon;

namespace SampleCsMain
{
  /// <summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  /// </summary>
  public class SampleCsMainPlugIn : PlugIn
  {
    public SampleCsMainPlugIn()
    {
      Instance = this;
    }

    /// <summary>
    /// Gets the only instance of the SampleCsMainPlugIn plug-in.
    /// </summary>
    public static SampleCsMainPlugIn Instance
    {
      get;
      private set;
    }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and mantain plug-in wide options in a document.

    /// <summary>
    /// Because this is the "master" plug-in and it might need 
    /// to write the string table to the 3dm file, load it when
    /// Rhino loads.
    /// </summary>
    public override PlugInLoadTime LoadTime
    {
      get { return PlugInLoadTime.AtStartup; }
    }

    /// <summary>
    /// Called when the plug-in is being loaded.
    /// </summary>
    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      // Register our panel
      System.Type panel_type = typeof(SampleCsMainPanel);
      Panels.RegisterPanel(this, panel_type, "Main", SampleCsMain.Properties.Resources.SampleCsMain);

      // Add an event handler so we know when documents are read.
      RhinoDoc.EndOpenDocument += new EventHandler<DocumentOpenEventArgs>(OnEndOpenDocument);

      // Add an event handler so we know when documents are closed.
      RhinoDoc.CloseDocument += new EventHandler<DocumentEventArgs>(OnCloseDocument);
      
      return LoadReturnCode.Success;
    }

    /// <summary>
    /// OnEndOpenDocument event handler.
    /// </summary>
    void OnEndOpenDocument(object sender, DocumentOpenEventArgs e)
    {
      // By the time we get here, our user data has been read from the document
      // (if there was any). So, call Refresh() to notify anyone watching for
      // this event.
      SampleCsStringTableHelpers.Refresh();
    }

    /// <summary>
    /// OnCloseDocument event handler.
    /// </summary>
    private void OnCloseDocument(object sender, DocumentEventArgs e)
    {
      // By the time we get here, our user data has been written to the document
      // (if there was any). So, call Clear() to notify anyone watching for
      // this event.
      SampleCsStringTableHelpers.Clear();
    }

    /// <summary>
    /// Called whenever a Rhino is about to save a .3dm file.  If you want to save
    //  plug-in document data when a model is saved in a version 5 .3dm file, then
    //  you must override this function to return true and you must override WriteDocument().
    /// </summary>
    protected override bool ShouldCallWriteDocument(FileWriteOptions options)
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      return string_table.ShouldCallWriteDocument(options);
    }

    /// <summary>
    /// Called when Rhino is saving a .3dm file to allow the plug-in to save document user data.
    /// </summary>
    protected override void WriteDocument(RhinoDoc doc, BinaryArchiveWriter archive, FileWriteOptions options)
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      string_table.WriteDocument(doc, archive, options);
    }

    /// <summary>
    /// Called whenever a Rhino document is being loaded and plug-in user data was
    /// encountered written by a plug-in with this plug-in's GUID.
    /// </summary>
    protected override void ReadDocument(RhinoDoc doc, BinaryArchiveReader archive, FileReadOptions options)
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      string_table.ReadDocument(doc, archive, options);
    }

    /// <summary>
    /// The tabbed dockbar panel (user control)
    /// </summary>
    public SampleCsMainPanel Panel
    {
      get;
      set;
    }
  }
}