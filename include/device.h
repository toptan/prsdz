#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <string>
#include <vector>

// Forward declarations.
class job;

/// \brief Modelije apstraktni uređaj u sistemu.
class device {
  public:
    /// \brief Konstruktor.
    ///
    /// \param avg_processing_time prosečno vreme obrade u mikro sekundama.
    /// \param name naziv uređaja.
    explicit device(long avg_processing_time, const std::string &name);

    /// \brief Destruktor.
    virtual ~device();

    /// \brief Vraća prosečno vreme obrade u mikrosekundama.
    ///
    /// \return prosečno vreme obrade.
    long avg_processing_time() const;

    /// \brief Vraća naziv uređaja.
    ///
    /// \return naziv uređaja.
    std::string name() const;

    /// \brief Vraća da li je uređaj "pun". Tačnije da li može da primi posao.
    ///
    /// \return može li uređaj da primi posao.
    virtual bool full() const = 0;

    /// \brief Vraća preostalo vreme obrade tekućeg posla.
    ///
    /// \return preostalo vreme obrade tekućeg posla.
    virtual long processing_time() const = 0;

    /// \brief Dodaje novi uređaj u listu sledećih uređaja.
    ///
    /// \param probability verovatnoća prelaska u uređaj.
    /// \param next_device uređaj.
    void add_next_device(double probability, device *next_device);

    /// \brief Dodaje novi posao u ovaj uređaj.
    ///
    /// \param job_to_process novi posao.
    virtual void add_job(job *new_job) = 0;

  protected:
    /// \brief Generiše vreme obrade za dodati posao.
    virtual void generate_processing_time() = 0;

  protected:
    const long m_avg_processing_time;  //!< Prosečno vreme obrade u mikrosekundama.
    std::string m_name;                //!< Naziv uređaja.
    long m_processing_time;            //!< Preostalo vreme obrade tekućeg posla u mikrosekundama.

    std::vector<device *> m_next_devices;  //!< Sledeći uređaji u lancu.
    std::vector<double> m_probabilities;   //!< Verovatnoće prelaska u sledeći uređaj.
};

#endif  // _DEVICE_H_
