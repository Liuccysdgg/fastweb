#pragma once

#include <string>
#include "base/buffer.h"
namespace ylib
{
    namespace codec
    {
        std::string md5(const ylib::buffer& data);
        std::string to_utf8(const std::string& gbk);
        std::string to_gbk(const std::string& utf8);


        namespace gzip
        {
            ylib::buffer en(const ylib::buffer& data);
            ylib::buffer de(const ylib::buffer& data);
        }

        namespace base64 {
            std::string en(const ylib::buffer& data);
            ylib::buffer de(const std::string& data);
        }
        namespace url {
            std::string en(const ylib::buffer& data);
            ylib::buffer de(const std::string& data);
        }
#if 1
        namespace des3 {
            ylib::buffer en(const ylib::buffer& data, const std::string& key);
            ylib::buffer de(const ylib::buffer& data, const std::string& key);
        }
#endif
        namespace aes {
            enum class variant {
                AES128 = 16,
                AES192 = 24,
                AES256 = 32
            };
            enum class mode {
                CBC,
                ECB
            };
            ylib::buffer en(const ylib::buffer& data, const std::string& key, ylib::codec::aes::variant variant, ylib::codec::aes::mode mode);
            ylib::buffer de(const ylib::buffer& ciphertext, const std::string& key, variant var, mode mod);
        }
    }
    
}
