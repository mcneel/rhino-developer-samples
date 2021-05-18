using Rhino.PlugIns;
using Rhino.UI.Controls;

namespace CommandPlugin
{
    ///<summary>
    /// <para>Every RhinoCommon .rhp assembly must have one and only one PlugIn-derived
    /// class. DO NOT create instances of this class yourself. It is the
    /// responsibility of Rhino to create an instance of this class.</para>
    /// <para>To complete plug-in information, please also see all PlugInDescription
    /// attributes in AssemblyInfo.cs (you might need to click "Project" ->
    /// "Show All Files" to see it in the "Solution Explorer" window).</para>
    ///</summary>
    public class CommandPluginPlugIn : Rhino.PlugIns.PlugIn

    {
        public CommandPluginPlugIn()
        {
            Instance = this;
        }

        ///<summary>Gets the only instance of the CommandPluginPlugIn plug-in.</summary>
        public static CommandPluginPlugIn Instance
        {
            get; private set;
        }

        // You can override methods here to change the plug-in behavior on
        // loading and shut down, add options pages to the Rhino _Option command
        // and maintain plug-in wide options in a document.
        protected override LoadReturnCode OnLoad(ref string errorMessage)
        {
            var result = base.OnLoad(ref errorMessage);
            if (result == LoadReturnCode.Success)
            {
                // Create a red icon for the command
                System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(16, 16, System.Drawing.Imaging.PixelFormat.Format24bppRgb);
                using (var g = System.Drawing.Graphics.FromImage(bitmap))
                {
                    g.Clear(System.Drawing.Color.Red);
                }

                // Add a UI command
                RenderContentMenu.AddMenuItem(this.Id, "Test UI Command", 5, RenderContentMenu.SeparatorStyle.None, true, System.Drawing.Icon.FromHandle(bitmap.GetHicon()), CommandAction, CommandEnabled);
            }
            return result;
        }

        // Function that will be called when the command is executed
        private Rhino.Commands.Result CommandAction(Rhino.Render.RenderContentCollection collection)
        {

            if (collection.Count() > 0)
            {
                Rhino.Render.ContentCollectionIterator iterator = collection.Iterator();
                Rhino.Render.RenderContent content = iterator.First();

                if (content != null)
                {
                    Eto.Forms.MessageBox.Show(content.Xml, "C#", Eto.Forms.MessageBoxType.Information);

                    iterator.DeleteThis();
                    return Rhino.Commands.Result.Success;
                }

                iterator.DeleteThis();
            }

            return Rhino.Commands.Result.Failure;
        }

        // Function that will be called to check if the command should be visible.
        private bool CommandEnabled(Rhino.Render.RenderContentCollection collection, RenderContentMenu.Context control)
        {
            // Unknown control
            if (control == RenderContentMenu.Context.Unknown)
                return true;
            // The main content editor thumbnail list.
            else if (control == RenderContentMenu.Context.MainThumbnailList)
                return true;
            // The main content editor tree control.
            else if (control == RenderContentMenu.Context.MainTree)
                return false;
            // The content editor preview thumbnail list.
            else if (control == RenderContentMenu.Context.EditorPreview)
                return true;
            // A sub-node control.
            else if (control == RenderContentMenu.Context.SubNodeControl)
                return true;
            //  A 'Create New' button - AKA [+] button.
            else if (control == RenderContentMenu.Context.ColorButton)
                return true;
            // An old-style content control. 
            else if (control == RenderContentMenu.Context.CreateNewButton)
                return true;
            //  A new-style content control.
            else if (control == RenderContentMenu.Context.ContentControl)
                return true;
            //  A new-style content control's drop-down thumbnail list.
            else if (control == RenderContentMenu.Context.NewContentControl)
                return true;
            // A breadcrumb control.
            else if (control == RenderContentMenu.Context.NewContentControlDropDown)
                return true;
            // A floating preview. 
            else if (control == RenderContentMenu.Context.BreadcrumbControl)
                return true;
            // Spanner menu.
            else if (control == RenderContentMenu.Context.FloatingPreview)
                return true;
            // Spanner menu in modal editor.
            else if (control == RenderContentMenu.Context.Spanner)
                return true;
            // Content type section. 
            else if (control == RenderContentMenu.Context.SpannerModal)
                return true;
            // Content type browser 'new' page. 
            else if (control == RenderContentMenu.Context.ContentTypeSection)
                return true;
            // Content type browser 'existing' page.
            else if (control == RenderContentMenu.Context.ContentTypeBrowserNew)
                return true;
            // Content instance browser.
            else if (control == RenderContentMenu.Context.ContentTypeBrowserExisting)
                return true;
            // Tool-tip preview.
            else if (control == RenderContentMenu.Context.ContentInstanceBrowser)
                return true;
            // Spanner menu.
            else if (control == RenderContentMenu.Context.ToolTipPreview)
                return true;
            else
                return true;
        }
    }
}