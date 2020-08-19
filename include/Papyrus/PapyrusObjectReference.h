#pragma once


namespace papyrusObjectReference
{
	using VM = RE::BSScript::IVirtualMachine;
	using StackID = RE::VMStackID;
	using Severity = RE::BSScript::ErrorLogger::Severity;

	inline constexpr float PI = 3.1415927f;


	std::vector<RE::TESForm*> AddAllInventoryItemsToArray(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);

	void AddAllInventoryItemsToList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list, bool a_noEquipped, bool a_noFavourited, bool a_noQuestItem);

	void AddKeywordToRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_add);

	std::vector<RE::TESObjectREFR*> FindAllReferencesOfFormType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, std::uint32_t a_formType, float a_radius);

	std::vector<RE::TESObjectREFR*> FindAllReferencesOfType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESForm* a_formOrList, float a_radius);

	std::vector<RE::TESObjectREFR*> FindAllReferencesWithKeyword(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_origin, RE::TESForm* a_formOrList, float a_radius, bool a_matchAll);

	RE::TESForm* FindFirstItemInList(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSListForm* a_list);

	RE::Actor* GetActorCause(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

	RE::Actor* GetClosestActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, bool a_ignorePlayer);

	std::vector<RE::BGSArtObject*> GetAllArtObjects(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

	std::vector<RE::TESEffectShader*> GetAllEffectShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

	float GetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader);

	RE::Actor* GetRandomActorFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer);

	std::uint32_t HasArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art, bool a_active);

	std::uint32_t HasEffectShader(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, bool a_active);

	bool IsLoadDoor(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_door);

	bool HasNiExtraData(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_name);

	void MoveToNearestNavmeshLocation(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

	bool RemoveKeywordFromRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove);

	void ReplaceKeywordOnRef(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSKeyword* a_remove, RE::BGSKeyword* a_add);

	void ScaleObject3D(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_nodeName, float a_radius);

	void SetEffectShaderDuration(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESEffectShader* a_effectShader, float a_time, bool a_absolute);

	void SetShaderType(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::TESObjectREFR* a_template, RE::BSFixedString a_filter, std::uint32_t a_shaderType, std::int32_t a_textureType, bool a_noWeapons, bool a_noAlpha);

	void SetupBodyPartGeometry(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_miscItem, RE::Actor* a_actor);

	void StopAllShaders_Impl(RE::TESObjectREFR* a_ref);

	void StopAllShaders(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref);

	void StopAllSkinAlphaShaders_Impl(RE::TESObjectREFR* a_ref);

	void StopArtObject(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BGSArtObject* a_art);

	void ToggleChildNode(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::TESObjectREFR* a_ref, RE::BSFixedString a_nodeName, bool a_disable);


	bool RegisterFuncs(VM* a_vm);
}