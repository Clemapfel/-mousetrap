
//
// Created by clem on 3/23/23.
//

#include <include/file_system.hpp>
#include <include/file_monitor.hpp>
#include <include/log.hpp>

#include <sstream>
#include <iostream>

namespace mousetrap
{
    FileDescriptor::FileDescriptor()
    : FileDescriptor(g_file_new_for_path(""))
    {}

    FileDescriptor::FileDescriptor(const FilePath& path)
    : FileDescriptor(g_file_new_for_path(path.c_str()))
    {}

    FileDescriptor::FileDescriptor(GFile* file)
    : _native(file)
    {
        if (_native != nullptr)
            g_object_ref(file);
    }

    FileDescriptor::~FileDescriptor()
    {
        if (_native != nullptr)
            g_object_unref(_native);
    }

    void FileDescriptor::create_from_path(const FilePath& path)
    {
        _native = g_file_new_for_path(path.c_str());
    }

    void FileDescriptor::create_from_uri(const FileURI& uri)
    {
        _native = g_file_new_for_uri(uri.c_str());
    }

    FileDescriptor::FileDescriptor(const FileDescriptor& other)
    : _native(other._native)
    {
        g_object_ref(other._native);
    }

    FileDescriptor::FileDescriptor(FileDescriptor&& other)
    : _native(other._native)
    {
        g_object_ref(other._native);
        other._native = nullptr;
    }

    FileDescriptor& FileDescriptor::operator=(const FileDescriptor& other)
    {
        g_object_ref(other._native);
        this->_native = other._native;

        return *this;
    }

    FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other)
    {
        g_object_ref(other._native);
        this->_native = other._native;
        other._native = nullptr;

        return *this;
    }

    FileDescriptor::operator GFile*()
    {
        return _native;
    }

    bool FileDescriptor::is_valid() const
    {
        return _native != nullptr and (is_file() or is_folder());
    }

    bool FileDescriptor::is_file() const
    {
        return g_file_test(get_path().c_str(), G_FILE_TEST_IS_REGULAR);
    }

    bool FileDescriptor::is_folder() const
    {
        return g_file_test(get_path().c_str(), G_FILE_TEST_IS_DIR);
    }

    FilePath FileDescriptor::get_name() const
    {
        return g_file_get_basename(_native);
    }

    FilePath FileDescriptor::get_path() const
    {
        char* path = G_IS_FILE(_native) ? g_file_get_path(_native) : nullptr;
        return path == nullptr ? "" : path;
    }

    FilePath FileDescriptor::get_path_relative_to(const FileDescriptor& other) const
    {
        return g_file_get_relative_path(_native, other._native);
    }

    FileDescriptor FileDescriptor::get_parent() const
    {
        return g_file_get_parent(_native);
    }

    FileURI FileDescriptor::get_uri() const
    {
        auto* uri = g_file_get_uri(_native);
        return uri == nullptr ? "" : uri;
    }

    std::string FileDescriptor::get_file_extension() const
    {
        auto name = get_name();

        size_t i = name.size()-1;
        while (i > 0 and name.at(i) != '.')
            i -= 1;

        return std::string(name.begin() + i, name.end());
    }

    std::string FileDescriptor::get_content_type() const
    {
        if (is_folder())
            return "folder";

        return query_info(G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE);
    }

    std::string FileDescriptor::query_info(const char* attribute_query_string) const
    {
        GError* error = nullptr;
        auto* info = g_file_query_info(_native, attribute_query_string, G_FILE_QUERY_INFO_NONE, nullptr, &error);

        if (error != nullptr)
        {
            std::stringstream str;
            str << "In FileDescriptor::query_info: Unable to retrieve info for attribute `" << attribute_query_string << "`: " << error->message << std::endl;
            log::critical(str.str(), MOUSETRAP_DOMAIN);
            g_error_free(error);
            return "";
        }

        auto* out = g_file_info_get_attribute_as_string(info, attribute_query_string);
        return out == nullptr ? "" : out;
    }

    bool FileDescriptor::operator==(const FileDescriptor& other) const
    {
        return g_file_equal(this->_native, other._native);
    }

    bool FileDescriptor::operator!=(const FileDescriptor& other) const
    {
        return not this->operator==(other);
    }

    size_t FileDescriptor::hash() const
    {
        return g_file_hash(_native);
    }

    FileMonitor FileDescriptor::create_monitor() const
    {
        GError* error = nullptr;
        auto out = FileMonitor(g_file_monitor(_native, (GFileMonitorFlags) FileMonitor::flags, nullptr, &error));

        if (error != nullptr)
        {
            std::cerr << "[ERROR] In FileDescriptor::create_monitor: " << error->message << std::endl;
            g_error_free(error);
        }

        return out;
    }
}