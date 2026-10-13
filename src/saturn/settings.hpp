#pragma once
#include "game_library.hpp"
#include <string>
namespace shining_saturn {
struct UserSettings {
 int version=1;
 int ui_scale=100;
 int video_filter=0;
 int performance_preset=0;
 bool vsync=true;
 bool menu_music=true;
 float menu_volume=0.65f;
 SortMode sort=SortMode::TitleAscending;
};
class SettingsStorage {
public: virtual ~SettingsStorage()=default; virtual bool read_text(const std::string& path,std::string& out)=0; virtual bool write_text(const std::string& path,const std::string& data)=0;
};
class SettingsManager {
public:
 explicit SettingsManager(SettingsStorage* storage):storage_(storage){}
 bool save(const std::string& path,const UserSettings& settings) const;
 bool load(const std::string& path,UserSettings& settings) const;
 static bool validate(UserSettings& settings);
private: SettingsStorage* storage_;
};
}