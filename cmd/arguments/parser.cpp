#include <iostream>

#include <boost/program_options.hpp>

#include "parser.h"

namespace cmd {

namespace po = boost::program_options;
CommandLineArgumentInterface::CommandLineArgumentInterface(int argc,
                                                           char **argv) {
  try {
    po::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")(
        "save_project_to", po::value<std::string>(),
        "path where you want to save the project")(
        "load_project_from", po::value<std::string>(),
        "path where you want to load a project from");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
      std::cout << desc << "\n";
      return;
    }

    if (vm.count("save_project_to")) {
      std::cout << "Project will be saved to: "
                << vm["save_project_to"].as<std::string>() << ".\n";
      save_path_ = vm["save_project_to"].as<std::string>();
    }
    if (vm.count("load_project_from")) {
      std::cout << "Load project from: "
                << vm["load_project_from"].as<std::string>() << ".\n";
      load_path_ = vm["load_project_from"].as<std::string>();
    }
  } catch (std::exception &e) {
    std::cerr << "error: " << e.what() << "\n";
    return;
  } catch (...) {
    std::cerr << "Exception of unknown type!\n";
  }
  error_ = false;
}
} // namespace cmd