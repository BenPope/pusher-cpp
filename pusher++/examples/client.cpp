//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <boost/program_options.hpp>
#include <pusher++/client.hpp>
#include <date/tz.h>
#include <iostream>

namespace
{
    boost::program_options::variables_map parse_options(int argc, char* argv[])
    {
        boost::program_options::options_description desc{"Options"};
        desc.add_options()
            ("key", boost::program_options::value<std::string>(), "Application Key")
            ("cluster", boost::program_options::value<std::string>()->default_value("mt1"), "Cluster: [ap1|ap2|eu|us2]mt1]")
            ("channel", boost::program_options::value<std::string>()->default_value("my-channel"), "Channel to subscribe to")
            ("event", boost::program_options::value<std::string>()->default_value("my-event"), "Event to subscribe to")
            ;
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if(!vm.count("key"))
        {
            std::cout << desc << '\n';
            std::exit(1);
        }

        return vm;
    }

    std::ostream& operator<<(std::ostream& os, pusher::event ev)
    {
        return os
            <<  date::make_zoned(date::current_zone(), std::chrono::system_clock::now()) << ": "
            << "{\"channel\":\"" << ev.channel
            << "\",\"event\":\"" << ev.name
            << "\",\"data\":\"" << ev.data << "\"}\n";
    }

    auto logger = [](pusher::event const& ev)
    {
        std::cout << ev;
    };
}

int main(int argc, char* argv[])
{
    auto options = parse_options(argc, argv);

    auto const key = options["key"].as<std::string>();
    auto const cluster = options["cluster"].as<std::string>();
    auto const channel_name = options["channel"].as<std::string>();
    auto const event_name = options["event"].as<std::string>();
    
    boost::asio::io_service ios;
    pusher::client<boost::asio::ip::tcp::socket> client{ios, key, cluster};
    client.connect();
    client.bind_all(logger);

    auto channel = client.channel(channel_name);
    channel.bind(event_name, [&client](pusher::event const& event)
    {
        if(event.data == "bye")
            client.disconnect();
    });
    ios.run();
}
