#ifndef _SIMULATOR_H_
#define _SIMULATOR_H_

#include <vector>

// Forward declarations
class wait_queue;
class processing_device;
class job;

/// \brief Klasa koja simulira rad sistema opisanog u zadatku.
class simulator {
  public:
    /// \brief Konstruktor.
    ///
    /// \param user_discs broj korisničkih diskova.
    /// \param processes broj procesa (stepen multiprogramiranja)
    explicit simulator(int number_of_user_discs, int number_of_processes);

    /// \brief Destruktor.
    virtual ~simulator();

  private:
    wait_queue *queue_cpu;  //!< Red za čekanje procesora.
    wait_queue *queue_sys;  //!< Red za čekanje sistemskih diskova.
    wait_queue *queue_usr;  //!< Red za čekanje korisničkih diskova.

    processing_device *cpu0;  //!< Prvi procesor.
    processing_device *cpu1;  //!< Drugi processor.

    processing_device *sys0;  //!< Prvi sistemski disk.
    processing_device *sys1;  //!< Drugi sistemski disk,

    std::vector<processing_device *> user_discs;  //!< Korisnički diskovi.
};

#endif  // _SIMULATOR_H_
