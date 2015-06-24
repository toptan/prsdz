#include <boost/program_options.hpp>
#include <buzen.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <simulator.h>

namespace po = boost::program_options;

/// \brief Struktura koja mapira opcije iz komandne linije.
struct options {
    options() : simulation_time(1440), processes(5), discs(2), buzen(false), run_all(false) {}
    int simulation_time;   //!< Vreme simulacije u minutima.
    int processes;         //!< Broj procesa.
    int discs;             //!< Broj korisničkih diskova.
    bool buzen;            //!< Startuj Bjuzenov analitički metod.
    bool run_all;          //!< Uradi sve prema postavci zadatka.
    std::string sim_file;  //!< Izlazni fajl za rezultate simulacije.
    std::string buz_file;  //!< Izlazni fajl za rezultate Bjuzenovog metoda.
    std::string cmp_file;  //!< Izlazni fajl za poređenje rezultata.
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
        ("sim-file", po::value<std::string>(&o.sim_file)->default_value(""), "Izlazni fajl za rezultate simulacije.")
        ("buz-file", po::value<std::string>(&o.buz_file)->default_value(""), "Izlazni fajl za rezultate Bjuzenovog metoda.")
        ("cmp-file", po::value<std::string>(&o.cmp_file)->default_value(""), "Izlazni fajl za poređenje rezultata.");
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
            buzen b(o.discs, o.processes);
            if (o.buz_file == "") {
                b.print_results(std::cout);
            } else {
                std::fstream out(o.buz_file, std::ofstream::out | std::ofstream::trunc);
                b.print_results(out);
                out.flush();
                out.close();
            }
        } else {
            simulator s(o.discs, o.processes, o.simulation_time);
            s.start();
            if (o.sim_file == "") {
                s.print_results(std::cout);
            } else {
                std::fstream out(o.sim_file, std::ofstream::out | std::ofstream::trunc);
                s.print_results(out);
                out.flush();
                out.close();
            }
        }
    } else {
        int discs[] = {2, 3, 4, 5};
        int processes[] = {5, 10, 15};
        std::ostream *buz_out;
        std::ostream *sim_out;
        if (o.buz_file == "") {
            buz_out = &std::cout;
        } else {
            buz_out = new std::fstream(o.buz_file, std::ofstream::out | std::ofstream::trunc);
        }
        if (o.sim_file == "") {
            sim_out = &std::cout;
        } else {
            sim_out = new std::fstream(o.sim_file, std::ofstream::out | std::ofstream::trunc);
        }
        for (auto k : discs) {
            for (auto n : processes) {
                buzen *b = new buzen(k, n);
                simulator *s = new simulator(k, n, o.simulation_time);
                s->start();
                b->print_results(*buz_out);
                s->print_results(*sim_out);
                delete s;
                delete b;
            }
        }
        if (buz_out != &std::cout) {
            buz_out->flush();
            delete buz_out;
        }
        if (sim_out != &std::cout) {
            sim_out->flush();
            delete sim_out;
        }
    }

    return 0;
}
