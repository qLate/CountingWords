#pragma once

#include <iostream>
#include <archive.h>
#include <archive_entry.h>
#include <filesystem>
#include <unordered_set>

void countsWordsInArchive(const std::string& filename);
void countWords(const std::string& path);