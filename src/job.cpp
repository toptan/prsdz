#include <job.h>
#include <device.h>
#include <sstream>

job::job(const std::string &name) : m_name(name), m_current_device(nullptr) {}

void job::set_current_device(device *the_device) { m_current_device = the_device; }

device *job::current_device() const { return m_current_device; }

std::string job::to_string() const {
    std::stringstream ss;
    ss << m_name << std::endl;
    ss << "\tLokacija: " << m_current_device->name() << std::endl;
    ss << "\tPreostalo vreme obrade: " << m_current_device->processing_time() << std::endl;

    return ss.str();
}
