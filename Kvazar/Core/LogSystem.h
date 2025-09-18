#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace Kvazar {

	using sink = spdlog::sinks::stdout_color_sink_mt;
	using logger = spdlog::logger;

	class LogSystem 
	{
	public:
		static std::shared_ptr<sink> kvazarSink;
		static std::shared_ptr<sink> clientSink;

		static std::shared_ptr<logger> kvazarLogger;
		static std::shared_ptr<logger> clientLogger;

		static void Initialize();
		static std::shared_ptr<logger> GetKvazarLogger() { return kvazarLogger; }
		static std::shared_ptr<logger> GetClientLogger() { return clientLogger; }

		template<typename... Args> static void KVTrace(Args&&... args)	  { kvazarLogger->trace(std::forward<Args>(args)...);    }
		template<typename... Args> static void KVDebug(Args&&... args)	  { kvazarLogger->debug(std::forward<Args>(args)...);	 }
		template<typename... Args> static void KVInfo(Args&&... args)	  { kvazarLogger->info(std::forward<Args>(args)...);	 }
		template<typename... Args> static void KVWarn(Args&&... args)	  { kvazarLogger->warn(std::forward<Args>(args)...);	 }
		template<typename... Args> static void KVError(Args&&... args)	  { kvazarLogger->error(std::forward<Args>(args)...);	 }
		template<typename... Args> static void KVCritical(Args&&... args) { kvazarLogger->critical(std::forward<Args>(args)...); }

		template<typename... Args> static void CLTrace(Args&&... args)	  { clientLogger->trace(std::forward<Args>(args)...);	 }
		template<typename... Args> static void CLDebug(Args&&... args)	  { clientLogger->debug(std::forward<Args>(args)...);	 }
		template<typename... Args> static void CLInfo(Args&&... args)	  { clientLogger->info(std::forward<Args>(args)...);	 }
		template<typename... Args> static void CLWarn(Args&&... args)	  { clientLogger->warn(std::forward<Args>(args)...);	 }
		template<typename... Args> static void CLError(Args&&... args)	  { clientLogger->error(std::forward<Args>(args)...);	 }
		template<typename... Args> static void CLCritical(Args&&... args) { clientLogger->critical(std::forward<Args>(args)...); }
	};

}

#define KVAZAR_TRACE(...)    Kvazar::LogSystem::KVTrace(__VA_ARGS__);
#define KVAZAR_DEBUG(...)	 Kvazar::LogSystem::KVDebug(__VA_ARGS__);
#define KVAZAR_INFO(...)	 Kvazar::LogSystem::KVInfo(__VA_ARGS__);
#define KVAZAR_WARN(...)	 Kvazar::LogSystem::KVWarn(__VA_ARGS__);
#define KVAZAR_ERROR(...)	 Kvazar::LogSystem::KVError(__VA_ARGS__);
#define KVAZAR_CRITICAL(...) Kvazar::LogSystem::KVCritical(__VA_ARGS__);


#define CLIENT_TRACE(...)	 Kvazar::LogSystem::CLTrace(__VA_ARGS__);
#define CLIENT_DEBUG(...)	 Kvazar::LogSystem::CLDebug(__VA_ARGS__);
#define CLIENT_INFO(...)	 Kvazar::LogSystem::CLInfo(__VA_ARGS__);
#define CLIENT_WARN(...)	 Kvazar::LogSystem::CLWarn(__VA_ARGS__);
#define CLIENT_CRITICAL(...) Kvazar::LogSystem::CLCritical(__VA_ARGS__);