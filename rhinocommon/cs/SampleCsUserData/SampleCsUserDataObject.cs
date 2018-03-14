using Rhino.DocObjects.Custom;

namespace SampleCsUserData
{
  /// <summary>
  /// Simple user data object that can be attached to objects or object attributes.
  /// </summary>
  [System.Runtime.InteropServices.Guid("3A84A0E6-3EB8-4737-AE2F-4C340A2A11CD")]
  public class SampleCsUserDataObject : UserData
  {
    public string Notes
    {
      get;
      set;
    }

    public bool IsValid => !string.IsNullOrEmpty(Notes);

    public override string Description => "SampleCsUserData";

    public override string ToString()
    {
      return Description;
    }

    protected override void OnDuplicate(UserData source)
    {
      if (source is SampleCsUserDataObject src)
      {
        Notes = src.Notes;
      }
    }

    public override bool ShouldWrite => IsValid;

    protected override bool Read(Rhino.FileIO.BinaryArchiveReader archive)
    {
      archive.Read3dmChunkVersion(out var major, out var minor);
      if (1 == major && 0 == minor)
        Notes = archive.ReadString();
      return !archive.ReadErrorOccured;
    }

    protected override bool Write(Rhino.FileIO.BinaryArchiveWriter archive)
    {
      archive.Write3dmChunkVersion(1, 0);
      archive.WriteString(Notes);
      return !archive.WriteErrorOccured;
    }
  }
}
