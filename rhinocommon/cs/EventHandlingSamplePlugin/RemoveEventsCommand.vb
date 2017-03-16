Imports System
Imports Rhino
Imports Rhino.Commands

Namespace EventHandling

    <System.Runtime.InteropServices.Guid("c4113551-8bba-41a1-8667-9eda110b57b4")> _
    Public Class RemoveEventsCommand
        Inherits Command

        Shared _instance As RemoveEventsCommand 

        Public Sub New()
            ' Rhino only creates one instance of each command class defined in a
            ' plug-in, so it is safe to store a refence in a static field.
            _instance = Me
        End Sub

        '''<summary>The only instance of this command.</summary>
        Public Shared ReadOnly Property Instance() As RemoveEventsCommand
            Get
                Return _instance
            End Get
        End Property

        '''<returns>The command name as it appears on the Rhino command line.</returns>
        Public Overrides ReadOnly Property EnglishName() As String
            Get
                Return "RemoveEventsCommand"
            End Get
        End Property

        Protected Overrides Function RunCommand(ByVal doc As RhinoDoc, ByVal mode As RunMode) As Result

            'Remove event handlers when run command
            RemoveHandler Rhino.RhinoDoc.AddRhinoObject, AddressOf MyEvents.AddObject_Handler
            RemoveHandler Rhino.RhinoDoc.BeginOpenDocument, AddressOf MyEvents.BeginOpenDocument_Handler
            RemoveHandler Rhino.RhinoDoc.BeginSaveDocument, AddressOf MyEvents.BeginSaveDocument_Handler
            RemoveHandler Rhino.RhinoDoc.CloseDocument, AddressOf MyEvents.CloseDocument_Handler
            RemoveHandler Rhino.RhinoDoc.DeleteRhinoObject, AddressOf MyEvents.DeleteRhinoObject_Handler
            RemoveHandler Rhino.RhinoDoc.DeselectAllObjects, AddressOf MyEvents.DeselectAllObjects_Handler
            RemoveHandler Rhino.RhinoDoc.DeselectObjects, AddressOf MyEvents.DeselectObjects_Handler
            RemoveHandler Rhino.RhinoDoc.DocumentPropertiesChanged, AddressOf MyEvents.DocumentPropertiesChanged_Handler
            RemoveHandler Rhino.RhinoDoc.EndOpenDocument, AddressOf MyEvents.EndOpenDocument_Handler
            RemoveHandler Rhino.RhinoDoc.EndSaveDocument, AddressOf MyEvents.EndSaveDocument_Handler
            RemoveHandler Rhino.RhinoDoc.GroupTableEvent, AddressOf MyEvents.GroupTableEvent_Handler
            RemoveHandler Rhino.RhinoDoc.LayerTableEvent, AddressOf MyEvents.LayerTableEvent_Handler
            RemoveHandler Rhino.RhinoDoc.MaterialTableEvent, AddressOf MyEvents.MaterialTableEvent_Handler
            RemoveHandler Rhino.RhinoDoc.ModifyObjectAttributes, AddressOf MyEvents.ModifyObjectAttributesHandler
            RemoveHandler Rhino.RhinoDoc.NewDocument, AddressOf MyEvents.NewDocument_Handler
            RemoveHandler Rhino.RhinoDoc.PurgeRhinoObject, AddressOf MyEvents.PurgeRhinoObject_Handler
            RemoveHandler Rhino.RhinoDoc.ReplaceRhinoObject, AddressOf MyEvents.ReplaceRhinoObject_Handler
            RemoveHandler Rhino.RhinoDoc.SelectObjects, AddressOf MyEvents.SelectObjects_Handler
            RemoveHandler Rhino.RhinoDoc.UndeleteRhinoObject, AddressOf MyEvents.UndeleteRhinoObject_Handler
            Return Result.Success

        End Function
    End Class
End Namespace