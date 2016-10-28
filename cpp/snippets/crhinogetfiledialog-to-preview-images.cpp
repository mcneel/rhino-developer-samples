CRhinoCommand::result CCommandTest::RunCommand( const CRhinoCommandContext& context )
{
  ON_wString filename;

  // Prompt for a bitmap filename
  CRhinoGetFileDialog gf;
  gf.SetScriptMode( context.IsInteractive() ? FALSE : TRUE );
  BOOL rc = gf.DisplayFileDialog(
        CRhinoGetFileDialog::open_bitmap_dialog,
        filename,
        CWnd::FromHandle( RhinoApp().MainWnd() )
        );
  if( !rc )
    return cancel;

  // Verify filename
  filename = gf.FileName();
  filename.TrimLeftAndRight();
  if( filename.IsEmpty() )
    return cancel;

  // Verify the file
  if( !CRhinoFileUtilities::FileExists(filename) )
  {
    ON_wString error = L"The specified file was not found.\n";
    if( context.IsInteractive() )
      ::RhinoMessageBox( error, L"Test", MB_OK | MB_ICONEXCLAMATION );
    else
      RhinoApp().Print( error );
    return CRhinoCommand::failure;
  }

  // Verify the bitmap
  CRhinoDib dib;
  if( !dib.ReadFromFile(filename) )
  {
    ON_wString error = L"The specified file cannot be identifed as a supported type.\n";
    if( context.IsInteractive() )
      ::RhinoMessageBox( error, L"Test", MB_OK | MB_ICONEXCLAMATION );
    else
      RhinoApp().Print( error );
    return CRhinoCommand::failure;
  }

  // To do...

  return success;
}
