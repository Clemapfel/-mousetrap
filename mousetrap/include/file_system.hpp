//
// Copyright (c) Clemens Cords (mail@clemens-cords.com), created 3/23/23
//

#pragma once

#include <gtk/gtk.h>

#include <include/file_monitor.hpp>

#include <string>

namespace mousetrap
{
    /// @brief absolute path
    using FilePath = std::string;

    /// @brief uri, formatted according to
    using FileURI = std::string;

    /// @brief non-mutating file descriptor, may point to no file at all
    class FileDescriptor
    {
        public:
            /// @brief default ctor, unitialized
            FileDescriptor();

            /// @brief construct from path
            /// @param path absolute path
            FileDescriptor(const FilePath& path);

            /// @brief construct from GFile, for internal use only
            /// @param file GFile
            FileDescriptor(GFile*);

            /// @brief destruct, does not mutate file on disk
            ~FileDescriptor();

            /// @brief copy ctor
            /// @param other
            FileDescriptor(const FileDescriptor& other);

            /// @brief move ctor
            /// @param other
            FileDescriptor(FileDescriptor&& other);

            /// @brief copy assignment
            /// @param other
            /// @return reference to self after assignment
            FileDescriptor& operator=(const FileDescriptor& other);

            /// @brief move assignment
            /// @param other
            /// @return reference to self after assignment
            FileDescriptor& operator=(FileDescriptor&& other);

            /// @brief expose as GFile, for internal use only
            operator GFile*();

            /// @brief create a file descriptor for a given path
            /// @param path absolute path
            /// @return true if path points to a valid file or folder, false otherwse
            bool create_from_path(const FilePath& path);

            /// @brief create a file descriptor for a given uri
            /// @param uri uri
            /// @return true if uri points to a valid file or folder, false otherwise
            bool create_from_uri(const FileURI& uri);

            /// @brief get the name of the file, including file extension
            /// @return name, even if file does not exist
            std::string get_name() const;

            /// @brief get the absolute path of the file
            /// @return absolute path, even if file does not exist
            FilePath get_path() const;

            /// @brief get a relative path, from self to other
            /// @param other other file descriptor
            /// @return relative file path, even if either file does not exist
            FilePath get_path_relative_to(const FileDescriptor& other) const;

            /// @brief get uri of the file
            /// @return uri, even if file does not exist
            FileURI  get_uri() const;

            /// @brief get parent of file as file descriptor, parent may not exist
            /// @return file descriptor
            [[nodiscard]] FileDescriptor get_parent() const;

            /// @brief get file extension, empty string if file does not exist or doesn't have an extension
            /// @return extension as string
            std::string get_file_extension() const;

            /// @brief test if two file descriptors point to the same object
            /// @param other
            /// @return true if both file descriptors point to the same object, false otherwise
            bool operator==(const FileDescriptor& other) const;

            /// @brief test if two file descriptors point to the same object
            /// @param other
            /// @return false if both file descriptors point to the same object, true otherwise
            bool operator!=(const FileDescriptor& other) const;

            /// @brief check if a file exists
            /// @return true if descriptor points to a valid file or folder, false otherwise
            bool exists() const;

            /// @brief check if descriptor points to a folder
            /// @return true if folder, false otherwise
            bool is_folder() const;

            /// @brief check if descriptor points to a file
            /// @return true if file, false otherwise
            bool is_file() const;

            /// @brief check if descriptor is a symlink to another file
            /// @return true if symlink, false otherwise
            bool is_symlink() const;

            /// @brief if file is symlink, return descriptor to file self points to
            /// @return descriptor, if self is not symlink, descriptor will be invalid
            [[nodiscard]] FileDescriptor follow_symlink() const;

            /// @brief check if file is an executable
            /// @return true if executable, false otherwise
            bool is_executable() const;

            /// @brief get the content type of the file
            /// @return string
            std::string get_content_type() const;

            /// @brief query arbitrary info about a file, for a list of attributes available here: https://www.freedesktop.org/software/gstreamer-sdk/data/docs/2012.5/gio/GFileInfo.html
            /// @param attribute_query_string attribute name
            /// @return result as string
            std::string query_info(const char* attribute_query_string) const;

            /// @brief get a unique hash for the file
            /// @return hash
            size_t hash() const;

            /// @brief create a file monitor for the file
            /// @return monitor
            [[nodiscard]] FileMonitor create_monitor() const;

        private:
            GFile* _native;
    };

    /// @brief singleton file manager, mutates files
    class FileSystem
    {
        public:
            /// @brief uninstantiatable singleton instance
            FileSystem() = delete;

            /// @brief create a file at given path
            /// @param destination file descriptor
            /// @param should_replace_destination if true, replaces file at destination, if false and file exist, does nothing
            /// @return true if operation was succesfull, false otherwise
            static bool create_at(FileDescriptor& destination, bool should_replace_destination);

            /// @brief delete a file at given path
            /// @param destination file descriptor
            /// @return true if operation was succesfull, false otherwise
            static bool delete_at(FileDescriptor&);

            /// @brief copy a file from one path to another
            /// @param from original file that should be copied
            /// @param to target destination of file
            /// @param allow_overwrite if true and a file at destination already exists, will overwrite that file. If false and file exists, does nothing
            /// @param make_backup create a bakcup
            static bool copy(
                FileDescriptor& from,
                FileDescriptor& to,
                bool allow_overwrite = true,
                bool make_backup = false,
                bool follow_symlinks = false
            );

            static bool move(
                FileDescriptor& from,
                FileDescriptor& to,
                bool allow_overwrite = true,
                bool make_backup = false,
                bool follow_symlinks = false
            );

            static bool move_to_trash(FileDescriptor&);

            /// \param recursive: also list files in subfolders
            /// \param list_directories: should folders themself be included
            static std::vector<FileDescriptor> get_all_files_in_directory(
                const FilePath& path,
                bool recursive = false,
                bool list_directories = true
            );

    };
}

