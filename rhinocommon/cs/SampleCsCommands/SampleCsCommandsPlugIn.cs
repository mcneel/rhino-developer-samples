using System;
using System.IO;
using System.Text;
using Rhino;
using Rhino.PlugIns;

namespace SampleCsCommands
{
  ///<summary>
  /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
  /// class. DO NOT create instances of this class yourself. It is the
  /// responsibility of Rhino to create an instance of this class.</para>
  /// <para>To complete plug-in information, please also see all PlugInDescription
  /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
  /// "Show All Files" to see it in the "Solution Explorer" window).</para>
  ///</summary>
  public class SampleCsCommandsPlugIn : PlugIn
  {
    public SampleCsCommandsPlugIn()
    {
      Instance = this;
      IsolateIndex = 0;
    }

    /// <summary>
    /// Gets the only instance of the SampleCsCommandsPlugIn plug-in.
    /// </summary>
    public static SampleCsCommandsPlugIn Instance
    {
      get;
      private set;
    }

    /// <summary>
    /// Used by the Isolate and Unisolate commands.
    /// </summary>
    public int IsolateIndex { get; set; }

    // You can override methods here to change the plug-in behavior on
    // loading and shut down, add options pages to the Rhino _Option command
    // and mantain plug-in wide options in a document.

    protected override LoadReturnCode OnLoad(ref string errorMessage)
    {
      StagePlugInToolbarFile();

      RhinoDoc.CloseDocument += OnCloseDocument;
      return LoadReturnCode.Success;
    }

    private void OnCloseDocument(object sender, DocumentEventArgs e)
    {
      IsolateIndex = 0;
    }

    /// <summary>
    /// Forces the (re)staging of the plug-in's toolbar, or .rui, file.
    /// </summary>
    private void StagePlugInToolbarFile()
    {
      // QUESTION: 
      // When we install a newer version of our plugin, with changes to the toolbar,
      // the new toolbar is not copied over the old toolbar. Is there a way to do this?
      //
      // ANSWER:
      // the first time a plug-in is loaded, Rhino looks for a .rui file with the same
      // name as the plug-in. If it is found, it is copied to the 
      // "%APPDATA%\McNeel\Rhinoceros\5.0\Plug-ins\[plug-in name] ([plug-in UUID)\settings" 
      // folder and opened. It is copied, or staged, to ensure that it is writable and to 
      // provide a way to get the default one back.
      //
      // If your update your plug-in, Rhino will not re-stage the .rui file because it already
      // exists. You can get Rhino to re-stage the .rui file by deleting it in %APPDATA% and 
      // restarting which will cause Rhino to copy the file again since it no longer exists. 
      //
      // EXAMPLE:

      // Get the version number of our plug-in, that was last used, from our settings file.
      var plugin_version = Settings.GetString("PlugInVersion", null);

      if (!string.IsNullOrEmpty(plugin_version))
      {
        // If the version number of the plug-in that was last used does not match the
        // version number of this plug-in, proceed.
        if (0 != string.Compare(Version, plugin_version, StringComparison.OrdinalIgnoreCase))
        {
          // Build a path to the user's staged .rui file.
          var sb = new StringBuilder();
          sb.Append(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData));
          sb.Append("\\McNeel\\Rhinoceros\\7.0\\Plug-ins\\");
          sb.AppendFormat("{0} ({1})", Name, Id);
          sb.Append("\\settings\\");
          sb.AppendFormat("{0}.rui", Assembly.GetName().Name);

          var path = sb.ToString();

          // Verify the .rui file exists.
          if (File.Exists(path))
          {
            try
            {
              // Delete the .rui file.
              File.Delete(path);
            }
            catch
            {
              // ignored
            }
          }

          // Save the version number of this plug-in to our settings file.
          Settings.SetString("PlugInVersion", Version);
        }
      }
    }
  }
}