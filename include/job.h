#ifndef _JOB_H_
#define _JOB_H_

#include <string>

// Forward declarations
class device;

/// \brief Modeluje jedan posao u sistemu.
class job {
  public:
    /// \brief Konstruktor.
    ///
    /// \param name Ime posla.
    /// \param initial_device Prvi uređaj da bi se znalo kad je posao završio jedan ciklus.
    explicit job(const std::string &name, device *initial_device);

    /// \brief Postavlje trenutni uređaj u kome se ovaj posao nalazi.
    ///
    /// \param the_device Uređaj.
    void set_current_device(device *the_device);

    /// \brief Vreća uređaj u komes se ovaj posao nalazi.
    ///
    /// \return Uređaj u kome se posao nalazi.
    device *current_device() const;

    /// \brief Vraća tekstualnu reprezentaciju posla.
    ///
    /// \return Tekstualna reprezentacija.
    std::string to_string() const;

    /// \brief Vrši vremenski skok, ažuriranjem vrednosti tajmera u poslu.
    ///
    /// \param amount Pomeraj u mikorsekundama.
    void time_jump(long amount);

    /// \brief Vraća broj završenih ciklusa ovoj posla.
    ///
    /// \return Broj završenih ciklusa.
    long cycles() const { return m_cycles; }

    /// \brief Vraća ukupno vreme ovog posla provedeno u čekanju i obradi.
    ///
    /// \return Ukupno vreme.
    long total_time() const { return m_total_time; }

  private:
    std::string m_name;              //!< Naziv posla.
    device *m_current_device;        //!< Uređaj u kome se posao nalazi.
    const device *m_initial_device;  //!< Početni uređaj da bi se znalo kad je gotov ciklus.
    long m_cycles;                   //!< Broj završenih ciklusa.
    long m_cycle_time;               //!< Vreme proteklo u trenutnom ciklusu.
    long m_total_time;               //!< Ukupno proteklo vreme.
};

#endif  // _JOB_H_
