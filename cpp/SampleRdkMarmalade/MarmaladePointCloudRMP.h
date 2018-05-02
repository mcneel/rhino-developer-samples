
#pragma once

#include "MarmaladePlugIn.h"

//1>MarmaladeRdkPlugIn.cpp(62): error C2259: 'CMarmaladeContentIOPlugIn': cannot instantiate abstract class
//1>MarmaladeRdkPlugIn.cpp(62): note: 'UUID CRhRdkExtension::PlugInId(void) const': is abstract
//1>c:\program files\rhino 6 sdk\inc\RhRdkExtension.h(29): note: see declaration of 'CRhRdkExtension::PlugInId'
//1>MarmaladeRdkPlugIn.cpp(95): error C2039: 'Kind': is not a member of 'IRhRdkContentEditor'
//1>c:\program files\rhino 6 sdk\inc\RhRdkPlugIn.h(35): note: see declaration of 'IRhRdkContentEditor'
//1>MarmaladeRdkPlugIn.cpp(95): error C2065: 'RDK_KIND_MATERIAL_is_deprecated___See_CRhRdkContent_Kinds_enum': undeclared identifier
//1>MarmaladeRdkPlugIn.cpp(95): error C2789: 'bEnable': an object of const-qualified type must be initialized
//1>MarmaladeRdkPlugIn.cpp(95): note: see declaration of 'bEnable'
//1>MarmaladeRdkPlugIn.cpp(134): error C2511: 'HBITMAP CMarmaladeRdkPlugIn::CreatePreview(const CSize &,eRhRdkRenderQuality,const IRhRdkPreviewSceneServer *)': overloaded member function not found in 'CMarmaladeRdkPlugIn'
//1>e:\dev\mcneel\rhino-developer-samples\cpp\samplerdkmarmalade\MarmaladeRdkPlugIn.h(8): note: see declaration of 'CMarmaladeRdkPlugIn'
//1>MarmaladeRdkPlugIn.cpp(190): error C2511: 'HBITMAP CMarmaladeRdkPlugIn::CreatePreview(const CSize &,const CRhRdkTexture &)': overloaded member function not found in 'CMarmaladeRdkPlugIn'
//1>e:\dev\mcneel\rhino-developer-samples\cpp\samplerdkmarmalade\MarmaladeRdkPlugIn.h(8): note: see declaration of 'CMarmaladeRdkPlugIn'
//1>MarmaladeRdkPlugIn.cpp(223): error C2664: 'bool CRhinoApp::RunScript(const CRhinoScriptContext &)': cannot convert argument 1 from 'const wchar_t [21]' to 'const CRhinoScriptContext &'
//1>MarmaladeRdkPlugIn.cpp(223): note: Reason: cannot convert from 'const wchar_t [21]' to 'const CRhinoScriptContext'
//1>MarmaladeRdkPlugIn.cpp(223): note: No constructor could take the source type, or constructor overload resolution was ambiguous
//1>Done building project "Marmalade.vcxproj" -- FAILED.
//========== Build: 0 succeeded, 1 failed, 0 up-to-date, 0 skipped ==========
//

class CMarmaladePointCloudRMP : public CRhRdkCustomRenderMeshProvider
{
public:
	CMarmaladePointCloudRMP(void);
	virtual ~CMarmaladePointCloudRMP(void);

public:
	virtual UUID PlugInId(void) const override;
	virtual UUID ProviderId(void) const override;
	virtual ON_wString Name(void) const override;

	virtual bool IsViewDependent(void) const override { return true; } /////??????
	virtual bool IsRequestingPlugInDependent(void) const override { return false; } /////??????
	virtual bool IsPreviewAndStandardSameMesh(void) const override { return false; } /////??????
	virtual CRhRdkVariant GetParameter(const CRhinoObject&, const wchar_t*) const { return 0; }
	virtual void SetParameter(const CRhinoObject&, const wchar_t*,const CRhRdkVariant&) override { }

	virtual bool WillBuildCustomMesh(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                 const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const override;

	virtual ON_BoundingBox BoundingBox(const ON_Viewport& vp, const CRhinoObject* pObject, const CRhinoDoc& doc,
	                                   const UUID& uuidRequestingPlugIn, const CDisplayPipelineAttributes* pAttributes) const override;

	virtual bool BuildCustomMeshes(const ON_Viewport& vp, const UUID& uuidRequestingPlugIn, const CRhinoDoc& doc,
	                               IRhRdkCustomRenderMeshes& crmInOut, const CDisplayPipelineAttributes* pAttributes,
	                               bool bWillBuildCustomMeshCheck = true) const override;
};
