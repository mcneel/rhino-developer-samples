using System;
using System.Collections.ObjectModel;
using System.Collections.Generic;
using System.Linq;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Eto.Drawing;
using Eto.Forms;
using Rhino.Input.Custom;

namespace SampleCsEto.Commands
{
  /// <summary>
  /// SampleCsEtoOrderCurves command
  /// </summary>
  public class SampleCsEtoOrderCurvesCommand : Rhino.Commands.Command
  {
    public override string EnglishName => "SampleCsEtoOrderCurves";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var go = new GetObject();
      go.SetCommandPrompt("Select curves");
      go.GeometryFilter = ObjectType.Curve;
      go.SubObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      var curves = new ObservableCollection<CurveItem>();
      foreach (var objref in go.Objects())
      {
        var rhinoObj = objref.Object();
        if (null == rhinoObj)
          return Result.Failure;

        if (rhinoObj is CurveObject curveObj)
        {
          var curveItem = new CurveItem(curveObj);
          curves.Add(curveItem);
        }
      }

      var dialog = new OrderCurvesDialog(curves);
      dialog.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow);
      if (dialog.Result == Result.Success)
      {
        foreach (var id in dialog.OrderedIds)
        {
          RhinoApp.WriteLine(id.ToString());
        }
      }

      return Result.Success;
    }
  }

  /// <summary>
  /// OrderCurves view (dialog)
  /// </summary>
  internal sealed class OrderCurvesDialog : Rhino.UI.Forms.CommandDialog
  {
    private readonly ObservableCollection<CurveItem> m_curves;
    private GridView m_grid;
    private PointF? m_mousedown_point;
    private bool m_dragging;
    private static readonly string g_drag_type = typeof(OrderCurvesDialog).FullName;
    private static readonly int g_drag_offset = 4;

    /// <summary>
    /// Public constructor
    /// </summary>
    public OrderCurvesDialog(ObservableCollection<CurveItem> curves)
    {
      m_curves = curves;
      Size = new Size(350, 350);
      Content = CreateLayout();
    }

    /// <summary>
    /// Returns the ids in order.
    /// </summary>
    public Guid[] OrderedIds
    {
      get { return m_curves.Select(c => c.Id).ToArray(); }
    }

    /// <summary>
    /// Create the form layout
    /// </summary>
    private TableLayout CreateLayout()
    {
      var label = new Label { Text = "Drag curves to reorder:" };
      var gridRow = CreateGridTableRow();
      return new TableLayout
      {
        Padding =  new Padding(8),
        Spacing =  new Size(5, 5),
        Rows = { label, gridRow }
      };
    }

    /// <summary>
    /// Create the grid and tablerow
    /// </summary>
    private TableRow CreateGridTableRow()
    {
      m_grid = new GridView
      {
        AllowDrop = true,
        AllowMultipleSelection = true,
        DataStore = m_curves,
        ShowHeader = true
      };

      m_grid.Columns.Add(new GridColumn
      {
        DataCell = new TextBoxCell
        {
          Binding = Binding.Property<CurveItem, string>(c => c.Text)
        },
        AutoSize = true,
        Editable = false,
        HeaderText = "Curve",
        Resizable = true,
        Sortable = false
      });

      m_grid.MouseDown += OnGridViewMouseDown;
      m_grid.MouseMove += OnGridViewMouseMove;
      m_grid.MouseUp += OnGridViewMouseUp;
      m_grid.DragEnter += OnGridViewDragEnter;
      m_grid.DragOver += OnGridViewDragOver;
      m_grid.DragDrop += OnGridViewDragDrop;

      var tableRow = new TableRow(m_grid) { ScaleHeight = true };
      return tableRow;
    }

    /// <summary>
    /// GridView.MouseDown handler
    /// </summary>
    private void OnGridViewMouseDown(object sender, MouseEventArgs e)
    {
      base.OnMouseDown(e);
      m_mousedown_point = e.Location;
      m_dragging = false;
    }

    /// <summary>
    /// GridView.MouseMove handler
    /// </summary>
    private void OnGridViewMouseMove(object sender, MouseEventArgs e)
    {
      base.OnMouseMove(e);
      if (!m_dragging
        && e.Buttons == MouseButtons.Primary
        && m_mousedown_point != null
        && m_mousedown_point.Value.Distance(e.Location) >= g_drag_offset)
      {
        var cell = m_grid.GetCellAt(e.Location);
        if (cell.Item != null)
        {
          var data = new DataObject();
          data.SetObject(m_grid.SelectedRows.ToArray(), g_drag_type);
          m_grid.DoDragDrop(data, DragEffects.Move);
          m_dragging = true;
          e.Handled = true;
        }
      }
    }

    /// <summary>
    /// GridView.MouseUp handler
    /// </summary>
    private void OnGridViewMouseUp(object sender, MouseEventArgs e)
    {
      base.OnMouseUp(e);
      m_mousedown_point = null;
      m_dragging = false;
    }

    /// <summary>
    /// GridView.DragEnter handler
    /// </summary>
    private void OnGridViewDragEnter(object sender, DragEventArgs e)
    {
      base.OnDragEnter(e);
      if (e.Data.Contains(g_drag_type))
        e.Effects = DragEffects.Move;
    }

    /// <summary>
    /// GridView.DragOver handler
    /// </summary>
    private void OnGridViewDragOver(object sender, DragEventArgs e)
    {
      base.OnDragOver(e);
      var info = m_grid.GetDragInfo(e);
      if (info != null && e.Data.Contains(g_drag_type))
      {
        info.RestrictToInsert();
        e.Effects = DragEffects.Move;
      }
    }

    /// <summary>
    /// GridView.DragDrop handler
    /// </summary>
    private void OnGridViewDragDrop(object sender, DragEventArgs e)
    {
      base.OnDragDrop(e);
      var info = m_grid.GetDragInfo(e);
      if (info != null && e.Data.Contains(g_drag_type))
      {
        var index = info.InsertIndex;
        if (index >= 0 && e.Data.GetObject(g_drag_type) is int[] source_rows)
        {
          var data = new List<CurveItem>();
          foreach (var row in source_rows.OrderByDescending(r => r))
          {
            var item = m_curves[row];
            data.Add(item);
            m_curves.RemoveAt(row);
            if (row < index)
              index--;
          }

          var selectedIndex = index;
          foreach (var item in data)
            m_curves.Insert(index++, item);

          m_grid.SelectedRow = selectedIndex;
        }
      }
    }
  }

  /// <summary>
  /// Curve item (plain old CLR object)
  /// </summary>
  internal class CurveItem
  {
    public System.Guid Id { get; }
    public string Text { get; }

    public CurveItem(CurveObject curveObject)
    {
      if (null != curveObject)
      {
        Id = curveObject.Id;
        Text = curveObject.Name;
        if (string.IsNullOrEmpty(Text))
          Text = Id.ToString();
      }
    }
  }
}