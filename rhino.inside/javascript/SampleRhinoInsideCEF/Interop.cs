using CefSharp;
using CefSharp.WinForms;
using System;
using System.Collections.Generic;
using System.Dynamic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SampleRhinoInsideCEF
{
    public class Interop
    {
        public static ChromiumWebBrowser Browser;
        static SampleRhinoInsideJavascriptTaskManagerLib.RhinoTaskManager rtm;
        public Interop(ChromiumWebBrowser browser) 
        {
            Browser = browser;
            rtm = new SampleRhinoInsideJavascriptTaskManagerLib.RhinoTaskManager();
        }

        public void ShowDev()
        {
            Browser.ShowDevTools();
        }

        public async Task<object> startGrasshopper()
        {
            Func<object, Task<object>> cb = (data) => PushPreview(data.ToString());
            dynamic input = new ExpandoObject();
            input.cb = cb;

            return await rtm.StartGrasshopperTask(input);
        }

        public async Task<object> PushPreview(string data)
        {
            await Browser?.GetMainFrame().EvaluateScriptAsync("onGhObjectAdded(" + data + ");");
            return "Pushed data";
        }
    }
}
