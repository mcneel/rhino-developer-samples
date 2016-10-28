Imports System
Imports Rhino
Imports Rhino.Commands

Namespace examples_vb

    <System.Runtime.InteropServices.Guid("5d9f0591-2974-4cc7-9f10-9d41730927e4")> _
    Public Class vb_testcommand
        Inherits Command

        Shared _instance As vb_testcommand 

        Public Sub New()
            ' Rhino only creates one instance of each command class defined in a
            ' plug-in, so it is safe to store a refence in a static field.
            _instance = Me
        End Sub

        '''<summary>The only instance of this command.</summary>
        Public Shared ReadOnly Property Instance() As vb_testcommand
            Get
                Return _instance
            End Get
        End Property

        '''<returns>The command name as it appears on the Rhino command line.</returns>
        Public Overrides ReadOnly Property EnglishName() As String
            Get
                Return "vb_testcommand"
            End Get
        End Property

        Protected Overrides Function RunCommand(ByVal doc As RhinoDoc, ByVal mode As RunMode) As Result

            ' TODO: complete command.
            Return Result.Success

        End Function
    End Class
End Namespace