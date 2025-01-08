using Eto.Drawing;
using Eto.Forms;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Input.Custom;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;

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
      GetObject go = new GetObject();
      go.SetCommandPrompt("Select curves");
      go.GeometryFilter = ObjectType.Curve;
      go.SubObjectSelect = false;
      go.GetMultiple(1, 0);
      if (go.CommandResult() != Result.Success)
        return go.CommandResult();

      ObservableCollection<CurveItem> curves = new ObservableCollection<CurveItem>();
      foreach (ObjRef objref in go.Objects())
      {
        RhinoObject rhinoObj = objref.Object();
        if (null == rhinoObj)
          return Result.Failure;

        if (rhinoObj is CurveObject curveObj)
        {
          CurveItem curveItem = new CurveItem(curveObj);
          curves.Add(curveItem);
        }
      }

      OrderCurvesDialog dialog = new OrderCurvesDialog(curves);
      dialog.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow);
      if (dialog.Result == Result.Success)
      {
        foreach (Guid id in dialog.OrderedIds)
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
      Label label = new Label { Text = "Drag curves to reorder:" };
      TableRow gridRow = CreateGridTableRow();
      return new TableLayout
      {
        Padding = new Padding(8),
        Spacing = new Size(5, 5),
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

      TableRow tableRow = new TableRow(m_grid) { ScaleHeight = true };
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
        GridCell cell = m_grid.GetCellAt(e.Location);
        if (cell.Item != null)
        {
          DataObject data = new DataObject();
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
      GridViewDragInfo info = m_grid.GetDragInfo(e);
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
      GridViewDragInfo info = m_grid.GetDragInfo(e);
      if (info != null && e.Data.Contains(g_drag_type))
      {
        int index = info.InsertIndex;
        if (index >= 0 && e.Data.GetObject(g_drag_type) is int[] source_rows)
        {
          List<CurveItem> data = new List<CurveItem>();
          foreach (int row in source_rows.OrderByDescending(r => r))
          {
            CurveItem item = m_curves[row];
            data.Add(item);
            m_curves.RemoveAt(row);
            if (row < index)
              index--;
          }

          int selectedIndex = index;
          foreach (CurveItem item in data)
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