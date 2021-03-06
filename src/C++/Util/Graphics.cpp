#include "Util/Graphics.h"

namespace util
{
	namespace COLOR
	{
		RE::NiColor Blend(const RE::NiColor& a_src, const RE::NiColor& a_dest, BLEND_MODE a_mode, float a_alpha)
		{
			RE::NiColor color;

			switch (a_mode) {
			case BLEND_MODE::kDarken:
				color = detail::darken(a_src, a_dest);
				break;
			case BLEND_MODE::kMultiply:
				color = detail::multiply(a_src, a_dest);
				break;
			case BLEND_MODE::kColorBurn:
				color = detail::colorBurn(a_src, a_dest);
				break;
			case BLEND_MODE::kLinearBurn:
				color = detail::linearBurn(a_src, a_dest);
				break;
			case BLEND_MODE::kDarkerColor:
				color = detail::darkerColor(a_src, a_dest);
				break;
			case BLEND_MODE::kLighten:
				color = detail::lighten(a_src, a_dest);
				break;
			case BLEND_MODE::kScreen:
				color = detail::screen(a_src, a_dest);
				break;
			case BLEND_MODE::kColorDodge:
				color = detail::colorDodge(a_src, a_dest);
				break;
			case BLEND_MODE::kLinearDodge:
				color = detail::linearDodge(a_src, a_dest);
				break;
			case BLEND_MODE::kLighterColor:
				color = detail::lighterColor(a_src, a_dest);
				break;
			case BLEND_MODE::kOverlay:
				color = detail::overlay(a_src, a_dest);
				break;
			case BLEND_MODE::kSoftLight:
				color = detail::softLight(a_src, a_dest);
				break;
			case BLEND_MODE::kHardLight:
				color = detail::hardLight(a_src, a_dest);
				break;
			case BLEND_MODE::kVividLight:
				color = detail::vividLight(a_src, a_dest);
				break;
			case BLEND_MODE::kLinearLight:
				color = detail::linearLight(a_src, a_dest);
				break;
			case BLEND_MODE::kPinLight:
				color = detail::pinLight(a_src, a_dest);
				break;
			case BLEND_MODE::kHardMix:
				color = detail::hardMix(a_src, a_dest);
				break;
			case BLEND_MODE::kDifference:
				color = detail::difference(a_src, a_dest);
				break;
			case BLEND_MODE::kExclusion:
				color = detail::exclusion(a_src, a_dest);
				break;
			case BLEND_MODE::kSubtract:
				color = detail::subtract(a_src, a_dest);
				break;
			case BLEND_MODE::kDivide:
				color = detail::divide(a_src, a_dest);
				break;
			default:
				color = a_dest;
			}
			color = detail::clamp(color, 0.0, 1.0);

			return detail::mix(a_src, color, std::max(0.0f, a_alpha));
		}

		float CalcLuminance(const RE::NiColor& a_src)
		{
			return 0.2126f * a_src.red + 0.7152f * a_src.green + 0.0722f * a_src.blue;
		}

		RE::NiColor Mix(const RE::NiColor& a_src, const RE::NiColor& a_dest, float a_percentage)
		{
			return detail::mix(a_src, a_dest, a_percentage);
		}
	}

	namespace MATERIAL
	{
		std::string_view MATERIAL::get_material(MAT a_mat)
		{
			return materialMap.at(a_mat);
		}
	}

	namespace RESET
	{
		void stop_all_skin_shaders(RE::TESObjectREFR* a_ref)
		{
			using Flags = RE::EffectShaderData::Flags;

			if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists) {
				auto handle = a_ref->CreateRefHandle();
				processLists->GetShaderEffects([&](RE::ShaderReferenceEffect& a_shaderEffect) {
					if (a_shaderEffect.target == handle) {
						if (const auto effectData = a_shaderEffect.effectData; effectData &&
																			   effectData->data.flags.all(Flags::kSkinOnly) &&
																			   !effectData->holesTexture.textureName.empty()) {
							a_shaderEffect.finished = true;
						}
					}
					return true;
				});
			}
		}

		std::pair<bool, ResetData> get_data(RE::NiAVObject* a_object)
		{
			ResetData resetData;
			bool success = false;

			if (!a_object->extra || a_object->extraDataSize == 0) {
				return { success, resetData };
			}

			auto& [toggle, tintSkin, tintHair, alphaSkin, TXSTFace, alphaHDPT, TXST, TXSTSkin, shader] = resetData;

			std::span<RE::NiExtraData*> span(a_object->extra, a_object->extraDataSize);
			for (auto& extraData : span) {
				if (!extraData) {
					continue;
				}
				if (const auto name = extraData->GetName(); !name.empty()) {
					switch (string::const_hash(name)) {
					case string::const_hash(EXTRA::TOGGLE):
						{
							toggle = static_cast<RE::NiStringsExtraData*>(extraData);
							success = true;
						}
						break;
					case string::const_hash(EXTRA::SKIN_TINT):
						{
							tintSkin = static_cast<RE::NiIntegerExtraData*>(extraData);
							success = true;
						}
						break;
					case string::const_hash(EXTRA::HAIR_TINT):
						{
							tintHair = static_cast<RE::NiIntegerExtraData*>(extraData);
							success = true;
						}
						break;
					case string::const_hash(EXTRA::SKIN_ALPHA):
						{
							alphaSkin = static_cast<RE::NiBooleanExtraData*>(extraData);
							success = true;
						}
						break;
					case string::const_hash(EXTRA::FACE_TXST):
						{
							TXSTFace = static_cast<RE::NiStringsExtraData*>(extraData);
							success = true;
						}
						break;
					default:
						if (boost::icontains(name.c_str(), EXTRA::HEADPART)) {
							alphaHDPT.emplace_back(static_cast<RE::NiIntegerExtraData*>(extraData));
							success = true;

						} else if (boost::icontains(name.c_str(), EXTRA::TXST)) {
							TXST.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
							success = true;

						} else if (boost::icontains(name.c_str(), EXTRA::SKIN_TXST)) {
							TXSTSkin.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
							success = true;

						} else if (boost::icontains(name.c_str(), EXTRA::SHADER)) {
							shader.emplace_back(static_cast<RE::NiStringsExtraData*>(extraData));
							success = true;
						}
						break;
					}
				}
			}

			return { success, resetData };
		}

		void Toggle(RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
		{
			if (a_data && a_data->value && a_data->size > 0) {
				std::span<char*> span(a_data->value, a_data->size);
				for (auto& string : span) {
					if (!string::is_empty(string)) {
						if (const auto object = a_root->GetObjectByName(string); object) {
							object->CullNode(false);
						}
					}
				}
				a_root->RemoveExtraData(a_data->GetName());
			}
		}

		void SkinTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data)
		{
			if (a_data) {
				const auto actorbase = a_actor->GetActorBase();
				const auto facePart = actorbase ? actorbase->GetCurrentHeadPartByType(HeadPartType::kFace) : nullptr;
				const auto faceNode = a_actor->GetFaceNodeSkinned();

				if (faceNode && facePart) {
					if (const auto faceGen = RE::BSFaceGenManager::GetSingleton(); faceGen) {
						faceGen->PrepareHeadPartForShaders(faceNode, facePart, actorbase);
					}
					a_root->UpdateBodyTint(actorbase->bodyTintColor);
					a_root->RemoveExtraData(a_data->GetName());
				}
			}
		}

		void HairTint(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiIntegerExtraData* a_data)
		{
			if (a_data) {
				const auto actorBase = a_actor->GetActorBase();
				const auto headData = actorBase ? actorBase->headRelatedData : nullptr;
				const auto colorForm = headData ? headData->hairColor : nullptr;

				if (colorForm) {
					a_root->UpdateHairColor(colorForm->color);
					a_root->RemoveExtraData(a_data->GetName());
				}
			}
		}

		void SkinAlpha(RE::NiAVObject* a_root, RE::NiBooleanExtraData* a_data)
		{
			if (a_data) {
				a_root->UpdateMaterialAlpha(1.0f, true);
				a_root->RemoveExtraData(a_data->GetName());
			}
		}

		void HeadPartAlpha(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiIntegerExtraData*>& a_vec)
		{
			for (auto& data : a_vec) {
				if (data) {
					if (const auto object = a_actor->GetHeadPartObject(static_cast<HeadPartType>(data->value)); object) {
						object->UpdateMaterialAlpha(1.0f, false);
						a_root->RemoveExtraData(data->GetName());
					}
				}
			}
		}

		void reset_textureset(RE::NiAVObject* a_object, RE::BSShaderTextureSet* a_txst, bool a_doOnlySkin, std::string_view a_folder)
		{
			using Texture = RE::BSTextureSet::Texture;

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				auto effect = a_geometry->properties[States::kEffect];
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
				if (lightingShader) {
					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						if (!a_doOnlySkin) {
							if (auto textureSet = material->textureSet.get(); !a_folder.empty() && textureSet) {
								std::string sourcePath{ textureSet->GetTexturePath(Texture::kDiffuse) };
								if (TEXTURE::sanitize_path(sourcePath); sourcePath.find(a_folder) == std::string::npos) {
									return RE::BSVisit::BSVisitControl::kContinue;
								}
							}
						} else {
							if (const auto feature = material->GetFeature(); feature != Feature::kFaceGenRGBTint && feature != Feature::kFaceGen) {
								return RE::BSVisit::BSVisitControl::kContinue;
							}
						}

						if (auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create()); newMaterial) {
							newMaterial->CopyMembers(material);
							newMaterial->ClearTextures();
							newMaterial->OnLoadTextureSet(0, a_txst);
							lightingShader->SetMaterial(newMaterial, true);
							lightingShader->InitializeGeometry(a_geometry);
							lightingShader->InitializeShader(a_geometry);
							newMaterial->~BSLightingShaderMaterialBase();
							RE::free(newMaterial);
						}
					}
				}
				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}

		void FaceTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, RE::NiStringsExtraData* a_data)
		{
			if (a_data && a_data->value) {
				const auto textureset = TEXTURE::create_textureset(a_data->value);
				const auto faceObject = a_actor->GetHeadPartObject(HeadPartType::kFace);
				if (textureset && faceObject) {
					reset_textureset(faceObject, textureset, true, std::string());
					a_root->RemoveExtraData(a_data->GetName());
				}
			}
		}

		void ArmorTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const RE::BSFixedString& a_folderName, const std::vector<RE::NiStringsExtraData*>& a_vec)
		{
			if (a_vec.empty()) {
				return;
			}

			std::string folder{ a_folderName };
			TEXTURE::sanitize_path(folder);

			for (auto& data : a_vec) {
				if (data && data->value && data->size > 0) {
					RE::FormID formID = 0;
					if (std::string armorID{ data->value[data->size - 1] }; !armorID.empty()) {
						try {
							formID = string::lexical_cast<RE::FormID>(armorID);
						} catch (...) {
							continue;
						}
					}
					auto armor = a_actor->GetWornArmor(formID);
					if (!armor) {
						armor = a_actor->GetSkin();
					}
					if (armor) {
						const auto armorAddon = armor->GetArmorAddon(a_actor->GetRace());
						const auto textureset = TEXTURE::create_textureset(data->value);
						if (textureset && armorAddon) {
							a_actor->VisitArmorAddon(armor, armorAddon, [&](bool, RE::NiAVObject& a_obj) -> bool {
								reset_textureset(&a_obj, textureset, false, folder);
								return true;
							});
							a_root->RemoveExtraData(data->GetName());
						}
					}
				}
			}
		}

		void SkinTXST(RE::Actor* a_actor, RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
		{
			using Slot = RE::BGSBipedObjectForm::BipedObjectSlot;

			if (a_vec.empty()) {
				return;
			}

			for (auto& data : a_vec) {
				if (data && data->value && data->size > 0) {
					auto slot = Slot::kNone;
					if (std::string slotMaskstr{ data->value[data->size - 1] }; !slotMaskstr.empty()) {
						try {
							slot = string::lexical_cast<Slot>(slotMaskstr);
						} catch (...) {
							continue;
						}
					}
					if (const auto skinArmor = a_actor->GetSkin(slot); skinArmor) {
						const auto armorAddon = skinArmor->GetArmorAddonByMask(a_actor->GetRace(), slot);
						const auto textureset = TEXTURE::create_textureset(data->value);
						if (textureset && armorAddon) {
							a_actor->VisitArmorAddon(skinArmor, armorAddon, [&](bool, RE::NiAVObject& a_obj) -> bool {
								reset_textureset(&a_obj, textureset, true, std::string());
								return true;
							});
							a_root->RemoveExtraData(data->GetName());
						}
					}
				}
			}
		}

		std::pair<bool, ShaderData> get_original_shaders(RE::NiStringsExtraData* a_data)
		{
			ShaderData shaderData;
			bool result = true;

			if (a_data && a_data->value && a_data->size > 0) {
				auto& [textureSet, feature, flags, emissiveColor, emissiveMult] = shaderData;
				if (const auto new_txst = TEXTURE::create_textureset(a_data->value)) {
					try {
						textureSet = new_txst;
						feature = string::lexical_cast<Feature>(a_data->value[9]);
						flags = string::lexical_cast<std::uint64_t>(a_data->value[10]);
						emissiveColor = RE::NiColor(
							string::lexical_cast<std::uint32_t>(a_data->value[11]));
						emissiveMult = string::lexical_cast<float>(a_data->value[12]);
					} catch (...) {
						result = false;
					}
				} else {
					result = false;
				}
			}

			return { result, shaderData };
		}

		void reset_shaderdata(RE::NiAVObject* a_object, std::vector<RE::BSFixedString>& a_geometries)
		{
			using Flag = RE::BSShaderProperty::EShaderPropertyFlag;

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				if (std::ranges::find(a_geometries, a_geometry->name) == a_geometries.end()) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}

				auto effect = a_geometry->properties[States::kEffect];
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
				if (lightingShader) {
					auto originalData = lightingShader->GetExtraData<RE::NiStringsExtraData>(EXTRA::ORIG_SHADER);
					if (!originalData) {
						return RE::BSVisit::BSVisitControl::kContinue;
					}

					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						bool result;
						ShaderData shaderData;

						std::tie(result, shaderData) = get_original_shaders(originalData);
						if (!result) {
							logger::warn("unable to get original shader values for {}", a_geometry->name);
							return RE::BSVisit::BSVisitControl::kContinue;
						}

						auto& [textureSet, feature, flags, emissiveColor, emissiveMult] = shaderData;

						if (const auto newMaterial = RE::BSLightingShaderMaterialBase::CreateMaterial(feature); newMaterial) {
							newMaterial->CopyBaseMembers(material);
							newMaterial->ClearTextures();
							newMaterial->OnLoadTextureSet(0, textureSet);

							lightingShader->flags = static_cast<RE::BSShaderProperty::EShaderPropertyFlag>(flags);
							lightingShader->lastRenderPassState = std::numeric_limits<std::int32_t>::max();
							if (lightingShader->flags.all(Flag::kOwnEmit)) {
								if (!lightingShader->emissiveColor) {
									lightingShader->emissiveColor = new RE::NiColor();
								}
								lightingShader->emissiveColor->red = emissiveColor.red;
								lightingShader->emissiveColor->green = emissiveColor.green;
								lightingShader->emissiveColor->blue = emissiveColor.blue;
							}
							lightingShader->emissiveMult = emissiveMult;

							lightingShader->SetMaterial(newMaterial, true);
							lightingShader->InitializeGeometry(a_geometry);
							lightingShader->InitializeShader(a_geometry);

							newMaterial->~BSLightingShaderMaterialBase();
							RE::free(newMaterial);

							lightingShader->RemoveExtraData(originalData->GetName());
							a_geometry->RemoveExtraData(originalData->GetName());
						}
					}
				}

				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}

		void MaterialShader(RE::NiAVObject* a_root, const std::vector<RE::NiStringsExtraData*>& a_vec)
		{
			if (a_vec.empty()) {
				return;
			}

			for (auto& data : a_vec) {
				if (data && data->value && data->size > 0) {
					std::vector<RE::BSFixedString> vec({ data->value, data->value + data->size });
					reset_shaderdata(a_root, vec);

					a_root->RemoveExtraData(data->GetName());
				}
			}
		}
	}

	namespace SET
	{
		void tint_face(RE::Actor* a_actor, const RE::NiColor& a_color)
		{
			const auto object = a_actor->GetHeadPartObject(RE::BGSHeadPart::HeadPartType::kFace);
			const auto geometry = object ? object->AsGeometry() : nullptr;

			if (geometry) {
				using Feature = RE::BSShaderMaterial::Feature;
				using Flag = RE::BSShaderProperty::EShaderPropertyFlag8;
				using States = RE::BSGeometry::States;

				using Material = RE::BSLightingShaderMaterialBase;

				auto effect = geometry->properties[States::kEffect];
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
				if (lightingShader) {
					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material && material->GetFeature() == Feature::kFaceGen) {
						auto facegenTint = Material::CreateMaterial<RE::BSLightingShaderMaterialFacegenTint>();
						if (facegenTint) {
							facegenTint->CopyBaseMembers(material);
							lightingShader->SetFlags(Flag::kFace, false);
							lightingShader->SetFlags(Flag::kFaceGenRGBTint, true);
							lightingShader->SetMaterial(facegenTint, true);
							lightingShader->InitializeShader(geometry);
							facegenTint->~BSLightingShaderMaterialFacegenTint();
							RE::free(facegenTint);
						}
					}
				}

				geometry->UpdateBodyTint(a_color);
			}
		}

		void update_color_data(RE::NiAVObject* a_root, const RE::BSFixedString& a_name, const RE::NiColor& a_color)
		{
			if (const auto data = a_root->GetExtraData<RE::NiIntegerExtraData>(a_name); data) {
				if (a_color != RE::NiColor(data->value)) {
					data->value = RE::NiColor::ColorToInt(a_color);
				}
			} else {
				if (auto newData = RE::NiIntegerExtraData::Create(a_name, RE::NiColor::ColorToInt(a_color)); newData) {
					a_root->AddExtraData(newData);
				}
			}
		}

		void Toggle(RE::NiAVObject* a_root, RE::NiAVObject* a_node, bool a_cull)
		{
			a_node->CullNode(a_cull);

			if (auto data = a_root->GetExtraData<RE::NiStringsExtraData>(EXTRA::TOGGLE); data) {
				a_cull ? data->Insert(a_node->name) : data->Remove(a_node->name);
			} else if (a_cull) {
				std::vector<RE::BSFixedString> vec{ a_node->name };
				if (const auto newData = RE::NiStringsExtraData::Create(EXTRA::TOGGLE, vec); newData) {
					a_root->AddExtraData(newData);
				}
			}
		}

		void ArmorTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::int32_t a_type, std::string_view a_tgtPath, bool& replaced)
		{
			using Feature = RE::BSShaderMaterial::Feature;
			using Texture = RE::BSTextureSet::Texture;

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
				if (lightingShader) {
					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						if (const auto textureSet = material->textureSet.get(); textureSet) {
							std::string sourcePath{ textureSet->GetTexturePath(Texture::kDiffuse) };
							TEXTURE::sanitize_path(sourcePath);

							if (sourcePath == a_tgtPath) {
								if (auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create()); newMaterial) {
									newMaterial->CopyMembers(material);
									newMaterial->ClearTextures();

									if (a_type == -1) {
										newMaterial->OnLoadTextureSet(0, a_txst);
									} else {
										if (auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
											const auto BSTextureType = static_cast<Texture>(a_type);
											for (const auto& type : TEXTURE::types) {
												if (type != BSTextureType) {
													newTextureSet->SetTexturePath(type, textureSet->GetTexturePath(type));
												}
											}
											newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));
											newMaterial->OnLoadTextureSet(0, newTextureSet);
										}
									}

									lightingShader->SetMaterial(newMaterial, true);
									lightingShader->InitializeGeometry(a_geometry);
									lightingShader->InitializeShader(a_geometry);
									newMaterial->~BSLightingShaderMaterialBase();
									RE::free(newMaterial);

									replaced = true;
								}
							}
						}
					}
				}

				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}

		void SkinTXST(RE::NiAVObject* a_object, RE::BGSTextureSet* a_txst, std::vector<RE::BSFixedString>& a_vec, std::int32_t a_type)
		{
			using Feature = RE::BSShaderMaterial::Feature;
			using Texture = RE::BSTextureSet::Texture;

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				auto effect = a_geometry->properties[RE::BSGeometry::States::kEffect];
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());
				if (lightingShader) {
					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					if (material) {
						auto const feature = material->GetFeature();
						if (const auto textureSet = material->textureSet.get(); textureSet && stl::is(feature, Feature::kFaceGenRGBTint, Feature::kFaceGen)) {
							if (a_vec.empty()) {
								a_vec.reserve(Texture::kTotal);
								for (auto& type : TEXTURE::types) {
									a_vec.emplace_back(textureSet->GetTexturePath(type));
								}
							}
							if (auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(material->Create()); newMaterial) {
								newMaterial->CopyMembers(material);
								newMaterial->ClearTextures();

								if (a_type == -1) {
									if (feature == Feature::kFaceGen) {
										if (auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
											for (auto& type : TEXTURE::types) {
												if (type != Texture::kMultilayer) {
													newTextureSet->SetTexturePath(type, a_txst->GetTexturePath(type));
												}
											}
											newTextureSet->SetTexturePath(Texture::kMultilayer, textureSet->GetTexturePath(Texture::kMultilayer));
											newMaterial->OnLoadTextureSet(0, newTextureSet);
										}
									} else {
										newMaterial->OnLoadTextureSet(0, a_txst);
									}
								} else {
									if (auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
										const auto BSTextureType = static_cast<Texture>(a_type);
										for (const auto& type : TEXTURE::types) {
											if (type != BSTextureType) {
												newTextureSet->SetTexturePath(type, material->textureSet->GetTexturePath(type));
											}
										}
										newTextureSet->SetTexturePath(BSTextureType, a_txst->GetTexturePath(BSTextureType));
										newMaterial->OnLoadTextureSet(0, newTextureSet);
									}
								}

								lightingShader->SetMaterial(newMaterial, true);
								lightingShader->InitializeGeometry(a_geometry);
								lightingShader->InitializeShader(a_geometry);
								newMaterial->~BSLightingShaderMaterialBase();
								RE::free(newMaterial);
							}
						}
					}
				}

				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}

		void ArmorSkinTXST(RE::Actor* a_actor, RE::BGSTextureSet* a_txst, RE::BIPED_MODEL::BipedObjectSlot a_slot, std::int32_t a_type)
		{
			const auto skinArmor = a_actor->GetSkin(a_slot);
			if (!skinArmor) {
				return;
			}

			const auto foundAddon = skinArmor->GetArmorAddonByMask(a_actor->GetRace(), a_slot);
			if (!foundAddon) {
				return;
			}

			auto task = SKSE::GetTaskInterface();
			task->AddTask([a_actor, a_txst, a_slot, a_type, skinArmor, foundAddon]() {
				std::vector<RE::BSFixedString> result;
				a_actor->VisitArmorAddon(skinArmor, foundAddon, [&](bool, RE::NiAVObject& a_obj) -> bool {
					SET::SkinTXST(&a_obj, a_txst, result, a_type);
					return true;
				});

				const auto root = a_actor->Get3D(false);
				if (!result.empty() && root) {
					auto slotMaskStr = std::to_string(stl::to_underlying(a_slot));
					auto name = "PO3_SKINTXST - " + slotMaskStr;
					result.emplace_back(slotMaskStr);

					SET::add_data_if_none<RE::NiStringsExtraData>(root, name, result);
				}
			});
		}

		void ShaderType(RE::NiAVObject* a_object, const RE::BSGeometry* a_template, std::string_view a_path, std::int32_t a_textureType, std::vector<RE::BSFixedString>& a_vec, const std::vector<bool>& a_params)
		{
			using State = RE::BSGeometry::States;
			using Feature = RE::BSShaderMaterial::Feature;
			using Texture = RE::BSTextureSet::Texture;
			using Flags = RE::BSShaderProperty::EShaderPropertyFlag8;
			using VertexFlags = RE::NiSkinPartition::Vertex::Flags;

			bool noWeap = a_params[0];
			bool noAlpha = a_params[1];
			bool isActor = a_params[2];

			RE::BSVisit::TraverseScenegraphGeometries(a_object, [&](RE::BSGeometry* a_geometry) -> RE::BSVisit::BSVisitControl {
				if (const bool hasNormals = a_geometry->HasVertexFlag(VertexFlags::kNormal); !hasNormals) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}
				if (const auto parent = a_geometry->parent; parent && parent->AsFadeNode() && noWeap && isActor) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}
				if (const auto alpha = a_geometry->properties[State::kProperty].get(); alpha && noAlpha) {
					return RE::BSVisit::BSVisitControl::kContinue;
				}

				auto effect = a_geometry->properties[State::kEffect];
				auto lightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(effect.get());

				auto tempEffect = a_template->properties[State::kEffect];
				auto tempLightingShader = netimmerse_cast<RE::BSLightingShaderProperty*>(tempEffect.get());

				if (lightingShader && tempLightingShader) {
					auto material = static_cast<RE::BSLightingShaderMaterialBase*>(lightingShader->material);
					auto tempMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(tempLightingShader->material);

					if (material && tempMaterial) {
						const auto textureSet = material->textureSet.get();
						const auto tempTextureSet = tempMaterial->textureSet.get();

						if (textureSet && tempTextureSet) {
							std::string sourceDiffuse{ textureSet->GetTexturePath(Texture::kDiffuse) };
							TEXTURE::sanitize_path(sourceDiffuse);
							if (!a_path.empty() && sourceDiffuse.find(a_path) == std::string::npos) {
								return RE::BSVisit::BSVisitControl::kContinue;
							}

							const auto oldFeature = material->GetFeature();
							const auto oldFlags = lightingShader->flags.get();
							const auto oldEmissiveColor = lightingShader->emissiveColor ? RE::NiColor::ColorToString(*lightingShader->emissiveColor) : "000000"sv;
							const auto oldEmissiveMult = lightingShader->emissiveMult;

							const auto newFeature = tempMaterial->GetFeature();

							if (oldFeature != newFeature) {
								if (const auto data = lightingShader->GetExtraData<RE::NiStringsExtraData>(EXTRA::ORIG_SHADER); !data) {
									std::vector<RE::BSFixedString> resetData;
									resetData.reserve(12);
									for (auto& type : TEXTURE::types) {
										resetData.emplace_back(textureSet->GetTexturePath(type));  //0-8
									}
									resetData.emplace_back(std::to_string(stl::to_underlying(oldFeature)));  //9
									resetData.emplace_back(std::to_string(stl::to_underlying(oldFlags)));    //10
									resetData.emplace_back(oldEmissiveColor);                                //11
									resetData.emplace_back(std::to_string(oldEmissiveMult));                 //12

									auto newData = RE::NiStringsExtraData::Create(EXTRA::ORIG_SHADER, resetData);
									if (newData && lightingShader->AddExtraData(newData)) {
										a_vec.push_back(a_geometry->name);
									}
								}

								if (auto newMaterial = static_cast<RE::BSLightingShaderMaterialBase*>(tempMaterial->Create()); newMaterial) {
									newMaterial->CopyMembers(tempMaterial);
									newMaterial->ClearTextures();

									if (a_textureType != -1) {
										if (auto newTextureSet = RE::BSShaderTextureSet::Create(); newTextureSet) {
											const auto BSTextureType = static_cast<Texture>(a_textureType);
											for (auto& type : TEXTURE::types) {
												if (type != BSTextureType) {
													newTextureSet->SetTexturePath(type, tempTextureSet->GetTexturePath(type));
												}
											}
											newTextureSet->SetTexturePath(BSTextureType, textureSet->GetTexturePath(BSTextureType));
											newMaterial->OnLoadTextureSet(0, newTextureSet);
										}
									}

									lightingShader->CopyMembers(tempLightingShader);
									lightingShader->SetFlags(Flags::kSkinned, a_geometry->HasVertexFlag(VertexFlags::kSkinned));

									lightingShader->SetMaterial(newMaterial, true);
									lightingShader->InitializeGeometry(a_geometry);
									lightingShader->InitializeShader(a_geometry);

									newMaterial->~BSLightingShaderMaterialBase();
									RE::free(newMaterial);
								}
							}
						}
					}
				}

				return RE::BSVisit::BSVisitControl::kContinue;
			});
		}
	}

	namespace TEXTURE
	{
		void sanitize_path(std::string& a_path)
		{
			std::ranges::transform(a_path, a_path.begin(),
				[](char c) { return static_cast<char>(std::tolower(c)); });

			a_path = std::regex_replace(a_path, std::regex("/+|\\\\+"), "\\");
			a_path = std::regex_replace(a_path, std::regex("^\\\\+"), "");
			a_path = std::regex_replace(a_path, std::regex(R"(.*?[^\s]textures\\|^textures\\)", std::regex_constants::icase), "");
		}

		RE::BSShaderTextureSet* create_textureset(char** a_value)
		{
			if (auto textureset = RE::BSShaderTextureSet::Create(); textureset) {
				for (const auto& type : TEXTURE::types) {
					if (!string::is_empty(a_value[type])) {
						textureset->SetTexturePath(type, a_value[type]);
					}
				}
				return textureset;
			}
			return nullptr;
		}
	}
}
