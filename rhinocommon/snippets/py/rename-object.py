import rhinoscriptsyntax as rs

obj_id = rs.GetObject("Select object to change name")
object_new_name = rs.GetString("New object name")

rs.ObjectName(obj_id, object_new_name)
