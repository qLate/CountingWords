#include "wordCounter.h"

void countsWordsInArchive(const std::string& filename)
{
	archive* archive = archive_read_new();
	archive_read_support_format_all(archive);
	archive_read_support_filter_all(archive);

	int r = archive_read_open_filename(archive, filename.c_str(), 1048576);
	if (r != ARCHIVE_OK) {
		std::cerr << "Error opening archive: " << archive_error_string(archive) << std::endl;
		return;
	}
	archive_entry* entry;
	while (archive_read_next_header(archive, &entry) == ARCHIVE_OK) {
		std::string archiveName = archive_entry_pathname(entry);
		if (archive_entry_filetype(entry) == AE_IFREG && archiveName.size() > 4 && archiveName.compare(archiveName.size() - 4, 4, ".txt") == 0) {
			size_t size = archive_entry_size(entry);
			char* buffer = new char[size];
			la_ssize_t readSize = archive_read_data(archive, buffer, size);

			if (readSize <= 0) {
				if (readSize < 0)
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

void countWords(const std::string& path)
{
	const std::unordered_set<std::string> archiveExtensions = { ".ZIP", ".tar", ".tar.gz", ".tar.bz2" };

	for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
		if (entry.is_regular_file()) {
			std::string filename = entry.path().filename().string();
			if (archiveExtensions.find(filename.substr(filename.find_last_of('.'))) != archiveExtensions.end()) {
				countsWordsInArchive(entry.path().string());
			}
		}
	}
}
