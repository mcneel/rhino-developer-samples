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

/// <summary>
/// Returns true if the Rhino main window has been re-parented to some other application window.
/// Returns true if the Rhino main window parent is the Windows Desktop.
/// </summary>
bool IsRhinoReparented();

/// <summary>
/// Returns module handle where "this" function is running in: EXE or DLL.
/// </summary>
HMODULE FancyGetModuleHandle();


/// <summary>
/// CRhinoDoc::BeginUndoRecord and CRhinoDoc::EndUndoRecord helper.
/// Undo record will be ended when classes goes out of scope.
/// Useful in modeless user interface code that modifies document objects.
/// Not useful Rhino command, as Rhino's command handler tracks undo records.
/// </summary>
class CRhinoDocUndoRecordHelper
{
public:
  /// <summary>
  /// Begin a CRhinoDoc undo record.
  /// </summary>
  /// <param name="doc">The active document.</param>
  /// <param name="pszDescription">The undo description.</param>
  CRhinoDocUndoRecordHelper(CRhinoDoc& doc, const wchar_t* pszDescription);

  /// <summary>
  /// Begin a CRhinoDoc undo record.
  /// </summary>
  /// <param name="docRuntimeSerialNumber">The active document's runtime serial number.</param>
  /// <param name="pszDescription">The undo description.</param>
  CRhinoDocUndoRecordHelper(unsigned int docRuntimeSerialNumber, const wchar_t* pszDescription);

  /// <summary>
  /// Class destructor, ends the undo record.
  /// </summary>
  ~CRhinoDocUndoRecordHelper();

  /// <summary>
  /// Ends the undo record.
  /// </summary>
  /// <returns>true if the undo record was ended.</returns>
  /// <remarks>The class destructor calls this method.</returns>
  bool EndUndoRecord();

private:
  unsigned int m_docRuntimeSerialNumber = 0;
  unsigned int m_undoRecordSerialNumber = 0;
};


/// <summary>
/// Specifies the known system colors.
/// </summary>
enum class ON_KnownColor : unsigned char
{
  Unset = 0,
  AliceBlue = 1,
  AntiqueWhite = 2,
  Aqua = 3,
  Aquamarine = 4,
  Azure = 5,
  Beige = 6,
  Bisque = 7,
  Black = 8,
  BlanchedAlmond = 9,
  Blue = 10,
  BlueViolet = 11,
  Brown = 12,
  BurlyWood = 13,
  CadetBlue = 14,
  Chartreuse = 15,
  Chocolate = 16,
  Coral = 17,
  CornflowerBlue = 18,
  Cornsilk = 19,
  Crimson = 20,
  Cyan = 21,
  DarkBlue = 22,
  DarkCyan = 23,
  DarkGoldenrod = 24,
  DarkGray = 25,
  DarkGreen = 26,
  DarkKhaki = 27,
  DarkMagenta = 28,
  DarkOliveGreen = 29,
  DarkOrange = 30,
  DarkOrchid = 31,
  DarkRed = 32,
  DarkSalmon = 33,
  DarkSeaGreen = 34,
  DarkSlateBlue = 35,
  DarkSlateGray = 36,
  DarkTurquoise = 37,
  DarkViolet = 38,
  DeepPink = 39,
  DeepSkyBlue = 40,
  DimGray = 41,
  DodgerBlue = 42,
  Firebrick = 43,
  FloralWhite = 44,
  ForestGreen = 45,
  Fuchsia = 46,
  Gainsboro = 47,
  GhostWhite = 48,
  Gold = 49,
  Goldenrod = 50,
  Gray = 51,
  Green = 52,
  GreenYellow = 53,
  Honeydew = 54,
  HotPink = 55,
  IndianRed = 56,
  Indigo = 57,
  Ivory = 58,
  Khaki = 59,
  Lavender = 60,
  LavenderBlush = 61,
  LawnGreen = 62,
  LemonChiffon = 63,
  LightBlue = 64,
  LightCoral = 65,
  LightCyan = 66,
  LightGoldenrodYellow = 67,
  LightGray = 68,
  LightGreen = 69,
  LightPink = 70,
  LightSalmon = 71,
  LightSeaGreen = 72,
  LightSkyBlue = 73,
  LightSlateGray = 74,
  LightSteelBlue = 75,
  LightYellow = 76,
  Lime = 77,
  LimeGreen = 78,
  Linen = 79,
  Magenta = 80,
  Maroon = 81,
  MediumAquamarine = 82,
  MediumBlue = 83,
  MediumOrchid = 84,
  MediumPurple = 85,
  MediumSeaGreen = 86,
  MediumSlateBlue = 87,
  MediumSpringGreen = 88,
  MediumTurquoise = 89,
  MediumVioletRed = 90,
  MidnightBlue = 91,
  MintCream = 92,
  MistyRose = 93,
  Moccasin = 94,
  NavajoWhite = 95,
  Navy = 96,
  OldLace = 97,
  Olive = 98,
  OliveDrab = 99,
  Orange = 100,
  OrangeRed = 101,
  Orchid = 102,
  PaleGoldenrod = 103,
  PaleGreen = 104,
  PaleTurquoise = 105,
  PaleVioletRed = 106,
  PapayaWhip = 107,
  PeachPuff = 108,
  Peru = 109,
  Pink = 110,
  Plum = 111,
  PowderBlue = 112,
  Purple = 113,
  RebeccaPurple = 114,
  Red = 115,
  RosyBrown = 116,
  RoyalBlue = 117,
  SaddleBrown = 118,
  Salmon = 119,
  SandyBrown = 120,
  SeaGreen = 121,
  SeaShell = 122,
  Sienna = 123,
  Silver = 124,
  SkyBlue = 125,
  SlateBlue = 126,
  SlateGray = 127,
  Snow = 128,
  SpringGreen = 129,
  SteelBlue = 130,
  Tan = 131,
  Teal = 132,
  Thistle = 133,
  Tomato = 134,
  Turquoise = 135,
  Violet = 136,
  Wheat = 137,
  White = 138,
  WhiteSmoke = 139,
  Yellow = 140,
  YellowGreen = 141
};

/// <summary>
/// Gets a known color from an unsigned value.
/// </summary>
ON_KnownColor ON_KnownColorFromUnsigned(unsigned int known_color_as_unsigned);

/// <summary>
/// Gets a known color as an ON_Color.
/// </summary>
ON_Color ON_ColorFromKnownColor(ON_KnownColor knownColor);

/// <summary>
/// Get a random known color as an ON_Color.
/// </summary>
ON_Color ON_ColorFromRandomKnownColor();
