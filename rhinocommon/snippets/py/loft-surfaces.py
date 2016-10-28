import rhinoscriptsyntax as rs

def RunCommand():
  crvids = rs.GetObjects(message="select curves to loft", filter=rs.filter.curve, minimum_count=2)
  if not crvids: return

  rs.AddLoftSrf(object_ids=crvids, loft_type = 3) #3 = tight

if __name__ == "__main__":
  RunCommand()
