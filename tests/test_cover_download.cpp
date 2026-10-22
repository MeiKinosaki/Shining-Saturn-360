#include "../src/saturn/cover_download.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <map>
using namespace shining_saturn;

class FakeNetwork final : public CoverNetworkBackend {
public:
    bool reachable=true;
    CoverHttpResponse response{200,"image/png",{0x89,'P','N','G',0x0D,0x0A,0x1A,0x0A,1,2,3}};
    unsigned calls=0;
    bool get(const std::string&,CoverHttpResponse& out) override {
        ++calls; if(!reachable)return false; out=response; return true;
    }
};
class FakeStorage final : public CoverCacheStorage {
public:
    std::map<std::string,std::vector<std::uint8_t>> files;
    bool exists(const std::string& p) override { return files.find(p)!=files.end(); }
    bool write_binary(const std::string& p,const std::vector<std::uint8_t>& b) override {
        files[p]=b; return true;
    }
};
int main() {
    FakeNetwork net; FakeStorage fs; CoverDownloadService service(&net,&fs);
    auto first=service.fetch("https://covers.example/game.png","Hdd1:\\ShiningSaturn360\\Covers\\game.png");
    assert(first.success && !first.cache_hit && net.calls==1);
    auto second=service.fetch("https://covers.example/game.png","Hdd1:\\ShiningSaturn360\\Covers\\game.png");
    assert(second.success && second.cache_hit && net.calls==1);
    net.response.status_code=404;
    auto missing=service.fetch("https://covers.example/missing.png","missing.png");
    assert(!missing.success && missing.error=="http_status");
    net.response.status_code=200; net.response.body={1,2,3,4};
    auto bad=service.fetch("https://covers.example/bad.png","bad.png");
    assert(!bad.success && bad.error=="invalid_image");
    net.response.body={0xFF,0xD8,0xFF,0x00};
    auto jpeg=service.fetch("https://covers.example/game.jpg","game.jpg");
    assert(jpeg.success);
    net.reachable=false;
    auto offline=service.fetch("https://covers.example/offline.png","offline.png");
    assert(!offline.success && offline.error=="network_error");
    std::cout << "cover download/cache tests passed\n";
}
