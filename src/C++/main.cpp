#include "Hooks/EventHook.h"
#include "Papyrus/Registration.h"
#include "Serialization/Manager.h"

static std::vector<std::string> DetectOldVersion()
{
	std::vector<std::string> vec;

	const auto papyrusExtender64Handle = GetModuleHandle("po3_papyrusextender64");

	std::string message;
	std::string info;

	if (papyrusExtender64Handle != nullptr) {
		logger::error("Outdated Papyrus Extender64 version detected");

		info = "Papyrus Extender - Plugin Conflict";

		message = R"(An outdated Papyrus Extender plugin has been found. This may cause script errors and CTDs with the latest version. 

Please remove po3_papyrusextender64.dll (and PO3_SKSEFunctions.pex) from any mods that include this plugin, such as "Diziet's Player Home Bath Undressing for SkyrimSE")";
	}

	if (!message.empty() && !info.empty()) {
		message += R"(


Click Ok to continue, or Cancel to quit the game)";
		vec.push_back(message);
		vec.push_back(info);
	}

	return vec;
}

void OnInit(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kPostLoad:
		{
			auto vec = DetectOldVersion();
			if (!vec.empty() && vec.size() == 2) {
				auto id = WinAPI::MessageBox(nullptr, vec[0].c_str(), vec[1].c_str(), 0x00000001);
				if (id == 2) {
					std::_Exit(EXIT_FAILURE);
				}
			}
		}
		break;
	case SKSE::MessagingInterface::kDataLoaded:
		{
			Papyrus::Events::RegisterScriptEvents();
			Papyrus::Events::RegisterStoryEvents();

			Hook::HookEvents();
		}
		break;
	default:
		break;
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= "po3_papyrusextender64.log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("[%H:%M:%S] [%l] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "powerofthree's Papyrus Extender";
	a_info->version = Version::MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("loaded");

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(115);

	const auto papyrus = SKSE::GetPapyrusInterface();
	papyrus->Register(Papyrus::Script::Register);

	const auto serialization = SKSE::GetSerializationInterface();
	serialization->SetUniqueID(Serialization::kPapyrusExtender);
	serialization->SetSaveCallback(Serialization::SaveCallback);
	serialization->SetLoadCallback(Serialization::LoadCallback);

	const auto messaging = SKSE::GetMessagingInterface();
	messaging->RegisterListener(OnInit);

	return true;
}


extern "C" DLLEXPORT const char* APIENTRY GetPluginVersion()
{
	return Version::NAME.data();
}
