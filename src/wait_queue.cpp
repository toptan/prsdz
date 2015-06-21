#include <wait_queue.h>
#include <job.h>

wait_queue::wait_queue(const std::string &name) : device(0, name), m_jobs() {}

wait_queue::wait_queue(const std::string &name, const std::deque<job *> &jobs)
    : device(0, name), m_jobs(jobs) {}

wait_queue::~wait_queue() {}

void wait_queue::add_job(job *new_job) { m_jobs.push_back(new_job); }

void wait_queue::time_jump(long amount) {
    // TODO: Dodaj time jump logiku za red čekanja.
}
