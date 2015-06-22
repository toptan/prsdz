#ifndef _PROCESSING_DEVICE_H_
#define _PROCESSING_DEVICE_H_

#include <device.h>
#include <string>

// Forward declarations.
class job;

/// \brief Modeluje uređaj koji vrši stvarnu obradu, kao CPU ili disk.
class processing_device : public device {
  public:
    /// \brief Konstruktor.
    ///
    /// \param avg_processing_time prosečno vreme obrade u mikrosekundama.
    /// \param name naziv uređaja.
    explicit processing_device(long avg_processing_time, const std::string &name);

    /// \brief Destruktor.
    virtual ~processing_device();

    /// \brief Vraća da li je uređaj "pun", tj. da li već ima posao i vrši obradu.
    ///
    /// \return \code true ako uređaj već vrši obradu.
    virtual bool full() const override { return m_job != nullptr; }

    /// \brief Vraća preostalo vreme obrade tekućeg posla u mikrosekundama.
    ///
    /// \return preostalo vreme obrade tekućeg posla u mikrosekundama.
    virtual long processing_time() const override { return m_processing_time; }

    /// \brief Dodaje novi posao uređaju.
    ///
    /// \param new_job novi posao.
    virtual void add_job(job *new_job) override;

    /// \copydoc device::time_jump(long)
    virtual void time_jump(long amount) override;

  protected:
    /// \brief Generiše vreme obrade tekućeg posla.
    virtual void generate_processing_time() override;

  private:
    job *m_job;  //!< Posao koji se trenutno obrađuje.
};

#endif  // _PROCESSING_DEVICE_H_
