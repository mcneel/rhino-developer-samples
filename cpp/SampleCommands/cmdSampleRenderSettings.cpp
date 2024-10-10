
#include "stdafx.h"

#pragma warning (push)
#pragma warning (disable: 4996)
#pragma region SampleRenderSettings command

// NOTE: In the following examples, any variables that end with '_old' are using the old method of
// accessing the objects. These examples are included for comparison with the new method but they
// should not be used. You should only use the examples that have variables ending in "_new" and
// which use ON_3dmRenderSettings with variables called 'rs' and 'rs_write'.

class CCommandSampleRenderSettings : public CRhinoCommand
{
public:
  virtual UUID CommandUUID() override { static const UUID uuid = { 0x121bc621, 0xca21, 0x44b7, { 0xa1, 0xd7, 0x4b, 0x34, 0x2c, 0xb2, 0x1b, 0x1 } }; return uuid; }
  virtual const wchar_t* EnglishCommandName() override { return L"SampleRenderSettings"; }
  virtual CRhinoCommand::result RunCommand(const CRhinoCommandContext& context) override;

private:
  bool Sun(CRhinoDoc& doc) const;
  bool GroundPlane(CRhinoDoc& doc) const;
  bool SafeFrame(CRhinoDoc& doc) const;
  bool Skylight(CRhinoDoc& doc) const;
  bool LinearWorkflow(CRhinoDoc& doc) const;
  bool Dithering(CRhinoDoc& doc) const;
  bool RenderChannels(CRhinoDoc& doc) const;
  bool PostEffects(CRhinoDoc& doc) const;
  bool CurrentEnvironment(CRhinoDoc& doc) const;
};

// The one and only CCommandSampleRenderSettings object.
static class CCommandSampleRenderSettings theSampleRenderSettingsCommand;

CRhinoCommand::result CCommandSampleRenderSettings::RunCommand(const CRhinoCommandContext& context)
{
  auto* doc = context.Document();
  if (nullptr == doc)
    return failure;

  if (!Sun(*doc))
    return failure;

  if (!GroundPlane(*doc))
    return failure;

  if (!SafeFrame(*doc))
    return failure;

  if (!Skylight(*doc))
    return failure;

  if (!LinearWorkflow(*doc))
    return failure;

  if (!Dithering(*doc))
    return failure;

  if (!RenderChannels(*doc))
    return failure;

  if (!PostEffects(*doc)) // To test this, make sure you add the Bloom post effect first.
    return failure;

  if (!CurrentEnvironment(*doc))
    return failure;

  return success;
}

// Sun.

bool CCommandSampleRenderSettings::Sun(CRhinoDoc& doc) const
{
  // Old-style access method was directly in the document.
  const auto& sun_old = doc.Sun();

  // New-style access is through the document's render settings.
  const auto& rs = doc.Properties().RenderSettings();

  // Reading a property the old way.
  const auto value_old = sun_old.Latitude();

  // Reading a property the new way.
  const auto value_new = rs.Sun().Latitude();

  // Writing a property the old way required a call to BeginChange() to get a non-const object.
  auto& sun_write_old = sun_old.BeginChange(RhRdkChangeContext::UI);

  // Non-const method was called on the non-const object.
  sun_write_old.SetLatitude(51.0);

  // Writing a property the new way requires copying the render settings to a non-const object.
  ON_3dmRenderSettings rs_write = rs;

  // Non-const methods must be called on the non-const copy.
  rs_write.Sun().SetLatitude(51.0);

  // Writing the old way required a call to EndChange() on the non-const document object.
  sun_write_old.EndChange();

  // Writing the new way requires a call to SetRenderSettings() passing the non-const render settings.
  doc.Properties().SetRenderSettings(rs_write);

  return true;
}

// Ground Plane.

bool CCommandSampleRenderSettings::GroundPlane(CRhinoDoc& doc) const
{
  // Old-style access method was directly in the document.
  const auto& gp_old = doc.GroundPlane();

  // New-style access is through the document's render settings.
  const auto& rs = doc.Properties().RenderSettings();

  // Reading a property the old way.
  const auto value_old = gp_old.Altitude();

  // Reading a property the new way.
  const auto value_new = rs.GroundPlane().Altitude();

  // Writing a property the old way required a call to BeginChange() to get a non-const object.
  auto& gp_write_old = gp_old.BeginChange(RhRdkChangeContext::UI);

  // Non-const method was called on the non-const object.
  gp_write_old.SetAltitude(25.0);

  // Writing a property the new way requires copying the render settings to a non-const object.
  ON_3dmRenderSettings rs_write = rs;

  // Non-const methods must be called on the non-const copy.
  rs_write.GroundPlane().SetAltitude(25.0);

  // Writing the old way required a call to EndChange() on the non-const document object.
  gp_write_old.EndChange();

  // Writing the new way requires a call to SetRenderSettings() passing the non-const render settings.
  doc.Properties().SetRenderSettings(rs_write);

  return true;
}

// Safe Frame.

bool CCommandSampleRenderSettings::SafeFrame(CRhinoDoc& doc) const
{
  // Old-style access method was directly in the document.
  const auto& sf = doc.SafeFrame();

  // New-style access is through the document's render settings.
  const auto& rs = doc.Properties().RenderSettings();

  // Reading a property the old way.
  const auto value_old = sf.ActionFrameXScale();

  // Reading a property the new way.
  const auto value_new = rs.SafeFrame().ActionFrameXScale();

  // Writing a property the old way required a call to BeginChange() to get a non-const object.
  auto& sf_write_old = sf.BeginChange(RhRdkChangeContext::UI);

  // Non-const method was called on the non-const object.
  sf_write_old.SetActionFrameXScale(2.0);

  // Writing a property the new way requires copying the render settings to a non-const object.
  ON_3dmRenderSettings rs_write = rs;

  // Non-const methods must be called on the non-const copy.
  rs_write.SafeFrame().SetActionFrameXScale(2.0);

  // Writing the old way required a call to EndChange() on the non-const document object.
  sf_write_old.EndChange();

  // Writing the new way requires a call to SetRenderSettings() passing the non-const render settings.
  doc.Properties().SetRenderSettings(rs_write);

  return true;
}

// Skylight.

bool CCommandSampleRenderSettings::Skylight(CRhinoDoc& doc) const
{
  // Old-style access method was directly in the document.
  const auto& sl_old = doc.Skylight();

  // New-style access is through the document's render settings.
  const auto& rs = doc.Properties().RenderSettings();

  // Reading a property the old way.
  const auto value_old = sl_old.Enabled();

  // Reading a property the new way.
  const auto value_new = rs.Skylight().Enabled();

  // Writing a property the old way required a call to BeginChange() to get a non-const object.
  auto& sl_write_old = sl_old.BeginChange(RhRdkChangeContext::UI);

  // Non-const method was called on the non-const object.
  sl_write_old.SetEnabled(true);

  // Writing a property the new way requires copying the render settings to a non-const object.
  ON_3dmRenderSettings rs_write = rs;

  // Non-const methods must be called on the non-const copy.
  rs_write.Skylight().SetEnabled(true);

  // Writing the old way required a call to EndChange() on the non-const document object.
  sl_write_old.EndChange();

  // Writing the new way requires a call to SetRenderSettings() passing the non-const render settings.
  doc.Properties().SetRenderSettings(rs_write);

  return true;
}

// Linear Workflow.

bool CCommandSampleRenderSettings::LinearWorkflow(CRhinoDoc& doc) const
{
  // Old-style access method was directly in the document.
  const auto& lw_old = doc.LinearWorkflow();

  // New-style access is through the document's render settings.
  const auto& rs = doc.Properties().RenderSettings();

  // Reading a property the old way.
  const auto value_old = lw_old.PostProcessGamma();

  // Reading a property the new way.
  const auto value_new = rs.LinearWorkflow().PostProcessGamma();

  // Writing a property the old way required a call to BeginChange() to get a non-const object.
  auto& lw_write_old = lw_old.BeginChange(RhRdkChangeContext::UI);

  // Non-const method was called on the non-const object.
  lw_write_old.SetPostProcessGamma(2.6f);

  // Writing a property the new way requires copying the render settings to a non-const object.
  ON_3dmRenderSettings rs_write = rs;

  // Non-const methods must be called on the non-const copy.
  rs_write.LinearWorkflow().SetPostProcessGamma(2.6f);

  // Writing the old way required a call to EndChange() on the non-const document object.
  lw_write_old.EndChange();

  // Writing the new way requires a call to SetRenderSettings() passing the non-const render settings.
  doc.Properties().SetRenderSettings(rs_write);

  return true;
}

// Dithering.

bool CCommandSampleRenderSettings::Dithering(CRhinoDoc& doc) const
{
  // Old-style access method was directly in the document.
  const auto& dit_old = doc.Dithering();

  // New-style access is through the document's render settings.
  const auto& rs = doc.Properties().RenderSettings();

  // Reading a property the old way.
  const auto value_old = dit_old.Method();

  // Reading a property the new way.
  const auto value_new = rs.Dithering().Method();

  // Writing a property the old way required a call to BeginChange() to get a non-const object.
  auto& dit_write_old = dit_old.BeginChange(RhRdkChangeContext::UI);

  // Non-const method was called on the non-const object.
  dit_write_old.SetMethod(IRhRdkDithering::Methods::SimpleNoise);

  // Writing a property the new way requires copying the render settings to a non-const object.
  ON_3dmRenderSettings rs_write = rs;

  // Non-const methods must be called on the non-const copy.
  rs_write.Dithering().SetMethod(ON_Dithering::Methods::SimpleNoise);

  // Writing the old way required a call to EndChange() on the non-const document object.
  dit_write_old.EndChange();

  // Writing the new way requires a call to SetRenderSettings() passing the non-const render settings.
  doc.Properties().SetRenderSettings(rs_write);

  return true;
}

// Render Channels.

bool CCommandSampleRenderSettings::RenderChannels(CRhinoDoc& doc) const
{
  // Old-style access method was directly in the document.
  const auto& rch_old = doc.RenderChannels();

  // New-style access is through the document's render settings.
  const auto& rs = doc.Properties().RenderSettings();

  // Reading a property the old way.
  const auto value_old = rch_old.Mode();

  // Reading a property the new way.
  const auto value_new = rs.RenderChannels().Mode();

  // Writing a property the old way required a call to BeginChange() to get a non-const object.
  auto& rch_write_old = rch_old.BeginChange(RhRdkChangeContext::UI);

  // Non-const method was called on the non-const object.
  rch_write_old.SetMode(IRhRdkRenderChannels::Modes::Custom);

  // Writing a property the new way requires copying the render settings to a non-const object.
  ON_3dmRenderSettings rs_write = rs;

  // Non-const methods must be called on the non-const copy.
  rs_write.RenderChannels().SetMode(ON_RenderChannels::Modes::Custom);

  // Writing the old way required a call to EndChange() on the non-const document object.
  rch_write_old.EndChange();

  // Writing the new way requires a call to SetRenderSettings() passing the non-const render settings.
  doc.Properties().SetRenderSettings(rs_write);

  return true;
}

// Post Effects.

bool CCommandSampleRenderSettings::PostEffects(CRhinoDoc& doc) const
{
  // Old-style access method was directly in the document.
  const auto& post_effects_old = doc.PostEffects();

  // New-style access is through the document's render settings.
  const auto& rs = doc.Properties().RenderSettings();

  // We will test using the 'radius' property of the bloom post effect.
  const auto param_name = SS_PEP_BLOOM_RADIUS;
  const auto post_effect_id = uuidPostEffect_Bloom;

  const auto value_to_write = 2.5f;

  // Reading a property the old way. First get the bloom post effect.
  const auto* post_effect_old = post_effects_old.PostEffectFromId(post_effect_id);
  if (nullptr == post_effect_old)
    return false;

  // Reading a property the old way.
  CRhRdkVariant value_old;
  if (!post_effect_old->GetParameter(param_name, value_old))
    return false;

  // Reading a property the new way. First get the bloom post effect from the const render settings.
  const auto* post_effect_new = rs.PostEffects().PostEffectFromId(post_effect_id);
  if (nullptr == post_effect_new)
    return false;

  // Reading a property the new way.
  const auto value_new = post_effect_new->GetParameter(param_name);

  // Writing a property the old way required a call to BeginChange() to get a non-const object.
  auto& post_effect_write_old = post_effect_old->BeginChange(RhRdkChangeContext::UI);

  // Non-const method was called on the non-const object.
  post_effect_write_old.SetParameter(param_name, value_to_write);

  // Writing a property the new way requires copying the render settings to a non-const object.
  ON_3dmRenderSettings rs_write = rs;

  // Writing a property the new way. First get the bloom post effect from the non-const render settings copy.
  auto* post_effect_write_new = rs_write.PostEffects().PostEffectFromId(post_effect_id);
  if (nullptr == post_effect_write_new)
    return false;

  // Non-const methods must be called on the non-const copy.
  post_effect_write_new->SetParameter(param_name, value_to_write);

  // Writing the old way required a call to EndChange() on the non-const document object.
  post_effect_write_old.EndChange();

  // Writing the new way requires a call to SetRenderSettings() passing the non-const render settings.
  doc.Properties().SetRenderSettings(rs_write);

  return true;
}

bool CCommandSampleRenderSettings::CurrentEnvironment(CRhinoDoc& doc) const
{
  // Old-style environment usage and purpose.
  using ICEU = IRhRdkCurrentEnvironment::Usage;
  using ICEP = IRhRdkCurrentEnvironment::Purpose;

  // New-style environment usage and purpose.
  using OCEU = ON_3dmRenderSettings::EnvironmentUsage;
  using OCEP = ON_3dmRenderSettings::EnvironmentPurpose;

  // Old-style current environment access was directly in the document.
  const auto& ce = doc.CurrentEnvironment();

  // New-style current environment access is through the document's render settings.
  const auto& rs = doc.Properties().RenderSettings();

  // Reading environment on-ness for each usage the old way.
  const auto back_on_old = ce.On(ICEU::Background);
  const auto refl_on_old = ce.On(ICEU::Reflection);
  const auto skyl_on_old = ce.On(ICEU::Skylighting);

  // Reading environment override for each usage the new way.
  // When an override is enabled for a particular usage, it enables that usage's id to override
  // the background environment's id.
  // NOTE: 'Background' is allowed for convenience even though it's not really an override.
  // See the comments at the definition of ON_3dmRenderSettings::RenderEnvironmentOverride().
  const auto back_on_new = rs.RenderEnvironmentOverride(OCEU::Background);
  const auto refl_on_new = rs.RenderEnvironmentOverride(OCEU::Reflection);
  const auto skyl_on_new = rs.RenderEnvironmentOverride(OCEU::Skylighting);

  // Reading environment id for each usage the old way.
  const auto back_id_old = ce.Get(ICEU::Background,  ICEP::Simple);
  const auto refl_id_old = ce.Get(ICEU::Reflection,  ICEP::Simple);
  const auto skyl_id_old = ce.Get(ICEU::Skylighting, ICEP::Simple);

  // Reading environment id for each usage the new way. Note that the 'Standard' purpose
  // has the same meaning as the old-style 'Simple' purpose.
  const auto back_id_new = rs.RenderEnvironmentId(OCEU::Background,  OCEP::Standard);
  const auto refl_id_new = rs.RenderEnvironmentId(OCEU::Reflection,  OCEP::Standard);
  const auto skyl_id_new = rs.RenderEnvironmentId(OCEU::Skylighting, OCEP::Standard);

  // Writing the old way required a call to BeginChange() to get a non-const object.
  auto& ce_write = ce.BeginChange(RhRdkChangeContext::UI);

  // Writing the new way requires copying the render settings to a non-const object.
  ON_3dmRenderSettings rs_write = rs;

  // Writing old-style environment on-ness for each usage to the non-const current environment.
  // NOTE: It's not possible to set the background 'on' state like this.
  ce_write.SetOn(ICEU::Reflection,  true);
  ce_write.SetOn(ICEU::Skylighting, false);

  // Writing new-style environment override for each usage to the non-const render settings.
  // NOTE: It's not possible to set the background like this because it's not an override.
  // See the comments at the definition of ON_3dmRenderSettings::SetRenderEnvironmentOverride().
  rs_write.SetRenderEnvironmentOverride(OCEU::Reflection,  true);
  rs_write.SetRenderEnvironmentOverride(OCEU::Skylighting, false);

  UUID back_id = { }; // Set this UUID to something sensible.
  UUID refl_id = { }; // Set this UUID to something sensible.
  UUID skyl_id = { }; // Set this UUID to something sensible.

  // Writing old-style environment id for each usage to the non-const current environment.
  ce_write.Set(ICEU::Background,  back_id);
  ce_write.Set(ICEU::Reflection,  refl_id);
  ce_write.Set(ICEU::Skylighting, skyl_id);

  // Writing new-style environment id for each usage to the non-const render settings.
  rs_write.SetRenderEnvironmentId(OCEU::Background,  back_id);
  rs_write.SetRenderEnvironmentId(OCEU::Reflection,  refl_id);
  rs_write.SetRenderEnvironmentId(OCEU::Skylighting, skyl_id);

  // Writing the old way required a call to EndChange() on the non-const current environment.
  ce_write.EndChange();

  // Writing the new way requires a call to SetRenderSettings() passing the non-const render settings.
  doc.Properties().SetRenderSettings(rs_write);

  return true;
}

#pragma endregion
#pragma warning (pop)
