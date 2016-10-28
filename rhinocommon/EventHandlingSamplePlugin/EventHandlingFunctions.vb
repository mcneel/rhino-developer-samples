Imports System
Imports System.Collections.Generic
Imports Rhino
Imports Rhino.Commands
Imports Rhino.Geometry
Imports Rhino.Input
Imports Rhino.Input.Custom

Namespace EventHandling

#Region "Event handling functions"
    Class MyEvents
        'List all event handling functions

        Public Shared Sub AddObject_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.RhinoObjectEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("AddObject event")
        End Sub

        Public Shared Sub BeginOpenDocument_Handler(ByVal sender As Object, ByVal args As Rhino.DocumentEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("BeginOpenDocument event")
        End Sub

        Public Shared Sub BeginSaveDocument_Handler(ByVal sender As Object, ByVal args As Rhino.DocumentEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("BeginSaveDocument event")
        End Sub

        Public Shared Sub CloseDocument_Handler(ByVal sender As Object, ByVal args As Rhino.DocumentEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("CloseDocument event")
        End Sub

        Public Shared Sub DeleteRhinoObject_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.RhinoObjectEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("DeleteRhinoObject event")
        End Sub

        Public Shared Sub DeselectAllObjects_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.RhinoDeselectAllObjectsEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("DeselectAllObjects event")
        End Sub

        Public Shared Sub DeselectObjects_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.RhinoObjectSelectionEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("DeselectObjects event")
        End Sub

        Public Shared Sub DocumentPropertiesChanged_Handler(ByVal sender As Object, ByVal args As Rhino.DocumentEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("DocumentPropertiesChanged event")
        End Sub

        Public Shared Sub EndOpenDocument_Handler(ByVal sender As Object, ByVal args As Rhino.DocumentEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("EndOpenDocument event")
        End Sub

        Public Shared Sub EndSaveDocument_Handler(ByVal sender As Object, ByVal args As Rhino.DocumentEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("EndSaveDocument event")
        End Sub

        Public Shared Sub GroupTableEvent_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.Tables.GroupTableEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("GroupTable event")
        End Sub

        Public Shared Sub LayerTableEvent_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.Tables.LayerTableEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("LayerTable event")
        End Sub

        Public Shared Sub MaterialTableEvent_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.Tables.MaterialTableEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("MaterialTable event")
        End Sub

        Public Shared Sub ModifyObjectAttributesHandler(ByVal sender As Object, ByVal args As Rhino.DocObjects.RhinoModifyObjectAttributesEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("ModifyObjectAttributes event")
        End Sub

        Public Shared Sub NewDocument_Handler(ByVal sender As Object, ByVal args As Rhino.DocumentEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("NewDocument event")
        End Sub

        Public Shared Sub PurgeRhinoObject_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.RhinoObjectEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("PurgeRhinoObject event")
        End Sub

        Public Shared Sub ReplaceRhinoObject_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.RhinoReplaceObjectEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("ReplaceRhinoObject event")
        End Sub

        Public Shared Sub SelectObjects_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.RhinoObjectSelectionEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("SelectObjects event")
        End Sub

        Public Shared Sub UndeleteRhinoObject_Handler(ByVal sender As Object, ByVal args As Rhino.DocObjects.RhinoObjectEventArgs)
            'Add event handling code
            Rhino.RhinoApp.WriteLine("UndeleteRhinoObject event")
        End Sub
    End Class
#End Region

End Namespace