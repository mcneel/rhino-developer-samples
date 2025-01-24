using Rhino.Geometry;
using System;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using System.Runtime.Serialization.Formatters.Binary;

namespace SampleCsDeserializeEmbeddedResource
{
  public static class SampleCsGeometryHelper
  {
    /// <summary>
    /// Writes a GeometryBase-derived object to a file.
    /// </summary>
    public static bool WriteToFile(string path, GeometryBase geometry)
    {
      if (string.IsNullOrEmpty(path))
        return false;

      if (null == geometry)
        return false;

      byte[] bytes = ToBytes(geometry);
      if (null == bytes || 0 == bytes.Length)
        return false;

      bool rc;
      try
      {
        using (FileStream stream = new FileStream(path, FileMode.Create, FileAccess.Write))
        {
          stream.Write(bytes, 0, bytes.Length);
          rc = true;
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
        rc = false;
      }

      return rc;
    }

    /// <summary>
    /// Reads a GeometryBase-derived object from an embedded resource.
    /// </summary>
    public static GeometryBase ReadFromEmbeddedResource(string resource)
    {
      if (string.IsNullOrEmpty(resource))
        return null;

      try
      {
        Assembly assembly = Assembly.GetExecutingAssembly();
        using (Stream stream = assembly.GetManifestResourceStream(resource))
        {
          if (null != stream)
          {
            int bytes_to_read = (int)stream.Length;
            if (bytes_to_read > 0)
            {
              byte[] bytes = new byte[bytes_to_read];
              int bytes_read = stream.Read(bytes, 0, bytes_to_read);
              if (bytes_read == bytes_to_read)
              {
                GeometryBase geometry = ToGeometryBase(bytes);
                return geometry;
              }
            }
          }
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
      }

      return null;
    }

    /// <summary>
    /// Converts an object that inherits from GeometryBase to an array of bytes.
    /// </summary>
    private static byte[] ToBytes(GeometryBase src)
    {
      byte[] rc = new byte[0];

      if (null == src)
        return rc;

      try
      {
        BinaryFormatter formatter = new BinaryFormatter();
        using (MemoryStream stream = new MemoryStream())
        {
          formatter.Serialize(stream, src);
          rc = stream.ToArray();
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
      }

      return rc;
    }

    /// <summary>
    /// Creates an object that inherits from GeometryBase from an array of bytes.
    /// </summary>
    private static GeometryBase ToGeometryBase(byte[] bytes)
    {
      if (null == bytes || 0 == bytes.Length)
        return null;

      GeometryBase rc = null;
      try
      {
        using (MemoryStream stream = new MemoryStream())
        {
          BinaryFormatter formatter = new BinaryFormatter();
          stream.Write(bytes, 0, bytes.Length);
          stream.Seek(0, SeekOrigin.Begin);
          GeometryBase geometry = formatter.Deserialize(stream) as GeometryBase;
          if (null != geometry && geometry.IsValid)
            rc = geometry;
        }
      }
      catch (Exception e)
      {
        Debug.WriteLine(e.Message);
      }

      return rc;
    }
  }
}
