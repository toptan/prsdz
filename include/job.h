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
    explicit job(const std::string &name);

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

  private:
    std::string m_name;        //!< Naziv posla.
    device *m_current_device;  //!< Uređaj u kome se posao nalazi.
};

#endif  // _JOB_H_
