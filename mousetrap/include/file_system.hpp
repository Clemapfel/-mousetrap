//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/23/23
//

#pragma once

#include <gtk/gtk.h>

#include <include/file_monitor.hpp>

#include <string>

namespace mousetrap
{
    using FilePath = std::string;
    using FileURI = std::string;

    /// @brief non-mutating file descriptor, may point to no file at all
    class FileDescriptor
    {
        public:
            FileDescriptor();
            FileDescriptor(const FilePath& path);
            FileDescriptor(GFile*);
            ~FileDescriptor();

            FileDescriptor(const FileDescriptor& other);
            FileDescriptor(FileDescriptor&& other);
            FileDescriptor& operator=(const FileDescriptor& other);
            FileDescriptor& operator=(FileDescriptor&& other);

            operator GFile*();

            void create_from_path(const FilePath& path);
            void create_from_uri(const FileURI& uri);

            FilePath get_name() const;
            FilePath get_path() const;
            FilePath get_path_relative_to(const FileDescriptor& other) const;
            FileURI  get_uri() const;
            FileDescriptor get_parent() const;

            std::string get_file_extension() const;

            bool operator==(const FileDescriptor& other) const;
            bool operator!=(const FileDescriptor& other) const;

            bool is_valid() const;
            bool is_folder() const;
            bool is_file() const;

            std::string get_content_type() const;

            /// \note list of attributes available here: https://www.freedesktop.org/software/gstreamer-sdk/data/docs/2012.5/gio/GFileInfo.html
            std::string query_info(const char* attribute_query_string) const;
            size_t hash() const;

            FileMonitor create_monitor() const;

        private:
            GFile* _native;
    };
}

template<>
struct std::hash<mousetrap::FileDescriptor>
{
    std::size_t operator()(const mousetrap::FileDescriptor& file) const noexcept
    {
        return file.hash();
    }
};