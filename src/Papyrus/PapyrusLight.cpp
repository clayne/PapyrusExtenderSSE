#include "Papyrus/PapyrusLight.h"


RE::BGSColorForm* papyrusLight::GetLightColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return nullptr;
	}

	auto factory = RE::IFormFactory::GetFormFactoryByType(RE::FormType::ColorForm);
	auto color = static_cast<RE::BGSColorForm*>(factory->Create());
	if (color) {
		color->flags.reset(RE::BGSColorForm::Flag::kPlayable);
		color->color = a_light->data.color;
		return color;
	}

	return nullptr;
}


float papyrusLight::GetLightFade(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_light->fade;
}


float papyrusLight::GetLightFOV(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return a_light->data.fov;
}


float papyrusLight::GetLightRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return 0.0f;
	}

	return static_cast<float>(a_light->data.radius);  //should ideally return as std::uint32_t but messed the return
}


float papyrusLight::GetLightShadowDepthBias(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_lightObject)
{
	if (!a_lightObject) {
		a_vm->TraceStack("ObjectReference is None", a_stackID, Severity::kWarning);
		return 1.0f;
	}

	auto a_light = a_lightObject->As<RE::TESObjectLIGH>();
	if (!a_light) {
		a_vm->TraceStack("ObjectReference is not a Light form", a_stackID, Severity::kWarning);
		return 1.0f;
	}

	auto xLightData = a_lightObject->extraList.GetByType<RE::ExtraLightData>();
	return xLightData ? xLightData->data.shadowDepthBias : 1.0f;
}


std::uint32_t papyrusLight::GetLightType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light)
{
	using FLAGS = RE::TES_LIGHT_FLAGS;

	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return 0;
	}

	const auto flags = a_light->data.flags;
	if ((flags & FLAGS::kHemiShadow) == FLAGS::kHemiShadow) {
		return 1;
	} else if ((flags & FLAGS::kNone) == FLAGS::kNone) {
		return 2;
	} else if ((flags & FLAGS::kOmniShadow) == FLAGS::kOmniShadow) {
		return 3;
	} else if ((flags & FLAGS::kSpotlight) == FLAGS::kSpotlight) {
		return 4;
	} else if ((flags & FLAGS::kSpotShadow) == FLAGS::kSpotShadow) {
		return 5;
	} else {
		return 0;
	}
}


void papyrusLight::SetLightColor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, RE::BGSColorForm* a_color)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	} else if (!a_color) {
		a_vm->TraceStack("Colorform is None", a_stackID, Severity::kWarning);
		return;
	}

	a_light->data.color = a_color->color;
}


void papyrusLight::SetLightFade(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, float a_fade)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	}

	a_light->fade = a_fade;
}


void papyrusLight::SetLightFOV(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, float a_fov)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	}

	a_light->data.fov = a_fov;
}


void papyrusLight::SetLightRadius(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, float a_radius)
{
	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	}

	a_light->data.radius = static_cast<std::uint32_t>(a_radius);
}


void papyrusLight::SetLightShadowDepthBias(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_lightObject, float a_depthBias)
{
	if (!a_lightObject) {
		a_vm->TraceStack("ObjectReference is None", a_stackID, Severity::kWarning);
		return;
	}

	auto a_light = a_lightObject->As<RE::TESObjectLIGH>();
	if (!a_light) {
		a_vm->TraceStack("ObjectReference is not a Light form", a_stackID, Severity::kWarning);
		return;
	}

	auto xLightData = a_lightObject->extraList.GetByType<RE::ExtraLightData>();
	if (xLightData) {
		xLightData->data.shadowDepthBias = a_depthBias;
	} else {
		auto newLightData = new RE::ExtraLightData();
		if (newLightData) {
			newLightData->data.shadowDepthBias = a_depthBias;
			a_lightObject->extraList.Add(newLightData);
		}
	}
}


void papyrusLight::SetLightType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectLIGH* a_light, std::uint32_t a_type)
{
	using FLAGS = RE::TES_LIGHT_FLAGS;

	if (!a_light) {
		a_vm->TraceStack("Light is None", a_stackID, Severity::kWarning);
		return;
	}

	auto& flags = a_light->data.flags;
	switch (a_type) {
	case 1:
		flags = flags & FLAGS::kType | FLAGS::kHemiShadow;
		break;
	case 2:
		flags = flags & FLAGS::kType | FLAGS::kNone;
		break;
	case 3:
		flags = flags & FLAGS::kType | FLAGS::kOmniShadow;
		break;
	case 4:
		flags = flags & FLAGS::kType | FLAGS::kSpotlight;
		break;
	case 5:
		flags = flags & FLAGS::kType | FLAGS::kSpotShadow;
		break;
	default:
		break;
	}
}


bool papyrusLight::RegisterFuncs(VM* a_vm)
{
	if (!a_vm) {
		logger::critical("papyrusLight - couldn't get VMState");
		return false;
	}

	a_vm->RegisterFunction("GetLightColor", "PO3_SKSEFunctions", GetLightColor);

	a_vm->RegisterFunction("GetLightFade", "PO3_SKSEFunctions", GetLightFade, true);

	a_vm->RegisterFunction("GetLightFOV", "PO3_SKSEFunctions", GetLightFOV, true);

	a_vm->RegisterFunction("GetLightRadius", "PO3_SKSEFunctions", GetLightRadius, true);

	a_vm->RegisterFunction("GetLightShadowDepthBias", "PO3_SKSEFunctions", GetLightShadowDepthBias);

	a_vm->RegisterFunction("GetLightType", "PO3_SKSEFunctions", GetLightType);

	a_vm->RegisterFunction("SetLightRadius", "PO3_SKSEFunctions", SetLightRadius);

	a_vm->RegisterFunction("SetLightColor", "PO3_SKSEFunctions", SetLightColor);

	a_vm->RegisterFunction("SetLightFade", "PO3_SKSEFunctions", SetLightFade);

	a_vm->RegisterFunction("SetLightFOV", "PO3_SKSEFunctions", SetLightFOV);

	a_vm->RegisterFunction("SetLightShadowDepthBias", "PO3_SKSEFunctions", SetLightShadowDepthBias);

	a_vm->RegisterFunction("SetLightType", "PO3_SKSEFunctions", SetLightType);

	return true;
}