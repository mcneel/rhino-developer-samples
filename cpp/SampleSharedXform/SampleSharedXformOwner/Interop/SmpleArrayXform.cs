using System;
using Rhino.Geometry;

namespace SampleSharedXformOwner.Interop
{
  /// <summary>
  /// Represents a wrapper to an unmanaged "array" (list) of ON_Xform instances.
  /// </summary>
  internal class SmpleArrayXform : IDisposable
  {
    internal IntPtr m_ptr; // ON_SimpleArray<ON_Xform>

    /// <summary>
    /// Gets the const (immutable) pointer of this array.
    /// </summary>
    public IntPtr ConstPointer() => m_ptr;

    /// <summary>
    /// Gets the non-const pointer (for modification) of this array.
    /// </summary>
    public IntPtr NonConstPointer() => m_ptr;

    /// <summary>
    /// Initializes a new <see cref="SmpleArrayXform"/> class.
    /// </summary>
    public SmpleArrayXform()
    {
      m_ptr = UnsafeNativeMethods.ON_SimpleArrayXform_New();
    }

    /// <summary>
    /// Get the transform at index.
    /// </summary>
    public Transform this[int index]
    {
      get
      {
        var rc = new Transform();
        UnsafeNativeMethods.ON_SimpleArrayXform_Get(m_ptr, index, ref rc);
        return rc;
      }
    }

    /// <summary>
    /// Appends a new <see cref="Transform"/> at the end of this array.
    /// </summary>
    public void Append(Transform xform)
    {
      UnsafeNativeMethods.ON_SimpleArrayXform_Append(m_ptr, ref xform);
    }

    /// <summary>
    /// Gets the amount of elements in this array.
    /// </summary>
    public int Count => UnsafeNativeMethods.ON_SimpleArrayXform_Count(m_ptr);

    /// <summary>
    /// Returns the managed counterpart of the unmanaged array.
    /// </summary>
    public Transform[] ToArray()
    {
      var count = Count;
      if (0 == count)
        return new Transform[0];

      var rc = new Transform[count];
      UnsafeNativeMethods.ON_SimpleArrayXform_CopyValues(m_ptr, rc);
      return rc;
    }

    #region IDisposable implementation

    /// <summary>
    /// Passively reclaims unmanaged resources when the class user did not explicitly call Dispose().
    /// </summary>
    ~SmpleArrayXform()
    {
      Dispose(false);
    }

    /// <summary>
    /// Actively reclaims unmanaged resources that this instance uses.
    /// </summary>
    public void Dispose()
    {
      Dispose(true);
      GC.SuppressFinalize(this);
    }

    /// <summary>
    /// This method is called with argument true when class user calls Dispose(), 
    /// while with argument false when the Garbage Collector invokes the finalizer,
    /// or Finalize() method. You must reclaim all used unmanaged resources in both cases,
    /// and can use this chance to call Dispose on disposable fields if the argument is true.
    /// Also, you must call the base virtual method within your overriding method.
    /// </summary>
    protected virtual void Dispose(bool disposing)
    {
      if (m_ptr != IntPtr.Zero)
        UnsafeNativeMethods.ON_SimpleArrayXform_Delete(m_ptr);
      m_ptr = IntPtr.Zero;
    }

    #endregion
  }
}
