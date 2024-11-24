/*
 * platform.h
 *
 *      Author: Andreas Volz
 */

#ifndef PLATFORM_H
#define PLATFORM_H

// filesystem ->
#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
error "Missing the <filesystem> header."
#endif
// <- filesystem

#include <string>
#include <vector>

namespace platform
{
enum class OutputMode { IGNORE, CAPTURE };

char *strdup(const char *s);

#if defined(_MSC_VER) || defined(WIN32)
typedef int mode_t;
#endif

int mkdir(const std::string &pathname, mode_t mode = 0);

int executeProcess(const std::string& program, const std::vector<std::string>& args,
                   OutputMode mode = OutputMode::IGNORE, std::string* output = nullptr);

} /* namespace platform */

#endif /* PLATFORM_H */
