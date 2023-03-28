//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/28/23
//

#pragma once

#include <gtk/gtk.h>

#include <string>
#include <map>

namespace mousetrap
{
    /// @brief log levels, changes formatting and behavior
    enum class LogLevel
    {
        /// @brief error, exits application
        FATAL = G_LOG_LEVEL_ERROR,

        /// @brief critical, signals that an error occurred but does not exit runtime
        CRITICAL = G_LOG_LEVEL_CRITICAL,

        /// @brief warning
        WARNING = G_LOG_LEVEL_WARNING,

        /// @brief info
        INFO = G_LOG_LEVEL_INFO,

        /// @brief debug
        DEBUG = G_LOG_LEVEL_DEBUG
    };

    /// @brief log domain
    using LogDomain = const char*;

    /// @brief domain for errors inside mousetrap, for internal use only
    constexpr const char* MOUSETRAP_DOMAIN = "mousetrap";

    /// @brief default user domain if no domain is specified
    constexpr const char* USER_DOMAIN = "debug";

    /// @brief uninstantiatable singleton, provides global logging functionality
    struct log
    {
        public:
            /// @brief ctor deleted, singleton instance that cannot be instantiated
            log() = delete;

            /// @brief initiailze the logging suite, automatically called during construction of mousetrap::Application
            static void initialize();

            /// @brief print a debug message, not printed unless set_surpress_debug is set to false
            /// @param message message
            /// @param domain logging domain, optional
            static void debug(const std::string& message, LogDomain = USER_DOMAIN);

            /// @brief print a info message, usually reserved to benign logging, printed unless set_surpress_info is set to true
            /// @param message message
            /// @param domain logging domain, optional
            static void info(const std::string& message, LogDomain = USER_DOMAIN);

            /// @brief
            /// @param message message
            /// @param domain logging domain, optional
            static void warning(const std::string& message, LogDomain = USER_DOMAIN);

            /// @brief
            /// @param message message
            /// @param domain logging domain, optional
            static void critical(const std::string& message, LogDomain = USER_DOMAIN);

            /// @brief
            /// @param message message
            /// @param domain logging domain, optional
            static void fatal(const std::string& message, LogDomain = USER_DOMAIN);

            /// @brief surpress debug level log message for given log domain. true by default
            /// @param domain logging domain
            /// @param b true if debug messages should be surpressed, false otherwise
            static void set_surpress_debug(LogDomain, bool);

            /// @brief surpress info level log message for given log domain. true by default
            /// @param domain logging domain
            /// @param b true if info messages should be surpressed, false otherwise
            static void set_surpress_info(LogDomain, bool);

        private:
            static inline bool _initialized = false;
            static inline bool _forward_to_file = false;
            static GLogWriterOutput log_writer(GLogLevelFlags log_level, const GLogField* fields, gsize n_fields, gpointer);

            static inline std::map<LogDomain, bool> _allow_debug;
            static inline std::map<LogDomain, bool> _allow_info;
    };

}