#include <device.h>
#include <exception>
#include <numeric>

device::device(long avg_processing_time, const std::string &name)
    : m_avg_processing_time(avg_processing_time),
      m_name(name),
      m_processing_time(0),
      m_total_work_time(0),
      m_next_devices(),
      m_probabilities(),
      m_ranges() {}

device::~device() {}

long device::processing_time() const { return m_processing_time; }

long device::avg_processing_time() const { return m_avg_processing_time; }

std::string device::name() const { return m_name; }

void device::add_next_device(double probability, device *next_device) {
    double temp = std::accumulate(m_probabilities.begin(), m_probabilities.end(), 0.0);
    if (temp + probability > 1) {
        throw std::invalid_argument("Trenutni zbir verovatnoća prelazaka u naredne uređaje je " +
                                    std::to_string(temp) + ". Dodavanje uređaja sa verovatnoćom " +
                                    std::to_string(probability) +
                                    " bi dalo zbir verovatnoća veći od 1 što je nemoguće.");
    }
    m_next_devices.push_back(next_device);
    m_probabilities.push_back(probability);
    m_ranges.emplace_back(temp, temp + probability, next_device);
}

void device::check_consistency() {
    double sum = std::accumulate(m_probabilities.begin(), m_probabilities.end(), 0.0);
    if (sum != 1) {
        throw std::invalid_argument("Suma verovatnoća prelazaka u naredne uređaje za uređaj '" +
                                    m_name + "' je " + std::to_string(sum) +
                                    ". Ova suma mora biti 1.");
    }
}
