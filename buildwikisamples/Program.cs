using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Text;
using Newtonsoft.Json;

namespace buildwikisamples
{
  class Program
  {
    static void Main(string[] args)
    {
      var wiki_sample_titles = JsonConvert.DeserializeObject<List<SampleTitle>>(JSON_WIKI_SAMPLE_TITLES);
      var sample_titles = JsonConvert.DeserializeObject<List<SampleTitle>>(JSON_SAMPLE_TITLES);
      g_sample_titles = wiki_sample_titles.Concat(sample_titles).ToList();
      GenerateWikiFilesFromSource();
      GenerateMdFilesFromWikiFiles();
    }

    static void GenerateWikiFilesFromSource()
    {
      string[] csfiles = System.IO.Directory.GetFiles(System.IO.Path.Combine(ROOT_DIR, "rhinocommon\\examples\\cs"), "ex_*");
      string vbdir = System.IO.Path.Combine(ROOT_DIR, "rhinocommon\\examples\\vbnet");
      string pydir = System.IO.Path.Combine(ROOT_DIR, "rhinocommon\\examples\\py");

      for (int i = 0; i < csfiles.Length; i++)
      {
        string filename = System.IO.Path.GetFileNameWithoutExtension(csfiles[i]);
        //if (filename != "ex_evnormal") continue;
        string wikiname = filename.Substring(3) + ".txt"; // prune "ex_"
        string wikifile = System.IO.Path.Combine(g_wikidir, wikiname);
        StringBuilder sb = new StringBuilder();
        sb.AppendLine("===== " + filename.Substring(3) + " =====");
        sb.AppendLine();
        sb.AppendLine("===== C# =====");
        sb.AppendLine("<code c#>");
        string[] lines = System.IO.File.ReadAllLines(csfiles[i]);
        bool inblock = false;
        for (int j = 0; j < lines.Length; j++)
        {
          if (!inblock)
          {
            if (lines[j].StartsWith("{"))
              inblock = true;
            continue;
          }
          if (lines[j].StartsWith("}"))
            break;
          if (lines[j].Length < 2)
            sb.AppendLine(lines[j]);
          else
            sb.AppendLine(lines[j].Substring(2));
        }
        sb.AppendLine("</code><!--cs-->");

        string vbpath = System.IO.Path.Combine(vbdir, filename + ".vb");
        sb.AppendLine("===== VB.NET =====");
        sb.AppendLine("<code vb>");
        if (System.IO.File.Exists(vbpath))
        {
          lines = System.IO.File.ReadAllLines(vbpath);
          inblock = false;
          for (int j = 0; j < lines.Length; j++)
          {
            if (!inblock)
            {
              if (lines[j].StartsWith(" "))
                inblock = true;
              else
                continue;
            }
            if (lines[j].Trim().StartsWith("End Class"))
            {
              if (lines[j].StartsWith("  "))
                sb.AppendLine(lines[j].Substring(2));
              break;
            }
            if (lines[j].Length < 2)
              sb.AppendLine(lines[j]);
            else
              sb.AppendLine(lines[j].Substring(2));
          }
        }
        else
        {
          sb.AppendLine("no vb code sample available");
        }
        sb.AppendLine("</code><!--vb-->");

        string pypath = System.IO.Path.Combine(pydir, filename + ".py");
        sb.AppendLine("===== Python =====");
        sb.AppendLine("<code python>");
        if (System.IO.File.Exists(pypath))
        {
          lines = System.IO.File.ReadAllLines(pypath);
          for (int j = 0; j < lines.Length; j++)
          {
            sb.AppendLine(lines[j]);
          }
        }
        else
        {
          sb.AppendLine("no python code sample available");
        }
        sb.AppendLine("</code><!--py-->");
        sb.AppendLine("{{tag>Developer rhinocommon}}");
        System.IO.File.WriteAllText(wikifile, sb.ToString());
      }
    }

    static void GenerateMdFilesFromWikiFiles()
    {
      const string output = @"Z:\src_osx\mcneel.com\RhinoDocsMainGhpSite\developer-rhino3d-com\_samples\rhinocommon";

      var eol = Environment.NewLine;

      Func<string, string> keywords = 
        s => string.Format("['{0}']", s.ToLower().Replace("'","").Split(' ')
                                                         .Except(new List<string>{"from", "this", "__no_title__"})
                                                         .Where(w => w.Length > 3)
                                                         .Aggregate((current, next) => current + "', '" + next));
        
      Func<string, int, string> front_matter = 
        (s, idx) => string.Format("---{0}layout: code-sample{0}title: {1}{0}author: {0}categories: ['Other'] {0}platforms: ['Cross-Platform']{0}apis: ['RhinoCommon']{0}" +
          "languages: ['C#', 'Python', 'VB.NET']{0}keywords: {2}{0}order: {3}{0}description:  {0}---{0}", eol, s, keywords(s), ++idx);

      Func<string, string> title_from_wiki_file = fn => 
        g_sample_titles.Where(st => st.fn == Path.GetFileNameWithoutExtension(fn)).Select(st => st.title).SingleOrDefault() 
        ?? string.Format("__NO_TITLE__ {0}", Path.GetFileNameWithoutExtension(fn));

      var mdfiles =
        Directory.GetFiles(g_wikidir)
          .Select((fn, idx) => Tuple.Create<string, int, string>(fn, idx, title_from_wiki_file(fn)))
          .ToDictionary(
            tpl => Path.GetFileNameWithoutExtension(tpl.Item1), 
            tpl => File.ReadAllLines(tpl.Item1)
                    .Select(ln => 
                      ln.Equals("<code c#>") ? "```cs" :
                        ln.Equals("<code vb>") ? "```vbnet" :
                          ln.Equals("<code python>") ? "```python" :
                            ln.Equals("</code><!--cs-->") ? "```" + eol + "{: #cs .tab-pane .fade .in .active}" + eol :
                              ln.Equals("</code><!--vb-->") ? "```" + eol + "{: #vb .tab-pane .fade .in}" + eol :
                                ln.Equals("</code><!--py-->") ? "```" + eol + "{: #py .tab-pane .fade .in}" + eol :
                                  ln.Equals("===== C# =====") ? "" :
                                    ln.Equals("===== VB.NET =====") ? "" :
                                      ln.Equals("===== Python =====") ? "" :
                                        ln.StartsWith("{{tag") ? "" :
                                          ln.StartsWith("=====") && ln.EndsWith("=====") ? front_matter(tpl.Item3, tpl.Item2) : 
                                            ln)
          );          

        mdfiles.ToList().ForEach(kvp => {
          File.WriteAllLines(Path.Combine(output, kvp.Key + ".md"), kvp.Value);
          Console.WriteLine("writting {0}.md ...", kvp.Key);
        });
    }

    private const string ROOT_DIR = @"C:\dev\rhino\wip\src4\DotNetSDK\";
    static readonly string g_wikidir = System.IO.Path.Combine(ROOT_DIR, "rhinocommon\\examples\\wiki");
    private const string DOCCUWIKIDIR = @"Z:\src_osx\mcneel.com\wikidata\pages\developer\rhinocommonsamples";

    // see map_wiki_sample_titles_to_cs_samples_as_json.linq file
    private const string JSON_WIKI_SAMPLE_TITLES = "[{\"fn\":\"activeviewport\", \"fstLn\":\"===== Sample: Determine Active Viewport =====\", \"title\":\"Determine Active Viewport\"}, {\"fn\":\"addbackgroundbitmap\", \"fstLn\":\"===== Sample: Add Background Bitmap =====\", \"title\":\"Add Background Bitmap\"}, {\"fn\":\"addbrepbox\", \"fstLn\":\"===== Sample: Add Brep Box =====\", \"title\":\"Add Brep Box\"}, {\"fn\":\"addchildlayer\", \"fstLn\":\"===== Sample: Add Child Layer =====\", \"title\":\"Add Child Layer\"}, {\"fn\":\"addcircle\", \"fstLn\":\"===== Sample: Add Circle =====\", \"title\":\"Add Circle\"}, {\"fn\":\"addclippingplane\", \"fstLn\":\"===== Sample: Add Clipping Plane =====\", \"title\":\"Add Clipping Plane\"}, {\"fn\":\"addcommand\", \"fstLn\":\"======Adding Commands to RhinoCommon projects======\", \"title\":\"Adding Commands to RhinoCommon projects\"}, {\"fn\":\"addcone\", \"fstLn\":\"===== Sample: Add Cone =====\", \"title\":\"Add Cone\"}, {\"fn\":\"addcylinder\", \"fstLn\":\"===== Sample: Add Cylinder to Rhino =====\", \"title\":\"Add Cylinder to Rhino\"}, {\"fn\":\"addlayer\", \"fstLn\":\"===== Sample: Add Layer =====\", \"title\":\"Add Layer\"}, {\"fn\":\"addlayout\", \"fstLn\":\"===== Sample: Add a layout with a detail view =====\", \"title\":\"Add a layout with a detail view\"}, {\"fn\":\"addline\", \"fstLn\":\"===== Sample: Add Line =====\", \"title\":\"Add Line\"}, {\"fn\":\"addlineardimension\", \"fstLn\":\"===== Sample: Add Linear Dimension =====\", \"title\":\"Add Linear Dimension\"}, {\"fn\":\"addlineardimension2\", \"fstLn\":\"===== Sample: Add Linear Dimension2 =====\", \"title\":\"Add Linear Dimension2\"}, {\"fn\":\"addmaterial\", \"fstLn\":\"===== Sample: Add a basic Material =====\", \"title\":\"Add a basic Material\"}, {\"fn\":\"addmesh\", \"fstLn\":\"===== Sample: Add Mesh =====\", \"title\":\"Add Mesh\"}, {\"fn\":\"addnamedview\", \"fstLn\":\"===== Sample: Add Named View =====\", \"title\":\"Add Named View\"}, {\"fn\":\"addnurbscircle\", \"fstLn\":\"===== Sample: Add NURBS Circle =====\", \"title\":\"Add NURBS Circle\"}, {\"fn\":\"addnurbscurve\", \"fstLn\":\"===== Sample: Add NURBS Curve =====\", \"title\":\"Add NURBS Curve\"}, {\"fn\":\"addobjectstogroup\", \"fstLn\":\"===== Sample: Add Objects to Group =====\", \"title\":\"Add Objects to Group\"}, {\"fn\":\"addradialdimension\", \"fstLn\":\"===== Sample: Create Radial Dimensions =====\", \"title\":\"Create Radial Dimensions\"}, {\"fn\":\"addsphere\", \"fstLn\":\"===== Sample: Add Sphere =====\", \"title\":\"Add Sphere\"}, {\"fn\":\"addtext\", \"fstLn\":\"===== Sample: Add Text =====\", \"title\":\"Add Text\"}, {\"fn\":\"addtexture\", \"fstLn\":\"===== Sample: Add Texture =====\", \"title\":\"Add Texture\"}, {\"fn\":\"addtorus\", \"fstLn\":\"===== Sample: Add Torus =====\", \"title\":\"Add Torus\"}, {\"fn\":\"addtruncatedcone\", \"fstLn\":\"===== Sample: Add Truncated Cone =====\", \"title\":\"Add Truncated Cone\"}, {\"fn\":\"advanceddisplay\", \"fstLn\":\"===== Sample: Modifying Advanced Display Settings =====\", \"title\":\"Modifying Advanced Display Settings\"}, {\"fn\":\"analysismode\", \"fstLn\":\"===== Sample: Visual Analysis Modes =====\", \"title\":\"Visual Analysis Modes\"}, {\"fn\":\"arclengthpoint\", \"fstLn\":\"===== Sample: Find point on curve at distance =====\", \"title\":\"Find point on curve at distance\"}, {\"fn\":\"arraybydistance\", \"fstLn\":\"===== Sample: Array By Distance Command =====\", \"title\":\"Array By Distance Command\"}, {\"fn\":\"blockinsertionpoint\", \"fstLn\":\"===== Sample: Obtain insertion point of block =====\", \"title\":\"Obtain insertion point of block\"}, {\"fn\":\"booleandifference\", \"fstLn\":\"===== Sample: Boolean Difference =====\", \"title\":\"Boolean Difference\"}, {\"fn\":\"circlecenter\", \"fstLn\":\"===== Sample: Find Circles and their centers =====\", \"title\":\"Find Circles and their centers\"}, {\"fn\":\"closestpoint\", \"fstLn\":\"===== Sample: Closest Point Calculations with an RTree =====\", \"title\":\"Closest Point Calculations with an RTree\"}, {\"fn\":\"commandlineoptions\", \"fstLn\":\"===== Sample: Add Command Line Options =====\", \"title\":\"Add Command Line Options\"}, {\"fn\":\"conduitarrowheads\", \"fstLn\":\"====== Sample: Drawing Arrowheads in a Display Conduit ======\", \"title\":\"Drawing Arrowheads in a Display Conduit\"}, {\"fn\":\"conduitbitmap\", \"fstLn\":\"===== Sample: draw a non-square bitmap in a display conduit =====\", \"title\":\"draw a non-square bitmap in a display conduit\"}, {\"fn\":\"constrainedcopy\", \"fstLn\":\"===== Sample: Use a constrained GetPoint to copy a curve =====\", \"title\":\"Use a constrained GetPoint to copy a curve\"}, {\"fn\":\"copygroups\", \"fstLn\":\"===== Sample: Duplicating Objects With Group =====\", \"title\":\"Duplicating Objects With Group\"}, {\"fn\":\"createblock\", \"fstLn\":\"===== Sample: Add a block definition =====\", \"title\":\"Add a block definition\"}, {\"fn\":\"createmeshfrombrep\", \"fstLn\":\"===== Sample: Create Meshes from Brep =====\", \"title\":\"Create Meshes from Brep\"}, {\"fn\":\"createsurfaceexample\", \"fstLn\":\"===== Sample: Create a NURBS Surface =====\", \"title\":\"Create a NURBS Surface\"}, {\"fn\":\"crvdeviation\", \"fstLn\":\"===== Sample: Determine the Deviation between two Curves =====\", \"title\":\"Determine the Deviation between two Curves\"}, {\"fn\":\"curveboundingbox\", \"fstLn\":\"===== Sample: Curve Bounding Box (world and plane oriented) =====\", \"title\":\"Curve Bounding Box (world and plane oriented)\"}, {\"fn\":\"curvebrepbox\", \"fstLn\":\"===== Sample: Create a Box From a Curve's Bounding Box =====\", \"title\":\"Create a Box From a Curve's Bounding Box\"}, {\"fn\":\"curveclosestpoint\", \"fstLn\":\"===== Sample: Find the Parameter of a Curve at a Point =====\", \"title\":\"Find the Parameter of a Curve at a Point\"}, {\"fn\":\"curvereverse\", \"fstLn\":\"===== Sample: Reversing the Direction of Curves =====\", \"title\":\"Reversing the Direction of Curves\"}, {\"fn\":\"curvesurfaceintersect\", \"fstLn\":\"===== Sample: Curve-Surface Intersection =====\", \"title\":\"Curve-Surface Intersection\"}, {\"fn\":\"customgeometryfilter\", \"fstLn\":\"===== Sample: Create a specialized GetObject with a Custom Geometry Filter =====\", \"title\":\"Create a specialized GetObject with a Custom Geometry Filter\"}, {\"fn\":\"customundo\", \"fstLn\":\"===== Sample: Custom Undo =====\", \"title\":\"Custom Undo\"}, {\"fn\":\"deleteblock\", \"fstLn\":\"===== Sample: Delete Instance Definition (Block) =====\", \"title\":\"Delete Instance Definition (Block)\"}, {\"fn\":\"determinecurrentlanguage\", \"fstLn\":\"===== Sample: Determine Rhino's Language Setting =====\", \"title\":\"Determine Rhino's Language Setting\"}, {\"fn\":\"dimstyle\", \"fstLn\":\"===== Sample: Modifying an Annotation Object's Dimension Style =====\", \"title\":\"Modifying an Annotation Object's Dimension Style\"}, {\"fn\":\"displayconduit\", \"fstLn\":\"====== Display Conduit Introduction for RhinoCommon ======\", \"title\":\"Display Conduit Introduction for RhinoCommon\"}, {\"fn\":\"displayorder\", \"fstLn\":\"===== Sample: Display Order =====\", \"title\":\"Display Order\"}, {\"fn\":\"displayprecision\", \"fstLn\":\"===== Sample: Changing Display Precision =====\", \"title\":\"Changing Display Precision\"}, {\"fn\":\"dividebylength\", \"fstLn\":\"===== Sample: Divide Curve By Length =====\", \"title\":\"Divide Curve By Length\"}, {\"fn\":\"dividecurvesegments\", \"fstLn\":\"===== Sample: Divide a Curve by Segments =====\", \"title\":\"Divide a Curve by Segments\"}, {\"fn\":\"dividecurvestraight\", \"fstLn\":\"===== Sample: Divide a curve using equi-distance points =====\", \"title\":\"Divide a curve using equi-distance points\"}, {\"fn\":\"dotneteventwatcher\", \"fstLn\":\"====== UI Controls and multiple threads (Event Watchers) ======\", \"title\":\"UI Controls and multiple threads (Event Watchers)\"}, {\"fn\":\"dotnetexpresseditions\", \"fstLn\":\"====== Make Plug-ins with Visual Studio Express 2013 for Windows Desktop ======\", \"title\":\"Make Plug-ins with Visual Studio Express 2013 for Windows Desktop\"}, {\"fn\":\"dotnetmovepointobjects\", \"fstLn\":\"===== Sample: Moving Point Objects =====\", \"title\":\"Moving Point Objects\"}, {\"fn\":\"dotnetmovepointobjectsnonuniform\", \"fstLn\":\"===== Sample: Moving Point Objects (non-uniform) =====\", \"title\":\"Moving Point Objects (non-uniform)\"}, {\"fn\":\"drawoverlay\", \"fstLn\":\"===== Sample: Display conduit to draw overlay text =====\", \"title\":\"Display conduit to draw overlay text\"}, {\"fn\":\"drawstring\", \"fstLn\":\"===== Sample: Dynamically Drawing Text Strings =====\", \"title\":\"Dynamically Drawing Text Strings\"}, {\"fn\":\"dupborder\", \"fstLn\":\"===== Sample: Duplicate borders of a surface =====\", \"title\":\"Duplicate borders of a surface\"}, {\"fn\":\"duplicateobject\", \"fstLn\":\"===== Sample: Clone (Copy, Duplicate) a Rhino Object =====\", \"title\":\"Clone (Copy, Duplicate) a Rhino Object\"}, {\"fn\":\"dupmeshboundary\", \"fstLn\":\"===== Sample: Bounding Polyline from Naked Edges of Open Mesh =====\", \"title\":\"Bounding Polyline from Naked Edges of Open Mesh\"}, {\"fn\":\"edgesrf\", \"fstLn\":\"===== Sample: Create a Surface from Edge Curves=====\", \"title\":\"Create a Surface from Edge Curves\"}, {\"fn\":\"edittext\", \"fstLn\":\"===== Sample: Edit Text =====\", \"title\":\"Edit Text\"}, {\"fn\":\"elevation\", \"fstLn\":\"===== Sample: Furthest Z value on Surface for Given X,Y =====\", \"title\":\"Furthest Z value on Surface for Given X,Y\"}, {\"fn\":\"evnormal\", \"fstLn\":\"===== Sample: Determining the Normal Direction of a Brep Face =====\", \"title\":\"Determining the Normal Direction of a Brep Face\"}, {\"fn\":\"explodehatch\", \"fstLn\":\"===== Sample: Explode a Hatch =====\", \"title\":\"Explode a Hatch\"}, {\"fn\":\"extendcurve\", \"fstLn\":\"===== Sample: Extend a Curve Object =====\", \"title\":\"Extend a Curve Object\"}, {\"fn\":\"extendsurface\", \"fstLn\":\"===== Sample: Extend a Surface =====\", \"title\":\"Extend a Surface\"}, {\"fn\":\"extractisocurve\", \"fstLn\":\"===== Sample: Extracting Isoparametric Curves from Surfaces =====\", \"title\":\"Extracting Isoparametric Curves from Surfaces\"}, {\"fn\":\"extractthumbnail\", \"fstLn\":\"===== Sample: Extract Preview Image =====\", \"title\":\"Extract Preview Image\"}, {\"fn\":\"facearea\", \"fstLn\":\"===== Sample: Brep Face Area =====\", \"title\":\"Brep Face Area\"}, {\"fn\":\"filletcurves\", \"fstLn\":\"===== Sample: fillet curves =====\", \"title\":\"fillet curves\"}, {\"fn\":\"findobjectsbyname\", \"fstLn\":\"===== Sample: Find Objects by Name =====\", \"title\":\"Find Objects by Name\"}, {\"fn\":\"getangle\", \"fstLn\":\"===== Sample: Interactively Pick an Angle =====\", \"title\":\"Interactively Pick an Angle\"}, {\"fn\":\"getedgefaces\", \"fstLn\":\"===== Sample: Getting Brep Faces from Edges =====\", \"title\":\"Getting Brep Faces from Edges\"}, {\"fn\":\"getobjectoption\", \"fstLn\":\"====== Object Selection with Command Line Options ======\", \"title\":\"Object Selection with Command Line Options\"}, {\"fn\":\"getpointdynamicdraw\", \"fstLn\":\"===== Sample: Dynamically Draw Geometry when Picking Points =====\", \"title\":\"Dynamically Draw Geometry when Picking Points\"}, {\"fn\":\"gettext\", \"fstLn\":\"===== Sample: Retrieving Annotation Text =====\", \"title\":\"Retrieving Annotation Text\"}, {\"fn\":\"getuuid\", \"fstLn\":\"===== Sample: Get an Object's UUID =====\", \"title\":\"Get an Object's UUID\"}, {\"fn\":\"gumball\", \"fstLn\":\"===== Sample: Add Gumball =====\", \"title\":\"Add Gumball\"}, {\"fn\":\"hatchcurve\", \"fstLn\":\"===== Sample: Creating a hatch from a curve =====\", \"title\":\"Creating a hatch from a curve\"}, {\"fn\":\"instancedefinitionobjects\", \"fstLn\":\"===== Sample: List Block Definition Geometry =====\", \"title\":\"List Block Definition Geometry\"}, {\"fn\":\"intersectcurves\", \"fstLn\":\"===== Sample: Calculate Curve Intersections =====\", \"title\":\"Calculate Curve Intersections\"}, {\"fn\":\"intersectlinecircle\", \"fstLn\":\"===== Sample: Intersecting a Line with a Circle =====\", \"title\":\"Intersecting a Line with a Circle\"}, {\"fn\":\"intersectlines\", \"fstLn\":\"===== Sample: Intersecting line curves =====\", \"title\":\"Intersecting line curves\"}, {\"fn\":\"intertknot\", \"fstLn\":\"===== Sample: Insert knot in curve =====\", \"title\":\"Insert knot in curve\"}, {\"fn\":\"isbrepbox\", \"fstLn\":\"===== Sample: IsBrepBox Test=====\", \"title\":\"IsBrepBox Test\"}, {\"fn\":\"isocurvedensity\", \"fstLn\":\"===== Sample: Adjusting Surface Isocurve Density =====\", \"title\":\"Adjusting Surface Isocurve Density\"}, {\"fn\":\"issurfaceinplane\", \"fstLn\":\"===== Sample: Is Surface In Plane =====\", \"title\":\"Is Surface In Plane\"}, {\"fn\":\"leader\", \"fstLn\":\"===== Sample: Creating a Leader =====\", \"title\":\"Creating a Leader\"}, {\"fn\":\"locklayer\", \"fstLn\":\"===== Sample: Lock a Layer =====\", \"title\":\"Lock a Layer\"}, {\"fn\":\"loft\", \"fstLn\":\"===== Sample: Loft Surfaces =====\", \"title\":\"Loft Surfaces\"}, {\"fn\":\"makerhinocontours\", \"fstLn\":\"===== Sample: Create Contour Curves =====\", \"title\":\"Create Contour Curves\"}, {\"fn\":\"maximizeview\", \"fstLn\":\"===== Sample: Maximize a View =====\", \"title\":\"Maximize a View\"}, {\"fn\":\"meshdrawing\", \"fstLn\":\"===== Sample: Mesh Drawing =====\", \"title\":\"Mesh Drawing\"}, {\"fn\":\"meshvolume\", \"fstLn\":\"===== Sample: Volume of Meshes =====\", \"title\":\"Volume of Meshes\"}, {\"fn\":\"modifylightcolor\", \"fstLn\":\"===== Sample: Modify a Light's Color =====\", \"title\":\"Modify a Light's Color\"}, {\"fn\":\"modifyobjectcolor\", \"fstLn\":\"===== Sample: Modify an Object's Color =====\", \"title\":\"Modify an Object's Color\"}, {\"fn\":\"mousecallback\", \"fstLn\":\"===== Sample: handle mouse events =====\", \"title\":\"handle mouse events\"}, {\"fn\":\"moveobjectstocurrentlayer\", \"fstLn\":\"===== Sample: Move Selected Objects to Current Layer =====\", \"title\":\"Move Selected Objects to Current Layer\"}, {\"fn\":\"nurbscurveincreasedegree\", \"fstLn\":\"===== Sample: Increasing the degree of a Nurbs curve =====\", \"title\":\"Increasing the degree of a Nurbs curve\"}, {\"fn\":\"nurbssurfaceincreasedegree\", \"fstLn\":\"===== Sample: Increasing the degree of a Nurbs surface =====\", \"title\":\"Increasing the degree of a Nurbs surface\"}, {\"fn\":\"objectdecoration\", \"fstLn\":\"===== Sample: Adding Arrowheads to Curves =====\", \"title\":\"Adding Arrowheads to Curves\"}, {\"fn\":\"objectdisplaymode\", \"fstLn\":\"===== Sample: Set object display mode =====\", \"title\":\"Set object display mode\"}, {\"fn\":\"objectiterator\", \"fstLn\":\"===== Sample: Iterate through Rhino's Geometry Table =====\", \"title\":\"Iterate through Rhino's Geometry Table\"}, {\"fn\":\"objectlayer\", \"fstLn\":\"===== Sample: Determine Object's Layer =====\", \"title\":\"Determine Object's Layer\"}, {\"fn\":\"objectname\", \"fstLn\":\"===== Sample: Modify an Object's Name =====\", \"title\":\"Modify an Object's Name\"}, {\"fn\":\"offsetcurve\", \"fstLn\":\"===== Sample: Offset a Curve =====\", \"title\":\"Offset a Curve\"}, {\"fn\":\"orientonsrf\", \"fstLn\":\"===== Sample: Orient On Surface =====\", \"title\":\"Orient On Surface\"}, {\"fn\":\"ortho\", \"fstLn\":\"===== Sample: Enabling Orthogonal Mode =====\", \"title\":\"Enabling Orthogonal Mode\"}, {\"fn\":\"pickobject\", \"fstLn\":\"===== Sample: Pick Points in a Conduit =====\", \"title\":\"Pick Points in a Conduit\"}, {\"fn\":\"pickpoint\", \"fstLn\":\"===== Sample: Pick and Select Point Objects =====\", \"title\":\"Pick and Select Point Objects\"}, {\"fn\":\"planesurface\", \"fstLn\":\"===== Sample: Create a Plane Surface =====\", \"title\":\"Create a Plane Surface\"}, {\"fn\":\"pointatcursor\", \"fstLn\":\"===== Sample: Get the point at the current cursor position =====\", \"title\":\"Get the point at the current cursor position\"}, {\"fn\":\"prepostpick\", \"fstLn\":\"===== Sample: Pre-Pick and Post-Pick Objects =====\", \"title\":\"Pre-Pick and Post-Pick Objects\"}, {\"fn\":\"principalcurvature\", \"fstLn\":\"===== Sample: Calculate Surface Curvature =====\", \"title\":\"Calculate Surface Curvature\"}, {\"fn\":\"printinstancedefinitions\", \"fstLn\":\"===== Sample: Print Instance Definition (Block) Names =====\", \"title\":\"Print Instance Definition (Block) Names\"}, {\"fn\":\"printinstancedefinitiontree\", \"fstLn\":\"===== Sample: Print Instance Definition (Block) Tree =====\", \"title\":\"Print Instance Definition (Block) Tree\"}, {\"fn\":\"projectpointstobreps\", \"fstLn\":\"===== Sample: Projecting Points to Breps =====\", \"title\":\"Projecting Points to Breps\"}, {\"fn\":\"projectpointstomeshes\", \"fstLn\":\"===== Sample: Projecting points to Meshes =====\", \"title\":\"Projecting points to Meshes\"}, {\"fn\":\"renameblock\", \"fstLn\":\"===== Sample: Rename Instance Definition (Block) =====\", \"title\":\"Rename Instance Definition (Block)\"}, {\"fn\":\"renamelayer\", \"fstLn\":\"===== Sample: Rename a Layer =====\", \"title\":\"Rename a Layer\"}, {\"fn\":\"reparameterizecrv\", \"fstLn\":\"===== Sample: Reparamemterize a Curve =====\", \"title\":\"Reparamemterize a Curve\"}, {\"fn\":\"replacecolordialog\", \"fstLn\":\"===== Sample: Replace Rhino's Color Picking Dialog =====\", \"title\":\"Replace Rhino's Color Picking Dialog\"}, {\"fn\":\"replacehatchpattern\", \"fstLn\":\"===== Sample: Replacing a Hatch Object's Pattern =====\", \"title\":\"Replacing a Hatch Object's Pattern\"}, {\"fn\":\"rhinogettransform\", \"fstLn\":\"===== Sample: Showing Dynamic Object Transformations =====\", \"title\":\"Showing Dynamic Object Transformations\"}, {\"fn\":\"rhinopageviewwidthheight\", \"fstLn\":\"===== Set RhinoPageView Width and Height =====\", \"title\":\"Set RhinoPageView Width and Height\"}, {\"fn\":\"screencaptureview\", \"fstLn\":\"===== Sample: Screen Capture a Viewport =====\", \"title\":\"Screen Capture a Viewport\"}, {\"fn\":\"selectgroupobject\", \"fstLn\":\"===== Sample: Select Objects in an Object's Groups =====\", \"title\":\"Select Objects in an Object's Groups\"}, {\"fn\":\"sellayer\", \"fstLn\":\"===== Sample: Select objects on layer =====\", \"title\":\"Select objects on layer\"}, {\"fn\":\"setactiveview\", \"fstLn\":\"===== Sample: Get and Set the Active View =====\", \"title\":\"Get and Set the Active View\"}, {\"fn\":\"setviewname\", \"fstLn\":\"===== Sample: Setting a Viewport's Title =====\", \"title\":\"Setting a Viewport's Title\"}, {\"fn\":\"sortpoints\", \"fstLn\":\"===== Sample: Sorting 3D points =====\", \"title\":\"Sorting 3D points\"}, {\"fn\":\"splitbrepwithplane\", \"fstLn\":\"===== Sample: Split a Set of BReps with a Plane =====\", \"title\":\"Split a Set of BReps with a Plane\"}, {\"fn\":\"srfpt\", \"fstLn\":\"===== Sample: Surface from Corner Points =====\", \"title\":\"Surface from Corner Points\"}, {\"fn\":\"sweep1\", \"fstLn\":\"===== Sample: Sweeping Surfaces using Sweep1 =====\", \"title\":\"Sweeping Surfaces using Sweep1\"}, {\"fn\":\"textjustify\", \"fstLn\":\"===== Sample: Justifying Text Entities =====\", \"title\":\"Justifying Text Entities\"}, {\"fn\":\"tightboundingbox\", \"fstLn\":\"===== Sample: Tight Bounding Boxes for Breps =====\", \"title\":\"Tight Bounding Boxes for Breps\"}, {\"fn\":\"transformbrep\", \"fstLn\":\"===== Sample: Transform a Brep =====\", \"title\":\"Transform a Brep\"}, {\"fn\":\"unrollsurface\", \"fstLn\":\"===== Sample: Unrolling a developable surface =====\", \"title\":\"Unrolling a developable surface\"}, {\"fn\":\"unrollsurface2\", \"fstLn\":\"===== Sample: Unroll developable surface and associated mesh =====\", \"title\":\"Unroll developable surface and associated mesh\"}, {\"fn\":\"userdata\", \"fstLn\":\"====== Info: RhinoCommon object plug-in user data ======\", \"title\":\"Info: RhinoCommon object plug-in user data\"}, {\"fn\":\"viewportresolution\", \"fstLn\":\"===== Sample: Print Active Viewport Resolution =====\", \"title\":\"Print Active Viewport Resolution\"}, {\"fn\":\"zoomtoobject\", \"fstLn\":\"===== Sample: Zoom to a Selected Object =====\", \"title\":\"Zoom to a Selected Object\"}]";

    // see map_titles_to_samples_as_json.linq file
    private const string JSON_SAMPLE_TITLES = "[{\"fn\":\"addmeshbox\", \"fstLn\":\"\", \"title\":\"Add Mesh Box\"}, {\"fn\":\"boxshell\", \"fstLn\":\"\", \"title\":\"Box Shell\"}, {\"fn\":\"conduitdrawshadedmesh\", \"fstLn\":\"\", \"title\":\"Conduit Draw Shaded Mesh\"}, {\"fn\":\"createspiral\", \"fstLn\":\"\", \"title\":\"Create Spiral\"}, {\"fn\":\"custompython\", \"fstLn\":\"\", \"title\":\"Custom Python\"}, {\"fn\":\"dividebysegments\", \"fstLn\":\"\", \"title\":\"Divide By Segments\"}, {\"fn\":\"exportcontrolpoints\", \"fstLn\":\"\", \"title\":\"Export Control Points\"}, {\"fn\":\"extractrendermesh\", \"fstLn\":\"\", \"title\":\"Extract Render Mesh\"}, {\"fn\":\"extrudebrepface\", \"fstLn\":\"\", \"title\":\"Extrude Brep Face\"}, {\"fn\":\"getmultiplewithoptions\", \"fstLn\":\"\", \"title\":\"Get Multiple With Options\"}, {\"fn\":\"historyCommand\", \"fstLn\":\"\", \"title\":\"History Command\"}, {\"fn\":\"import3dm\", \"fstLn\":\"\", \"title\":\"Import 3dm\"}, {\"fn\":\"insertknot\", \"fstLn\":\"\", \"title\":\"Insert Knot\"}, {\"fn\":\"instancearchivefilestatus\", \"fstLn\":\"\", \"title\":\"Instance Archive File Status\"}, {\"fn\":\"linebetweencurves\", \"fstLn\":\"\", \"title\":\"Line Between Curves\"}, {\"fn\":\"movecplane\", \"fstLn\":\"\", \"title\":\"Move CPlane\"}, {\"fn\":\"movegripobjects\", \"fstLn\":\"\", \"title\":\"Move Grip Objects\"}, {\"fn\":\"nestedblock\", \"fstLn\":\"\", \"title\":\"Nested Block\"}, {\"fn\":\"objectcolor\", \"fstLn\":\"\", \"title\":\"Object Color\"}, {\"fn\":\"pickobjects\", \"fstLn\":\"\", \"title\":\"Pick Objects\"}, {\"fn\":\"projectpointstomeshesex\", \"fstLn\":\"\", \"title\":\"Project Points to Mesh\"}, {\"fn\":\"selbyusertext\", \"fstLn\":\"\", \"title\":\"Select by User Text\"}, {\"fn\":\"showsurfacedirection\", \"fstLn\":\"\", \"title\":\"Show Surface Direction\"}, {\"fn\":\"singlecolorbackfaces\", \"fstLn\":\"\", \"title\":\"Single Color Back Faces\"}, {\"fn\":\"spacemorph\", \"fstLn\":\"\", \"title\":\"Space Morph\"}, {\"fn\":\"spritedrawing\", \"fstLn\":\"\", \"title\":\"Sprite Drawing\"}, {\"fn\":\"ssget\", \"fstLn\":\"\", \"title\":\"ssget\"}, {\"fn\":\"sweep2\", \"fstLn\":\"\", \"title\":\"Sweep2\"}, {\"fn\":\"tweakcolors\", \"fstLn\":\"\", \"title\":\"Tweak Colors\"}, {\"fn\":\"tweencurve\", \"fstLn\":\"\", \"title\":\"Tween Curve\"}, {\"fn\":\"userdataCommand\", \"fstLn\":\"\", \"title\":\"User Data Command\"}]";

    private static List<SampleTitle> g_sample_titles = null;
  }

  class SampleTitle {
    public string fn {get; set;}
    public string fstLn {get; set;}
    public string title {get; set;}
  }
}
