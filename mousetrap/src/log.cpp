//
// Created by clem on 3/28/23.
//

#include <include/log.hpp>
#include <iostream>

namespace mousetrap
{
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
            if (not level_read and std::string(fields[i].key) == std::string("MOUSETRAP_LEVEL"))
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

            if (priority_read and domain_read and message_read)
                break;
        }

        std::cout << level << std::endl;

        // only allow debug when explicitly enabled
        auto debug_it = _surpress_debug.find(domain);
        if (level == std::string("DEBUG"))
            if (debug_it == _surpress_debug.end() or debug_it->second == false;)
                return G_LOG_WRITER_HANDLED;

        // always allow info unless explicitly disabled
        auto info_it = _surpress_info.find(domain);
        if (level == std::string("INFO"))
            if (info_it != _surpress_info.end() and info_it->second == true)
                return G_LOG_WRITER_HANDLED;

        return g_log_writer_standard_streams(log_level, fields, n_fields, nullptr);
    }

    void log::debug(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::DEBUG, 
            "MESSAGE", message.c_str(),
            "MOUSETRAP_LEVEL", "DEBUG"
        );
    }

    void log::info(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::INFO,
             "MESSAGE", message.c_str(),
             "MOUSETRAP_LEVEL", "INFO"
        );    
    }

    void log::warning(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::WARNING,
             "MESSAGE", message.c_str(),
             "MOUSETRAP_LEVEL", "WARNING"
        );    
    }

    void log::critical(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::CRITICAL,
             "MESSAGE", message.c_str(),
             "MOUSETRAP_LEVEL", "CRITICAL"
        );    
    }

    void log::fatal(const std::string& message, LogDomain domain)
    {
        g_log_structured(domain, (GLogLevelFlags) LogLevel::FATAL,
             "MESSAGE", message.c_str(),
             "MOUSETRAP_LEVEL", "FATAL"
        );    
    }

    void log::set_surpress_debug(LogDomain domain, bool b)
    {
        _surpress_debug.insert_or_assign({domain, not b});
    }

    void log::set_surpress_info(LogDomain domain, bool b)
    {
        _surpress_info.insert_or_assign({domain, b});
    }
}
