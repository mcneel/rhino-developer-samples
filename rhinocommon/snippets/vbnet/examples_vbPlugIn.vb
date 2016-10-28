'''<summary>
''' Every RhinoCommon plug-in must have one and only one PlugIn derived class.
''' DO NOT create an instance of this class. It is the responsibility of Rhino
''' to create an instance of this class.
'''</summary>
Public Class examples_vbPlugIn
  Inherits Rhino.PlugIns.PlugIn

  Private Shared m_theplugin As examples_vbPlugIn

  Public Sub New()
    m_theplugin = Me
  End Sub

  public Shared ReadOnly Property ThePlugIn() As examples_vbPlugIn
    Get
      Return m_theplugin
    End Get
  End Property

  
End Class
