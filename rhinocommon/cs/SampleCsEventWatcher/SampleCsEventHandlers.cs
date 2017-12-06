using System;
using System.Diagnostics;
using System.Reflection;
using Rhino;
using Rhino.Commands;
using Rhino.Display;

namespace SampleCsEventWatcher
{
  /// <summary>
  /// SampleCsEventHandlers event handling class
  /// </summary>
  internal class SampleCsEventHandlers
  {
    /// <summary>
    /// Private constructor
    /// </summary>
    private SampleCsEventHandlers()
    {
      IsEnabled = false;
    }

    /// <summary>
    /// The one and only SampleCsEventHandlers object
    /// </summary>
    private static SampleCsEventHandlers g_instance;

    /// <summary>
    /// Returns the one and only instance of this object
    /// </summary>
    public static SampleCsEventHandlers Instance => g_instance ?? (g_instance = new SampleCsEventHandlers());

    /// <summary>
    /// Returns the enabled status
    /// </summary>
    public bool IsEnabled { get; private set; }

    /// <summary>
    /// Enables or disables the event handlers
    /// </summary>
    public void Enable(bool enable)
    {
      if (enable != IsEnabled)
      {
        if (enable)
        {
          RhinoApp.AppSettingsChanged += OnAppSettingsChanged;
          RhinoApp.Closing += OnClosing;
          RhinoApp.EscapeKeyPressed += OnEscapeKeyPressed;
          RhinoApp.Idle += OnIdle;
          RhinoApp.Initialized += OnInitialized;
          RhinoApp.RendererChanged += OnRendererChanged;

          RhinoDoc.NewDocument += OnNewDocument;
          RhinoDoc.BeginOpenDocument += OnBeginOpenDocument;
          RhinoDoc.EndOpenDocument += OnEndOpenDocument;
          RhinoDoc.BeginSaveDocument += OnBeginSaveDocument;
          RhinoDoc.EndSaveDocument += OnEndSaveDocument;
          RhinoDoc.CloseDocument += OnCloseDocument;

          RhinoDoc.AddRhinoObject += OnAddRhinoObject;
          RhinoDoc.ReplaceRhinoObject += OnReplaceRhinoObject;
          RhinoDoc.DeleteRhinoObject += OnDeleteRhinoObject;
          RhinoDoc.UndeleteRhinoObject += OnUndeleteRhinoObject;
          RhinoDoc.PurgeRhinoObject += OnPurgeRhinoObject;
          RhinoDoc.ModifyObjectAttributes += OnModifyObjectAttributes;
          RhinoDoc.BeforeTransformObjects += OnBeforeTransformObjects;

          RhinoDoc.SelectObjects += OnSelectObjects;
          RhinoDoc.DeselectObjects += OnDeselectObjects;
          RhinoDoc.DeselectAllObjects += OnDeselectAllObjects;

          RhinoDoc.GroupTableEvent += OnGroupTableEvent;
          RhinoDoc.InstanceDefinitionTableEvent += OnInstanceDefinitionTableEvent;
          RhinoDoc.LayerTableEvent += OnLayerTableEvent;
          RhinoDoc.LightTableEvent += OnLightTableEvent;
          RhinoDoc.MaterialTableEvent += OnMaterialTableEvent;
          RhinoDoc.DimensionStyleTableEvent += OnDimensionStyleTableEvent;
          RhinoDoc.DocumentPropertiesChanged += OnDocumentPropertiesChanged;

          RhinoDoc.RenderEnvironmentTableEvent += OnRenderEnvironmentTableEvent;
          RhinoDoc.RenderTextureTableEvent += OnRenderTextureTableEvent;
          RhinoDoc.TextureMappingEvent += OnTextureMappingEvent;

          RhinoView.Create += OnCreateViewEventHandler;
          RhinoView.Rename += OnRenameViewEventHandler;
          RhinoView.SetActive += OnSetActiveViewEventHandler;
          RhinoView.Destroy += OnDestroyViewEventHandler;

          Command.BeginCommand += OnBeginCommand;
          Command.EndCommand += OnEndCommand;
          Command.UndoRedo += OnUndoRedo;
        }
        else
        {
          RhinoApp.AppSettingsChanged -= OnAppSettingsChanged;
          RhinoApp.Closing -= OnClosing;
          RhinoApp.EscapeKeyPressed -= OnEscapeKeyPressed;
          RhinoApp.Idle -= OnIdle;
          RhinoApp.Initialized -= OnInitialized;
          RhinoApp.RendererChanged -= OnRendererChanged;

          RhinoDoc.NewDocument -= OnNewDocument;
          RhinoDoc.BeginOpenDocument -= OnBeginOpenDocument;
          RhinoDoc.EndOpenDocument -= OnEndOpenDocument;
          RhinoDoc.BeginSaveDocument -= OnBeginSaveDocument;
          RhinoDoc.EndSaveDocument -= OnEndSaveDocument;
          RhinoDoc.CloseDocument -= OnCloseDocument;

          RhinoDoc.AddRhinoObject -= OnAddRhinoObject;
          RhinoDoc.ReplaceRhinoObject -= OnReplaceRhinoObject;
          RhinoDoc.DeleteRhinoObject -= OnDeleteRhinoObject;
          RhinoDoc.UndeleteRhinoObject -= OnUndeleteRhinoObject;
          RhinoDoc.PurgeRhinoObject -= OnPurgeRhinoObject;
          RhinoDoc.ModifyObjectAttributes -= OnModifyObjectAttributes;
          RhinoDoc.BeforeTransformObjects -= OnBeforeTransformObjects;

          RhinoDoc.SelectObjects -= OnSelectObjects;
          RhinoDoc.DeselectObjects -= OnDeselectObjects;
          RhinoDoc.DeselectAllObjects -= OnDeselectAllObjects;

          RhinoDoc.GroupTableEvent -= OnGroupTableEvent;
          RhinoDoc.InstanceDefinitionTableEvent -= OnInstanceDefinitionTableEvent;
          RhinoDoc.LayerTableEvent -= OnLayerTableEvent;
          RhinoDoc.LightTableEvent -= OnLightTableEvent;
          RhinoDoc.MaterialTableEvent -= OnMaterialTableEvent;
          RhinoDoc.DimensionStyleTableEvent -= OnDimensionStyleTableEvent;
          RhinoDoc.DocumentPropertiesChanged -= OnDocumentPropertiesChanged;

          RhinoDoc.RenderEnvironmentTableEvent -= OnRenderEnvironmentTableEvent;
          RhinoDoc.RenderTextureTableEvent -= OnRenderTextureTableEvent;
          RhinoDoc.TextureMappingEvent -= OnTextureMappingEvent;

          RhinoView.Create -= OnCreateViewEventHandler;
          RhinoView.Rename -= OnRenameViewEventHandler;
          RhinoView.SetActive -= OnSetActiveViewEventHandler;
          RhinoView.Destroy -= OnDestroyViewEventHandler;

          Command.BeginCommand -= OnBeginCommand;
          Command.EndCommand -= OnEndCommand;
          Command.UndoRedo -= OnUndoRedo;
        }
      }
      IsEnabled = enable;
    }

    #region Application Events

    /// <summary>
    /// Called when the Rhino application is initialized
    /// </summary>
    public static void OnInitialized(object sender, EventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when the Rhino application is closing
    /// </summary>
    public static void OnClosing(object sender, EventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when an Rhino application setting has changed
    /// </summary>
    public static void OnAppSettingsChanged(object sender, EventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when the active rendering plug-in has changed
    /// </summary>
    public static void OnRendererChanged(object sender, EventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when Rhino enters an idle state
    /// </summary>
    public static void OnIdle(object sender, EventArgs e)
    {
      //DebugWriteMethod();
    }

    /// <summary>
    /// Called when the escape key has been pressed
    /// </summary>
    public static void OnEscapeKeyPressed(object sender, EventArgs e)
    {
      DebugWriteMethod();
    }

    #endregion

    #region File Events

    /// <summary>
    /// Called when Rhino creates a new document
    /// </summary>
    public static void OnNewDocument(object sender, DocumentEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when Rhino begins to open a new document
    /// </summary>
    public static void OnBeginOpenDocument(object sender, DocumentOpenEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when Rhino finishes opening a new document
    /// </summary>
    public static void OnEndOpenDocument(object sender, DocumentOpenEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when Rhino begins to save the active document
    /// </summary>
    public static void OnBeginSaveDocument(object sender, DocumentSaveEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when Rhino finishes saving a document
    /// </summary>
    public static void OnEndSaveDocument(object sender, DocumentSaveEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when Rhino closes the active document
    /// </summary>
    public static void OnCloseDocument(object sender, DocumentEventArgs e)
    {
      DebugWriteMethod();
    }

    #endregion

    #region Object Events

    /// <summary>
    /// Called if a new object is added to the document.
    /// </summary>
    public static void OnAddRhinoObject(object sender, Rhino.DocObjects.RhinoObjectEventArgs e)
    {
      RhinoApp.WriteLine("> AddRhinoObject ({0})", e.ObjectId);
    }

    /// <summary>
    /// Called if an extisting object is being replaced.
    /// </summary>
    public static void OnReplaceRhinoObject(object sender, Rhino.DocObjects.RhinoReplaceObjectEventArgs e)
    {
      RhinoApp.WriteLine("> ReplaceRhinoObject ({0})", e.ObjectId);
      RhinoApp.WriteLine("     UndoActive = {0}", e.Document.UndoActive);
      RhinoApp.WriteLine("     RedoActive = {0}", e.Document.RedoActive);
    }

    /// <summary>
    /// Called when an object is deleted
    /// </summary>
    public static void OnDeleteRhinoObject(object sender, Rhino.DocObjects.RhinoObjectEventArgs e)
    {
      RhinoApp.WriteLine("> DeleteRhinoObject ({0})", e.ObjectId);
    }

    /// <summary>
    /// Called when an object is undeleted
    /// </summary>
    public static void OnUndeleteRhinoObject(object sender, Rhino.DocObjects.RhinoObjectEventArgs e)
    {
      RhinoApp.WriteLine("> UndeleteRhinoObject ({0})", e.ObjectId);
    }

    /// <summary>
    /// Called when Rhino permanently deletes an object
    /// </summary>
    public static void OnPurgeRhinoObject(object sender, Rhino.DocObjects.RhinoObjectEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when an object's attributes have been modified
    /// </summary>
    public static void OnModifyObjectAttributes(object sender, Rhino.DocObjects.RhinoModifyObjectAttributesEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called before objects are being transformed
    /// </summary>
    public static void OnBeforeTransformObjects(object sender, Rhino.DocObjects.RhinoTransformObjectsEventArgs e)
    {
      DebugWriteMethod();
    }

    #endregion

    #region Object Select Events

    /// <summary>
    /// Called when objects are selected
    /// </summary>
    public static void OnSelectObjects(object sender, Rhino.DocObjects.RhinoObjectSelectionEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when objects are deselected
    /// </summary>
    public static void OnDeselectObjects(object sender, Rhino.DocObjects.RhinoObjectSelectionEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when all objects are deselected
    /// </summary>
    public static void OnDeselectAllObjects(object sender, Rhino.DocObjects.RhinoDeselectAllObjectsEventArgs e)
    {
      DebugWriteMethod();
    }

    #endregion

    #region Document and Table Events

    /// <summary>
    /// A group table event has occurred
    /// </summary>
    public static void OnGroupTableEvent(object sender, Rhino.DocObjects.Tables.GroupTableEventArgs e)
    {
      RhinoApp.WriteLine("> GroupTableEvent");
      RhinoApp.WriteLine("    EventType = {0} **", e.EventType);
    }

    /// <summary>
    /// An instance definition table event has occurred
    /// </summary>
    public static void OnInstanceDefinitionTableEvent(object sender, Rhino.DocObjects.Tables.InstanceDefinitionTableEventArgs e)
    {
      RhinoApp.WriteLine("> InstanceDefinitionTableEvent");
      RhinoApp.WriteLine("    EventType = {0} **", e.EventType);
    }

    /// <summary>
    /// A layer table event has occurred
    /// </summary>
    public static void OnLayerTableEvent(object sender, Rhino.DocObjects.Tables.LayerTableEventArgs e)
    {
      RhinoApp.WriteLine("> LayerTableEvent");
      RhinoApp.WriteLine("    EventType = {0} **", e.EventType);
    }

    /// <summary>
    /// A light table event has occurred
    /// </summary>
    public static void OnLightTableEvent(object sender, Rhino.DocObjects.Tables.LightTableEventArgs e)
    {
      RhinoApp.WriteLine("> LightTableEvent");
      RhinoApp.WriteLine("    EventType = {0} **", e.EventType);
    }

    /// <summary>
    /// A material table event has occurred
    /// </summary>
    public static void OnMaterialTableEvent(object sender, Rhino.DocObjects.Tables.MaterialTableEventArgs e)
    {
      RhinoApp.WriteLine("> MaterialTableEvent");
      RhinoApp.WriteLine("    EventType = {0} **", e.EventType);
    }

    /// <summary>
    /// An annotation style table event has occurred
    /// </summary>
    public static void OnDimensionStyleTableEvent(object sender, Rhino.DocObjects.Tables.DimStyleTableEventArgs e)
    {
      RhinoApp.WriteLine("> DimensionStyleTableEvent");
      RhinoApp.WriteLine("    EventType = {0} **", e.EventType);
    }

    /// <summary>
    /// Called when the properties of the active document are changed
    /// </summary>
    public static void OnDocumentPropertiesChanged(object sender, DocumentEventArgs e)
    {
      DebugWriteMethod();
    }

    #endregion

    #region Render Events

    /// <summary>
    /// A render environemnt table event has occurred
    /// </summary>
    public static void OnRenderEnvironmentTableEvent(object sender, RhinoDoc.RenderContentTableEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// A render texture table event has occurred
    /// </summary>
    public static void OnRenderTextureTableEvent(object sender, RhinoDoc.RenderContentTableEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// A texture mapping event has occurred
    /// </summary>
    public static void OnTextureMappingEvent(object sender, RhinoDoc.TextureMappingEventArgs e)
    {
      DebugWriteMethod();
    }

    #endregion

    #region View Events

    /// <summary>
    /// A view was created
    /// </summary>
    public static void OnCreateViewEventHandler(object sender, ViewEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// A view was renamed
    /// </summary>
    public static void OnRenameViewEventHandler(object sender, ViewEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// A view was make active
    /// </summary>
    public static void OnSetActiveViewEventHandler(object sender, ViewEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// A view was destroyed
    /// </summary>
    public static void OnDestroyViewEventHandler(object sender, ViewEventArgs e)
    {
      DebugWriteMethod();
    }

    #endregion

    #region Command Events

    /// <summary>
    /// Called when a command begins
    /// </summary>
    public static void OnBeginCommand(object sender, CommandEventArgs e)
    {
      RhinoApp.WriteLine("> BeginCommand ({0})", e.CommandEnglishName);
    }

    /// <summary>
    /// Called when a command ends
    /// </summary>
    public static void OnEndCommand(object sender, CommandEventArgs e)
    {
      RhinoApp.WriteLine("> EndCommand ({0})", e.CommandEnglishName);
    }

    /// <summary>
    /// Called when a command is undone/redone
    /// </summary>
    public static void OnUndoRedo(object sender, UndoRedoEventArgs e)
    {
      DebugWriteMethod();

      //RhinoApp.WriteLine("> Undo/Redo");

      //string event_type = "None";
      //if (e.IsBeginRecording)
      //  event_type = "Begin Recording";
      //else if (e.IsEndRecording)
      //  event_type = "End Recording";
      //else if (e.IsBeginUndo)
      //  event_type = "Begin Undo";
      //else if (e.IsEndUndo)
      //  event_type = "End Undo";
      //else if (e.IsBeginRedo)
      //  event_type = "Begin Redo";
      //else if (e.IsEndRedo)
      //  event_type = "End Redo";
      //else if (e.IsPurgeRecord)
      //  event_type = "Purge Record";

      //RhinoApp.WriteLine("    EventType = {0}", event_type);
    }

    #endregion

    private static void DebugWriteMethod()
    {
      //try
      //{
      //  var method_name = new StackTrace().GetFrame(1).GetMethod().Name;
      //  RhinoApp.WriteLine("> {0}", method_name);
      //}
      //catch
      //{
      //  // ignored
      //}
    }
  }
}
