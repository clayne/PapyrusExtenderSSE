#include "Game/EventHandler.h"
#include "Serialization/EventHolder.h"

namespace Event
{
	namespace FallLongDistance
	{
		struct CalcDoDamage
		{
			static float thunk(RE::Actor* a_this, float a_fallDistance, float a_defaultMult)
			{
				const auto fallDamage = func(a_this, a_fallDistance, a_defaultMult);
				if (fallDamage > 0.0f) {
					GameEventHolder::GetSingleton()->actorFallLongDistance.QueueEvent(a_this, a_this, a_fallDistance, fallDamage);
				}
				return fallDamage;
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		inline void Install()
		{
			REL::Relocation<std::uintptr_t> take_ragdoll_damage{ REL_ID(36346, 37336), 0x35 };
			stl::write_thunk_call<CalcDoDamage>(take_ragdoll_damage.address());

			REL::Relocation<std::uintptr_t> process_movefinish_event{ REL_ID(36973, 37998), OFFSET(0xAE, 0xAB) };
			stl::write_thunk_call<CalcDoDamage>(process_movefinish_event.address());

			logger::info("Hooked Fall Damage"sv);
		}
	}

	namespace BooksRead
	{
		struct Read
		{
			static bool thunk(RE::TESObjectBOOK* a_this, RE::TESObjectREFR* a_reader)
			{
				const auto result = func(a_this, a_reader);
				if (a_this && a_reader->IsPlayerRef()) {
					GameEventHolder::GetSingleton()->booksRead.QueueEvent(a_this);
				}
				return result;
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		inline void Install()
		{
			REL::Relocation<std::uintptr_t> bookMenu{ REL_ID(50122, 51053), OFFSET_3(0x22D, 0x231, 0x295) };
			stl::write_thunk_call<Read>(bookMenu.address());

			logger::info("Hooked Book Read"sv);
		}
	}

	namespace ItemCrafted
	{
		struct StoryItemCraft
		{
			RE::ObjectRefHandle objectHandle;  // 00
			RE::BGSLocation* location;         // 08
			RE::TESForm* form;                 // 10
		};
		static_assert(sizeof(StoryItemCraft) == 0x18);

		struct StoryCraftItem
		{
			static StoryItemCraft* thunk(StoryItemCraft* a_event, RE::TESObjectREFR* a_refr, RE::BGSLocation* a_loc, RE::TESForm* a_form)
			{
				GameEventHolder::GetSingleton()->itemCrafted.QueueEvent(a_refr, a_loc, a_form);

				return func(a_event, a_refr, a_loc, a_form);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		inline void Install()
		{
			REL::Relocation<std::uintptr_t> smithing{ REL_ID(50477, 51370), OFFSET(0x17D, 0x1B3) };
			stl::write_thunk_call<StoryCraftItem>(smithing.address());

			REL::Relocation<std::uintptr_t> tempering{ REL_ID(50476, 51369), OFFSET(0x11E, 0x227) };
			stl::write_thunk_call<StoryCraftItem>(tempering.address());

			REL::Relocation<std::uintptr_t> enchanting{ REL_ID(50450, 51355), OFFSET(0x2FC, 0x2FA) };
			stl::write_thunk_call<StoryCraftItem>(enchanting.address());

			REL::Relocation<std::uintptr_t> alchemy{ REL_ID(50449, 51354), OFFSET(0x29E, 0x296) };
			stl::write_thunk_call<StoryCraftItem>(alchemy.address());

			logger::info("Hooked Item Crafted"sv);
		}
	}

	namespace Resurrect
	{
		struct Resurrect
		{
			static void thunk(RE::Character* a_this, bool a_resetInventory, bool a_attach3D)
			{
				func(a_this, a_resetInventory, a_attach3D);

				GameEventHolder::GetSingleton()->actorResurrect.QueueEvent(a_this, a_this, a_resetInventory);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		void Install()
		{
			stl::write_vfunc<RE::Character,
#ifndef SKYRIMVR
				0x0AB,
#else
				0x0AD,
#endif
				Resurrect>();

			logger::info("Hooked Actor Resurrect"sv);
		}
	}

	namespace Reanimate
	{
		struct Start
		{
			static void thunk(RE::ReanimateEffect* a_this)
			{
				func(a_this);

				if (a_this->flags.none(RE::ActiveEffect::Flag::kDispelled)) {  //effect can get dispelled in original func

					const auto zombiePtr = a_this->commandedActor.get();
					const auto zombie = zombiePtr.get();
					if (!zombie) {
						return;
					}

					if (zombie->boolBits.all(RE::Actor::BOOL_BITS::kParalyzed)) {
						const auto root = zombie->Get3D(false);
						const auto charController = zombie->GetCharController();
						if (root && charController) {
							zombie->boolBits.reset(RE::Actor::BOOL_BITS::kParalyzed);

							root->UpdateRigidConstraints(false);

							std::uint32_t filterInfo = 0;
							charController->GetCollisionFilterInfo(filterInfo);

							root->SetCollisionLayerAndGroup(RE::COL_LAYER::kDeadBip, filterInfo >> 16);
						}
					}

					const auto caster = a_this->caster.get();
					if (caster) {
						GameEventHolder::GetSingleton()->actorReanimateStart.QueueEvent(zombie, zombie, caster.get());
					}
				}
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		struct Stop
		{
			static void thunk(RE::ReanimateEffect* a_this)
			{
				const auto zombiePtr = a_this->commandedActor.get();
				const auto caster = a_this->caster.get();
				if (zombiePtr && caster) {
					const auto zombie = zombiePtr.get();
					GameEventHolder::GetSingleton()->actorReanimateStop.QueueEvent(zombie, zombie, caster.get());
				}

				func(a_this);
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		void Install()
		{
			stl::write_vfunc<RE::ReanimateEffect, 0x14, Start>();
			stl::write_vfunc<RE::ReanimateEffect, 0x15, Stop>();

			logger::info("Hooked Actor Reanimate Start"sv);
			logger::info("Hooked Actor Reanimate Stop"sv);
		}
	};

	//got rid of asm hook, the fact that it worked in SE without issues was a miracle
	namespace Weather
	{
		struct SetCurrentWeather
		{
			static void thunk(RE::TESRegion* a_region, RE::TESWeather* a_currentWeather)
			{
				if (currentWeather != a_currentWeather) {
					currentWeather = a_currentWeather;

					const auto sky = RE::Sky::GetSingleton();
					const auto lastWeather = sky ? sky->lastWeather : nullptr;

					GameEventHolder::GetSingleton()->weatherChange.QueueEvent(lastWeather, currentWeather);
				}

				func(a_region, a_currentWeather);
			}
			static inline REL::Relocation<decltype(thunk)> func;

		private:
			static inline RE::TESWeather* currentWeather;
		};

		void Install()
		{
			REL::Relocation<std::uintptr_t> target{ REL_ID(25684, 26231), OFFSET(0x44F, 0x46C) };
			stl::write_thunk_call<SetCurrentWeather>(target.address());

			logger::info("Hooked Weather Change"sv);
		}
	}
}

namespace Event::Combat
{
	namespace MagicEffectApply
	{
		struct MagicTargetApply
		{
			static bool thunk(RE::MagicTarget* a_this, RE::MagicTarget::AddTargetData* a_data)
			{
				auto appliedEffect = func(a_this, a_data);

				if (const auto target = a_this && a_data ? a_this->GetTargetStatsObject() : nullptr; target) {
					const auto effect = a_data->effect;
					if (const auto baseEffect = effect ? effect->baseEffect : nullptr; baseEffect) {
						GameEventHolder::GetSingleton()->magicApply.QueueEvent(
							target,
							[=](const Filter::MagicEffectApply& a_filter, bool a_match) {  //capture by reference [&] bad
								return a_match == a_filter.PassesFilter(baseEffect);
							},
							a_data->caster, baseEffect, a_data->magicItem, appliedEffect);
					}
				}

				return appliedEffect;
			}
			static inline REL::Relocation<decltype(thunk)> func;
		};

		static void Install()
		{
			REL::Relocation<std::uintptr_t> target{ REL_ID(33742, 34526), OFFSET(0x1E8, 0x20B) };
			stl::write_thunk_call<MagicTargetApply>(target.address());

			logger::info("Hooked Magic Effect Apply"sv);
		}
	}

	namespace Hit
	{
		namespace Magic
		{
			struct SendHitEvent
			{
				static void thunk(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
				{
					using HitFlag = RE::TESHitEvent::Flag;

					const auto aggressor = a_event.cause.get();
					const auto target = a_event.target.get();
					const auto source = RE::TESForm::LookupByID(a_event.source);
					const auto projectile = RE::TESForm::LookupByID<RE::BGSProjectile>(a_event.projectile);

					if (target) {
						const auto powerAttack = a_event.flags.any(RE::TESHitEvent::Flag::kPowerAttack);
						const auto sneakAttack = a_event.flags.any(RE::TESHitEvent::Flag::kSneakAttack);
						const auto bashAttack = a_event.flags.any(RE::TESHitEvent::Flag::kBashAttack);
						const auto hitBlocked = a_event.flags.any(RE::TESHitEvent::Flag::kHitBlocked);

						GameEventHolder::GetSingleton()->onHit.QueueEvent(
							target,
							[=](const Filter::Hit& a_filter, bool a_match) {
								return a_match == a_filter.PassesFilter(aggressor, source, projectile, powerAttack, sneakAttack, bashAttack, hitBlocked);
							},
							aggressor, source, projectile, powerAttack, sneakAttack, bashAttack, hitBlocked);
					}

					if (aggressor) {
						GameEventHolder::GetSingleton()->magicHit.QueueEvent(aggressor, target, source, projectile);  // NOLINT(readability-suspicious-call-argument)
					}

					func(a_source, a_event);
				}
				static inline REL::Relocation<decltype(thunk)> func;
			};

			void Install()
			{
				REL::Relocation<std::uintptr_t> target{ REL_ID(37832, 38786), OFFSET(0x1C3, 0x29B) };
				stl::write_thunk_call<SendHitEvent>(target.address());

				logger::info("Hooked Magic Hit"sv);
			}
		}

		namespace Weapon
		{
			namespace Actor
			{
				REL::Relocation<std::uintptr_t> target{ REL_ID(37633, 38586), OFFSET(0x16A, 0xFA) };

				struct SendHitEvent
				{
#ifdef SKYRIM_AE
					// SendHitEventStruct was inlined
					static void thunk(RE::AIProcess* a_targetProcess, RE::HitData& a_data)
					{
						func(a_targetProcess, a_data);

						const auto hitTarget = a_targetProcess ? a_targetProcess->GetUserData() : nullptr;

						const auto aggressor = a_data.aggressor.get();
						const auto source = a_data.weapon ? a_data.weapon : RE::TESForm::LookupByID<RE::TESObjectWEAP>(0x1F4);  //unarmed

						if (hitTarget) {
							const auto powerAttack = a_data.flags.any(RE::HitData::Flag::kPowerAttack);
							const auto sneakAttack = a_data.flags.any(RE::HitData::Flag::kSneakAttack);
							const auto bashAttack = a_data.flags.any(RE::HitData::Flag::kBash, RE::HitData::Flag::kTimedBash);
							const auto hitBlocked = a_data.flags.any(RE::HitData::Flag::kBlocked, RE::HitData::Flag::kBlockWithWeapon);

							GameEventHolder::GetSingleton()->onHit.QueueEvent(
								hitTarget,
								[=](const Filter::Hit& a_filter, bool a_match) {
									return a_match == a_filter.PassesFilter(aggressor.get(), source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
								},
								aggressor.get(), source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
						}

						if (aggressor && hitTarget) {
							GameEventHolder::GetSingleton()->weaponHit.QueueEvent(aggressor.get(), hitTarget, source, nullptr, stl::to_underlying(*a_data.flags));
						}
					}
#else
					static void thunk(RE::ScriptEventSourceHolder* a_holder,
						RE::NiPointer<RE::TESObjectREFR>& a_target,
						RE::NiPointer<RE::TESObjectREFR>& a_aggressor,
						RE::FormID a_source,
						RE::FormID a_projectile,
						RE::HitData& a_data)
					{
						const auto aggressor = a_aggressor.get();
						const auto hitTarget = a_target.get();
						const auto source = RE::TESForm::LookupByID(a_source);

						if (hitTarget) {
							const auto powerAttack = a_event.flags.any(RE::TESHitEvent::Flag::kPowerAttack);
							const auto sneakAttack = a_event.flags.any(RE::TESHitEvent::Flag::kSneakAttack);
							const auto bashAttack = a_event.flags.any(RE::TESHitEvent::Flag::kBashAttack);
							const auto hitBlocked = a_event.flags.any(RE::TESHitEvent::Flag::kHitBlocked);

							GameEventHolder::GetSingleton()->onHit.QueueEvent(
								hitTarget,
								[=](const Filter::Hit& a_filter, bool a_match) {
									return a_match == a_filter.PassesFilter(aggressor, source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
								},
								aggressor, source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
						}

						if (aggressor) {
							GameEventHolder::GetSingleton()->weaponHit.QueueEvent(aggressor, hitTarget, source, nullptr, stl::to_underlying(a_data.flags));
						}

						func(a_holder, a_target, a_aggressor, a_source, a_projectile, a_data);
					}
#endif
					static inline REL::Relocation<decltype(thunk)> func;
				};
			}

			namespace Static
			{
				REL::Relocation<std::uintptr_t> target{ REL_ID(37674, 38628), OFFSET(0x6C7, 0x785) };

				struct SendHitEvent
				{
					static void thunk(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
					{
						const auto aggressor = a_event.cause.get();
						const auto hitTarget = a_event.target.get();
						const auto source = RE::TESForm::LookupByID(a_event.source);

						if (hitTarget) {
							const auto powerAttack = a_event.flags.any(RE::TESHitEvent::Flag::kPowerAttack);
							const auto sneakAttack = a_event.flags.any(RE::TESHitEvent::Flag::kSneakAttack);
							const auto bashAttack = a_event.flags.any(RE::TESHitEvent::Flag::kBashAttack);
							const auto hitBlocked = a_event.flags.any(RE::TESHitEvent::Flag::kHitBlocked);

							GameEventHolder::GetSingleton()->onHit.QueueEvent(
								hitTarget,
								[=](const Filter::Hit& a_filter, bool a_match) {
									return a_match == a_filter.PassesFilter(aggressor, source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
								},
								aggressor, source, nullptr, powerAttack, sneakAttack, bashAttack, hitBlocked);
						}

						if (aggressor) {
							GameEventHolder::GetSingleton()->weaponHit.QueueEvent(aggressor, hitTarget, source, nullptr, 0);  // NOLINT(readability-suspicious-call-argument)
						}

						func(a_source, a_event);
					}
					static inline REL::Relocation<decltype(thunk)> func;
				};
			}

			namespace Projectile
			{
				REL::Relocation<std::uintptr_t> target{ REL_ID(43022, 44213), OFFSET(0x38D, 0x357) };

				struct SendHitEvent
				{
					static void thunk(RE::BSTEventSource<RE::TESHitEvent>& a_source, RE::TESHitEvent& a_event)
					{
						const auto aggressor = a_event.cause.get();
						const auto hitTarget = a_event.target.get();
						const auto source = RE::TESForm::LookupByID(a_event.source);
						const auto projectile = RE::TESForm::LookupByID<RE::BGSProjectile>(a_event.projectile);

						if (hitTarget) {
							const auto powerAttack = a_event.flags.any(RE::TESHitEvent::Flag::kPowerAttack);
							const auto sneakAttack = a_event.flags.any(RE::TESHitEvent::Flag::kSneakAttack);
							const auto bashAttack = a_event.flags.any(RE::TESHitEvent::Flag::kBashAttack);
							const auto hitBlocked = a_event.flags.any(RE::TESHitEvent::Flag::kHitBlocked);

							GameEventHolder::GetSingleton()->onHit.QueueEvent(
								hitTarget,
								[=](const Filter::Hit& a_filter, bool a_match) {
									return a_match == a_filter.PassesFilter(aggressor, source, projectile, powerAttack, sneakAttack, bashAttack, hitBlocked);
								},
								aggressor, source, projectile, powerAttack, sneakAttack, bashAttack, hitBlocked);
						}

						if (aggressor) {
							if (projectile && projectile->data.types.all(RE::BGSProjectileData::Type::kArrow)) {
								GameEventHolder::GetSingleton()->weaponHit.QueueEvent(aggressor, hitTarget, source, projectile, 0);
							}
							GameEventHolder::GetSingleton()->projectileHit.QueueEvent(aggressor, hitTarget, source, projectile);
						}

						func(a_source, a_event);
					}
					static inline REL::Relocation<decltype(thunk)> func;
				};
			}

			inline void Install()
			{
				stl::write_thunk_call<Actor::SendHitEvent>(Actor::target.address());
				stl::write_thunk_call<Static::SendHitEvent>(Static::target.address());
				stl::write_thunk_call<Projectile::SendHitEvent>(Projectile::target.address());

				logger::info("Hooked Weapon Hit"sv);
			}
		}
	}
}

namespace Event
{
	void RegisterHookEvents()
	{
		logger::info("{:*^30}", "HOOKED EVENTS"sv);

		FallLongDistance::Install();
		ItemCrafted::Install();
		BooksRead::Install();
		Resurrect::Install();
		Reanimate::Install();
		Weather::Install();

		Combat::MagicEffectApply::Install();
		Combat::Hit::Magic::Install();
		Combat::Hit::Weapon::Install();
	}
}
