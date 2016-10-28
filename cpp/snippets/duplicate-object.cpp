const CRhinoObject* object = ..... // some object
CRhinoObject* duplicate = object->Duplicate();
if( duplicate )
{
  if( context.m_doc.AddObject(duplicate) )
    context.m_doc.Redraw;
  else
    delete duplicate;
}
