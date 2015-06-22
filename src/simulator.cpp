#include <iomanip>
#include <iostream>
#include <job.h>
#include <processing_device.h>
#include <simulator.h>
#include <wait_queue.h>

const uint64_t minut = 6000000l;

simulator::simulator(int number_of_user_discs, int number_of_processes, int simulation_time)
    : cpus(),
      sys_discs(),
      user_discs(),
      jobs(),
      processing_devices(),
      wait_queues(),
      m_simulation_time(simulation_time) {
    queue_cpu = new wait_queue("CPU QUEUE");
    queue_sys = new wait_queue("SYSTEM DISCS QUEUE");
    queue_usr = new wait_queue("USER DISCS QUEUE");
    wait_queues.push_back(queue_cpu);
    wait_queues.push_back(queue_sys);
    wait_queues.push_back(queue_usr);

    cpu0 = new processing_device(8000, "CPU0");
    cpu1 = new processing_device(8000, "CPU1");
    processing_devices.push_back(cpu0);
    processing_devices.push_back(cpu1);
    cpus.push_back(cpu0);
    cpus.push_back(cpu1);

    sys0 = new processing_device(20000, "SYSTEM DISC0");
    sys1 = new processing_device(20000, "SYSTEM DISC1");
    processing_devices.push_back(sys0);
    processing_devices.push_back(sys1);
    sys_discs.push_back(sys0);
    sys_discs.push_back(sys1);

    for (auto i = 0; i < number_of_user_discs; i++) {
        std::string name = "USER DISC";
        auto user_disc = new processing_device(25000, name + std::to_string(i));
        user_discs.push_back(user_disc);
        processing_devices.push_back(user_disc);
    }

    queue_cpu->add_next_device(0.5, cpu0);
    queue_cpu->add_next_device(0.5, cpu1);

    cpu0->add_next_device(0.1, queue_cpu);
    cpu0->add_next_device(0.2, queue_sys);
    cpu0->add_next_device(0.7, queue_usr);

    cpu1->add_next_device(0.1, queue_cpu);
    cpu1->add_next_device(0.2, queue_sys);
    cpu1->add_next_device(0.7, queue_usr);

    queue_sys->add_next_device(0.5, sys0);
    queue_sys->add_next_device(0.5, sys1);

    sys0->add_next_device(0.4, queue_cpu);
    sys0->add_next_device(0.6, queue_cpu);

    sys1->add_next_device(0.4, queue_cpu);
    sys1->add_next_device(0.6, queue_cpu);

    for (auto i = 0; i < number_of_user_discs; i++) {
        queue_usr->add_next_device(1.0 / number_of_user_discs, user_discs[i]);
        user_discs[i]->add_next_device(1.0, queue_cpu);
    }

    queue_cpu->check_consistency();
    queue_sys->check_consistency();
    queue_usr->check_consistency();
    cpu0->check_consistency();
    cpu1->check_consistency();
    sys0->check_consistency();
    sys1->check_consistency();
    for (const auto &user_disc : user_discs) {
        user_disc->check_consistency();
    }

    for (auto i = 0; i < number_of_processes; i++) {
        std::string name = "JOB " + std::to_string(i);
        job *new_job = new job(name, queue_cpu);
        jobs.push_back(new_job);
        queue_cpu->add_job(new_job);
    }
}

simulator::~simulator() {
    delete queue_cpu;
    delete queue_sys;
    delete queue_usr;

    delete cpu0;
    delete cpu1;

    delete sys0;
    delete sys1;

    while (!user_discs.empty()) {
        auto user_disc = user_discs.back();
        user_discs.pop_back();
        delete user_disc;
    }

    while (!jobs.empty()) {
        auto job = jobs.back();
        jobs.pop_back();
        delete job;
    }
}

void simulator::start() {
    std::cout << "Pokrećem simulaciju u trajanju od " << m_simulation_time << " minuta."
              << std::endl;

    uint64_t simulation_time = m_simulation_time * minut;
    uint64_t elapsed = 0;
    queue_cpu->move_jobs();
    while (elapsed < simulation_time) {
        elapsed += step();
    }
}

long simulator::step() {
    long jump = min_time_jump();
    for (auto device : processing_devices) {
        device->time_jump(jump);
    }
    for (auto wait_queue : wait_queues) {
        wait_queue->time_jump(jump);
        wait_queue->move_jobs();
    }

    return jump;
}

long simulator::min_time_jump() const {
    long minimum = 0;
    std::vector<device *>::size_type i;

    for (i = 0; i < processing_devices.size(); i++) {
        if (processing_devices[i]->full()) {
            minimum = processing_devices[i]->processing_time();
            break;
        }
    }

    for (; i < processing_devices.size(); i++) {
        if (processing_devices[i]->full() && processing_devices[i]->processing_time() < minimum) {
            minimum = processing_devices[i]->processing_time();
        }
    }

    return minimum;
}

void simulator::print_stats() const {
    std::cout << "TRENUTNO STANJE U SIMULATORU" << std::endl;
    for (const auto job : jobs) {
        std::cout << job->to_string();
    }
}

void simulator::print_results() const {
    statistics s = calculate_statistics();
    std::cout << "Rezultati za " << user_discs.size() << " korisničkih diskova i " << jobs.size()
              << " poslova." << std::endl;
    std::cout << "CPU 0" << std::endl;
    std::cout << "\tIskorišćenost        : " << s.U_cpu0 << std::endl;
    std::cout << "\tProtok               : " << s.X_cpu0 << std::endl;
    std::cout << "\tProsečan broj poslova: " << s.J_cpu0 << std::endl;
    std::cout << "CPU 1" << std::endl;
    std::cout << "\tIskorišćenost        : " << s.U_cpu1 << std::endl;
    std::cout << "\tProtok               : " << s.X_cpu1 << std::endl;
    std::cout << "\tProsečan broj poslova: " << s.J_cpu1 << std::endl;
    std::cout << "CPU proseci" << std::endl;
    std::cout << "\tIskorišćenost        : " << s.U_cpu_avg << std::endl;
    std::cout << "\tProtok               : " << s.X_cpu_avg << std::endl;
    std::cout << "\tProsečan broj poslova: " << s.J_cpu_avg << std::endl;

    std::cout << "Sistemski disk 0" << std::endl;
    std::cout << "\tIskorišćenost        : " << s.U_sys0 << std::endl;
    std::cout << "\tProtok               : " << s.X_sys0 << std::endl;
    std::cout << "\tProsečan broj poslova: " << s.J_sys0 << std::endl;
    std::cout << "Sistemski disk 1" << std::endl;
    std::cout << "\tIskorišćenost        : " << s.U_sys1 << std::endl;
    std::cout << "\tProtok               : " << s.X_sys1 << std::endl;
    std::cout << "\tProsečan broj poslova: " << s.J_sys1 << std::endl;
    std::cout << "Sistemski diskovi proseci" << std::endl;
    std::cout << "\tIskorišćenost        : " << s.U_sys_avg << std::endl;
    std::cout << "\tProtok               : " << s.X_sys_avg << std::endl;
    std::cout << "\tProsečan broj poslova: " << s.J_sys_avg << std::endl;

    for (size_t i = 0; i < s.U_usr.size(); i++) {
        std::cout << "Korisnički disk " << i << std::endl;
        std::cout << "\tIskorišćenost        : " << s.U_usr[i] << std::endl;
        std::cout << "\tProtok               : " << s.X_usr[i] << std::endl;
        std::cout << "\tProsečan broj poslova: " << s.J_usr[i] << std::endl;
    }
    std::cout << "Korisnički diskovi proseci" << std::endl;
    std::cout << "\tIskorišćenost        : " << s.U_usr_avg << std::endl;
    std::cout << "\tProtok               : " << s.X_usr_avg << std::endl;
    std::cout << "\tProsečan broj poslova: " << s.J_usr_avg << std::endl;

    std::cout << "Vreme odziva sistema: " << s.T << " ms." << std::endl;

    if (s.U_cpu_avg > s.U_sys_avg && s.U_cpu_avg > s.U_usr_avg) {
        std::cout << "Kritičan resurs su procesori jer im je iskorišćenje najveće." << std::endl;
    }
    if (s.U_sys_avg > s.U_cpu_avg && s.U_sys_avg > s.U_usr_avg) {
        std::cout << "Kritičan resurs su sistemski diskovi jer im je iskorišćenje najveće."
                  << std::endl;
    }
    if (s.U_usr_avg > s.U_cpu_avg && s.U_usr_avg > s.U_sys_avg) {
        std::cout << "Kritičan resurs su korisnički diskovi jer im je iskorišćenje najveće."
                  << std::endl;
    }
}

statistics simulator::calculate_statistics() const {
    statistics s;
    long total_time = 0;
    long cycles = 0;
    for (const auto job : jobs) {
        total_time += job->total_time();
        cycles += job->cycles();
    }
    uint64_t sim_time = m_simulation_time * minut;  // Vreme simulacije u mikrosekundama.
    s.T = total_time / (cycles * 1000.0);
    // Iskorišćenosti
    s.U_cpu0 = static_cast<double>(cpu0->total_work_time()) / sim_time;
    s.U_cpu1 = static_cast<double>(cpu1->total_work_time()) / sim_time;
    s.U_cpu_avg = (s.U_cpu0 + s.U_cpu1) / 2;
    s.U_sys0 = static_cast<double>(sys0->total_work_time()) / sim_time;
    s.U_sys1 = static_cast<double>(sys1->total_work_time()) / sim_time;
    s.U_sys_avg = (s.U_sys0 + s.U_sys1) / 2;
    for (auto device : user_discs) {
        double U = static_cast<double>(device->total_work_time()) / sim_time;
        s.U_usr.push_back(U);
        s.U_usr_avg += U;
    }
    s.U_usr_avg = s.U_usr_avg / user_discs.size();

    // Protok
    s.X_cpu0 = 1000000l * (static_cast<double>(cpu0->total_work_time()) / sim_time) /
               cpu0->avg_processing_time();
    s.X_cpu1 = 1000000l * (static_cast<double>(cpu1->total_work_time()) / sim_time) /
               cpu1->avg_processing_time();
    s.X_cpu_avg = (s.X_cpu0 + s.X_cpu1) / 2;
    s.X_sys0 = 1000000l * (static_cast<double>(sys0->total_work_time()) / sim_time) /
               cpu0->avg_processing_time();
    s.X_sys1 = 1000000l * (static_cast<double>(sys1->total_work_time()) / sim_time) /
               cpu1->avg_processing_time();
    s.X_sys_avg = (s.X_sys0 + s.X_sys1) / 2;
    for (auto device : user_discs) {
        double X = 1000000l * (static_cast<double>(device->total_work_time()) / sim_time) /
                   device->avg_processing_time();
        s.X_usr.push_back(X);
        s.X_usr_avg += X;
    }
    s.X_usr_avg = s.X_usr_avg / user_discs.size();

    // Prosečan broj poslova
    s.J_cpu0 = 1000.0 * s.T * (static_cast<double>(cpu0->total_work_time()) / sim_time) /
               cpu0->avg_processing_time();
    s.J_cpu1 = 1000.0 * s.T * (static_cast<double>(cpu1->total_work_time()) / sim_time) /
               cpu1->avg_processing_time();
    s.J_cpu_avg = (s.J_cpu0 + s.J_cpu1) / 2;
    s.J_sys0 = 1000.0 * s.T * (static_cast<double>(sys0->total_work_time()) / sim_time) /
               sys0->avg_processing_time();
    s.J_sys1 = 1000.0 * s.T * (static_cast<double>(sys1->total_work_time()) / sim_time) /
               sys1->avg_processing_time();
    s.J_sys_avg = (s.J_sys0 + s.J_sys1) / 2;
    for (auto device : user_discs) {
        double J = 1000.0 * s.T * (static_cast<double>(sys1->total_work_time()) / sim_time) /
                   device->avg_processing_time();
        s.J_usr.push_back(J);
        s.J_usr_avg += J;
    }
    s.J_usr_avg = s.J_usr_avg / user_discs.size();

    return s;
}
