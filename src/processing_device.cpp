#include <cstdlib>
#include <exception>
#include <job.h>
#include <processing_device.h>

processing_device::processing_device(long avg_processing_time, const std::string &name)
    : device(avg_processing_time, name), m_job(nullptr) {}

processing_device::~processing_device() {}

void processing_device::add_job(job *new_job) {
    if (m_processing_time > 0) {
        throw std::logic_error("Uređaj '" + m_name +
                               "' već vrši obradu i ne može da preuzme novi posao.");
    }
    m_job = new_job;
}

void processing_device::time_jump(long amount) {
    if (m_job == nullptr) {
        // Ništa se ne obrađuje, pa ovde ne radimo ništa.
        return;
    }
    if (m_processing_time < amount) {
        throw std::invalid_argument("Vremenski pomeraj je " + std::to_string(amount) +
                                    " mikro sekundi, a preostalo vreme obrade posla je " +
                                    std::to_string(m_processing_time) + " mikrosekundi.");
    }
    m_processing_time = m_processing_time - amount;
    if (m_processing_time == 0.0) {

    }
}

void processing_device::generate_processing_time() {
    // Malo da razbacamo vreme oko prosečne vrednosti.
    double faktor = 0.0;
    while (faktor == 0.0) {
        faktor = 2 * drand48();
    }
    m_processing_time = m_avg_processing_time * faktor;
}
