from scriptcontext import doc
from Rhino.Geometry import *

text_entity = TextEntity()
text_entity.Plane = Plane.WorldXY
text_entity.Text = "Hello Rhino!"
text_entity.Justification = TextJustification.MiddleCenter
text_entity.FontIndex = doc.Fonts.FindOrCreate("Arial", False, False)

doc.Objects.AddText(text_entity)
doc.Views.Redraw()