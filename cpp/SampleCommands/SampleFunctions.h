#pragma once

/// <summary>
/// Evaluate torsion of a curve at a parmeter.
/// </summary>
/// <param name="curve">Curve to evaluate.</param>
/// <param name="t">Evaluation parameter.</param>
/// <returns>The torsion if successful.</returns>
/// <remarks>See Barrett O'Neill, Elementary Differential Geometry, page 69.</remarks>
double ON_CurveTorsion(const ON_Curve& curve, double t);

/// <summary>
/// Create a blend curve with G0 continuity between two existing curves.
/// </summary>
/// <param name="crvA">Curve to blend from (blending will occur at curve end point).</param>
/// <param name="crvB">Curve to blend to (blending will occur at curve start point).</param>
/// <returns>An ON_Curve representing the blend between A and B.</returns>
/// <remarks>
/// CRITICAL: Memory for the resulting curve is allocated. It is the calling
/// functions responsibility to clean up the memory.
/// </remarks>
ON_Curve* ON_BlendG0Curve(
  const ON_Curve* crvA,
  const ON_Curve* crvB
);

/// <summary>
/// Create a blend curve with G1 continuity between two existing curves.
/// </summary>
/// <param name="crvA">Curve to blend from (blending will occur at curve end point).</param>
/// <param name="crvB">Curve to blend to (blending will occur at curve start point).</param>
/// <param name="bulgeA">Bulge factor at curveA end of blend. Values near 1.0 work best.</param>
/// <param name="bulgeB">Bulge factor at curveB end of blend. Values near 1.0 work best.</param>
/// <returns>An ON_Curve representing the blend between A and B.</returns>
/// <remarks>
/// CRITICAL: Memory for the resulting curve is allocated. It is the calling
/// functions responsibility to clean up the memory.
/// </remarks>
ON_Curve* ON_BlendG1Curve(
  const ON_Curve* crvA,
  const ON_Curve* crvB,
  double bulgeA,
  double bulgeB
);

/// <summary>
/// Create a blend curve with G2 continuity between two existing curves.
/// </summary>
/// <param name="crvA">Curve to blend from (blending will occur at curve end point).</param>
/// <param name="crvB">Curve to blend to (blending will occur at curve start point).</param>
/// <param name="bulgeA">Bulge factor at curveA end of blend. Values near 1.0 work best.</param>
/// <param name="bulgeB">Bulge factor at curveB end of blend. Values near 1.0 work best.</param>
/// <returns>An ON_Curve representing the blend between A and B.</returns>
/// <remarks>
/// CRITICAL: Memory for the resulting curve is allocated. It is the calling
/// functions responsibility to clean up the memory.
/// </remarks>
ON_Curve* ON_BlendG2Curve(
  const ON_Curve* crvA,
  const ON_Curve* crvB,
  double bulgeA,
  double bulgeB
);

/// <summary>
/// Get the BRep definition of a trimmed surface.
/// </summary>
/// <param name="srf">Surface that will be trimmed.</param>
/// <param name="crv2d">
/// Closed, 2d parameter space boundary curve that defines the
/// outer boundary of the trimmed surface.
/// </param>
/// <param name="tol">Tolerance for fitting 3d edge curves.</param>
/// <returns>
/// An ON_Brep representation of the trimmed surface with a single face.
/// </returns>
/// <remarks>
/// CRITICAL: Memory for the resulting BRep is allocated. It is the calling
/// functions responsibility to clean up the memory.
/// </remarks>
ON_Brep* ON_BrepFromSurfaceAndBoundary(
  const ON_Surface& srf,
  const ON_Curve& crv2d,
  double tol
);

/// <summary>
/// Returns the  bitmap preview image from a 3dm file.
/// </summary>
/// <param name="pszFilePath">The full path to the 3dm file.</param>
/// <returns>An HBITMAP if successful, or nullptr if the 3dm file does not 
/// contain a preview image or if there was an error reading the file.
/// </returns>
/// <remarks>
/// CRITICAL: Memory for the bitmap is allocated. It is the calling
/// functions responsibility to clean up the memory by calling the
/// Win32 DeleteObject() function.
/// </remarks>
HBITMAP ON_ReadBitmapPreviewImage(const wchar_t* pszFilePath);

/// <summary>
/// CRhinoHatchPatternTable helpers for system hatch patterns
/// </summary>
class CRhinoHatchTableHelper
{
public:
  /// <summary>
  /// Returns the Solid hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Solid(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Hatch1 hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Hatch1(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Hatch2 hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Hatch2(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Hatch3 hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Hatch3(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the HatchDash hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* HatchDash(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Grid hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Grid(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Grid60 hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Grid60(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Plus hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Plus(CRhinoHatchPatternTable& table);
  /// <summary>
  /// Returns the Squares hatch pattern
  /// </summary>
  static const CRhinoHatchPattern* Squares(CRhinoHatchPatternTable& table);

private:
  static const CRhinoHatchPattern* FindOrCreateHatchPattern(CRhinoHatchPatternTable& table, const ON_HatchPattern& hatch_pattern);
};


/// <summary>
/// Returns true if Rhino was started as a standalone executable.
/// Returns false if Rhino was started by some other application or process.
/// </summary>
bool IsRhinoRunningAsExe();

/// <summary>
/// Returns true if Rhino has input focus.
/// </summary>
bool RhinoHasFocus();
