#include "../src/saturn/settings.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <limits>
using namespace shining_saturn;
struct Store:SettingsStorage{std::map<std::string,std::string> files;bool read_text(const std::string&p,std::string&o)override{auto i=files.find(p);if(i==files.end())return false;o=i->second;return true;}bool write_text(const std::string&p,const std::string&d)override{files[p]=d;return true;}};
int main(){Store store;SettingsManager m(&store);UserSettings s;s.ui_scale=200;s.menu_volume=1.5f;s.video_filter=8;s.sort=SortMode::YearDescending;assert(m.save("config.ini",s));UserSettings loaded;assert(m.load("config.ini",loaded));assert(loaded.ui_scale==150&&loaded.menu_volume==1.0f&&loaded.video_filter==3&&loaded.sort==SortMode::YearDescending);loaded.ui_scale=50;loaded.menu_volume=-1;assert(m.save("config.ini",loaded));assert(m.load("config.ini",loaded));assert(loaded.ui_scale==75&&loaded.menu_volume==0.0f);s.menu_volume=std::numeric_limits<float>::quiet_NaN();assert(m.save("nan.ini",s));assert(m.load("nan.ini",loaded));assert(loaded.menu_volume==0.65f);store.files["bad.ini"]="version=999\n";assert(!m.load("bad.ini",loaded));assert(!m.load("missing.ini",loaded));std::cout<<"Settings tests passed\n";}