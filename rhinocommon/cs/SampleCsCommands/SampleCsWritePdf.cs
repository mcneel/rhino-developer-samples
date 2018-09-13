using System;
using Rhino;
using Rhino.Commands;
using Rhino.FileIO;

namespace SampleCsCommands
{
  public class SampleCsWritePdf : Command
  {
    public override string EnglishName => "SampleCsWritePdf";

    protected override Result RunCommand(RhinoDoc doc, RunMode mode)
    {
      var pdf = FilePdf.Create();

      var pdf_doc = pdf.PdfDocumentImplementation() as Xfinium.Pdf.PdfFixedDocument;
      if (null == pdf_doc)
        return Result.Failure;

      var cover_page = pdf_doc.Pages.Add();

      using (var font = new System.Drawing.Font("Arial", 30))
      {
        var pdf_font = new Xfinium.Pdf.Graphics.PdfUnicodeTrueTypeFont(font, false) { Underline = true };
        var color = new Xfinium.Pdf.Graphics.PdfRgbColor(0, 0, 0);
        var appearance = new Xfinium.Pdf.Graphics.PdfStringAppearanceOptions(pdf_font, null, new Xfinium.Pdf.Graphics.PdfBrush(color));
        var layout = new Xfinium.Pdf.Graphics.PdfStringLayoutOptions
        {
          HorizontalAlign = Xfinium.Pdf.Graphics.PdfStringHorizontalAlign.Center,
          X = cover_page.Width / 2,
          Y = cover_page.Height / 2
        };
        cover_page.Graphics.DrawString(EnglishName, appearance, layout);
      }

      const int w = (int)(8.5 * 300);
      const int h = 11 * 300;

      var views = doc.Views.GetViewList(true, false);
      for (var i = 0; i < views.Length; i++)
      {
        var view = views[i];
        var viewcapture = new Rhino.Display.ViewCaptureSettings(view, new System.Drawing.Size(w, h), 300);
        var page_index = pdf.AddPage(viewcapture) - 1;
        using (var font = new System.Drawing.Font("Arial", 80))
        {
          var pdf_font = new Xfinium.Pdf.Graphics.PdfUnicodeTrueTypeFont(font, false);
          var color = new Xfinium.Pdf.Graphics.PdfRgbColor(40, 40, 40);
          var pen = new Xfinium.Pdf.Graphics.PdfPen(color, 3);
          var appearance = new Xfinium.Pdf.Graphics.PdfStringAppearanceOptions(pdf_font, pen, null);
          var layout = new Xfinium.Pdf.Graphics.PdfStringLayoutOptions
          {
            HorizontalAlign = Xfinium.Pdf.Graphics.PdfStringHorizontalAlign.Center,
            Rotation = 45,
            X = cover_page.Width / 2,
            Y = cover_page.Height / 2
          };
          pdf_doc.Pages[page_index].Graphics.DrawString("Top Secret", appearance, layout);
        }

        using (var font = new System.Drawing.Font("Arial", 20))
        {
          var pdf_font = new Xfinium.Pdf.Graphics.PdfUnicodeTrueTypeFont(font, false);
          var color = new Xfinium.Pdf.Graphics.PdfRgbColor(0, 0, 0);
          var appearance = new Xfinium.Pdf.Graphics.PdfStringAppearanceOptions(pdf_font, null, new Xfinium.Pdf.Graphics.PdfBrush(color));
          var layout = new Xfinium.Pdf.Graphics.PdfStringLayoutOptions
          {
            HorizontalAlign = Xfinium.Pdf.Graphics.PdfStringHorizontalAlign.Right,
            X = cover_page.Width * 0.9,
            Y = cover_page.Height * 0.9
          };
          pdf_doc.Pages[page_index].Graphics.DrawString($"Page {i + 1} of {views.Length}", appearance, layout);
        }
      }

      var path = Environment.GetFolderPath(Environment.SpecialFolder.DesktopDirectory);
      path = System.IO.Path.Combine(path, EnglishName + ".pdf");
      pdf.Write(path);

      return Result.Success;
    }
  }
}