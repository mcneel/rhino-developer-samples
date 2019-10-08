using System;
using System.Runtime.InteropServices;
using Rhino.Geometry;

namespace SampleSharedXformOwner.Interop
{
  /// <summary>
  /// Utility class to return the name of the import library.
  /// </summary>
  internal class Import
  {
    /// <summary>
    /// Private constructor
    /// </summary>
    private Import()
    {
    }

    /// <summary>
    /// Do not add a file extension.
    /// </summary>
    public const string lib = "SampleSharedXformLib";
  }

  /// <summary>
  /// UnsafeNativeMethods class
  /// </summary>
  internal class UnsafeNativeMethods
  {
    // ON_XformTable interface

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int ON_XformTable_Count();

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern bool ON_XformTable_Contains(Guid guid);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern bool ON_XformTable_Add(Guid guid, IntPtr ptr);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern bool ON_XformTable_Update(Guid guid, IntPtr ptr);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern bool ON_XformTable_Remove(Guid guid);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int ON_XformTable_Ids(IntPtr guids);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern uint ON_XformTable_RuntimeSerialNumber(Guid guid);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int ON_XformTable_Transforms(Guid guid, IntPtr ptr);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void ON_XformTable_Empty();

    // ON_SimpleArray<ON_Xform> interface

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern IntPtr ON_SimpleArrayXform_New();

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void ON_SimpleArrayXform_CopyValues(IntPtr ptr, [In, Out] Transform[] xforms);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void ON_SimpleArrayXform_Append(IntPtr ptr, ref Transform xform);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern int ON_SimpleArrayXform_Count(IntPtr ptr);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern Guid ON_SimpleArrayXform_Get(IntPtr ptr, int index, ref Transform xform);

    [DllImport(Import.lib, CallingConvention = CallingConvention.Cdecl)]
    internal static extern void ON_SimpleArrayXform_Delete(IntPtr ptr);
  }
}
