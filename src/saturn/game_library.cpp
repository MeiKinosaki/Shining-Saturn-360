#include "game_library.hpp"
#include <algorithm>
#include <cctype>
#include <utility>
#include <fstream>
#include <iomanip>
namespace shining_saturn { namespace {
std::string lower(std::string s){std::transform(s.begin(),s.end(),s.begin(),[](unsigned char c){return static_cast<char>(std::tolower(c));});return s;}
bool contains(const std::string& s,const std::string& q){return q.empty()||lower(s).find(lower(q))!=std::string::npos;}
}
bool GameLibrary::add(GameEntry g){if(g.path.empty())return false;auto i=std::find_if(games_.begin(),games_.end(),[&](const GameEntry& x){return x.path==g.path;});if(i!=games_.end())*i=std::move(g);else games_.push_back(std::move(g));filter(GameFilter{});return true;}
bool GameLibrary::set_favorite(const std::string& p,bool f){auto i=std::find_if(games_.begin(),games_.end(),[&](const GameEntry& g){return g.path==p;});if(i==games_.end())return false;i->favorite=f;return true;}

bool GameLibrary::save_favorites(const std::string& file_path) const {
 std::ofstream out(file_path.c_str(), std::ios::out | std::ios::trunc);
 if (!out) return false;
 out << "SHINING_SATURN_FAVORITES_V1\\n";
 for (const auto& g : games_) if (g.favorite) {
  for (unsigned char ch : g.path) out << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned>(ch);
  out << '\\n';
 }
 out.flush();
 return static_cast<bool>(out);
}
bool GameLibrary::load_favorites(const std::string& file_path) {
 std::ifstream in(file_path.c_str());
 if (!in) return false;
 std::string line;
 if (!std::getline(in,line) || line != "SHINING_SATURN_FAVORITES_V1") return false;
 for (auto& g : games_) g.favorite = false;
 while (std::getline(in,line)) {
  if (line.size() % 2 != 0) continue;
  std::string path;
  bool valid = true;
  for (std::size_t i=0; i<line.size(); i+=2) {
   auto hex = [](char c)->int { if(c>='0'&&c<='9')return c-'0'; if(c>='a'&&c<='f')return c-'a'+10; if(c>='A'&&c<='F')return c-'A'+10; return -1; };
   int hi=hex(line[i]), lo=hex(line[i+1]);
   if(hi<0||lo<0){valid=false;break;}
   path.push_back(static_cast<char>((hi<<4)|lo));
  }
  if (!valid) continue;
  for (auto& g : games_) if (g.path == path) g.favorite = true;
 }
 return !in.bad();
}
\nvoid GameLibrary::filter(const GameFilter& f,SortMode sort){visible_.clear();for(const auto& g:games_){if(!contains(g.title,f.text)||!contains(g.genre,f.genre)||!contains(g.developer,f.developer)||!contains(g.region,f.region)||(f.year&&g.year!=f.year)||(f.save_filter==1&&!g.has_save_data&&!g.has_save_state)||(f.save_filter==2&&(g.has_save_data||g.has_save_state)))continue;visible_.push_back(g);}std::sort(visible_.begin(),visible_.end(),[sort](const GameEntry&a,const GameEntry&b){switch(sort){case SortMode::TitleDescending:return a.title>b.title;case SortMode::YearAscending:return a.year<b.year;case SortMode::YearDescending:return a.year>b.year;default:return a.title<b.title;}});}
}