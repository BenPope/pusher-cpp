//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef PUSHERPP_DETAIL_SERVER_CRYPTO_HPP
#define PUSHERPP_DETAIL_SERVER_CRYPTO_HPP

#include <crypto++/filters.h>
#include <crypto++/hex.h>
#include <crypto++/hmac.h>
#include <crypto++/md5.h>
#include <crypto++/sha.h>

#include <string>

namespace pusher { namespace detail { namespace server
{
    template<typename... ArgsT>
    std::string hex_encode(ArgsT&&... input)
    {
        std::string encoded;
        CryptoPP::StringSource ss(std::forward<ArgsT>(input)..., true,
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(encoded), false
            )
        );
        return encoded;
    }

    inline std::string md5_digest(std::string const& input)
    {
        byte digest[CryptoPP::Weak::MD5::DIGESTSIZE];
        CryptoPP::Weak::MD5 hash;
        hash.CalculateDigest(digest, reinterpret_cast<const byte*>(input.data()), input.length() );
        return hex_encode(digest, sizeof(digest));
    }

    inline std::string auth_signature(std::string const& input, std::string const& secret_key)
    {
        CryptoPP::HMAC<CryptoPP::SHA256> hmac(reinterpret_cast<const unsigned char*>(secret_key.data()), secret_key.size());
        std::string mac;
        CryptoPP::StringSource ss(input, true,
            new CryptoPP::HashFilter(hmac,
                new CryptoPP::StringSink(mac)
            ) // HashFilter
        ); // StringSource
        return hex_encode(mac);
    }


}}} // pusher::detail::server

#endif // PUSHERPP_DETAIL_SERVER_CRYPTO_HPP
