using System;

namespace SampleCsCommon
{
  public class SampleCsStringTableHelpers
  {
    #region Public Methods
    /// <summary>
    /// Returns the number of strings in the string table.
    /// </summary>
    public static int Count()
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      return string_table.Count;
    }

    /// <summary>
    /// Returns a string from the string table.
    /// </summary>
    public static string GetAt(int index)
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      return string_table.GetAt(index);
    }

    /// <summary>
    /// Find a string in the string table.
    /// </summary>
    public static int Find(string str)
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      int rc = string_table.Find(str);
      if (rc >= 0)
      {
        // Invoke event
        if (null != OnAdd)
          OnAdd.Invoke(str);
      }
      return rc;
    }

    /// <summary>
    /// Adds a string to the string table.
    /// </summary>
    public static int Add(string str)
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      int rc = string_table.Add(str);
      if (rc >= 0)
      {
        // Invoke event
        if (null != OnAdd)
          OnAdd.Invoke(str);
      }
      return rc;
    }

    /// <summary>
    /// Inserts a string into the string table at a zero-based index position.
    /// </summary>
    public static int Insert(int index, string str)
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      int rc = string_table.Insert(index, str);
      if (rc >= 0)
      {
        // Invoke event
        if (null != OnInsert)
          OnInsert.Invoke(index, str);
      }
      return rc;
    }

    /// <summary>
    /// Removes a string from the string table.
    /// </summary>
    public static bool Remove(string str)
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      bool rc = string_table.Remove(str);
      if (rc)
      {
        // Invoke event
        if (null != OnRemove)
          OnRemove.Invoke(str);
      }
      return rc;
    }

    /// <summary>
    /// Removes a string from the string table.
    /// </summary>
    public static bool RemoveAt(int index)
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      bool rc = string_table.RemoveAt(index);
      if (rc)
      {
        // Invoke event
        if (null != OnRemoveAt)
          OnRemoveAt.Invoke(index);
      }
      return rc;
    }

    /// <summary>
    /// Clears the string table.
    /// </summary>
    public static void Clear()
    {
      SampleCsStringTable string_table = SampleCsStringTable.Instance;
      string_table.Clear();

      // Invoke event
      if (null != OnClear)
        OnClear.Invoke();
    }

    /// <summary>
    /// Easy way to post a refresh event.
    /// </summary>
    public static void Refresh()
    {
      // Invoke event
      if (null != OnRefresh)
        OnRefresh.Invoke();
    }
    #endregion

    #region Delegates
    public delegate void OnAddEvent(string str);
    public delegate void OnInsertEvent(int index, string str);
    public delegate void OnRemoveEvent(string str);
    public delegate void OnRemoveAtEvent(int index);
    public delegate void OnClearEvent();
    public delegate void OnRefreshEvent();
    #endregion

    #region Events
    public static event OnAddEvent OnAdd;
    public static event OnInsertEvent OnInsert;
    public static event OnRemoveEvent OnRemove;
    public static event OnRemoveAtEvent OnRemoveAt;
    public static event OnClearEvent OnClear;
    public static event OnRefreshEvent OnRefresh;

    #endregion

  }
}
