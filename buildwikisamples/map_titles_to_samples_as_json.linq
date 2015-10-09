<Query Kind="Statements">
  <Reference Relative="..\..\..\..\bin\Debug\RhinoCommon.dll">C:\dev\rhino\wip\src4\bin\Debug\RhinoCommon.dll</Reference>
  <NuGetReference>Newtonsoft.Json</NuGetReference>
  <NuGetReference>Rx-Main</NuGetReference>
  <NuGetReference>Rx-Testing</NuGetReference>
  <Namespace>System</Namespace>
  <Namespace>System.IO</Namespace>
  <Namespace>System.Reactive</Namespace>
  <Namespace>System.Reactive.Concurrency</Namespace>
  <Namespace>System.Reactive.Disposables</Namespace>
  <Namespace>System.Reactive.Joins</Namespace>
  <Namespace>System.Reactive.Linq</Namespace>
  <Namespace>System.Reactive.PlatformServices</Namespace>
  <Namespace>System.Reactive.Subjects</Namespace>
  <Namespace>System.Reactive.Threading.Tasks</Namespace>
</Query>

var samples_path = @"Z:\src_osx\mcneel.com\RhinoDocsMainGhpSite\developer-rhino3d-com\_samples\rhinocommon";

var missing_titles = Directory.GetFiles(samples_path)
  .Where (fn => Path.GetExtension(fn) == ".md")
  .Select(fn => new KeyValuePair<string, string>(Path.GetFileNameWithoutExtension(fn), File.ReadAllLines(fn).Where (l => l.Contains("__NO_TITLE__")).SingleOrDefault()))
  .Where (kvp => kvp.Value != null)
  .ToDictionary (kvp => kvp.Key, kvp => kvp.Value);
  
missing_titles["addmeshbox"] = "Add Mesh Box";
missing_titles["boxshell"] = "Box Shell"; 
missing_titles["conduitdrawshadedmesh"] = "Conduit Draw Shaded Mesh"; 
missing_titles["createspiral"] = "Create Spiral"; 
missing_titles["custompython"] = "Custom Python"; 
missing_titles["dividebysegments"] = "Divide By Segments"; 
missing_titles["exportcontrolpoints"] = "Export Control Points"; 
missing_titles["extractrendermesh"] = "Extract Render Mesh"; 
missing_titles["extrudebrepface"] = "Extrude Brep Face"; 
missing_titles["getmultiplewithoptions"] = "Get Multiple With Options"; 
missing_titles["historyCommand"] = "History Command"; 
missing_titles["import3dm"] = "Import 3dm"; 
missing_titles["insertknot"] = "Insert Knot"; 
missing_titles["instancearchivefilestatus"] = "Instance Archive File Status"; 
missing_titles["linebetweencurves"] = "Line Between Curves"; 
missing_titles["movecplane"] = "Move CPlane"; 
missing_titles["movegripobjects"] = "Move Grip Objects"; 
missing_titles["nestedblock"] = "Nested Block"; 
missing_titles["objectcolor"] = "Object Color"; 
missing_titles["pickobjects"] = "Pick Objects"; 
missing_titles["projectpointstomeshesex"] = "Project Points to Mesh"; 
missing_titles["selbyusertext"] = "Select by User Text"; 
missing_titles["showsurfacedirection"] = "Show Surface Direction"; 
missing_titles["singlecolorbackfaces"] = "Single Color Back Faces"; 
missing_titles["spacemorph"] = "Space Morph"; 
missing_titles["spritedrawing"] = "Sprite Drawing"; 
missing_titles["ssget"] = "ssget"; 
missing_titles["sweep2"] = "Sweep2"; 
missing_titles["tweakcolors"] = "Tweak Colors"; 
missing_titles["tweencurve"] = "Tween Curve"; 
missing_titles["userdataCommand"] = "User Data Command"; 

missing_titles
  .Where (m => m.Value.Contains("__NO_TITLE__"))
  .Select (m => string.Format("missing_titles[\"{0}\"] = \"__NO_TITLE__\";", m.Key))
  .Dump();
  
var json_titles =
  "\"[" +
  missing_titles
    .Select(kvp => "{" + string.Format("\\\"fn\\\":\\\"{0}\\\", \\\"fstLn\\\":\\\"\\\", \\\"title\\\":\\\"{1}\\\"", 
      kvp.Key, kvp.Value) + "}")
    .Aggregate((current, next) => current + ", " + next)
  + "]\"";
json_titles.Dump();