#include "../src/saturn/menu_music.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace shining_saturn;
struct Fake:MenuMusicBackend{bool played=false,loop=false,paused=false,stopped=false;float volume=-1;std::string path;bool play(const std::string&p,bool l,float v)override{path=p;loop=l;volume=v;played=true;return true;}void set_volume(float v)override{volume=v;}void pause()override{paused=true;}void resume()override{paused=false;}void stop()override{stopped=true;played=false;}};
int main(){Fake b;MenuMusicController m(&b);MenuMusicSettings s;s.file_path="theme.wav";s.volume=2;assert(m.start(s)&&m.is_playing());assert(b.path=="theme.wav"&&b.loop&&std::fabs(b.volume-1)<.001f);m.set_volume(-1);assert(std::fabs(b.volume)<.001f);m.pause();assert(b.paused);m.resume();assert(!b.paused);m.set_loop(false);assert(!m.settings().loop);m.set_enabled(false);assert(!m.is_playing()&&b.stopped);s.enabled=false;assert(!m.start(s));MenuMusicController n; s.enabled=true;assert(!n.start(s));std::cout<<"MenuMusic tests passed\n";}