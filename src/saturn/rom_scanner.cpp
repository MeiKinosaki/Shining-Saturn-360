#include "rom_scanner.hpp"
#include <algorithm>
#include <cctype>

namespace shining_saturn { namespace {
std::string lower_ext(const std::string& p) {
 auto slash=p.find_last_of("/\\\\"); auto dot=p.find_last_of('.');
 if(dot==std::string::npos || (slash!=std::string::npos && dot<slash)) return {};
 std::string e=p.substr(dot); std::transform(e.begin(),e.end(),e.begin(),[](unsigned char x){return static_cast<char>(std::tolower(x));}); return e;
}
std::string base_name(const std::string& p) {
 auto slash=p.find_last_of("/\\\\"); auto name=p.substr(slash==std::string::npos?0:slash+1);
 auto dot=name.find_last_of('.'); if(dot!=std::string::npos)name.resize(dot); return name;
}
}
bool RomScanner::is_supported_rom(const std::string& p) {
 const auto e=lower_ext(p);
 return e==".cue"||e==".iso"||e==".chd"||e==".ccd"||e==".bin"||e==".img"||e==".mds";
}
std::string RomScanner::cover_for_rom(const std::string& rom,const LibraryPaths& paths,LibraryFileSystem* fs) {
 if(!fs)return {};
 const std::string base=base_name(rom);
 const std::string ext[]={".png",".jpg",".jpeg",".bmp"};
 for(const auto& e:ext) { std::string p=paths.covers+"\\"+base+e; if(fs->file_exists(p))return p; }
 return {};
}
ScanReport RomScanner::scan(GameLibrary& lib,const LibraryPaths& paths) const {
 ScanReport r; if(!fs_)return r;
 std::vector<std::string> files; r.directory_read=fs_->list_files(paths.roms,files); if(!r.directory_read)return r;
 for(const auto& p:files) {
  if(!is_supported_rom(p)){++r.skipped;continue;}
  ++r.discovered; GameEntry g; g.path=p; g.title=base_name(p); g.cover_path=cover_for_rom(p,paths,fs_);
  if(!g.cover_path.empty())++r.covers_found;
  if(lib.add(g))++r.added;else ++r.skipped;
 }
 return r;
}
}