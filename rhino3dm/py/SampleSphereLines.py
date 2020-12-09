# spherelines.py
# Sample script that demonstrates how to use Rhino3dm.py
# https://github.com/mcneel/rhino3dm
# This sample creates a 3dm file containing a group of randomly
# distributed lines defining an sphere.

import rhino3dm
import random
import math
import os

# Initial parameters
theta_min = 0.0
theta_max = math.pi
alpha_min = 0.0
alpha_max = math.pi * 2.0
sphere_radius = 100.0
num_lines = 1500

# Create a center point
center_pt = rhino3dm.Point3d(0.0, 0.0, 0.0)

# Create a File3dm object
model = rhino3dm.File3dm()

for i in range(num_lines):
	# Calculate random line end point
	random.seed(i * 100)
	theta = random.uniform(theta_min, theta_max)
	alpha = random.uniform(alpha_min, alpha_max)
	x = sphere_radius * math.sin(theta) * math.cos(alpha)
	y = sphere_radius * math.sin(theta) * math.sin(alpha)
	z = sphere_radius * math.cos(theta)
	end_pt = rhino3dm.Point3d(x, y, z)
	# Create line curve
	line_curve = rhino3dm.LineCurve(center_pt, end_pt)
	# Add to model
	model.Objects.AddCurve(line_curve)

# Full path to 3dm file to save
desktop = os.path.join(os.path.join(os.environ['USERPROFILE']), 'Desktop') 
filename = 'spherelines.3dm'
path = os.path.join(desktop, filename)

# Write model to disk
model.Write(path, 6)