#include <mbgl/util/logging.hpp>
#include <mbgl/util/enum.hpp>

#include <mbgl_wrapper/logging_to_func.h>

#include <fstream>

namespace mbgl {

mbgl_wrapper::log_pfn log_func = nullptr;

void setLogFunc(mbgl_wrapper::log_pfn log_func_) {
	log_func = log_func_;
}

void Log::platformRecord(EventSeverity severity, const std::string &msg) {
	if (log_func) {
		log_func(static_cast<mbgl_wrapper::log_severity_t>(severity), msg.c_str());
	}
}

} // namespace mbgl
