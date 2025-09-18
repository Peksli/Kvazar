#include "LogSystem.h"


namespace Kvazar {

	std::shared_ptr<sink> LogSystem::kvazarSink;
	std::shared_ptr<sink> LogSystem::clientSink;

	std::shared_ptr<logger> LogSystem::kvazarLogger;
	std::shared_ptr<logger> LogSystem::clientLogger;

	void LogSystem::Initialize() 
	{
		kvazarSink = std::make_shared<sink>();
		clientSink = std::make_shared<sink>();

		kvazarLogger = std::make_shared<logger>("kvazarLogger", kvazarSink);
		clientLogger = std::make_shared<logger>("clientLogger", clientSink);

		kvazarLogger->set_level(spdlog::level::trace);
		clientLogger->set_level(spdlog::level::trace);
	}

}