//
// Copyright 2010-2011 Ettus Research LLC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <uhd/utils/thread_priority.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/usrp/multi_usrp.hpp>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <complex>

namespace po = boost::program_options;

int UHD_SAFE_MAIN(int argc, char *argv[]){
    uhd::set_thread_priority_safe();

    //variables to be set by po
    std::string args;
    std::string clock_source;
    std::string time_source;
    std::vector<std::string> mboard_sensors;

    //setup the program options
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "help message")
        ("args", po::value<std::string>(&args)->default_value(""), "single uhd device address args")
        ("clock-source", po::value<std::string>(&clock_source)->default_value(""), "the clock source (gpsdo, external) or blank for default")
        ("time-source", po::value<std::string>(&clock_source)->default_value(""), "the time source (gpsdo, external) or blank for default")
    ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    //print the help message
    if (vm.count("help")){
        std::cout << boost::format("UHD Test PPS Input %s") % desc << std::endl;
        std::cout
                << std::endl
                << "Tests if the PPS input signal is working. Will throw an error if not."
                << std::endl
                << std::endl;
        return ~0;
    }

    //create a usrp device
    std::cout << std::endl;
    std::cout << boost::format("Creating the usrp device with: %s...") % args << std::endl;
    uhd::usrp::multi_usrp::sptr usrp = uhd::usrp::multi_usrp::make(args);
    std::cout << boost::format("Using Device: %s") % usrp->get_pp_string() << std::endl;

    //sleep off if gpsdo detected and time next pps already set
    boost::this_thread::sleep(boost::posix_time::seconds(1));

    //set time and clock sources if specified
    if (not clock_source.empty()) usrp->set_clock_source(clock_source);
    if (not time_source.empty()) usrp->set_time_source(time_source);

    mboard_sensors = usrp->get_mboard_sensor_names();
    for(int i = 0; i < mboard_sensors.size(); i++)
      std::cout << (usrp->get_mboard_sensor(mboard_sensors[i], 0)).to_pp_string()
                << std::endl;

    //set the time at an unknown pps (will throw if no pps)
    std::cout << std::endl << "Attempt to detect the PPS and set the time..." << std::endl << std::endl;
    usrp->set_time_unknown_pps(uhd::time_spec_t(0.0));
    std::cout << std::endl << "Success!" << std::endl << std::endl;
    return EXIT_SUCCESS;
}
