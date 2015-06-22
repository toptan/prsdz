#include <iostream>
#include <cstdlib>
#include <chrono>
#include <boost/program_options.hpp>
#include <simulator.h>

namespace po = boost::program_options;

/// \brief Struktura koja mapira opcije iz komandne linije.
struct options {
    options() : simulation_time(1440), processes(5), discs(2), buzen(false), run_all(false) {}
    int simulation_time;  //!< Vreme simulacije u minutima.
    int processes;        //!< Broj procesa.
    int discs;            //!< Broj korisničkih diskova.
    bool buzen;           //!< Startuj Bjuzenov analitički metod.
    bool run_all;         //!< Uradi sve prema postavci zadatka.
    std::string file;     //!< Izlazni fajl za rezultate ako se izvršava sve po postavci zadatka.
};

int main(int argc, char *argv[]) {
    options o;
    po::options_description description("Dozvoljene opcije");
    // clang-format off
    description.add_options()
        ("help", "Ova poruka")
        ("simulation-time", po::value<int>(&o.simulation_time)->default_value(1440), "Simulirano vreme u minutima.")
        ("processes", po::value<int>(&o.processes)->default_value(5), "Broj procesa. Stepen multiprogramiranja.")
        ("discs", po::value<int>(&o.discs)->default_value(2), "Broj korisničkih diskova.")
        ("buzen", po::value<bool>(&o.buzen)->default_value(false), "Startuj Bjuzenov analitički metod umesto simulacije.")
        ("run-all", po::value<bool>(&o.run_all)->default_value(false), "Pokreni sva izvršavanja prema postavci zadatka.")
        ("file", po::value<std::string>(&o.file)->default_value(""), "Izlazni fajl za rezultate ako se izvršava sve po postavci zadatka.");
    // clang-format on

    po::variables_map vmap;
    po::store(po::parse_command_line(argc, argv, description), vmap);
    po::notify(vmap);
    if (vmap.count("help")) {
        std::cout << description << std::endl;
        return 0;
    }
    srand48(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    if (!o.run_all) {
        if (o.buzen) {
            std::cout << "Bjuzenova analitička metoda" << std::endl;
        } else {
            simulator s(o.discs, o.processes, o.simulation_time);
            s.start();
            s.print_results();
        }
    } else {
        int discs[] = {2, 3, 4, 5};
        int processes[] = {5, 10, 15};
        for (auto k : discs) {
            for (auto n : processes) {
                simulator *s = new simulator(k, n, 1440);
                s->start();
                s->print_results();
                delete s;
            }
        }
    }

    return 0;
}
