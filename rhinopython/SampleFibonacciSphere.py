################################################################################
# SampleFibonacciSphere.py -- December 2014
# Copyright (c) Robert McNeel & Associates
################################################################################
import rhinoscriptsyntax as rs
import math, random

def FibonacciSphere(samples=1, randomize=True):
    rnd = 1
    if randomize:
        rnd = random.random() * samples

    points = []
    offset = 2/samples
    increment = math.pi * (3 - math.sqrt(5));

    for i in range(samples):
        y = ((i * offset) - 1) + (offset / 2);
        r = math.sqrt(1 - pow(y,2))

        phi = ((i + rnd) % samples) * increment

        x = math.cos(phi) * r
        z = math.sin(phi) * r

        points.append([x,y,z])

    return points

def SampleFibonacciSphere():
    samples = rs.GetInteger("Number of points", 10, 1)
    if samples is None: return
    rs.EnableRedraw(False)
    rs.AddSphere([0,0,0], 1)
    rs.AddPoints(FibonacciSphere(samples))
    rs.EnableRedraw(True)
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.
if __name__=="__main__":
    SampleFibonacciSphere()