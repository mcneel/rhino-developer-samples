################################################################################
# SampleGardenPath.py
# Copyright (c) 2017 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import rhinoscriptsyntax as rs
import math

# Creates a garden path and fills it with circular tiles.
def SampleGardenPath():
    
    # Acquire information for the garden path
    start_point = rs.GetPoint('Start point of path')
    if start_point is None: return
    
    end_point = rs.GetPoint('End point of path', start_point )
    if end_point is None: return
    
    half_width = rs.GetDistance(start_point, None, 'First width point', 'Half width of path')
    if half_width is None: return
    
    tile_radius = rs.GetReal('Radius of tiles', 1.0)
    if tile_radius is None: return
    if tile_radius <= 0.0: return
    
    tile_spacing = rs.GetReal('Distance between tiles', 1.0)
    if tile_spacing is None: return
    if tile_spacing < 0.0: return
    
	# To increase speed, disable redrawing
    rs.EnableRedraw(False)
    
    # Calculate angles
    angle_rc = rs.Angle(start_point, end_point)
    angle = angle_rc[0]
    length = rs.Distance(start_point, end_point)
    width = half_width * 2
    angle_p90 = angle + 90.0
    angle_m90 = angle - 90.0
  
    # Draw the outline of the path
    polyline = []
    polyline.append(rs.Polar(start_point, angle_m90, half_width))
    polyline.append(rs.Polar(polyline[0], angle, length))
    polyline.append(rs.Polar(polyline[1], angle_p90, width))
    polyline.append(rs.Polar(polyline[2], angle + 180.0, length))
    polyline.append(polyline[0])
    rs.AddPolyline(polyline)
    
    # Draw the rows of tiles
    plane = rs.WorldXYPlane()
    distance = tile_radius + tile_spacing
    offset = 0.0
    
    while (distance <= length - tile_radius):
    
        # Place one row of tiles given polyline along path and possibly offset it
        first = rs.Polar(start_point, angle, distance)
        current = rs.Polar(first, angle_p90, offset)
        next = current
    
        while (rs.Distance(first, next) < half_width - tile_radius):
            plane = rs.MovePlane(plane, next)
            rs.AddCircle(plane, tile_radius)
            next = rs.Polar(next, angle_p90, tile_spacing + tile_radius + tile_radius)
        
        next = rs.Polar(current, angle_m90, tile_spacing + tile_radius + tile_radius)
        
        while (rs.Distance(first, next) < half_width - tile_radius):
          plane = rs.MovePlane(plane, next)
          rs.AddCircle(plane, tile_radius)
          next = rs.Polar(next, angle_m90, tile_spacing + tile_radius + tile_radius)
        
        distance = distance + ((tile_spacing + tile_radius + tile_radius) * math.sin(60.0 * 180.0 / math.pi))
        
        if (offset == 0.0):
            offset = (tile_spacing + tile_radius + tile_radius) * math.cos(60.0 * 180.0 / math.pi)
        else:
            offset = 0.0
    
    rs.EnableRedraw(True)

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__ == "__main__":
    SampleGardenPath() 