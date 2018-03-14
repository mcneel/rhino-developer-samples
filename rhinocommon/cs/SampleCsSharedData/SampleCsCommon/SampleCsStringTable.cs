using System.Collections.Generic;
using System.Runtime.CompilerServices;
using Rhino;
using Rhino.FileIO;

[assembly: InternalsVisibleTo("SampleCsMain")]
namespace SampleCsCommon
{
  internal class SampleCsStringTable
  {
    #region Private Data Members
    private List<string> m_strings;
    private const int MAJOR = 1;
    private const int MINOR = 0;
    #endregion

    #region Construction
    /// <summary>
    /// Private constructor
    /// </summary>
    private SampleCsStringTable()
    {
      m_strings = new List<string>();
    }

    /// <summary>
    /// The one and only SampleCsStringTable object
    /// </summary>
    private static SampleCsStringTable g_string_table;

    /// <summary>
    /// Returns the one and only SampleCsStringTable object
    /// </summary>
    public static SampleCsStringTable Instance
    {
      get
      {
        if (null == g_string_table)
          g_string_table = new SampleCsStringTable();
        return g_string_table;
      }
    }
    #endregion

    #region Public Methods
    /// <summary>
    /// Returns the number of strings in the string table.
    /// </summary>
    public int Count
    {
      get { return m_strings.Count; }
    }

    /// <summary>
    /// Returns a string from the string table.
    /// </summary>
    public string GetAt(int index)
    {
      string str = null;
      if (index >= 0 && index < Count)
        str = m_strings[index];
      return str;
    }

    /// <summary>
    /// Find a string in the string table.
    /// </summary>
    public int Find(string str)
    {
      if (string.IsNullOrEmpty(str))
        return -1;

      return m_strings.FindIndex(s => s == str);
    }

    /// <summary>
    /// Adds a string to the string table.
    /// </summary>
    public int Add(string str)
    {
      if (string.IsNullOrEmpty(str))
        return -1;

      int index = Find(str);
      if (index >= 0) return index;

      m_strings.Add(str);

      return Count - 1;
    }

    /// <summary>
    /// Inserts a string into the string table at a zero-based index position.
    /// </summary>
    public int Insert(int index, string str)
    {
      if (index < 0 || index > Count)
        return -1;

      if (string.IsNullOrEmpty(str))
        return -1;

      m_strings.Insert(index, str);

      return index;
    }

    /// <summary>
    /// Removes a string from the string table.
    /// </summary>
    public bool Remove(string str)
    {
      if (string.IsNullOrEmpty(str))
        return false;

      int index = Find(str);

      return RemoveAt(index);
    }

    /// <summary>
    /// Removes a string from the string table.
    /// </summary>
    public bool RemoveAt(int index)
    {
      if (index >= 0 && index < Count)
      {
        m_strings.RemoveAt(index);
        return true;
      }

      return false;
    }

    /// <summary>
    /// Clears the string table
    /// </summary>
    public void Clear()
    {
      m_strings.Clear();

    }
    #endregion

    #region Owning Plug-in Methods
    /// <summary>
    /// Called whenever a Rhino is about to save a .3dm file.  If you want to save
    //  plug-in document data when a model is saved in a version 5 .3dm file, then
    //  you must override this function to return true and you must override WriteDocument().
    /// </summary>
    public bool ShouldCallWriteDocument(FileWriteOptions options)
    {
      return (
        Count > 0 &&
        false == options.WriteGeometryOnly &&
        false == options.WriteSelectedObjectsOnly
        );
    }

    /// <summary>
    /// Called when Rhino is saving a .3dm file to allow the plug-in to save document user data.
    /// </summary>
    public void WriteDocument(RhinoDoc doc, BinaryArchiveWriter archive, FileWriteOptions options)
    {
      archive.Write3dmChunkVersion(MAJOR, MINOR);
      archive.WriteStringArray(m_strings);
    }

    /// <summary>
    /// Called whenever a Rhino document is being loaded and plug-in user data was
    /// encountered written by a plug-in with this plug-in's GUID.
    /// </summary>
    public void ReadDocument(RhinoDoc doc, BinaryArchiveReader archive, FileReadOptions options)
    {
      archive.Read3dmChunkVersion(out var major, out var minor);
      if (MAJOR == major && MINOR == minor)
      {
        // Always read user data even though you might not use it...
        var strings = archive.ReadStringArray();
        if (null != strings && !options.ImportMode && !options.ImportReferenceMode)
          m_strings.AddRange(strings);
      }
    }

    /// <summary>
    /// OnCloseDocument event handler.
    /// </summary>
    public void OnCloseDocument(object sender, DocumentEventArgs e)
    {
      Clear();
    }
    #endregion
  }
}
