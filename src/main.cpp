#include <boost/program_options.hpp>
#include <buzen.h>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <simulator.h>
#include <statistics.h>

namespace po = boost::program_options;

/// \brief Struktura koja mapira opcije iz komandne linije.
struct options {
    options()
        : simulation_time(1440),
          processes(5),
          discs(2),
          buzen(false),
          comparison(false),
          run_all(false) {}
    int simulation_time;   //!< Vreme simulacije u minutima.
    int processes;         //!< Broj procesa.
    int discs;             //!< Broj korisničkih diskova.
    bool buzen;            //!< Startuj Bjuzenov analitički metod.
    bool comparison;       //!< Izvrši oba metoda i izbaci uporedne rezultate.
    bool run_all;          //!< Uradi sve prema postavci zadatka.
    std::string sim_file;  //!< Izlazni fajl za rezultate simulacije.
    std::string buz_file;  //!< Izlazni fajl za rezultate Bjuzenovog metoda.
    std::string cmp_file;  //!< Izlazni fajl za poređenje rezultata.
};

void print_comparison(const statistics &sim, const statistics &buz, std::ostream &out) {
    out << "Broj diskova: " << sim.user_discs << std::endl;
    out << "Broj procesa: " << sim.processes << std::endl;
    out << "                                Simulacija           Analitika             Razlika";
    out << std::endl;
    out << std::fixed;
    out.precision(6);

    out << "CPU 0 Iskorišćenost             ";
    out.width(10);
    out << sim.U_cpu0;
    out.width(20);
    out << buz.U_cpu0;
    out.width(20);
    out << sim.U_cpu0 - buz.U_cpu0 << std::endl;
    out << "CPU 0 Protok                    ";
    out.width(10);
    out << sim.X_cpu0;
    out.width(20);
    out << buz.X_cpu0;
    out.width(20);
    out << sim.X_cpu0 - buz.X_cpu0 << std::endl;
    out << "CPU 0 Prosečan broj poslova     ";
    out.width(10);
    out << sim.J_cpu0;
    out.width(20);
    out << buz.J_cpu0;
    out.width(20);
    out << sim.J_cpu0 - buz.J_cpu0 << std::endl;

    out << "CPU 1 Iskorišćenost             ";
    out.width(10);
    out << sim.U_cpu1;
    out.width(20);
    out << buz.U_cpu1;
    out.width(20);
    out << sim.U_cpu1 - buz.U_cpu1 << std::endl;
    out << "CPU 1 Protok                    ";
    out.width(10);
    out << sim.X_cpu1;
    out.width(20);
    out << buz.X_cpu1;
    out.width(20);
    out << sim.X_cpu1 - buz.X_cpu1 << std::endl;
    out << "CPU 1 Prosečan broj poslova     ";
    out.width(10);
    out << sim.J_cpu1;
    out.width(20);
    out << buz.J_cpu1;
    out.width(20);
    out << sim.J_cpu1 - buz.J_cpu1 << std::endl;

    out << "CPU Prosečna iskorišćenost      ";
    out.width(10);
    out << sim.U_cpu_avg;
    out.width(20);
    out << buz.U_cpu_avg;
    out.width(20);
    out << sim.U_cpu_avg - buz.U_cpu_avg << std::endl;
    out << "CPU Prosečan protok             ";
    out.width(10);
    out << sim.X_cpu_avg;
    out.width(20);
    out << buz.X_cpu_avg;
    out.width(20);
    out << sim.X_cpu_avg - buz.X_cpu_avg << std::endl;
    out << "CPU Prosečan broj poslova       ";
    out.width(10);
    out << sim.J_cpu_avg;
    out.width(20);
    out << buz.J_cpu_avg;
    out.width(20);
    out << sim.J_cpu_avg - buz.J_cpu_avg << std::endl;

    out << "SYS 0 Iskorišćenost             ";
    out.width(10);
    out << sim.U_sys0;
    out.width(20);
    out << buz.U_sys0;
    out.width(20);
    out << sim.U_sys0 - buz.U_sys0 << std::endl;
    out << "SYS 0 Protok                    ";
    out.width(10);
    out << sim.X_sys0;
    out.width(20);
    out << buz.X_sys0;
    out.width(20);
    out << sim.X_sys0 - buz.X_sys0 << std::endl;
    out << "SYS 0 Prosečan broj poslova     ";
    out.width(10);
    out << sim.J_sys0;
    out.width(20);
    out << buz.J_sys0;
    out.width(20);
    out << sim.J_sys0 - buz.J_sys0 << std::endl;

    out << "SYS 1 Iskorišćenost             ";
    out.width(10);
    out << sim.U_sys1;
    out.width(20);
    out << buz.U_sys1;
    out.width(20);
    out << sim.U_sys1 - buz.U_sys1 << std::endl;
    out << "SYS 1 Protok                    ";
    out.width(10);
    out << sim.X_sys1;
    out.width(20);
    out << buz.X_sys1;
    out.width(20);
    out << sim.X_sys1 - buz.X_sys1 << std::endl;
    out << "SYS 1 Prosečan broj poslova     ";
    out.width(10);
    out << sim.J_sys1;
    out.width(20);
    out << buz.J_sys1;
    out.width(20);
    out << sim.J_sys1 - buz.J_sys1 << std::endl;

    out << "SYS Prosečna iskorišćenost      ";
    out.width(10);
    out << sim.U_sys_avg;
    out.width(20);
    out << buz.U_sys_avg;
    out.width(20);
    out << sim.U_sys_avg - buz.U_sys_avg << std::endl;
    out << "SYS Prosečan protok             ";
    out.width(10);
    out << sim.X_sys_avg;
    out.width(20);
    out << buz.X_sys_avg;
    out.width(20);
    out << sim.X_sys_avg - buz.X_sys_avg << std::endl;
    out << "SYS Prosečan broj poslova       ";
    out.width(10);
    out << sim.J_sys_avg;
    out.width(20);
    out << buz.J_sys_avg;
    out.width(20);
    out << sim.J_sys_avg - buz.J_sys_avg << std::endl;

    for (auto i = 0u; i < sim.U_usr.size(); i++) {
        out << "USR " << i << " Iskorišćenost             ";
        out.width(10);
        out << sim.U_usr[i];
        out.width(20);
        out << buz.U_usr[i];
        out.width(20);
        out << sim.U_usr[i] - buz.U_usr[i] << std::endl;
        out << "USR " << i << " Protok                    ";
        out.width(10);
        out << sim.X_usr[i];
        out.width(20);
        out << buz.X_usr[i];
        out.width(20);
        out << sim.X_usr[i] - buz.X_usr[i] << std::endl;
        out << "USR " << i << " Prosečan broj poslova     ";
        out.width(10);
        out << sim.J_usr[i];
        out.width(20);
        out << buz.J_usr[i];
        out.width(20);
        out << sim.J_usr[i] - buz.J_usr[i] << std::endl;
    }

    out << "USR Prosečna iskorišćenost      ";
    out.width(10);
    out << sim.U_usr_avg;
    out.width(20);
    out << buz.U_usr_avg;
    out.width(20);
    out << sim.U_usr_avg - buz.U_usr_avg << std::endl;
    out << "USR Prosečan protok             ";
    out.width(10);
    out << sim.X_usr_avg;
    out.width(20);
    out << buz.X_usr_avg;
    out.width(20);
    out << sim.X_usr_avg - buz.X_usr_avg << std::endl;
    out << "USR Prosečan broj poslova       ";
    out.width(10);
    out << sim.J_usr_avg;
    out.width(20);
    out << buz.J_usr_avg;
    out.width(20);
    out << sim.J_usr_avg - buz.J_usr_avg << std::endl;

    out << "Vreme odziva sistema [ms]       ";
    out.width(10);
    out << sim.T;
    out.width(20);
    out << buz.T;
    out.width(20);
    out << sim.T - buz.T << std::endl;
    out << std::endl;
}

statistics run_simulation(const options &o) {
    simulator s(o.discs, o.processes, o.simulation_time);
    s.start();
    if (o.sim_file == "") {
        s.print_results(std::cout);
    } else {
        std::fstream out(o.sim_file, std::ofstream::out | std::ofstream::trunc);
        s.print_results(out);
        std::cout << "Simulacije je gotova. Rezultati su u fajlu " << o.sim_file << std::endl;
        out.flush();
        out.close();
    }
    return s.calculate_statistics();
}

statistics run_buzen(const options &o) {
    buzen b(o.discs, o.processes);
    if (o.buz_file == "") {
        b.print_results(std::cout);
    } else {
        std::fstream out(o.buz_file, std::ofstream::out | std::ofstream::trunc);
        b.print_results(out);
        std::cout << "Bjuzenov analitički metod je gotov. Rezultati su u fajlu " << o.buz_file
                  << std::endl;
        out.flush();
        out.close();
    }
    return b.calculate_results();
}

void run_comparison(const options &o) {
    statistics s = run_simulation(o);
    statistics b = run_buzen(o);
    if (o.cmp_file == "") {
        print_comparison(s, b, std::cout);
    } else {
        std::fstream out(o.cmp_file, std::ofstream::out | std::ofstream::trunc);
        print_comparison(s, b, out);
        std::cout << "Uporedni rezultati su zapisani u fajl " << o.cmp_file << std::endl;
        out.flush();
        out.close();
    }
}

void full_run(const options &o) {
    std::vector<statistics> s;
    std::vector<statistics> b;
    int discs[] = {2, 3, 4, 5};
    int processes[] = {5, 10, 15};
    std::ostream *buz_out;
    std::ostream *sim_out;
    std::ostream *cmp_out;
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
    if (o.cmp_file == "") {
        cmp_out = &std::cout;
    } else {
        cmp_out = new std::fstream(o.cmp_file, std::ofstream::out | std::ofstream::trunc);
    }

    for (auto k : discs) {
        for (auto n : processes) {
            buzen *buz = new buzen(k, n);
            simulator *sim = new simulator(k, n, o.simulation_time);
            sim->start();
            b.push_back(buz->calculate_results());
            s.push_back(sim->calculate_statistics());
            buz->print_results(*buz_out);
            sim->print_results(*sim_out);
            delete sim;
            delete buz;
        }
    }
    for (auto i = 0u; i < s.size(); i++) {
        print_comparison(s[i], b[i], *cmp_out);
    }
    if (buz_out != &std::cout) {
        buz_out->flush();
        delete buz_out;
    }
    if (sim_out != &std::cout) {
        sim_out->flush();
        delete sim_out;
    }
    if (cmp_out != &std::cout) {
        cmp_out->flush();
        delete cmp_out;
    }
}

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
        ("compare", po::value<bool>(&o.comparison)->default_value(false), "Izvršava oba metoda i daje uporedne rezultate.")
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
        if (!o.comparison) {
            if (o.buzen) {
                run_buzen(o);
            } else {
                run_simulation(o);
            }
        } else {
            run_comparison(o);
        }
    } else {
        full_run(o);
    }

    return 0;
}
