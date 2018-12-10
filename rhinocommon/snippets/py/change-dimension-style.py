from Rhino import *
from Rhino.DocObjects import *
from Rhino.Commands import *
from Rhino.Geometry import *
from scriptcontext import doc

def RunCommand():
    for annotation_object in doc.Objects.GetObjectList(ObjectType.Annotation):
        if not isinstance (annotation_object, AnnotationObjectBase):
            continue

        annotation = annotation_object.Geometry

        if annotation.Index == doc.DimStyles.CurrentDimensionStyleIndex:
            continue

        annotation.Index = doc.DimStyles.CurrentDimensionStyleIndex
        annotation_object.CommitChanges()

    doc.Views.Redraw()
    return Result.Success

if __name__ == "__main__":
    RunCommand()
