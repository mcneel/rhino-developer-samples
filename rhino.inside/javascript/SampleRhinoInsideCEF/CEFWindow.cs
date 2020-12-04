using CefSharp;
using CefSharp.WinForms;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SampleRhinoInsideCEF
{
    public partial class CEFWindow : Form
    {
        public static ChromiumWebBrowser Browser;
        public Interop Interop;

        public CEFWindow()
        {
            InitializeComponent();

            if (!Cef.IsInitialized)
                InitializeCef();

            // initialise one browser instance
            InitializeBrowser();

            Controls.Add(Browser);
        }

        private void InitializeCef()
        {
            Cef.EnableHighDPISupport();

            var assemblyLocation = Assembly.GetExecutingAssembly().Location;
            var assemblyPath = Path.GetDirectoryName(assemblyLocation);
            var pathSubprocess = Path.Combine(assemblyPath, "CefSharp.BrowserSubprocess.exe");
            CefSharpSettings.LegacyJavascriptBindingEnabled = true;
            CefSharpSettings.ConcurrentTaskExecution = true;
            var settings = new CefSettings
            {
                LogSeverity = LogSeverity.Verbose,
                LogFile = "ceflog.txt",
                BrowserSubprocessPath = pathSubprocess,

            };

            settings.CefCommandLineArgs.Add("allow-file-access-from-files", "1");
            settings.CefCommandLineArgs.Add("disable-web-security", "1");
            Cef.Initialize(settings);
        }

        private void InitializeBrowser()
        {
            var exePath = Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location);
            var projectRootDir = Directory.GetParent(exePath).Parent.FullName;
            var indexPath = Path.Combine(projectRootDir, "SampleRhinoInsideCEFApp", "index.html");

            Browser = new ChromiumWebBrowser(indexPath);

            // Allow the use of local resources in the browser
            Browser.BrowserSettings = new BrowserSettings
            {
                FileAccessFromFileUrls = CefState.Enabled,
                UniversalAccessFromFileUrls = CefState.Enabled
            };

            Browser.Dock = System.Windows.Forms.DockStyle.Fill;

            Interop = new Interop(Browser);
            // Browser.RegisterAsyncJsObject("Interop", Interop);
            Browser.JavascriptObjectRepository.Register("Interop", Interop, isAsync: true, options: BindingOptions.DefaultBinder);
            Browser.IsBrowserInitializedChanged += Browser_IsBrowserInitializedChanged;
            Browser.LoadingStateChanged += Browser_LoadingStateChanged;
        }

        private void Browser_LoadingStateChanged(object sender, LoadingStateChangedEventArgs e)
        {
            Debug.WriteLine("Browser State Changed");
        }

        private void Browser_IsBrowserInitializedChanged(object sender, EventArgs e)
        {
            Debug.WriteLine("Browser Initialized");
            Browser.ShowDevTools();
        }
    }
}
