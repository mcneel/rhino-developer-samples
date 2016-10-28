import rhinoscriptsyntax as rs

def RunCommand():
  points = [(1,1,0), (5,1,0), (5,5,0), (9,5,0)]
  rs.AddLeader(points)

if __name__ == "__main__":
    RunCommand()
