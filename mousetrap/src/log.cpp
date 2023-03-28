//
// Created by clem on 3/28/23.
//

#include <include/log.hpp>
#include <iostream>

namespace mousetrap
{
    namespace detail
    {
        constexpr const char* mousetrap_level_field = "MOUSETRAP_LEVEL";
    }

    void log::initialize()
    {
        if (_initialized)
            return;
        
        g_log_set_writer_func(log_writer, nullptr, nullptr);
        _initialized = true;
    }

    GLogWriterOutput log::log_writer(GLogLevelFlags log_level, const GLogField* fields, gsize n_fields, gpointer)
    {
        // see https://www.freedesktop.org/software/systemd/man/systemd.journal-fields.html

        const char* domain = nullptr;
        const char* message = nullptr;
        const char* level = nullptr;

        bool level_read = false;
        bool domain_read = false;
        bool message_read = false;

        for (size_t i = 0; i < n_fields; ++i)
        {
            std::cout << fields[i].key << " " << (const char*) fields[i].value << std::endl;

            if (not level_read and std::string(fields[i].key) == std::string(detail::mousetrap_level_field))
            {
                level = (const char*) fields[i].value;
                level_read = true;
                continue;
            }

            if (not domain_read and std::string(fields[i].key) == std::string("GLIB_DOMAIN"))
            {
                domain = (const char*) fields[i].value;
                domain_read = true;
                continue;
            }

            if (not message_read and std::string(fields[i].key) == std::string("MESSAGE"))
            {
                message = (const char*) fields[i].value;
                message_read = true;
                continue;
            }

            if (level_read and domain_read and message_read)
                break;
        }

        // if non-mousetrap message, use default rejection filter
        if (level == nullptr)
            if (g_log_writer_default_would_drop(log_level, domain))
                return G_LOG_WRITER_HANDLED;

        // reject debug unless enabled
        auto debug_it = _allow_debug.find(domain);
        if (level_read and std::string(level) == std::string("DEBUG"))
            if (debug_it == _allow_debug.end() or debug_it->second == false)
                return G_LOG_WRITER_HANDLED;

        // reject info unless enabled
        auto info_it = _allow_debug.find(domain);
        if (level_read and std::string(level) == std::string("INFO"))
            if (info_it == _allow_debug.end() or info_it->second == false)
                return G_LOG_WRITER_HANDLED;

        return g_log_writer_standard_streams(log_level, fields, n_fields, nullptr);
    }

    void log::debug(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::DEBUG,
             detail::mousetrap_level_field, "DEBUG",
             "MESSAGE", message.c_str()
        );
    }

    void log::info(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::INFO,
             detail::mousetrap_level_field, "INFO",
             "MESSAGE", message.c_str()
        );
    }

    void log::warning(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::WARNING,
             detail::mousetrap_level_field, "WARNING",
             "MESSAGE", message.c_str()
        );
    }

    void log::critical(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::CRITICAL,
             detail::mousetrap_level_field, "CRITICAL",
             "MESSAGE", message.c_str()
        );
    }

    void log::fatal(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::FATAL,
             detail::mousetrap_level_field, "FATAL",
             "MESSAGE", message.c_str()
        );
    }

    void log::set_surpress_debug(LogDomain domain, bool b)
    {
        _allow_debug.insert_or_assign(domain, not b);
    }

    void log::set_surpress_info(LogDomain domain, bool b)
    {
        _allow_info.insert_or_assign(domain, not b);
    }
}
