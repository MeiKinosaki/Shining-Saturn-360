#pragma once
#include "game_library.hpp"
#include <string>
#include <vector>

namespace shining_saturn {
// Filesystem adapter lets the same scanner be tested on a host and bound to
// the Xbox 360 filesystem implementation later.
class LibraryFileSystem {
public:
 virtual ~LibraryFileSystem() = default;
 virtual bool list_files(const std::string& directory, std::vector<std::string>& files) = 0;
 virtual bool file_exists(const std::string& path) = 0;
};
struct ScanReport { unsigned discovered=0, added=0, skipped=0, covers_found=0; bool directory_read=false; };
class RomScanner {
public:
 explicit RomScanner(LibraryFileSystem* fs):fs_(fs){}
 ScanReport scan(GameLibrary& library, const LibraryPaths& paths) const;
 static bool is_supported_rom(const std::string& path);
 static std::string cover_for_rom(const std::string& rom_path,const LibraryPaths& paths,LibraryFileSystem* fs);
private: LibraryFileSystem* fs_;
};
}