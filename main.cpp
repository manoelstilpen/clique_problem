#include <cstdlib>
#include <iostream>
#include <chrono>
#include <boost/program_options.hpp>

#include "Graph.hpp"
#include "Clique.hpp"
#include "IndependentSet.hpp"

using namespace std;
using namespace std::chrono;
namespace po = boost::program_options;

void show_help(const po::options_description& desc, const std::string& topic = ""){
    std::cout << desc << '\n';
    if (topic != "") {
        std::cout << "You asked for help on: " << topic << '\n';
    }
    exit( EXIT_SUCCESS );
}

bool argParse(int argc, char* argv[], string* method, string* path, bool* printClique){
    po::options_description desc("Usage");
    desc.add_options()
        ("help,h", po::value< std::string >()->implicit_value("")->notifier(
            [&desc](const std::string& topic) {
                show_help(desc, topic);
            }
        ),"Show help.")
        ("instance,i", po::value<string>(), "Instance path")
        ("method,m", po::value<string>()->default_value("auto"), "(Optional) Specify to method to use, may be <auto>, <mis> or <clique>")
        ("print,p", "(Optional) If enabled program will print all vertex in maximum clique");

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

    if(args.count("print")){
        *printClique = true;
    }

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
    bool printClique = false;

    if(!argParse(argc, argv, &method, &path, &printClique)){
        return 1;
    }

    Graph graph(path);

    high_resolution_clock::time_point t1 = high_resolution_clock::now();

        Clique clique(graph);
        //int max = clique.searchLargestClique();
        IndependentSet is(graph);
        int max = is.searchLargestIndependentSet();
        
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
        
    cout << max << "\t" << time_span.count() << endl;

    if(printClique){
        auto it = clique.getLargestCliqueBegin();
        for(;it != clique.getLargestCliqueEnd() ; it++){
            cout << *it << " ";
        }
        cout << endl;
    } 

    return 0;
}