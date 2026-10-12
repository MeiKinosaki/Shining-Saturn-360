#include "game_library.hpp"
#include <algorithm>
#include <cctype>
#include <utility>
namespace shining_saturn { namespace {
std::string lower(std::string s){std::transform(s.begin(),s.end(),s.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});return s;}
bool contains(const std::string& s,const std::string& q){return q.empty()||lower(s).find(lower(q))!=std::string::npos;}
}
bool GameLibrary::add(GameEntry g){if(g.path.empty())return false;auto i=std::find_if(games_.begin(),games_.end(),[&](const GameEntry& x){return x.path==g.path;});if(i!=games_.end())*i=std::move(g);else games_.push_back(std::move(g));filter(GameFilter{});return true;}
bool GameLibrary::set_favorite(const std::string& p,bool f){auto i=std::find_if(games_.begin(),games_.end(),[&](const GameEntry& g){return g.path==p;});if(i==games_.end())return false;i->favorite=f;return true;}
void GameLibrary::filter(const GameFilter& f,SortMode sort){visible_.clear();for(const auto& g:games_){if(!contains(g.title,f.text)||!contains(g.genre,f.genre)||!contains(g.developer,f.developer)||!contains(g.region,f.region)||(f.year&&g.year!=f.year)||(f.save_filter==1&&!g.has_save_data&&!g.has_save_state)||(f.save_filter==2&&(g.has_save_data||g.has_save_state)))continue;visible_.push_back(g);}std::sort(visible_.begin(),visible_.end(),[sort](const GameEntry&a,const GameEntry&b){switch(sort){case SortMode::TitleDescending:return a.title>b.title;case SortMode::YearAscending:return a.year<b.year;case SortMode::YearDescending:return a.year>b.year;default:return a.title<b.title;}});}
}