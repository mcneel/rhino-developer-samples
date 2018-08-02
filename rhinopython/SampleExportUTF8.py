# -*- coding: utf-8 -*-

################################################################################
# SampleExportUTF8.py
# Copyright (c) 2018 Robert McNeel & Associates.
# See License.md in the root of this repository for details.
################################################################################
import rhinoscriptsyntax as rs
import System

# Python is extremely finnicky with non-ascii strings. 
# Explicitly encode all strings to utf-8. 
def SampleExportUTF8():
    
    # extract texts in the model
    textList = []
    for o in rs.AllObjects():
        if rs.IsText(o):
        	# explicitly encode to utf-8
            s = rs.TextObjectText(o).encode('utf-8')
            textList.append(s)
    
    # create a filename variable
    path = System.Environment.GetFolderPath(System.Environment.SpecialFolder.Desktop)
    filename = System.IO.Path.Combine(path, 'SampleExportUTF8.csv');
    
    file = open(filename, 'w')
    
    # create and write a header for the CSV file
    headerList = [u'Index',u'中国',u'English',u'Français']
    
    # explicitly encode to utf-8
    headerList = [i.encode('utf-8') for i in headerList]
    header = u"{}\n".format(u';'.join(headerList))
    file.write(header)
    
    # create and write a line in CSV file for every text in the model
    lineList=[]
    i = 0
    for text in textList:
        line = [str(i),text]
        i += 1
        lineList.append(line)
    
    for line in lineList:
        fileLine = u';'.join(line)+u'\n'
        file.write(fileLine)
    
    file.close()
    
# Check to see if this file is being executed as the "main" python
# script instead of being used as a module by some other python script
# This allows us to use the module which ever way we want.   
if __name__ == "__main__":
    SampleExportUTF8()
