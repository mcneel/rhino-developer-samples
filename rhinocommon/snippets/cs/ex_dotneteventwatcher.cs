using System;
using Rhino;
using Rhino.Commands;
using Rhino.DocObjects;
using Rhino.Geometry;
using System.Threading;
using System.Windows;
using System.Windows.Controls;

namespace examples_cs
{
  public class ChangeUiFromDifferentThreadCommand : Command
  {
    public override string EnglishName { get { return "csChangeUIFromDifferentThread"; } }
    private RhinoDoc m_doc;
    private Label m_label;
    private Window m_window;

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      m_doc = doc;

      m_window = new Window {Title = "Object ID and Thread ID", Width = 500, Height = 75};
      m_label = new Label();
      m_window.Content = m_label;
      new System.Windows.Interop.WindowInteropHelper(m_window).Owner = Rhino.RhinoApp.MainWindowHandle();
      m_window.Show();


      // register the rhinoObjectAdded method with the AddRhinoObject event
      RhinoDoc.AddRhinoObject += RhinoObjectAdded;

      // add a sphere from the main UI thread.  All is good
      AddSphere(new Point3d(0,0,0));

      // add a sphere from a secondary thread. Not good: the rhinoObjectAdded method
      // doesn't work well when called from another thread
      var add_sphere_delegate = new Action<Point3d>(AddSphere);
      add_sphere_delegate.BeginInvoke(new Point3d(0, 10, 0), null, null);

      // handle the AddRhinoObject event with rhinoObjectAddedSafe which is
      // desgined to work no matter which thread the call is comming from.
      RhinoDoc.AddRhinoObject -= RhinoObjectAdded;
      RhinoDoc.AddRhinoObject += RhinoObjectAddedSafe;

      // try again adding a sphere from a secondary thread.  All is good!
      add_sphere_delegate.BeginInvoke(new Point3d(0, 20, 0), null, null);

      doc.Views.Redraw();

      return Result.Success;
    }

    private void AddSphere(Point3d center) {
      m_doc.Objects.AddSphere(new Sphere(center, 3));
    }

    private void RhinoObjectAdded(Object sender, RhinoObjectEventArgs e)
    {
      var message = String.Format("thread id = {0}, obj id = {1}",
            Thread.CurrentThread.ManagedThreadId,
            e.ObjectId.ToString());

      RhinoApp.WriteLine(message);

      try {
        // when a sphere is added from a secondary thread this line will
        // throw an exception because UI controls can only be accessed from
        // the main UI thread
        m_label.Content = message;
      } catch (InvalidOperationException ioe) {RhinoApp.WriteLine(ioe.Message);}
    }

    private void RhinoObjectAddedSafe(Object sender, RhinoObjectEventArgs e)
    {
      var message = String.Format("thread id = {0}, obj id = {1}",
            Thread.CurrentThread.ManagedThreadId,
            e.ObjectId.ToString());

      RhinoApp.WriteLine(message);

      // checks if the calling thread is the thread the dispatcher is associated with.
      // In other words, checks if the calling thread is the UI thread
      if (m_label.Dispatcher.CheckAccess())
        // if we're on the UI thread then just update the component
        m_label.Content = message;
      else
      {
        // invoke the setLabelTextDelegate on the thread the dispatcher is associated with, i.e., the UI thread
        var set_label_text_delegate = new Action<string>(txt => m_label.Content = txt);
        m_label.Dispatcher.BeginInvoke(set_label_text_delegate, new String[] { message });
      }
    }
  }
}
