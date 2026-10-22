#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace shining_saturn {

struct CoverHttpResponse {
    int status_code = 0;
    std::string content_type;
    std::vector<std::uint8_t> body;
};

class CoverNetworkBackend {
public:
    virtual ~CoverNetworkBackend() = default;
    // Platform implementation must use the console's configured network
    // connection (Wi-Fi or Ethernet); no transport is hard-coded here.
    virtual bool get(const std::string& url, CoverHttpResponse& response) = 0;
};

class CoverCacheStorage {
public:
    virtual ~CoverCacheStorage() = default;
    virtual bool exists(const std::string& path) = 0;
    virtual bool write_binary(const std::string& path,
                              const std::vector<std::uint8_t>& bytes) = 0;
};

struct CoverDownloadResult {
    bool success = false;
    bool cache_hit = false;
    std::string path;
    std::string error;
};

class CoverDownloadService {
public:
    explicit CoverDownloadService(CoverNetworkBackend* network,
                                  CoverCacheStorage* storage)
        : network_(network), storage_(storage) {}

    CoverDownloadResult fetch(const std::string& url,
                              const std::string& cache_path) const {
        CoverDownloadResult result;
        result.path = cache_path;
        if (!network_ || !storage_) { result.error = "backend_unavailable"; return result; }
        if (url.empty() || cache_path.empty()) { result.error = "invalid_argument"; return result; }
        if (storage_->exists(cache_path)) {
            result.success = true; result.cache_hit = true; return result;
        }
        CoverHttpResponse response;
        if (!network_->get(url, response)) { result.error = "network_error"; return result; }
        if (response.status_code != 200) { result.error = "http_status"; return result; }
        if (response.body.empty() || response.body.size() > MaxImageBytes) {
            result.error = "invalid_size"; return result;
        }
        if (!is_supported_image(response.body)) { result.error = "invalid_image"; return result; }
        if (!storage_->write_binary(cache_path, response.body)) {
            result.error = "cache_write_failed"; return result;
        }
        result.success = true;
        return result;
    }

    static constexpr std::size_t MaxImageBytes = 8u * 1024u * 1024u;

private:
    static bool is_supported_image(const std::vector<std::uint8_t>& b) {
        const bool png = b.size() >= 8 && b[0]==0x89 && b[1]=='P' && b[2]=='N' &&
                         b[3]=='G' && b[4]==0x0D && b[5]==0x0A && b[6]==0x1A && b[7]==0x0A;
        const bool jpeg = b.size() >= 3 && b[0]==0xFF && b[1]==0xD8 && b[2]==0xFF;
        return png || jpeg;
    }

    CoverNetworkBackend* network_;
    CoverCacheStorage* storage_;
};

} // namespace shining_saturn
