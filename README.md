# Rhino Developer Samples

<img align="left" src="https://github.com/mcneel/developer-rhino3d-com/blob/master/images/dev-logo-rhino-small.png">

This repository contains all the sample developer code for programming in the Rhino and Grasshopper ecosystem.

The source of this repository will be used to populate the [Samples](http://developer.rhino3d.com/samples/) section of the [Rhino Developer website](http://developer.rhino3d.com/).

Much like the Rhino Developer website, the version of Rhino that these samples are compatible with are determined by which branch of this repository one is viewing:

- The `master` branch corresponds to the version of Rhino currently in active development (the WIP branch).
- The `6` branch corresponds to Rhino 6.
- Older versions of Rhino will be archived into version-specific branches (for example: `5`).

## Samples by SDK or Language

The samples in this repository are organized by folders that correspond to the different SDKs that make up the Rhino development platform.  For more information about the different Rhino SDKs and supported languages, please see the [Rhino Developer website](http://developer.rhino3d.com/).  This repository contains samples for:

- [C/C++](cpp)
- [Grasshopper](grasshopper)
  - [C#](grasshopper/cs)
  - [Python](grasshopper/py)
  - [VB.NET](grasshopper/vb)
- [openNURBS](opennurbs)
  - [C/C++](opennurbs/cpp)
  - [C#](opennurbs/cs)
- [RhinoCommon](rhinocommon)
  - [C#](rhinocommon/cs)
  - [Python](rhinocommon/py)
  - [VB.NET](rhinocommon/vb)
- [RhinoMobile](rhinomobile) (C#)
- [RhinoPython](rhinopython)
- [RhinoScript](rhinoscript) (VBScript)
- [Zoo](zoo) (C# or VB)

## Building Samples

TODO: More info about building coming soon.

**Why are the sample projects prefixed with the word Sample?**

We gave each sample project a unique name in order to avoid command name collisions when these samples are run in Rhino.  The word Sample is prefixed to distinguish it from developer "Test" commands that sometimes used to test out new functionality in Rhino.  Since it is possible - and probable - that samples might be built, run, and tested in multiple languages, so we opted for a language code (C/C++ samples omit this simply because they came first), before the name of the sample.

## Testing

TODO: More info coming soon
