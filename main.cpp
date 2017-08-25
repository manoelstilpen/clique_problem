#include <cstdlib>
#include <iostream>
#include <boost/program_options.hpp>

#include "Graph.hpp"
#include "Clique.hpp"

using namespace std;
namespace po = boost::program_options;

void show_help(const po::options_description& desc, const std::string& topic = ""){
    std::cout << desc << '\n';
    if (topic != "") {
        std::cout << "You asked for help on: " << topic << '\n';
    }
    exit( EXIT_SUCCESS );
}

bool argParse(int argc, char* argv[], string* method, string* path){
    po::options_description desc("Usage");
    desc.add_options()
        ("help,h", po::value< std::string >()->implicit_value("")->notifier(
            [&desc](const std::string& topic) {
                show_help(desc, topic);
            }
        ),"Show help.")
        ("instance,i", po::value<string>(), "Instance path")
        ("method,m", po::value<string>()->default_value("auto"), "(Optional) Specify to method to use, may be <auto>, <mis> or <clique>");

    if (argc == 1) {
        show_help(desc);
        return false;
    }

    po::variables_map args;

    try {
        po::store(po::parse_command_line(argc, argv, desc), args);
    } catch (po::error const& e) {
        std::cerr << e.what() << '\n';
        exit( EXIT_FAILURE );
    }
    po::notify(args);

    *method = args["method"].as<string>();

    if(args.count("instance")){
        *path = args["instance"].as<string>();
    } else {
        cerr << "Instance parameter is required" << endl << endl;
        show_help(desc);
        return false;
    }

    return true;
}

int main(int argc, char** argv){
    string method = "";
    string path = "";

    if(!argParse(argc, argv, &method, &path)){
        return 1;
    }

    Graph graph(path);
    Clique clique(graph);
    
    cout << clique.searchLargestClique() << endl;

    return 0;
}