#include <device.h>

device::device(long avg_processing_time, const std::string &name)
    : m_avg_processing_time(avg_processing_time),
      m_name(name),
      m_processing_time(0),
      m_next_devices(),
      m_probabilities() {}

device::~device() {}

long device::processing_time() const { return m_processing_time; }

std::string device::name() const { return m_name; }

void device::add_next_device(double probability, device *next_device) {
    m_next_devices.push_back(next_device);
    m_probabilities.push_back(probability);
}
