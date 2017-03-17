################################################################################
# SampleExportCurvesAsJSON.py -- March 2017
# Copyright (c) Robert McNeel & Associates
################################################################################
import rhinoscriptsyntax as rs
import json
import time

# Sample code that exports the NURBS form of selected curves
# in some home-brewed JSON format.
def SampleExportCurvesAsJSON():
    
    # Select curves to export
    ids = rs.GetObjects('Select curves to export', 4, True, True)
    if not ids: return
    
    # Name of filename to creat
    fname = rs.SaveFileName('Save', 'JSON File (*.json)|*.json||')
    if not fname: return
    
    # The json data (dictionary)
    data = {}
    
    # The version of this data format
    data['version'] = 1.0
    
    # The Rhino version
    data['rhino'] = rs.ExeVersion()
    
    # The date
    data['date'] = time.strftime('%d/%m/%Y')
    
    # The number of curve records
    data['curve_count'] = len(ids)
    
    # The curve records (list)
    data['curves'] = []
    
    for id in ids: 
    
        # Create a curve record (dictionary)
        rec = {}
        
        # The id
        rec['id'] = id.ToString()
        
        # The dimension
        rec['dim'] = rs.CurveDim(id)
        
        # Is rational
        rec['rational'] = rs.IsCurveRational(id)
        
        # The degree
        rec['degree'] = rs.CurveDegree(id)
        
        # The control point count
        rec['cv_count'] = rs.CurvePointCount(id)
        
        # The control points
        rec['cvs'] = []
        pts = rs.CurvePoints(id)
        wht = rs.CurveWeights(id)
        for i in range(len(pts)):
            pt = pts[i]
            rec['cvs'].append([pt[0], pt[1], pt[2], wht[i]])
        
        # The knot count
        rec['knot_count'] = rs.CurveKnotCount(id)
        
        # The knots
        rec['knots'] = []
        knots = rs.CurveKnots(id)
        for i in range(len(knots)):
            rec['knots'].append(knots[i])
            
        # Some other (unnecessary) properties
        rec['closed'] = rs.IsCurveClosed(id)
        rec['periodic'] = rs.IsCurvePeriodic(id)
        rec['planar'] = rs.IsCurvePlanar(id)
            
        # Append the curve record
        data['curves'].append(rec)
    
    # Write the json data
    with open('c:/users/dale/desktop/data.json', 'w') as outfile:
        json.dump(data, outfile)

# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if( __name__ == '__main__' ):
    SampleExportCurvesAsJSON()