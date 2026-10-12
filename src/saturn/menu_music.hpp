#pragma once
#include <string>
namespace shining_saturn {
struct MenuMusicSettings { bool enabled=true; bool loop=true; float volume=0.65f; std::string file_path="Hdd1:\\ShiningSaturn360\\Music\\MenuTheme.wav"; };
class MenuMusicBackend { public: virtual ~MenuMusicBackend()=default; virtual bool play(const std::string&,bool,float)=0; virtual void set_volume(float)=0; virtual void pause()=0; virtual void resume()=0; virtual void stop()=0; };
class MenuMusicController {
public:
 explicit MenuMusicController(MenuMusicBackend* backend=nullptr);
 bool start(const MenuMusicSettings&); void set_enabled(bool); void set_volume(float); void set_loop(bool); void pause(); void resume(); void stop();
 const MenuMusicSettings& settings()const{return settings_;} bool is_playing()const{return playing_;}
private: MenuMusicBackend* backend_; MenuMusicSettings settings_; bool playing_=false,paused_=false;
};
}