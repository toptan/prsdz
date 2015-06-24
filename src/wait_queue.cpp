#include <cstdlib>
#include <job.h>
#include <wait_queue.h>
#include <iostream>

wait_queue::wait_queue(const std::string &device_name) : device(0, device_name), m_jobs() {}

wait_queue::wait_queue(const std::string &device_name, const std::deque<job *> &jobs)
    : device(0, device_name), m_jobs(jobs) {}

wait_queue::~wait_queue() {}

void wait_queue::add_job(job *new_job) {
    new_job->set_current_device(this);
    m_jobs.push_back(new_job);
}

void wait_queue::time_jump(long amount) {
    for (auto job: m_jobs) {
        job->time_jump(amount);
    }
}

void wait_queue::move_jobs() {
    while (!m_jobs.empty()) {
        //std::cout << "Moving " << m_jobs.size() << " jobs." << std::endl;
        if (!next_device_available()) {
            // Nema slobodnih uređaja, ne redimo ništa.
            //std::cout << "No more available devices." << std::endl;
            return;
        }
        double temp = drand48();
        for (const auto &range : m_ranges) {
            if (range.is_in_range(temp) && !range.range_device()->full()) {
                auto job = m_jobs.front();
                range.range_device()->add_job(job);
                m_jobs.pop_front();
                //std::cout << "Job added!" << std::endl;
                break;
            }
        }
        //std::cout << "Another try" << std::endl;
    }
}

bool wait_queue::next_device_available() const {
    for (const device *d : m_next_devices) {
        //std::cout << d->name() << (d->full() ? " is full" : " is empty.") << std::endl;
        if (!d->full()) {
            return true;
        }
    }
    return false;
}
