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
    public static SampleCsEventHandlers Instance
    {
      get { return g_instance ?? (g_instance = new SampleCsEventHandlers()); }
    }

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
    void OnAddRhinoObject(object sender, Rhino.DocObjects.RhinoObjectEventArgs e)
    {
      DebugWriteMethod();
    }

    void OnReplaceRhinoObject(object sender, Rhino.DocObjects.RhinoReplaceObjectEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when an object is deleted
    /// </summary>
    void OnDeleteRhinoObject(object sender, Rhino.DocObjects.RhinoObjectEventArgs e)
    {
      DebugWriteMethod();
    }

    void OnUndeleteRhinoObject(object sender, Rhino.DocObjects.RhinoObjectEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when Rhino permanently deletes an object
    /// </summary>
    void OnPurgeRhinoObject(object sender, Rhino.DocObjects.RhinoObjectEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when an object's attributes have been modified
    /// </summary>
    void OnModifyObjectAttributes(object sender, Rhino.DocObjects.RhinoModifyObjectAttributesEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called before objects are being transformed
    /// </summary>
    void OnBeforeTransformObjects(object sender, Rhino.DocObjects.RhinoTransformObjectsEventArgs e)
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
    void OnGroupTableEvent(object sender, Rhino.DocObjects.Tables.GroupTableEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// An instance definition table event has occurred
    /// </summary>
    void OnInstanceDefinitionTableEvent(object sender, Rhino.DocObjects.Tables.InstanceDefinitionTableEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// A layer table event has occurred
    /// </summary>
    void OnLayerTableEvent(object sender, Rhino.DocObjects.Tables.LayerTableEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// A light table event has occurred
    /// </summary>
    void OnLightTableEvent(object sender, Rhino.DocObjects.Tables.LightTableEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// A material table event has occurred
    /// </summary>
    void OnMaterialTableEvent(object sender, Rhino.DocObjects.Tables.MaterialTableEventArgs e)
    {
      DebugWriteMethod();
    }

    /// <summary>
    /// Called when the properties of the active document are changed
    /// </summary>
    void OnDocumentPropertiesChanged(object sender, DocumentEventArgs e)
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

    public static void OnBeginCommand(object sender, CommandEventArgs e)
    {
      DebugWriteMethod();
    }

    public static void OnEndCommand(object sender, CommandEventArgs e)
    {
      DebugWriteMethod();
    }

    public static void OnUndoRedo(object sender, UndoRedoEventArgs e)
    {
      DebugWriteMethod();
    }

    #endregion

    private static void DebugWriteMethod()
    {
#if DEBUG
      var class_name = MethodBase.GetCurrentMethod().DeclaringType.Name;
      var method_name = new StackTrace().GetFrame(1).GetMethod().Name;
      RhinoApp.WriteLine("** EVENT: {0}.{1} **", class_name, method_name);
#endif
    }
  }
}
