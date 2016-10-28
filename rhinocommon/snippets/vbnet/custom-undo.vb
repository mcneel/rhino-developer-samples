Partial Friend Class Examples
  Private Shared MyFavoriteNumber As Double = 0

  Public Shared Function CustomUndo(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	' Rhino automatically sets up an undo record when a command is run,
	' but... the undo record is not saved if nothing changes in the
	' document (objects added/deleted, layers changed,...)
	'
	' If we have a command that doesn't change things in the document,
	' but we want to have our own custom undo called then we need to do
	' a little extra work

	Dim d As Double = MyFavoriteNumber
	If Rhino.Input.RhinoGet.GetNumber("Favorite number", True, d) = Rhino.Commands.Result.Success Then
	  Dim current_value As Double = MyFavoriteNumber
	  doc.AddCustomUndoEvent("Favorite Number", AddressOf OnUndoFavoriteNumber, current_value)
	  MyFavoriteNumber = d
	End If
	Return Rhino.Commands.Result.Success
  End Function

  ' event handler for custom undo
  Private Shared Sub OnUndoFavoriteNumber(ByVal sender As Object, ByVal e As Rhino.Commands.CustomUndoEventArgs)
	' !!!!!!!!!!
	' NEVER change any setting in the Rhino document or application.  Rhino
	' handles ALL changes to the application and document and you will break
	' the Undo/Redo commands if you make any changes to the application or
	' document. This is meant only for your own private plugin data
	' !!!!!!!!!!

	' This function can be called either by undo or redo
	' In order to get redo to work, add another custom undo event with the
	' current value.  If you don't want redo to work, just skip adding
	' a custom undo event here
	Dim current_value As Double = MyFavoriteNumber
	e.Document.AddCustomUndoEvent("Favorite Number", AddressOf OnUndoFavoriteNumber, current_value)

	Dim old_value As Double = CDbl(e.Tag)
	RhinoApp.WriteLine("Going back to your favorite = {0}", old_value)
	MyFavoriteNumber = old_value
  End Sub
End Class
