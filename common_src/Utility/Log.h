#pragma once

#include "spdlog/sinks/ostream_sink.h"
#include "spdlog/fmt/bundled/std.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h"


std::string ThisThreadIdToString();


#ifdef _DEBUG
#define LOG_INFO(Message, ...)																													\
{																																				\
	const std::string utilityInfo = fmt::format("[{0}({1}), {2}(Line {3})] ", "ThreadId", ThisThreadIdToString(), __FUNCTION__, __LINE__);		\
	const std::string FinalMessage = utilityInfo + Message;																						\
	spdlog::info(FinalMessage, ##__VA_ARGS__);																									\
}

#define LOG_WARN(Message, ...)																													\
{																																				\
	const std::string utilityInfo = fmt::format("[{0}({1}), {2}(Line {3})] ", "ThreadId", ThisThreadIdToString(), __FUNCTION__, __LINE__);		\
	const std::string coloredMessage = fmt::format("\x1B[38;5;11m{}\x1B[0m", fmt::format(Message, ##__VA_ARGS__));								\
	const std::string FinalMessage = utilityInfo + coloredMessage;																				\
	spdlog::warn(FinalMessage, ##__VA_ARGS__);																									\
}

#define LOG_ERROR(Message, ...)																													\
{																																				\
	const std::string utilityInfo = fmt::format("[{0}({1}), {2}(Line {3})] ", "ThreadId", ThisThreadIdToString(), __FUNCTION__, __LINE__);		\
	const std::string coloredMessage = fmt::format("\x1B[38;5;9m{}\x1B[0m", fmt::format(Message, ##__VA_ARGS__));								\
	const std::string FinalMessage = utilityInfo + coloredMessage;																				\
	spdlog::error(FinalMessage, ##__VA_ARGS__);																									\
}

#define LOG_CRITICAL(Message, ...)																												\
{																																				\
	const std::string utilityInfo = fmt::format("[{0}({1}), {2}(Line {3})] ", "ThreadId", ThisThreadIdToString(), __FUNCTION__, __LINE__);		\
	const std::string coloredMessage = fmt::format("\x1B[38;5;9m{}\x1B[0m", fmt::format(Message, ##__VA_ARGS__));								\
	const std::string FinalMessage = utilityInfo + coloredMessage;																				\
	spdlog::critical(FinalMessage, ##__VA_ARGS__);																								\
}

#else
#define LOG_INFO(Message, ...)

#define LOG_WARN(Message, ...)

#define LOG_ERROR(Message, ...)

#define LOG_CRITICAL(Message, ...)

#endif // _DEBUG