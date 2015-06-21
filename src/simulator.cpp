#include <job.h>
#include <processing_device.h>
#include <simulator.h>
#include <wait_queue.h>

simulator::simulator(int number_of_user_discs, int numuber_of_processes) : user_discs() {
    queue_cpu = new wait_queue("CPU QUEUE");
    queue_sys = new wait_queue("SYSTEM DISCS QUEUE");
    queue_usr = new wait_queue("USER DISCS QUEUE");

    cpu0 = new processing_device(8000, "CPU0");
    cpu1 = new processing_device(8000, "CPU1");

    sys0 = new processing_device(20000, "SYSTEM DISC 0");
    sys1 = new processing_device(20000, "SYSTEM DISC 1");

    for (auto i = 0; i < number_of_user_discs; i++) {
        std::string name = "USER DISC ";
        user_discs.push_back(new processing_device(25000, name + std::to_string(i)));
    }

    queue_cpu->add_next_device(0.5, cpu0);
    queue_cpu->add_next_device(0.5, cpu0);

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
    for (const auto &user_disc: user_discs) {
        user_disc->check_consistency();
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
}
