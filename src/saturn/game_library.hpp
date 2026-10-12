#pragma once
#include <string>
#include <vector>
namespace shining_saturn {
struct GameEntry { std::string title,path,cover_path,region,developer,genre; int year=0; bool favorite=false,has_save_data=false,has_save_state=false; };
struct GameFilter { std::string text,genre,developer,region; int year=0; int save_filter=0; };
struct LibraryPaths { std::string roms="Hdd1:\\ShiningSaturn360\\Roms",favorites="Hdd1:\\ShiningSaturn360\\Favorites",save_states="Hdd1:\\ShiningSaturn360\\SaveState",save_data="Hdd1:\\ShiningSaturn360\\SaveData",covers="Hdd1:\\ShiningSaturn360\\Covers",config="Hdd1:\\ShiningSaturn360\\Config"; };
enum class SortMode { TitleAscending,TitleDescending,YearAscending,YearDescending };
class GameLibrary {
public:
 bool add(GameEntry game); bool set_favorite(const std::string& path,bool favorite);
 void filter(const GameFilter& filter,SortMode sort=SortMode::TitleAscending);
 const std::vector<GameEntry>& all()const{return games_;} const std::vector<GameEntry>& visible()const{return visible_;}
 const LibraryPaths& paths()const{return paths_;} void set_paths(const LibraryPaths& p){paths_=p;}
private: std::vector<GameEntry> games_,visible_; LibraryPaths paths_;
};
}