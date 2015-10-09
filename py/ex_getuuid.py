import rhinoscriptsyntax as rs

obj_id = rs.GetObject("Select object")
if obj_id <> None:
  print "The object's unique id is {0}".format(obj_id)