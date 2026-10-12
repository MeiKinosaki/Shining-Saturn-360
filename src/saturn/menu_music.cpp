#include "menu_music.hpp"
#include <algorithm>
namespace shining_saturn {
MenuMusicController::MenuMusicController(MenuMusicBackend* b):backend_(b){}
bool MenuMusicController::start(const MenuMusicSettings& s){settings_=s;settings_.volume=std::max(0.0f,std::min(1.0f,settings_.volume));playing_=false;paused_=false;if(!settings_.enabled||settings_.file_path.empty()||!backend_)return false;playing_=backend_->play(settings_.file_path,settings_.loop,settings_.volume);return playing_;}
void MenuMusicController::set_enabled(bool e){settings_.enabled=e;if(!backend_)return;if(!e){backend_->stop();playing_=false;paused_=false;}else if(!playing_){playing_=!settings_.file_path.empty()&&backend_->play(settings_.file_path,settings_.loop,settings_.volume);paused_=false;}}
void MenuMusicController::set_volume(float v){settings_.volume=std::max(0.0f,std::min(1.0f,v));if(backend_)backend_->set_volume(settings_.volume);}
void MenuMusicController::set_loop(bool l){settings_.loop=l;}
void MenuMusicController::pause(){if(backend_&&playing_&&!paused_){backend_->pause();paused_=true;}}
void MenuMusicController::resume(){if(backend_&&playing_&&paused_){backend_->resume();paused_=false;}}
void MenuMusicController::stop(){if(backend_)backend_->stop();playing_=false;paused_=false;}
}