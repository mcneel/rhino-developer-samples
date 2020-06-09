import System
import scriptcontext as sc
import Rhino.UI
import Eto

class SampleGridViewDialog(Rhino.UI.Forms.CommandDialog):
    def __init__(self, doc):
        self._doc = doc
        self.Title = 'Layers'
        self.Size = Eto.Drawing.Size(350, 350)
        self.ShowHelpButton = False
        self.Content = self.__create_layout()
        
    def __create_layout(self):
        # table
        layout = Eto.Forms.TableLayout()
        layout.Padding = Eto.Drawing.Padding(4)
        layout.Spacing = Eto.Drawing.Size(4, 4)
        # label
        label = Eto.Forms.Label()
        label.Text = 'Turns layers on or off:'
        layout.Rows.Add(label)
        # grid
        grid = self.__create_grid()
        row = Eto.Forms.TableRow(grid)
        row.ScaleHeight = True
        layout.Rows.Add(row)
        return layout
        
    def __create_grid(self):
        # grid
        grid = Eto.Forms.GridView()
        grid.Size = Eto.Drawing.Size(300,400)
        grid.ShowHeader = True
        # column 0
        col0 = Eto.Forms.GridColumn()
        col0.HeaderText = 'Layer'
        col0.DataCell = Eto.Forms.TextBoxCell(0)
        col0.AutoSize = True
        col0.Editable = False
        grid.Columns.Add(col0)
        # column `
        col1 = Eto.Forms.GridColumn()
        col1.HeaderText = 'Visible'
        col1.DataCell = Eto.Forms.CheckBoxCell(1)
        col1.AutoSize = True
        col1.Editable = True
        grid.Columns.Add(col1)
        # datastore
        self.__set_datastore(grid)
        return grid
    
    def __set_datastore(self, control):
        names = []
        visible = []
        for layer in self._doc.Layers:
            names.append(layer.Name)
            visible.append(layer.IsVisible)
        self._collection = [list(i) for i in zip(names, visible)]
        control.DataStore = self._collection
    
    def GetCollection(self):
        return self._collection
        
if __name__ == '__main__':
    dlg = SampleGridViewDialog(sc.doc)
    dlg.ShowModal(Rhino.UI.RhinoEtoApp.MainWindow)
    if dlg.Result == Rhino.Commands.Result.Success:
        print(dlg.GetCollection())