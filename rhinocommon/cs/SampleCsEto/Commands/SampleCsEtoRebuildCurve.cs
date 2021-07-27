using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input;
using System;

namespace SampleCsEto.Commands
{
  /// <summary>
  /// RebuildCurveArgs class
  /// </summary>
  internal class RebuildCurveArgs
  {
    public int PointCount { get; set; } = 0;
    public int Degree { get; set; } = 0;
    public bool DeleteInput { get; set; } = true;
    public bool PreserveTangents { get; set; } = false;

    public bool IsValid
    {
      get
      {
        if (
             PointCount < 2
          || PointCount > 32767
          || Degree < 1
          || Degree > 11
          )
          return false;
        return true;
      }
    }
  }

  /// <summary>
  /// RebuildCurveDialog dialog class
  /// </summary>
  internal class RebuildCurveDialog : Eto.Forms.Dialog<bool>
  {
    RebuildCurveArgs m_args;
    Eto.Forms.NumericStepper m_point_count_stepper;
    Eto.Forms.NumericStepper m_degree_stepper;
    Eto.Forms.CheckBox m_delete_input_checkbox;
    Eto.Forms.CheckBox m_preserve_tangents_checkbox;

    public RebuildCurveDialog(RebuildCurveArgs args)
    {
      m_args = args ?? throw new ArgumentNullException(nameof(args));

      Title = "Rebuild";
      Padding = new Eto.Drawing.Padding(5);

      var layout = new Eto.Forms.DynamicLayout
      {
        Padding = new Eto.Drawing.Padding(5),
        Spacing = new Eto.Drawing.Size(5, 5)
      };

      layout.AddRow(CreateSteppers());
      layout.AddRow(null); // spacer
      layout.AddRow(CreateCheckBoxes());
      layout.AddRow(null); // spacer
      layout.AddRow(CreateButtons());

      Content = layout;
    }

    public RebuildCurveArgs Results => m_args;

    private Eto.Forms.DynamicLayout CreateSteppers()
    {
      var label0 = new Eto.Forms.Label { Text = "Point count:" };
      var label1 = new Eto.Forms.Label { Text = "Degree:" };
      var label2 = new Eto.Forms.Label { Text = $"({m_args.PointCount})" };
      var label3 = new Eto.Forms.Label { Text = $"({m_args.Degree})" };

      m_point_count_stepper = new Eto.Forms.NumericStepper
      {
        Value = m_args.PointCount,
        MinValue = 2,
        MaxValue = 32767
      };

      m_degree_stepper = new Eto.Forms.NumericStepper
      {
        Value = m_args.Degree,
        MinValue = 1,
        MaxValue = 11
      };

      var layout = new Eto.Forms.DynamicLayout { Spacing = new Eto.Drawing.Size(5, 5) };
      layout.AddRow(label0, label2, m_point_count_stepper);
      layout.AddRow(label1, label3, m_degree_stepper);
      return layout;
    }

    private Eto.Forms.DynamicLayout CreateCheckBoxes()
    {
      m_delete_input_checkbox = new Eto.Forms.CheckBox
      {
        Text = "Delete input",
        Checked = m_args.DeleteInput,
        ThreeState = false
      };

      m_preserve_tangents_checkbox = new Eto.Forms.CheckBox
      {
        Text = "Preserve tangent end directions",
        Checked = m_args.PreserveTangents,
        ThreeState = false
      };

      var layout = new Eto.Forms.DynamicLayout { Spacing = new Eto.Drawing.Size(5, 5) };
      layout.AddRow(m_delete_input_checkbox);
      layout.AddRow(m_preserve_tangents_checkbox);
      return layout;
    }

    private Eto.Forms.DynamicLayout CreateButtons()
    {
      DefaultButton = new Eto.Forms.Button { Text = "OK" };
      DefaultButton.Click += DefaultButton_Click;

      AbortButton = new Eto.Forms.Button { Text = "Cancel" };
      AbortButton.Click += AbortButton_Click;

      var layout = new Eto.Forms.DynamicLayout { Spacing = new Eto.Drawing.Size(5, 5) };
      layout.AddRow(null, DefaultButton, AbortButton, null);
      return layout;
    }

    private void DefaultButton_Click(object sender, EventArgs e)
    {
      m_args.PointCount = (int) m_point_count_stepper.Value;
      m_args.Degree = (int) m_degree_stepper.Value;
      m_args.DeleteInput = (bool) m_delete_input_checkbox.Checked;
      m_args.PreserveTangents = (bool) m_preserve_tangents_checkbox.Checked;
      Close(true);
    }

    private void AbortButton_Click(object sender, EventArgs e)
    {
      Close(false);
    }

  }
  /// <summary>
  /// SampleCsEtoRebuildCurve command class
  /// </summary>
  public class SampleCsEtoRebuildCurve : Command
  {
    public override string EnglishName => "SampleCsEtoRebuildCurve";

    public bool DeleteInput { get; set; } = true;
    public bool PreserveTangents { get; set; } = false;

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var rc = RhinoGet.GetOneObject("Select curve to rebuild", true, ObjectType.Curve, out var objref);
      if (rc != Rhino.Commands.Result.Success)
        return rc;

      var curve = objref.Curve();
      if (null == curve)
        return Result.Failure;

      var nurb = curve.ToNurbsCurve();
      if (null == nurb)
        return Result.Failure;

      var args = new RebuildCurveArgs
      {
        PointCount = nurb.Points.Count,
        Degree = nurb.Degree,
        DeleteInput = DeleteInput,
        PreserveTangents = PreserveTangents
      };

      var dlg = new RebuildCurveDialog(args);
      var res = dlg.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow);
      if (res)
      {
        args = dlg.Results;
        if (args.IsValid)
        {
          nurb = curve.Rebuild(args.PointCount, args.Degree, args.PreserveTangents);
          if (null != nurb)
          {
            if (args.DeleteInput)
              doc.Objects.Replace(objref, nurb);
            else
              doc.Objects.Add(nurb, objref.Object().Attributes);
          }
          DeleteInput = args.DeleteInput;
          PreserveTangents = args.PreserveTangents;
        }
      }

      doc.Views.Redraw();

      return Result.Success;
    }
  }
}