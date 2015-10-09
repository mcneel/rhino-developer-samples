Imports System
Imports Rhino
Imports Rhino.Commands

Namespace EventHandling

    <System.Runtime.InteropServices.Guid("d0d2b371-90d3-4b31-9287-8ee3cbc3f002")> _
    Public Class AddEventsCommand
        Inherits Command

        Shared _instance As AddEventsCommand 

        Public Sub New()
            ' Rhino only creates one instance of each command class defined in a
            ' plug-in, so it is safe to store a refence in a static field.
            _instance = Me
        End Sub

        '''<summary>The only instance of this command.</summary>
        Public Shared ReadOnly Property Instance() As AddEventsCommand
            Get
                Return _instance
            End Get
        End Property

        '''<returns>The command name as it appears on the Rhino command line.</returns>
        Public Overrides ReadOnly Property EnglishName() As String
            Get
                Return "AddEventsCommand"
            End Get
        End Property

        Protected Overrides Function RunCommand(ByVal doc As RhinoDoc, ByVal mode As RunMode) As Result

            'Add event handlers when run command
            AddHandler Rhino.RhinoDoc.AddRhinoObject, AddressOf MyEvents.AddObject_Handler
            AddHandler Rhino.RhinoDoc.BeginOpenDocument, AddressOf MyEvents.BeginOpenDocument_Handler
            AddHandler Rhino.RhinoDoc.BeginSaveDocument, AddressOf MyEvents.BeginSaveDocument_Handler
            AddHandler Rhino.RhinoDoc.CloseDocument, AddressOf MyEvents.CloseDocument_Handler
            AddHandler Rhino.RhinoDoc.DeleteRhinoObject, AddressOf MyEvents.DeleteRhinoObject_Handler
            AddHandler Rhino.RhinoDoc.DeselectAllObjects, AddressOf MyEvents.DeselectAllObjects_Handler
            AddHandler Rhino.RhinoDoc.DeselectObjects, AddressOf MyEvents.DeselectObjects_Handler
            AddHandler Rhino.RhinoDoc.DocumentPropertiesChanged, AddressOf MyEvents.DocumentPropertiesChanged_Handler
            AddHandler Rhino.RhinoDoc.EndOpenDocument, AddressOf MyEvents.EndOpenDocument_Handler
            AddHandler Rhino.RhinoDoc.EndSaveDocument, AddressOf MyEvents.EndSaveDocument_Handler
            AddHandler Rhino.RhinoDoc.GroupTableEvent, AddressOf MyEvents.GroupTableEvent_Handler
            AddHandler Rhino.RhinoDoc.LayerTableEvent, AddressOf MyEvents.LayerTableEvent_Handler
            AddHandler Rhino.RhinoDoc.MaterialTableEvent, AddressOf MyEvents.MaterialTableEvent_Handler
            AddHandler Rhino.RhinoDoc.ModifyObjectAttributes, AddressOf MyEvents.ModifyObjectAttributesHandler
            AddHandler Rhino.RhinoDoc.NewDocument, AddressOf MyEvents.NewDocument_Handler
            AddHandler Rhino.RhinoDoc.PurgeRhinoObject, AddressOf MyEvents.PurgeRhinoObject_Handler
            AddHandler Rhino.RhinoDoc.ReplaceRhinoObject, AddressOf MyEvents.ReplaceRhinoObject_Handler
            AddHandler Rhino.RhinoDoc.SelectObjects, AddressOf MyEvents.SelectObjects_Handler
            AddHandler Rhino.RhinoDoc.UndeleteRhinoObject, AddressOf MyEvents.UndeleteRhinoObject_Handler

            Return Result.Success

        End Function
    End Class
End Namespace