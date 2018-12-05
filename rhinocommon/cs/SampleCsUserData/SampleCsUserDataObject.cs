using Rhino.DocObjects.Custom;
using Rhino.FileIO;

namespace SampleCsUserData
{
  /// <summary>
  /// Sample user data object that can be attached to geometry or object attributes.
  /// </summary>
  [System.Runtime.InteropServices.Guid("3A84A0E6-3EB8-4737-AE2F-4C340A2A11CD")]
  public class SampleCsUserDataObject : UserData
  {
    #region Private constants

    /// <summary>
    /// The major and minor version number of this data.
    /// </summary>
    private const int MAJOR_VERSION = 1;
    private const int MINOR_VERSION = 0;

    #endregion

    #region Public properties

    /// <summary>
    /// The notes field
    /// </summary>
    public string Notes { get; set; }

    /// <summary>
    /// Returns true of our user data is valid.
    /// </summary>
    public bool IsValid => !string.IsNullOrEmpty(Notes);

    #endregion

    #region Userdata overrides

    /// <summary>
    /// Descriptive name of the user data.
    /// </summary>
    public override string Description => "SampleCsUserData";

    public override string ToString()
    {
      return Description;
    }

    /// <summary>
    /// Is called when the object is being duplicated.
    /// </summary>
    protected override void OnDuplicate(UserData source)
    {
      if (source is SampleCsUserDataObject src)
      {
        Notes = src.Notes;
      }
    }

    /// <summary>
    /// If you want to save this user data in a 3dm file, override ShouldWrite and return
    /// true. If you do support serialization, you must also override the Read and Write
    /// functions.
    /// </summary>
    public override bool ShouldWrite => IsValid;

    /// <summary>
    /// Reads the content of this data from a stream archive.
    /// </summary>
    protected override bool Read(BinaryArchiveReader archive)
    {
      // Read the chuck version
      archive.Read3dmChunkVersion(out var major, out var minor);
      if (major == MAJOR_VERSION)
      {
        // Read 1.0 fields  here
        if (minor >= MINOR_VERSION)
        {
          Notes = archive.ReadString();
        }

        // Note, if you every roll the minor version number,
        // then read those fields here.
      }

      return !archive.ReadErrorOccured;
    }

    /// <summary>
    /// Writes the content of this data to a stream archive.
    /// </summary>
    protected override bool Write(BinaryArchiveWriter archive)
    {
      // Write the chuck version
      archive.Write3dmChunkVersion(MAJOR_VERSION, MINOR_VERSION);

      // Write 1.0 fields
      archive.WriteString(Notes);

      // Note, if you every roll the minor version number,
      // then write those fields here.

      return !archive.WriteErrorOccured;
    }

    #endregion
  }
}
