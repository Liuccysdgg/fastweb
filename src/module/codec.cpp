/*Software License

Copyright(C) 2024[liuyingjie]
License Terms
Usage Rights

Any individual or entity is free to use, copy, and distribute the binary form of this software without modification to the source code, without the need to disclose the source code.
If the source code is modified, the modifications must be open - sourced under the same license.This means that the modifications must be disclosed and accompanied by a copy of this license.
Future Versions Updates
From this version onwards, all future releases will be governed by the terms of the latest version of the license.This license will automatically be nullified and replaced by the new version.
Users must comply with the terms of the new license issued in future releases.
Liability and Disclaimer
This software is provided “as is”, without any express or implied warranties, including but not limited to the warranties of merchantability, fitness for a particular purpose, and non - infringement.In no event shall the author or copyright holder be liable for any claims, damages, or other liabilities, whether in an action of contract, tort, or otherwise, arising from, out of, or in connection with the software or the use or other dealings in the software.
Contact Information
If you have any questions, please contact us: 1585346868@qq.com Or visit our website fwlua.com.
*/

#include "codec.h"
#include "util/codec.h"
std::string module::codec::url_de(const std::string& value)
{
    return ylib::codec::url::de(value);
}

std::string module::codec::url_en(const std::string& value)
{
    return ylib::codec::url::en(value);
}

std::string module::codec::to_utf8(const std::string& value)
{
    return ylib::codec::to_utf8(value);
}

std::string module::codec::to_gbk(const std::string& value)
{
    return ylib::codec::to_gbk(value);
}

std::string module::codec::md5(const std::string& value)
{
    return ylib::codec::md5(value);
}

std::string module::codec::sha1(const std::string_view& value)
{
    return ylib::codec::sha1(ylib::buffer(value.data(), value.length())).to_hex();
}
std::string module::codec::sha256(const std::string_view& value)
{
    return ylib::codec::sha256(ylib::buffer(value.data(), value.length())).to_hex();
}
std::string module::codec::hmac_sha256(const std::string_view& key, const std::string_view& value)
{
    return ylib::codec::hmac_sha256(ylib::buffer(key.data(), key.length()),ylib::buffer(value.data(), value.length())).to_hex();
}
std::string module::codec::aes_en(const std::string_view& value, const std::string_view& key, const std::string& variant, const std::string& mode)
{
    ylib::codec::aes::variant v = ylib::codec::aes::variant::AES256;;
    ylib::codec::aes::mode m = ylib::codec::aes::mode::CBC;

    if (variant == "aes-256") 
        v = ylib::codec::aes::variant::AES256;
    else if (variant == "aes-192")
        v = ylib::codec::aes::variant::AES192;
    else if (variant == "aes-128")
        v = ylib::codec::aes::variant::AES128;

    if (variant == "cbc")
        m = ylib::codec::aes::mode::CBC;
    else if (variant == "ebc")
        m = ylib::codec::aes::mode::ECB;

    return ylib::codec::aes::en(ylib::buffer(value.data(), value.length()), ylib::buffer(key.data(), key.length()), v, m).to_hex();

}
std::string module::codec::aes_de(const std::string_view& value, const std::string_view& key, const std::string& variant, const std::string& mode)
{
    ylib::codec::aes::variant v = ylib::codec::aes::variant::AES256;;
    ylib::codec::aes::mode m = ylib::codec::aes::mode::CBC;

    if (variant == "aes-256")
        v = ylib::codec::aes::variant::AES256;
    else if (variant == "aes-192")
        v = ylib::codec::aes::variant::AES192;
    else if (variant == "aes-128")
        v = ylib::codec::aes::variant::AES128;

    if (variant == "cbc")
        m = ylib::codec::aes::mode::CBC;
    else if (variant == "ebc")
        m = ylib::codec::aes::mode::ECB;

    return ylib::codec::aes::de(ylib::buffer(value.data(), value.length()), ylib::buffer(key.data(), key.length()), v, m).to_hex();
}
void module::codec::regist(sol::state* lua)
{
    lua->new_usertype<module::codec>("fw_codec",
        "url_de", &module::codec::url_de,
        "url_en", &module::codec::url_en,
        "to_utf8", &module::codec::to_utf8,
        "to_gbk", &module::codec::to_gbk,
        "md5", &module::codec::md5,
        "sha1", &module::codec::sha1,
        "sha256", &module::codec::sha256,
        "hmac_sha256", &module::codec::hmac_sha256,
        "aes_decode", &module::codec::aes_de,
        "aes_encode", &module::codec::aes_en
    );
}
