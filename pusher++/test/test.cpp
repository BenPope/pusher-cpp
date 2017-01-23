//          Copyright Ben Pope 2017.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_TEST_MODULE pusher-test
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API

#include "test.hpp"

#include <boost/program_options.hpp>
#include <date/tz.h>
#include <pusher++/event.hpp>
#include <iostream>

namespace
{
    boost::program_options::variables_map options;
}

std::string app_id() { return options["app_id"].as<std::string>(); }
std::string key() { return options["key"].as<std::string>(); }
std::string secret() { return options["secret"].as<std::string>(); }
std::string cluster() { return options["cluster"].as<std::string>(); }

std::ostream& operator<<(std::ostream& os, pusher::event const& ev)
{
    return os
        <<  date::make_zoned(date::current_zone(), std::chrono::system_clock::now()) << ": "
        << "{\"channel\":\"" << ev.channel
        << "\",\"event\":\"" << ev.name
        << "\",\"data\":\"" << ev.data << "\"}\n";
}

namespace
{
    boost::program_options::variables_map parse_options(int argc, char* argv[])
    {
        namespace po = boost::program_options;
        po::options_description desc{"Options"};
        desc.add_options()
            ("app_id", po::value<std::string>(), "Application ID")
            ("key", po::value<std::string>(), "Key")
            ("secret", po::value<std::string>(), "Secret")
            ("cluster", po::value<std::string>()->default_value("eu"), "Cluster [mt1|ap1|eu]")
        ;
        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv).options(desc).allow_unregistered().run(), vm);
        po::notify(vm);

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
    options = parse_options(argc, argv);
    return boost::unit_test::unit_test_main(init_unit_test, argc, argv);
}
