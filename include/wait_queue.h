#ifndef _WAIT_QUEUE_H_
#define _WAIT_QUEUE_H_

#include <device.h>
#include <deque>

// Forward declarations.
class job;

/// \brief Modeluje red za čekanje.
class wait_queue : public device {
  public:
    /// \brief Konstruktor.
    ///
    /// \param device_name naziv reda za čekanje.
    explicit wait_queue(const std::string &device_name);

    /// \brief Konstruktor.
    ///
    /// \param device_name naziv reda za čekanje.
    /// \param jobs inicijalni skup poslova.
    explicit wait_queue(const std::string &device_name, const std::deque<job *> &jobs);

    /// \brief Destruktor.
    virtual ~wait_queue();

    /// \brief Uvek vraća \code false jer je red beskonačan.
    ///
    /// \return uvek \code false.
    virtual bool full() const override { return false; };

    /// \brief Uvek vraća \code 0 jer u redu za čekanje nema obrade.
    ///
    /// \return \code 0.
    virtual long processing_time() const override { return 0; };

    /// \brief Dodaje novi posao u ovaj red.
    ///
    /// \param new_job novi posao.
    virtual void add_job(job *new_job) override;

    /// \copydoc device::time_jump(long)
    virtual void time_jump(long amount) override;

    /// \brief Šalje poslove na uređaje ako mogu da ih prihvate.
    void move_jobs();

  protected:
    /// \brief Ovaj metod ne radi ništa pošto u redu nema obrade.
    virtual void generate_processing_time() override{};

    /// \brief Proverava da li je bar jedan od narednih uređaja u lancu spreman da prihvati posao.
    ///
    /// \return \code true ako je bar jedan od narednih uređaja slobodan.
    bool next_device_available() const;

  private:
    std::deque<job *> m_jobs;  //!< Poslovi koji čekaju u ovom redu.
};

#endif  // _WAIT_QUEUE_H_
