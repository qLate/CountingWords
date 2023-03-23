// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include "options_parser.h"
#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <unordered_set>

void print_filenames_in_archives(const std::string& folder_path) {
    const std::unordered_set<std::string> archive_extensions = {".ZIP", ".tar", ".tar.gz", ".tar.bz2"};
    std::vector<std::string> archive_filenames;

    for (const auto &entry: std::filesystem::recursive_directory_iterator(folder_path)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (archive_extensions.count(filename.substr(filename.find_last_of('.'))) > 0){
                archive_filenames.push_back(entry.path().string());
            }
        }
    }


    for (const std::string &archive_filename: archive_filenames) {

        struct archive *archive = archive_read_new();
        archive_read_support_format_all(archive);
        archive_read_support_filter_all(archive);

        int r = archive_read_open_filename(archive, archive_filename.c_str(), 1048576);
        if (r != ARCHIVE_OK) {
            std::cerr << "Error opening archive: " << archive_error_string(archive) << std::endl;
            continue;
        }
        la_ssize_t read_size;
        struct archive_entry *entry;
        while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
            std::string filename = archive_entry_pathname(entry);
            if (archive_entry_filetype(entry) == AE_IFREG && filename.size() > 4 && filename.compare(filename.size() - 4, 4, ".txt") == 0){
                std::cout << "Content of " << filename << ":" << std::endl;
                size_t size = archive_entry_size(entry);
                std::cout << "Size:" << size << std::endl;
                char* buffer = new char[size];
                read_size = archive_read_data(archive, buffer, size);
                if (read_size <= 0) {
                    if (read_size < 0)
                        std::cerr << "Error reading data from archive: " << archive_error_string(archive) << std::endl;
                    delete[] buffer;
                    break;
                }
                std::cout << std::string(buffer, size) << std::endl;
                delete[] buffer;
            }
            r = archive_read_data_skip(archive);
            if (r != ARCHIVE_OK) {
                std::cerr << "Error skipping data in archive: " << archive_error_string(archive) << std::endl;
                break;
            }
        }
        archive_read_free(archive);
    }
}

int main(int argc, char* argv[]) {
//    command_line_options_t command_line_options{argc, argv};
//    std::cout << "A flag value: " << command_line_options.get_A_flag() << std::endl;
    print_filenames_in_archives("G:\\0");
    return 0;
}
