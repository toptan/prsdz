#ifndef _PROBABILITY_RANGE_H_
#define _PROBABILITY_RANGE_H_

#include <utility>

class device;

/// \brief Ovo je pomoćna klasa radi lakšeg pronalaženja sledećeg uređaja u lancu na koji bi posao
/// posle obrade trebalo da ide.
class probability_range : public std::pair<double, double> {
  public:
    /// \brief Konstruktor.
    ///
    /// \param lower_bound Donja granica.
    /// \param upper_bound Gornja granica.
    /// \param the_device Uređaj na koji bi trebalo otići ako je vrednost unitar definisanog opsega.
    explicit probability_range(double lower_bound, double upper_bound, device *the_device)
        : std::pair<double, double>(lower_bound, upper_bound),
          m_lower_bound(lower_bound),
          m_upper_bound(upper_bound),
          m_device(the_device) {}

    /// \brief Proberava da li je vrednost unutar opsega.
    ///
    /// \param value Vrednost koja se proverava.
    ///
    /// \return \code true ako je vrednost u opsegu od (\ref probability_range::m_lower_bound, \ref
    /// probability_range::m_upper_bound]
    bool is_in_range(double value) const { return m_lower_bound < value && value <= m_upper_bound; }

    /// \brief Vraća uređaj pridružen ovom opsegu.
    ///
    /// \return Uređaj.
    device *range_device() const { return m_device; }

  private:
    const double m_lower_bound;  //!< Donja granica.
    const double m_upper_bound;  //!< Gornja granica.
    device *m_device;            //!< Uređaj.
};

#endif  // _PROBABILITY_RANGE_H_
