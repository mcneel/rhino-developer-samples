from scriptcontext import doc

activeViewport = doc.Views.ActiveView.ActiveViewport
print "Name = {0}: Width = {1}, Height = {2}".format(
    activeViewport.Name, activeViewport.Size.Width, activeViewport.Size.Height)
