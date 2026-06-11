---
description: Build, load, and run every updated Rhino sample plugin loadable on the current OS through the Rhino MCP, fixing bugs as we go
---

# Verify Rhino sample plugins

Build each sample plugin in this repo and then load it into the MCP-driven Rhino (for the rhino version that matches the lowest root branch e.g rhino-9x.) via `run_csharp`, run every command it registers, and have the result confirmed. The goal is catching runtime breakage a clean compile misses (commands that throw, ported APIs that misbehave, dead UI).

This command is cross-platform: it works whether the driven Rhino is on Windows or macOS. Detect the host OS first (see **Scope**) and let that decide which projects are in scope — never assume a platform.

`$ARGUMENTS` optionally narrows the run to a project name or path substring (e.g. `SampleCsEto`). If empty, do every loadable updated project.

## Choose mode

Before any work, ask with AskUserQuestion which mode:

- **Unattended (recommended for batches)** — verify programmatically: pre-stage geometry via the MCP, pre-select objects (`doc.Objects.Select(id)`) so `GetObject` prompts auto-resolve, then assert correctness from doc state (object counts, geometry, layers, return values). Only escalate to the user for genuinely visual artifacts (conduit overlay, dynamic-draw preview, display/colour change, dialog layout).
- **Interactive** — run each command, give a one-line "you should see X", and ask "Look right?" after each.

Pick once and stick to it unless the user changes it.

## Scope (depends on host OS)

First determine the OS of the running Rhino (`run_csharp` → `System.Environment.OSVersion.Platform`, or `Rhino.Runtime.HostUtils.RunningOnWindows` / `RunningOnOSX`). The OS decides what's loadable:

- **On Windows** — everything is in scope. Build Windows-only projects (`net9.0-windows`, `UseWindowsForms`/`UseWPF`, `RhinoWindows`) for the `net9.0-windows` TFM, and cross-platform projects for `net9.0`.
- **On macOS** — only **cross-platform** samples load. Include a project if its `<TargetFrameworks>` contains `net9.0` (not just `net9.0-windows`) AND it does NOT set `UseWindowsForms`/`UseWPF` or reference `RhinoWindows`. Skip-and-note anything Windows-only; record it in the wrap-up as "skipped: Windows-only, can't load on this OS" — do not attempt to build it.
- **Either OS** — console/exe projects (e.g. SampleGhFileAnalysis, SampleCsConsole) aren't loadable into Rhino — note and skip (or run standalone only if asked).

Pick the **TFM** per project accordingly: `net9.0-windows` for Windows-only projects (Windows host only), `net9.0` otherwise.

## Setup

1. For each, read the `.csproj`: classify loadable vs Windows-only vs console (rules above). TodoWrite one item per loadable project.
2. Inventory each loadable plugin's commands: grep its sources for classes deriving from `Rhino.Commands.Command` and collect every `EnglishName` override string. (After load you can cross-check via `Rhino.Commands.Command` registration, but source is the source of truth.)

## Build

Per project, from the repo root, building the TFM chosen for that project in **Scope** (`<TFM>` = `net9.0`, or `net9.0-windows` for Windows-only projects on a Windows host):

```
dotnet build <proj.csproj> -c Release -f <TFM>
```

- `-p:EnableWindowsTargeting=true` is required only when building a `net9.0-windows` project from a **non-Windows** host (a build-check, since it won't load there); on Windows it's unnecessary.
- On failure: diagnose, fix the source/csproj, rebuild. Common migration breakage is listed under **Known bug patterns**.
- On success: the artifact is `<projdir>/bin/Release/<TFM>/<Name>.rhp` (a few legacy projects emit to `bin/<TFM>/`; `<TargetExt>` is `.rhp`, GH outputs `.gha`). Capture the exact path; use the platform's path conventions (don't hard-code `/`-paths).

## Load

Load the built `.rhp` into Rhino via `run_csharp`. Note: in Rhino 9 `LoadPlugIn` returns a `Rhino.PlugIns.LoadPlugInResult` enum (not `bool`), and the `run_csharp` snippet has no implicit `using`s — fully-qualify BCL types (`System.Guid`, `System.Exception`) or it won't compile:

```cs
System.Guid id;
var res = Rhino.PlugIns.PlugIn.LoadPlugIn(@"<abs path to .rhp>", out id);
Rhino.RhinoApp.WriteLine($"load={res} id={id}");
```

- `LoadPlugIn` returning anything other than `Success` / throwing is a finding — diagnose (bad dependency, manifest, RhinoCommon version mismatch on the running Rhino).
- `LoadPlugIn` does NOT hot-replace an assembly already loaded this session. After a rebuild of an already-loaded plugin, spawn a fresh Rhino slot (`spawn_slot`) and load the new build there, or restart the default slot.
- Confirm the running Rhino is Rhino 9 first (`run_csharp` → `RhinoApp.Version`); these target RhinoCommon 9.x and won't load on 8.

## Per-command loop

For each command EnglishName in the loaded plugin:

1. TodoWrite: in_progress.
2. **Unattended:** pre-stage the geometry the command expects (curves, surfaces, meshes, layers, blocks) and pre-select objects where it calls `GetObject`/`GetOneObject` so prompts auto-resolve. For pure-logic commands, no staging needed.
3. Run it: `run_command("_<EnglishName>")`. (Custom commands aren't dash-scriptable, so command-line args generally won't feed `GetPoint`/`GetString`.)
4. If a prompt blocks (point/string input that can't be pre-satisfied): send a cancel (`run_command("_Cancel")` / Escape), mark the command **needs-interaction**, and move on. Don't hang the run.
5. On error/exception in output: diagnose → fix source with Edit → rebuild → reload (fresh slot) → re-run.
6. Confirm:
   - **Unattended:** check doc state via `run_csharp` (object counts, geometric properties, layer/material changes, return values). Green → mark done silently. Visual-only artifacts → ask the user.
   - **Interactive:** one-sentence "you should see X", ask "Look right?", wait.
7. TodoWrite: completed.

Unload or use a fresh slot between plugins if command names collide.

## Grasshopper (.gha)

For SampleGhTools / SampleGhTaskCapable / SampleGhPlatonics:

- Get GH's library folder cross-platform (don't hard-code an OS path): start GH (`g1/g2 start`) once, then `run_csharp` → `Grasshopper.Folders.DefaultAssemblyFolder`.
- Copy the built `.gha`s there. **GH only scans libraries at startup**, so after copying spawn a **fresh** Rhino slot and start GH in it — that load picks them up. Do NOT call `ComponentServer.LoadExternalFiles(...)` to rescan at runtime; it can crash Rhino.
- Confirm load: `run_csharp` → check `Grasshopper.Instances.ComponentServer.Libraries` contains each `SampleGh*` and `LoadingExceptions.Count == 0`.
- Then `g1/g2 search_components` to confirm each component registers, and `g1/g2 place_component` + `solve` to exercise one. (GH coverage is lighter than `.rhp` command coverage — confirm registration + a representative solve. Drive inputs by setting `PersistentData` on the component's input params via `run_csharp` when wiring sliders is awkward.)

## Wrap-up

Summarise:
- Plugins built + loaded + commands run/confirmed.
- Bugs fixed (file, command, one-line description).
- Commands marked needs-interaction (and why).
- Projects skipped (Windows-only on a non-Windows host / console) and why.
- The host OS the run was performed on.

Don't commit — leave fixes as working-tree changes for the user to review.
