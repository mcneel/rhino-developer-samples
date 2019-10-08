using System;
using System.Collections.Generic;
using Rhino.Geometry;
using Rhino.Runtime.InteropWrappers;
using SampleSharedXformOwner.Interop;

namespace SampleSharedXformOwner
{
  /// <summary>
  /// The transformation table is  a dictionary that maps Rhino object ids
  /// with a collection of 4x4 transforms.
  /// </summary>
  internal static class SampleSharedXformTable
  {
    /// <summary>
    /// Returns the number of Rhino objects in the transformation table.
    /// </summary>
    /// <returns></returns>
    public static int Count()
    {
      return UnsafeNativeMethods.ON_XformTable_Count();
    }

    /// <summary>
    /// Verifies a Rhino object that is in the transformation table.
    /// </summary>
    /// <param name="id">The Rhino object id.</param>
    /// <returns>true if the Rhino object is in the transformation table, false otherwise.</returns>
    public static bool Contains(Guid id)
    {
      return UnsafeNativeMethods.ON_XformTable_Contains(id);
    }

    /// <summary>
    /// Adds a new Rhino object and a collection of 4x4 transforms to the transformation table.
    /// </summary>
    /// <param name="id">The Rhino object id.</param>
    /// <param name="transforms">The 4x4 transforms.</param>
    /// <returns>true if successful, false otherwise.</returns>
    public static bool Add(Guid id, IEnumerable<Transform> transforms)
    {
      using (var arr = new SmpleArrayXform())
      {
        foreach (var xf in transforms)
          arr.Append(xf);

        var ptr = arr.ConstPointer();
        return UnsafeNativeMethods.ON_XformTable_Add(id, ptr);
      }
    }

    /// <summary>
    /// Updates a Rhino object, that already exists in the transformation table, with a new set of 4x4 transforms.
    /// </summary>
    /// <param name="id">The Rhino object id.</param>
    /// <param name="transforms">The 4x4 transforms.</param>
    /// <returns>true if successful, false otherwise.</returns>
    public static bool Update(Guid id, IEnumerable<Transform> transforms)
    {
      using (var arr = new SmpleArrayXform())
      {
        foreach (var xf in transforms)
          arr.Append(xf);

        var ptr = arr.ConstPointer();
        return UnsafeNativeMethods.ON_XformTable_Update(id, ptr);
      }
    }

    /// <summary>
    /// Removes a Rhino object from the transformation table.
    /// </summary>
    /// <param name="id">The Rhino object id.</param>
    /// <returns>true if successful, false otherwise.</returns>
    public static bool Remove(Guid id)
    {
      return UnsafeNativeMethods.ON_XformTable_Remove(id);
    }

    /// <summary>
    /// Returns the ids of the Rhino objects in the transformation table.
    /// </summary>
    /// <returns></returns>
    public static Guid[] Ids()
    {
      using (var arr = new SimpleArrayGuid())
      {
        var ptr = arr.NonConstPointer();
        var rc = UnsafeNativeMethods.ON_XformTable_Ids(ptr);
        return rc > 0
          ? arr.ToArray()
          : new Guid[0];
      }
    }

    /// <summary>
    /// Returns the runtime serial number of the transformations for a Rhino object.
    /// Use this value to determine if the object's transformations have changed.
    /// </summary>
    /// <param name="id">The Rhino object id.</param>
    /// <returns>true if successful, false otherwise.</returns>
    public static uint RuntimeSerialNumber(Guid id)
    {
      return UnsafeNativeMethods.ON_XformTable_RuntimeSerialNumber(id);
    }

    /// <summary>
    /// Returns the 4x4 transforms for a Rhino object.
    /// </summary>
    /// <param name="id">The Rhino object id.</param>
    /// <returns>An array of transforms if successful, otherwise an empty array.</returns>
    public static Transform[] Transforms(Guid id)
    {
      using (var arr = new SmpleArrayXform())
      {
        var ptr = arr.NonConstPointer();
        var rc = UnsafeNativeMethods.ON_XformTable_Transforms(id, ptr);
        return rc > 0
          ? arr.ToArray()
          : new Transform[0];
      }
    }

    /// <summary>
    /// Empties the contents of the transformation table. Call this function when
    /// Rhino closes the current document.
    /// </summary>
    public static void Empty()
    {
      UnsafeNativeMethods.ON_XformTable_Empty();
    }
  }
}
