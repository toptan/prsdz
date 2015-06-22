#include <job.h>
#include <device.h>
#include <iostream>
#include <sstream>

job::job(const std::string &name, device *initial_device)
    : m_name(name),
      m_current_device(nullptr),
      m_initial_device(initial_device),
      m_cycles(0),
      m_cycle_time(0),
      m_total_time(0) {}

void job::set_current_device(device *the_device) {
    if (m_current_device != nullptr && m_initial_device == the_device) {
        m_cycles++;
        m_total_time = m_total_time + m_cycle_time;
        m_cycle_time = 0;
    }
    m_current_device = the_device;
}

device *job::current_device() const { return m_current_device; }

std::string job::to_string() const {
    std::stringstream ss;
    ss << m_name << std::endl;
    ss << "\tLokacija              : " << m_current_device->name() << std::endl;
    ss << "\tPreostalo vreme obrade: " << m_current_device->processing_time() << " mikrosekundi"
       << std::endl;
    ss << "\tBroj ciklusa do sada  : " << m_cycles << std::endl;
    if (m_cycles != 0) {
        ss << "\tProsečno vreme obrade : " << m_total_time / m_cycles << " mikrosekundi"
           << std::endl;
    }
    return ss.str();
}

void job::time_jump(long amount) { m_cycle_time = m_cycle_time + amount; }
