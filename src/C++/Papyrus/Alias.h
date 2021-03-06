#pragma once

#include "Serialization/Events.h"

namespace Papyrus::Alias
{
	using namespace Serialization;

	inline void RegisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnActorKillRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a reference alias", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnActorReanimateStartRegSet::GetSingleton();
		regs->Register(refAlias);
	}

	inline void RegisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a reference alias", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnActorReanimateStopRegSet::GetSingleton();
		regs->Register(refAlias);
	}

	inline void RegisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a reference alias", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnActorResurrectRegSet::GetSingleton();
		regs->Register(refAlias);
	}

	inline void RegisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnBooksReadRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = ScriptEvents::OnCellFullyLoadedRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnCriticalHitRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnDisarmedRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnDragonSoulsGainedRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnItemHarvestedRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnLevelIncreaseRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnLocationDiscoveryRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		RE::BGSRefAlias* a_alias, 
		const RE::TESForm* a_effectFilter, 
		bool a_match)
	{
		if (!a_alias) {
			a_vm->TraceStack("Reference Alias is None", a_stackID);
			return;
		}
		if (!a_effectFilter) {
			a_vm->TraceStack("Effect Filter is None", a_stackID);
			return;
		}

		auto key = std::make_pair(a_effectFilter->GetFormID(), a_match);
		const auto regs = HookedEvents::OnMagicEffectApplyRegMap::GetSingleton();
		regs->Register(a_alias, key);
	}

	inline void RegisterForMagicHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Reference Alias is None", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnMagicHitRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto grab = ScriptEvents::OnGrabRegSet::GetSingleton();
		grab->Register(a_alias);

		const auto release = ScriptEvents::OnReleaseRegSet::GetSingleton();
		release->Register(a_alias);
	}

	inline void RegisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias, std::uint32_t a_formType)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		const auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
		load->Register(a_alias, formType);

		const auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
		unload->Register(a_alias, formType);
	}

	inline void RegisterForProjectileHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Reference Alias is None", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnProjectileHitRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		const RE::BGSBaseAlias* a_alias, 
		const RE::TESQuest* a_quest)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
		start->Register(a_alias, a_quest->GetFormID());

		auto stop = ScriptEvents::OnQuestStopRegMap::GetSingleton();
		stop->Register(a_alias, a_quest->GetFormID());
	}

	inline void RegisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		const RE::BGSBaseAlias* a_alias, 
		const RE::TESQuest* a_quest)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		const auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
		regs->Register(a_alias, a_quest->GetFormID());
	}

	inline void RegisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnSkillIncreaseRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnSoulsTrappedRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnSpellsLearnedRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnWeatherChangeRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void RegisterForWeaponHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Reference Alias is None", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnWeaponHitRegSet::GetSingleton();
		regs->Register(a_alias);
	}

	inline void UnregisterForActorKilled(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnActorKillRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForActorReanimateStart(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a reference alias", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnActorReanimateStartRegSet::GetSingleton();
		regs->Unregister(refAlias);
	}

	inline void UnregisterForActorReanimateStop(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a reference alias", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnActorReanimateStopRegSet::GetSingleton();
		regs->Unregister(refAlias);
	}

	inline void UnregisterForActorResurrected(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto refAlias = skyrim_cast<RE::BGSRefAlias*>(a_alias);
		if (!refAlias) {
			a_vm->TraceStack("Alias is not a reference alias", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnActorResurrectRegSet::GetSingleton();
		regs->Unregister(refAlias);
	}

	inline void UnregisterForBookRead(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnBooksReadRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForCellFullyLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = ScriptEvents::OnCellFullyLoadedRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForCriticalHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnCriticalHitRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForDisarmed(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnDisarmedRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForDragonSoulGained(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnDragonSoulsGainedRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForItemHarvested(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnItemHarvestedRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForLevelIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnLevelIncreaseRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForLocationDiscovery(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnLocationDiscoveryRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias, RE::TESForm* a_effectFilter, bool a_match)
	{
		if (!a_alias) {
			a_vm->TraceStack("Reference Alias is None", a_stackID);
			return;
		}
		if (!a_effectFilter) {
			a_vm->TraceStack("Effect Filter is None", a_stackID);
			return;
		}

		auto key = std::make_pair(a_effectFilter->GetFormID(), a_match);
		const auto regs = HookedEvents::OnMagicEffectApplyRegMap::GetSingleton();
		regs->Unregister(a_alias, key);
	}

	inline void UnregisterForAllMagicEffectApplyEx(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Reference Alias is None", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnMagicEffectApplyRegMap::GetSingleton();
		regs->UnregisterAll(a_alias);
	}

	inline void UnregisterForMagicHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Reference Alias is None", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnMagicHitRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForObjectGrab(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto grab = ScriptEvents::OnGrabRegSet::GetSingleton();
		grab->Unregister(a_alias);

		const auto release = ScriptEvents::OnReleaseRegSet::GetSingleton();
		release->Unregister(a_alias);
	}

	inline void UnregisterForObjectLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		const RE::BGSBaseAlias* a_alias, 
		std::uint32_t a_formType)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto formType = static_cast<RE::FormType>(a_formType);

		const auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
		load->Unregister(a_alias, formType);

		const auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
		unload->Unregister(a_alias, formType);
	}

	inline void UnregisterForAllObjectsLoaded(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto load = ScriptEvents::OnObjectLoadedRegMap::GetSingleton();
		load->UnregisterAll(a_alias);

		const auto unload = ScriptEvents::OnObjectUnloadedRegMap::GetSingleton();
		unload->UnregisterAll(a_alias);
	}

	inline void UnregisterForProjectileHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Reference Alias is None", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnProjectileHitRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForQuest(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		const RE::BGSBaseAlias* a_alias, 
		const RE::TESQuest* a_quest)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		const auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
		start->Unregister(a_alias, a_quest->GetFormID());

		const auto stop = ScriptEvents::OnQuestStartRegMap::GetSingleton();
		stop->Unregister(a_alias, a_quest->GetFormID());
	}

	inline void UnregisterForAllQuests(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		auto start = ScriptEvents::OnQuestStartRegMap::GetSingleton();
		start->UnregisterAll(a_alias);

		auto stop = ScriptEvents::OnQuestStartRegMap::GetSingleton();
		stop->UnregisterAll(a_alias);
	}

	inline void UnregisterForQuestStage(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, 
		const RE::BGSBaseAlias* a_alias, 
		const RE::TESQuest* a_quest)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}
		if (!a_quest) {
			a_vm->TraceStack("Quest is None", a_stackID);
			return;
		}

		const auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
		regs->Unregister(a_alias, a_quest->GetFormID());
	}

	inline void UnregisterForAllQuestStages(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = ScriptEvents::OnQuestStageRegMap::GetSingleton();
		regs->UnregisterAll(a_alias);
	}

	inline void UnregisterForShoutAttack(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForSkillIncrease(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnShoutAttackRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForSoulTrapped(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnSoulsTrappedRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForSpellLearned(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = StoryEvents::OnSpellsLearnedRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForWeatherChange(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, const RE::BGSBaseAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Alias is None", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnWeatherChangeRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void UnregisterForWeaponHit(VM* a_vm, StackID a_stackID, RE::StaticFunctionTag*, RE::BGSRefAlias* a_alias)
	{
		if (!a_alias) {
			a_vm->TraceStack("Reference Alias is None", a_stackID);
			return;
		}

		const auto regs = HookedEvents::OnWeaponHitRegSet::GetSingleton();
		regs->Unregister(a_alias);
	}

	inline void Register(VM& a_vm)
	{
		auto const obj = "PO3_Events_Alias"sv;

		BIND_EVENT(RegisterForActorKilled, true);
		BIND_EVENT(RegisterForActorReanimateStart, true);
		BIND_EVENT(RegisterForActorReanimateStop, true);
		BIND_EVENT(RegisterForActorResurrected, true);
		BIND_EVENT(RegisterForBookRead, true);
		BIND_EVENT(RegisterForCellFullyLoaded, true);
		BIND_EVENT(RegisterForCriticalHit, true);
		BIND_EVENT(RegisterForDisarmed, true);
		BIND_EVENT(RegisterForDragonSoulGained, true);
		BIND_EVENT(RegisterForItemHarvested, true);
		BIND_EVENT(RegisterForLevelIncrease, true);
		BIND_EVENT(RegisterForLocationDiscovery, true);
		BIND_EVENT(RegisterForMagicEffectApplyEx, true);
		BIND_EVENT(RegisterForMagicHit, true);
		BIND_EVENT(RegisterForObjectGrab, true);
		BIND_EVENT(RegisterForObjectLoaded, true);
		BIND_EVENT(RegisterForProjectileHit, true);
		BIND_EVENT(RegisterForQuest, true);
		BIND_EVENT(RegisterForQuestStage, true);
		BIND_EVENT(RegisterForShoutAttack, true);
		BIND_EVENT(RegisterForSkillIncrease, true);
		BIND_EVENT(RegisterForSoulTrapped, true);
		BIND_EVENT(RegisterForSpellLearned, true);
		BIND_EVENT(RegisterForWeatherChange, true);
		BIND_EVENT(RegisterForWeaponHit, true);
		BIND_EVENT(UnregisterForActorKilled, true);
		BIND_EVENT(UnregisterForActorReanimateStart, true);
		BIND_EVENT(UnregisterForActorReanimateStop, true);
		BIND_EVENT(UnregisterForActorResurrected, true);
		BIND_EVENT(UnregisterForBookRead, true);
		BIND_EVENT(UnregisterForCellFullyLoaded, true);
		BIND_EVENT(UnregisterForCriticalHit, true);
		BIND_EVENT(UnregisterForDisarmed, true);
		BIND_EVENT(UnregisterForDragonSoulGained, true);
		BIND_EVENT(UnregisterForItemHarvested, true);
		BIND_EVENT(UnregisterForLevelIncrease, true);
		BIND_EVENT(UnregisterForLocationDiscovery, true);
		BIND_EVENT(UnregisterForMagicEffectApplyEx, true);
		BIND_EVENT(UnregisterForAllMagicEffectApplyEx, true);
		BIND_EVENT(UnregisterForMagicHit, true);
		BIND_EVENT(UnregisterForObjectGrab, true);
		BIND_EVENT(UnregisterForObjectLoaded, true);
		BIND_EVENT(UnregisterForAllObjectsLoaded, true);
		BIND_EVENT(UnregisterForProjectileHit, true);
		BIND_EVENT(UnregisterForQuest, true);
		BIND_EVENT(UnregisterForAllQuests, true);
		BIND_EVENT(UnregisterForQuestStage, true);
		BIND_EVENT(UnregisterForAllQuestStages, true);
		BIND_EVENT(UnregisterForShoutAttack, true);
		BIND_EVENT(UnregisterForSkillIncrease, true);
		BIND_EVENT(UnregisterForSoulTrapped, true);
		BIND_EVENT(UnregisterForSpellLearned, true);
		BIND_EVENT(UnregisterForWeatherChange, true);
		BIND_EVENT(UnregisterForWeaponHit, true);

		logger::info("Registered alias functions"sv);
	}
}
