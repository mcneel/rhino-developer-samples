using System;
using System.Collections.Generic;
using Rhino.FileIO;

namespace SampleCsUserData
{
  /// <summary>
  /// SampleCsSimpleDocumentData
  /// </summary>
  public class SampleCsSimpleDocumentData
  {
    /// <summary>
    /// Class major and minor verson numbers
    /// </summary>
    private const int MAJOR = 1;
    private const int MINOR = 0;

    /// <summary>
    /// Public constructor
    /// </summary>
    public SampleCsSimpleDocumentData()
    {
      Value = new Random().Next();
    }

    /// <summary>
    /// Return our data
    /// </summary>
    public int Value { get; set; }

    /// <summary>
    /// Write to binary archive
    /// </summary>
    public bool Write(BinaryArchiveWriter archive)
    {
      var rc = false;
      if (null != archive)
      {
        try
        {
          archive.Write3dmChunkVersion(MAJOR, MINOR);
          archive.WriteInt(Value);
          rc = archive.WriteErrorOccured;
        }
        catch
        {
          // ignored
        }
      }
      return rc;
    }

    /// <summary>
    /// Read from binary archive
    /// </summary>
    public bool Read(BinaryArchiveReader archive)
    {
      var rc = false;
      if (null != archive)
      {
        try
        {
          int major, minor;
          archive.Read3dmChunkVersion(out major, out minor);
          if (major == MAJOR && minor == MINOR)
          {
            Value = archive.ReadInt();
            rc = archive.ReadErrorOccured;
          }
        }
        catch
        {
          // ignored
        }
      }
      return rc;
    }
  }

  /// <summary>
  /// SampleCsSimpleDocumentDataTable
  /// </summary>
  public class SampleCsSimpleDocumentDataTable : List<SampleCsSimpleDocumentData>
  {
    /// <summary>
    /// Class major and minor verson numbers
    /// </summary>
    private const int MAJOR = 1;
    private const int MINOR = 0;

    /// <summary>
    /// Write to binary archive
    /// </summary>
    public bool WriteDocument(BinaryArchiveWriter archive)
    {
      var rc = false;
      if (null != archive)
      {
        try
        {
          archive.Write3dmChunkVersion(MAJOR, MINOR);
          archive.WriteInt(Count);
          for (var i = 0; i < Count; i++)
            this[i].Write(archive);
          rc = archive.WriteErrorOccured;
        }
        catch
        {
          // ignored
        }
      }
      return rc;
    }

    /// <summary>
    /// Read from binary archive
    /// </summary>
    public bool ReadDocument(BinaryArchiveReader archive)
    {
      var rc = false;
      if (null != archive)
      {
        try
        {
          int major, minor;
          archive.Read3dmChunkVersion(out major, out minor);
          if (major == MAJOR && minor == MINOR)
          {
            var count = archive.ReadInt();
            for (var i = 0; i < count; i++)
            {
              var data = new SampleCsSimpleDocumentData();
              if (data.Read(archive))
                Add(data);
            }
            rc = archive.ReadErrorOccured;
          }
        }
        catch
        {
          // ignored
        }
      }
      return rc;
    }
  }
}
