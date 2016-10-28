Partial Friend Class Examples
  Public Shared Function AddNestedBlock(ByVal doc As RhinoDoc) As Rhino.Commands.Result
	Dim circle = New Circle(Point3d.Origin, 5)
	Dim curveList() As Curve = { New ArcCurve(circle) }
	Dim circleIndex = doc.InstanceDefinitions.Add("Circle", "Circle with radius of 5", Point3d.Origin, curveList)
	Dim transform = Transform.Identity
	Dim irefId = doc.InstanceDefinitions(circleIndex).Id
	Dim iref = New InstanceReferenceGeometry(irefId, transform)
	circle.Radius = circle.Radius * 2.0
	Dim blockList() As GeometryBase = { iref, New ArcCurve(circle) }
	Dim circle2Index = doc.InstanceDefinitions.Add("TwoCircles", "Nested block test", Point3d.Origin, blockList)
	doc.Objects.AddInstanceObject(circle2Index, transform)
	doc.Views.Redraw()
	Return Rhino.Commands.Result.Success
  End Function
End Class
