#include <cmath>
#include <exception>
#include <job.h>
#include <processing_device.h>

processing_device::processing_device(long avg_processing_time, const std::string &name)
    : device(avg_processing_time, name), m_job(nullptr) {}

processing_device::~processing_device() {}

void processing_device::add_job(job *new_job) {
    if (m_processing_time > 0) {
        throw std::logic_error("Uređaj '" + m_name
                               "' već vrši obradu i ne može da preuzme novi posao.");
    }
    m_job = job;
}

void processing_device::generate_processing_time() {
    // Malo da razbacamo vreme oko prosečne vrednosti.
    double faktor = 0.0;
    while (faktor == 0.0) {
        faktor = 2 * drand48();
    }
    m_processing_time = m_avg_processing_time * faktor;
}
