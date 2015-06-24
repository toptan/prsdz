#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include <statistics.h>
#include <vector>

// Forward declarations
class device;
class job;
class processing_device;
class wait_queue;

/// \brief Klasa koja simulira rad sistema opisanog u zadatku.
class simulator {
  public:
    /// \brief Konstruktor.
    ///
    /// \param user_discs Broj korisničkih diskova.
    /// \param processes Broj procesa (stepen multiprogramiranja).
    /// \param simulation_time Trajanje simulacije u minutima.
    explicit simulator(int number_of_user_discs, int number_of_processes, int simulation_time);

    /// \brief Destruktor.
    virtual ~simulator();

    /// \brief Pokreće simulator.
    void start();

    /// \brief Štampa trenutno stanje simulatora.
    void print_stats() const;

    /// \brief Štampa rezultate simulacije.
    void print_results() const;

    /// \brief Sračunava potrebne vrednosti.
    ///
    /// \return Struktura sa potrebnim vrednostima.
    statistics calculate_statistics() const;

  private:
    wait_queue *queue_cpu;  //!< Red za čekanje procesora.
    wait_queue *queue_sys;  //!< Red za čekanje sistemskih diskova.
    wait_queue *queue_usr;  //!< Red za čekanje korisničkih diskova.

    processing_device *cpu0;  //!< Prvi procesor.
    processing_device *cpu1;  //!< Drugi processor.

    processing_device *sys0;  //!< Prvi sistemski disk.
    processing_device *sys1;  //!< Drugi sistemski disk.

    std::vector<processing_device *> cpus;        //!< Procesori;
    std::vector<processing_device *> sys_discs;   //!< Sistemski diskovi.
    std::vector<processing_device *> user_discs;  //!< Korisnički diskovi.
    std::vector<job *> jobs;                      //!< Poslovi.
    std::vector<device *> processing_devices;     //!< Vektor uređaja koji rade obradu.
    std::vector<wait_queue *> wait_queues;        //!< Vektor redova čekanja.

    uint64_t m_simulation_time;  //!< Ukupno vreme simulacije u minutima.

  private:
    /// \brief Izvršava jedan korak simulacije i vraća koliko je trajao.
    ///
    /// \return Koliko je trajao korak u mikrosekundama.
    long step();

    /// \brief Izračunava minimalni vremenski skok u mikrosekundama.
    ///
    /// \return minimalni vremenski skok.
    long min_time_jump() const;
};

#endif  // _SIMULATOR_H_
