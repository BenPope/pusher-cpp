//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#include <pusher++/server.hpp>
#include <boost/program_options.hpp>
#include <iostream>

namespace
{
    boost::program_options::variables_map parse_options(int argc, char* argv[])
    {
        boost::program_options::options_description desc{"Options"};
        desc.add_options()
            ("app_id", boost::program_options::value<std::string>(), "Application ID")
            ("key", boost::program_options::value<std::string>(), "Key")
            ("secret", boost::program_options::value<std::string>(), "Secret")
            ("cluster", boost::program_options::value<std::string>()->default_value("eu"), "Cluster [mt1|ap1|eu]")
        ;
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
        boost::program_options::notify(vm);

        if(!vm.count("app_id") || !vm.count("key") || !vm.count("secret"))
        {
            std::cout << desc << '\n';
            std::exit(1);
        }

        return vm;
    }
}

int main(int argc, char* argv[])
{
    auto options = parse_options(argc, argv);

    auto const app_id = options["app_id"].as<std::string>();;
    auto const secret = options["secret"].as<std::string>();
    auto const key = options["key"].as<std::string>();
    auto const cluster = options["cluster"].as<std::string>();

    boost::asio::io_service ios;
    pusher::server<boost::asio::ip::tcp::socket> server{ios, app_id, key, secret, cluster};
    server.connect();

    std::string input;
    while(input != "bye" && std::cout << "\nMessage: " && std::getline(std::cin, input))
    {
        std::cout << server.trigger("test_channel", "test_name", input);
    }
    std::cout << '\n';
    ios.run();
}
