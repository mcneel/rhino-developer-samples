using System;
using Eto.Forms;
using Rhino.UI;

namespace Project
{
  public enum CommandType { Command1, Command2, Command3, Command4 };

  ///<summary>
  /// The UI implementation of of Section one
  ///</summary>
  public class SectionTwo : Section
  {
    private LocalizeStringPair m_caption;
    private ContextMenu m_context_menu;

    public override LocalizeStringPair Caption
    {
      get { return m_caption; }
    }

    ///<summary>
    /// The Heigth of the section
    ///</summary>
    public override int SectionHeight
    {
      get
      {
        return this.Content.Height;
      }
    }

    ///<summary>
    /// Constructor for SectionTwo
    ///</summary>
    public SectionTwo()
    {
      m_caption = new LocalizeStringPair("Section2", "Section2");
      InitializeComponents();
      InitializeLayout();
      SetupContextMenu();
    }

    private void InitializeComponents()
    {
      // Create contextmenu
      m_context_menu = new ContextMenu();
    }


    private void InitializeLayout()
    {

      TableLayout layout = new TableLayout()
      {
        Padding = 10,
        Rows =
                {
                    "This section implements a contextmenu with commands. Click alternate (usually right) mouse button to open contextmenu."
                }
      };
      Content = layout;
    }

    private void SetupContextMenu()
    {
      ContextMenu = m_context_menu;

      // Add four context menu items
      m_context_menu.Items.Add(new ContextMenuCommand(CommandType.Command1));
      m_context_menu.Items.AddSeparator();
      m_context_menu.Items.Add(new ContextMenuCommand(CommandType.Command2));
      m_context_menu.Items.Add(new ContextMenuCommand(CommandType.Command3));
      m_context_menu.Items.Add(new ContextMenuCommand(CommandType.Command4));

    }
  }

  ///<summary>
  /// ContextMenuCommand class represents a contextmenu item with image, text and executable command
  ///</summary>
  public class ContextMenuCommand : Eto.Forms.Command
  {
    private CommandType m_type;

    public ContextMenuCommand(CommandType type)
    {
      m_type = type;

      if (m_type == CommandType.Command1)
      {
        MenuText = "Command1";
        //Image = add image if needed;
      }

      if (m_type == CommandType.Command2)
      {
        MenuText = "Command2";
        //Image = add image if needed;
      }

      if (m_type == CommandType.Command3)
      {
        MenuText = "Command3";
        //Image = add image if needed;
      }

      if (m_type == CommandType.Command4)
      {
        MenuText = "Command4";
        //Image = add image if needed;
      }
    }

    protected override void OnExecuted(EventArgs e)
    {
      base.OnExecuted(e);
      ExecuteCommand(m_type);
    }

    private void ExecuteCommand(CommandType type)
    {
      if (type == CommandType.Command1)
        MessageBox.Show("Command1", "Command1", MessageBoxType.Information);
      if (type == CommandType.Command2)
        MessageBox.Show("Command2", "Command2", MessageBoxType.Information);
      if (type == CommandType.Command3)
        MessageBox.Show("Command3", "Command3", MessageBoxType.Information);
      if (type == CommandType.Command4)
        MessageBox.Show("Command4", "Command4", MessageBoxType.Information);
    }
  }
}
