#include <cstdlib>
#include <stdexcept>
#include <exception>
#include <job.h>
#include <processing_device.h>

processing_device::processing_device(long average_processing_time, const std::string &device_name)
    : device(average_processing_time, device_name), m_job(nullptr) {}

processing_device::~processing_device() {}

void processing_device::add_job(job *new_job) {
    if (m_processing_time > 0) {
        throw std::logic_error("Uređaj '" + m_name +
                               "' već vrši obradu i ne može da preuzme novi posao.");
    }
    m_job = new_job;
    m_job->set_current_device(this);
    generate_processing_time();
}

void processing_device::time_jump(long amount) {
    if (m_job == nullptr) {
        // Ništa se ne obrađuje, pa ovde ne radimo ništa.
        return;
    }
    if (m_processing_time < amount) {
        throw std::invalid_argument("Vremenski pomeraj je " + std::to_string(amount) +
                                    " mikrosekundi, a preostalo vreme obrade posla je " +
                                    std::to_string(m_processing_time) + " mikrosekundi.");
    }
    m_processing_time -= amount;
    m_total_work_time += amount;
    m_job->time_jump(amount);
    if (m_processing_time == 0.0) {
        double next = drand48();
        for (const auto &range : m_ranges) {
            if (range.is_in_range(next)) {
                // Nema potrebe za bilo kakvom proverom pošto sa uređaja koji radi obradu
                // u ovoj simulaciji uvek idemo u beskonačni red za čekanje ispred sledećeg
                // uređaja.
                range.range_device()->add_job(m_job);
                m_job = nullptr;
                return;
            }
        }
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
