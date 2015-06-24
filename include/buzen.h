#ifndef _BUZEN_H_
#define _BUZEN_H_

#include <ostream>
#include <statistics.h>
#include <vector>

/// \brief Klasa koja implementira Bjuzenovu analitičku metodu.
class buzen {
  public:
    /// \brief Konstruktor.
    ///
    /// \param number_of_discs Broj korisničkih diskova.
    /// \param number_of_processes Broj procesa (stepen multiprogramiranja)
    explicit buzen(int number_of_discs, int number_of_processes);

    /// \brief Destruktor.
    virtual ~buzen();

    /// \brief Štampa rezultate simulacije.
    ///
    /// \param out Izlazni stream.
    void print_results(std::ostream &out) const;

    /// \brief Kreira strukturu sa potrebnim vrednostima.
    ///
    /// \return Struktura sa potrebnim vrednostima.
    statistics calculate_results() const;

  private:
    const int m_number_of_discs;      //!< Broj korisničkih diskova.
    const int m_number_of_processes;  //!< Broj procesa.
    std::vector<double> x;            //!< Potražnje za resursima.
    std::vector<double> s;            //!< Prosečna vremena obrade svakog uređaja.
    std::vector<double> G;            //!< Poslednja kolona matrice.
    std::vector<double> J;            //!< Prosečan broj poslova u svakom uređaju.
    std::vector<double> U;            //!< Iskorišćenja uređaja-resursa.
    std::vector<double> X;            //!< Protoci kroz uređaje.

    const double pcc;  //!< Verovatnoća prelaza sa CPU nazad na neki od CPU-ova.
    const double pcs;  //!< Verovatnoća prelaza sa CPU na neki od sistemskih diskova.
    const double pcu;  //!< Verovatnoća prelaza sa CPU na neki od korisničkih diskova.
    const double psc;  //!< Verovatnoća prelaza sa sistemskog diska na neko od CPU-ova.
    const double psu;  //!< Verovatnoća prelaza sa sistemskog diska na neki od korisničkih diskova.
    const double puc;  //!< Verovatnoća prelaza sa korisničkog diska na neki od CPU-ova.
};

#endif  // _BUZEN_H_
