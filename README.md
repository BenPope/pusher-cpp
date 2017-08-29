# Pusher client and server built on Boost.Asio

## Contents

- [Introduction](#introduction)
- [Features](#features)
- [Usage](#usage)
- [Requirements](#requirements)
- [Status](#status)
- [Licence](#licence)
- [Contact](#contact)

## Introduction

Pusher++ is a header-only cross-platform C++ library for connecting to [Pusher](https://pusher.com). It's built on top of Boost.Asio via Beast for HTTP and Websocket support.

## Features

 * **Performance.** Supports both synchronous and asynchronous APIs.
 * **SSL support.** Connect securely to endpoints at Pusher and bypass problematic proxies.

## Usage

These examples are complete, but you will have to set your application specific credentials. The `examples` directory contains examples that parse the command line.

Example Server:

```C++
#include <pusher++/server.hpp>
#include <iostream>

int main()
{
    std::string const app_id = "Your App ID";
    std::string const secret = "Your Secret";
    std::string const key = "Your Key";
    
    boost::asio::io_service ios;
    pusher::server<boost::asio::ip::tcp::socket> server{ios, app_id, key, secret};
    server.connect();

    std::string input;
    while(input != "bye" && std::cout << "\nMessage: " && std::getline(std::cin, input))
    {
        std::cout << server.trigger("test_channel", "test_name", input);
    }
    ios.run();
}
```

Example Client:

```C++
#include <pusher++/client.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    std::string const key = "Your Key";
    
    boost::asio::io_service ios;
    pusher::client<boost::asio::ip::tcp::socket> client{ios, key};
    client.connect();
    
    client.bind_all([](pusher::event const& event)
    {
        std::cout << event.data << '\n';
    });

    auto test_channel = client.channel("test_channel");
    test_channel.bind("test_name", [&client](pusher::event const& event)
    {
        if(event.data == "bye")
            client.disconnect();
    });
    ios.run();
}

```

## Requirements

 * C++14
 * [Beast](https://boostorg/beast) >= 110
 * [Boost](http://www.boost.org) >= 1.63
 * [RapidJSON](https://github.com/miloyip/rapidjson/) >= v1.1.0
 * [Crypto++](https://github.com/weidai11/cryptopp) >= 5.6.5 (server)
 * [OpenSSL](https://www.openssl.org/) = 1.0.2 (optional)

## Status

Caveat emptor. This is very early work; it's incomplete and has not been used in anger. Everything is subject to change, but it may not be completed or even maintained.

However, you are free to use it under the terms set out below.

## Licence

Distributed under the Boost Software License, Version 1.0. (See accompanying file [LICENSE_1_0.txt](LICENSE_1_0.txt) or copy at [http://www.boost.org/LICENSE_1_0.txt](http://www.boost.org/LICENSE_1_0.txt))

## Contact

Please report issues or questions here:
https://github.com/BenPope/pusher-cpp/issues
