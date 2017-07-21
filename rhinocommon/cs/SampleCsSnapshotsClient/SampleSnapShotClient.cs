using System;
using Rhino;
using Rhino.DocObjects;
using Rhino.FileIO;
using Rhino.Geometry;

namespace SampleCsSnapshotsClient
{
  class SampleSnapShotClient : Rhino.DocObjects.SnapShots.SnapShotsClient
  {

    public SampleSnapShotClient()
    {

    }

    public override bool AnimateDocument(RhinoDoc doc, double dPos, BinaryArchiveReader archive_start, BinaryArchiveReader archive_stop)
    {
      return false;
    }

    public override bool AnimateObject(RhinoDoc doc, RhinoObject doc_object, ref Transform transform, double dPos, BinaryArchiveReader archive_start, BinaryArchiveReader archive_stop)
    {
      return false;
    }

    public override void AnimationStart(RhinoDoc doc, int iFrames)
    {
     
    }

    public override bool AnimationStop(RhinoDoc doc)
    {
      return false;
    }

    public override string Category()
    {
      return Rhino.DocObjects.SnapShots.SnapShotsClient.RenderingCategory();
    }

    public override Guid ClientId()
    {
      return new Guid("EAE3C9CF-5FBE-4311-88AF-A1B0C6CF96B2");
    }

    public override void ExtendBoundingBoxForDocumentAnimation(RhinoDoc doc, BinaryArchiveReader archive_start, BinaryArchiveReader archive_stop, ref BoundingBox bbox)
    {
      
    }

    public override void ExtendBoundingBoxForObjectAnimation(RhinoDoc doc, RhinoObject doc_object, ref Transform transform, BinaryArchiveReader archive_start, BinaryArchiveReader archive_stop, ref BoundingBox bbox)
    {
      
    }

    public override string Name()
    {
      return "Name";
    }

    public override bool ObjectTransformNotification(RhinoDoc doc, RhinoObject doc_object, ref Transform transform, BinaryArchiveReader archive)
    {
      return false;
    }

    public override Guid PlugInId()
    {
      return new Guid("888804b9-6529-4805-9e56-4101697c4664");
    }

    public override bool PrepareForDocumentAnimation(RhinoDoc doc, BinaryArchiveReader archive_start, BinaryArchiveReader archive_stop)
    {
      return false;
    }

    public override bool PrepareForObjectAnimation(RhinoDoc doc, RhinoObject doc_object, ref Transform transform, BinaryArchiveReader archive_start, BinaryArchiveReader archive_stop)
    {
      return false;
    }

    public override bool RestoreDocument(RhinoDoc doc, BinaryArchiveReader archive)
    {
      return false;
    }

    public override bool CompareDocument(RhinoDoc doc, BinaryArchiveReader archive_a, BinaryArchiveReader archive_b)
    {
      throw new NotImplementedException();
    }

    public override bool RestoreObject(RhinoDoc doc, RhinoObject doc_object, ref Transform transform, BinaryArchiveReader archive)
    {

      Rhino.Collections.ArchivableDictionary userdata = archive.ReadDictionary();

      string name = "";
      if (userdata.TryGetString("ObjName", out name))
      {
        doc_object.Attributes.Name = name;
        doc_object.CommitChanges();
      }
      return !archive.ReadErrorOccured;
    }

    public override bool CompareObject(RhinoDoc doc, RhinoObject doc_object, BinaryArchiveReader archive_a, BinaryArchiveReader archive_b)
    {
      throw new NotImplementedException();
    }

    public override bool SaveDocument(RhinoDoc doc, BinaryArchiveWriter archive)
    {
      return false;
    }

    public override bool SaveObject(RhinoDoc doc, RhinoObject doc_object, ref Transform transform, BinaryArchiveWriter archive)
    {
      string name = doc_object.Name;

      Rhino.Collections.ArchivableDictionary userdata = doc_object.UserDictionary;
      userdata.Set("ObjName", name);

      archive.WriteDictionary(userdata);

      return !archive.WriteErrorOccured;
    }

    public override void SnapshotRestored(RhinoDoc doc)
    {

    }

    public override bool SupportsAnimation()
    {
      return false;
    }

    public override bool SupportsDocument()
    {
      return false;
    }

    public override bool SupportsObject(RhinoObject doc_object)
    {
      return true;
    }

    public override bool SupportsObjects()
    {
      return true;
    }
  }
}
