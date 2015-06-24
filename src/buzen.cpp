#include <buzen.h>
#include <cmath>
#include <iostream>

buzen::buzen(int number_of_discs, int number_of_processes)
    : m_number_of_discs(number_of_discs),
      m_number_of_processes(number_of_processes),
      x(),
      s(),
      G(number_of_processes + 1, 0.0),
      J(number_of_discs + 4, 0.0),
      U(),
      pcc(0.05),
      pcs(0.1),
      pcu(0.7 / number_of_discs),
      psc(0.2),
      psu(0.6 / number_of_discs),
      puc(0.5) {
    (void)pcu;
    (void)psu;
    s.push_back(0.008);
    s.push_back(0.008);
    s.push_back(0.02);
    s.push_back(0.02);
    for (auto i = 0; i < m_number_of_discs; i++) {
        s.push_back(0.025);
    }
    x.push_back(1.0);
    x.push_back(1.0);
    double xs = 2 * pcs * (s[2] / s[0]) * 1.0;
    x.push_back(xs);
    x.push_back(xs);
    double xu =
        (s[4] / s[0]) * ((1 - 2 * pcc - 4 * psc * pcs) / (2 * m_number_of_discs * puc)) * 1.0;
    for (auto i = 0; i < m_number_of_discs; i++) {
        x.push_back(xu);
    }
    G[0] = 1.0;
    for (auto j = 0; j < m_number_of_discs; j++) {
        for (auto i = 1; i <= m_number_of_processes; i++) {
            G[i] = G[i] + x[j] * G[i - 1];
        }
    }

    double g = G[m_number_of_processes - 1] / G[m_number_of_processes];
    U.push_back(x[0] * g);
    U.push_back(x[1] * g);
    U.push_back(x[2] * g);
    U.push_back(x[3] * g);
    for (auto i = 4u; i < x.size(); i++) {
        U.push_back(x[i] * g);
    }
    for (auto i = 0u; i < s.size(); i++) {
        X.push_back(U[i] / s[i]);
    }
    for (auto i = 0u; i < J.size(); i++) {
        for (auto j = 1; j < m_number_of_processes; j++) {
            J[i] =
                J[i] + ((G[m_number_of_processes - j] / G[m_number_of_processes]) * pow(x[i], j));
        }
    }
}

buzen::~buzen() {}

void buzen::print_results(std::ostream &out) const {
    statistics o = calculate_results();
    out << "Bjuzen analitički rezultati. Broj korisničkih diskova " << o.user_discs
        << ", broj procesa " << o.processes << "." << std::endl;
    out << "CPU 0" << std::endl;
    out << "\tIskorišćenost        : " << o.U_cpu0 << std::endl;
    out << "\tProtok               : " << o.X_cpu0 << std::endl;
    out << "\tProsečan broj poslova: " << o.J_cpu0 << std::endl;
    out << "CPU 1" << std::endl;
    out << "\tIskorišćenost        : " << o.U_cpu1 << std::endl;
    out << "\tProtok               : " << o.X_cpu1 << std::endl;
    out << "\tProsečan broj poslova: " << o.J_cpu1 << std::endl;
    out << "CPU proseci" << std::endl;
    out << "\tIskorišćenost        : " << o.U_cpu_avg << std::endl;
    out << "\tProtok               : " << o.X_cpu_avg << std::endl;
    out << "\tProsečan broj poslova: " << o.J_cpu_avg << std::endl;

    out << "Sistemski disk 0" << std::endl;
    out << "\tIskorišćenost        : " << o.U_sys0 << std::endl;
    out << "\tProtok               : " << o.X_sys0 << std::endl;
    out << "\tProsečan broj poslova: " << o.J_sys0 << std::endl;
    out << "Sistemski disk 1" << std::endl;
    out << "\tIskorišćenost        : " << o.U_sys1 << std::endl;
    out << "\tProtok               : " << o.X_sys1 << std::endl;
    out << "\tProsečan broj poslova: " << o.J_sys1 << std::endl;
    out << "Sistemski diskovi proseci" << std::endl;
    out << "\tIskorišćenost        : " << o.U_sys_avg << std::endl;
    out << "\tProtok               : " << o.X_sys_avg << std::endl;
    out << "\tProsečan broj poslova: " << o.J_sys_avg << std::endl;

    for (auto i = 0u; i < o.U_usr.size(); i++) {
        out << "Korisnički disk " << i << std::endl;
        out << "\tIskorišćenost        : " << o.U_usr[i] << std::endl;
        out << "\tProtok               : " << o.X_usr[i] << std::endl;
        out << "\tProsečan broj poslova: " << o.J_usr[i] << std::endl;
    }
    out << "Korisnički diskovi proseci" << std::endl;
    out << "\tIskorišćenost        : " << o.U_usr_avg << std::endl;
    out << "\tProtok               : " << o.X_usr_avg << std::endl;
    out << "\tProsečan broj poslova: " << o.J_usr_avg << std::endl;

    out << "Vreme odziva sistema: " << o.T << " ms." << std::endl;

    if (o.U_cpu_avg > o.U_sys_avg && o.U_cpu_avg > o.U_usr_avg) {
        out << "Kritičan resurs su procesori jer im je iskorišćenje najveće." << std::endl;
    }
    if (o.U_sys_avg > o.U_cpu_avg && o.U_sys_avg > o.U_usr_avg) {
        out << "Kritičan resurs su sistemski diskovi jer im je iskorišćenje najveće." << std::endl;
    }
    if (o.U_usr_avg > o.U_cpu_avg && o.U_usr_avg > o.U_sys_avg) {
        out << "Kritičan resurs su korisnički diskovi jer im je iskorišćenje najveće." << std::endl;
    }
}

statistics buzen::calculate_results() const {
    statistics o;

    o.user_discs = m_number_of_discs;
    o.processes = m_number_of_processes;

    o.U_cpu0 = U[0];
    o.U_cpu1 = U[1];
    o.U_cpu_avg = (o.U_cpu0 + o.U_cpu1) / 2;
    o.U_sys0 = U[2];
    o.U_sys1 = U[3];
    o.U_sys_avg = (o.U_sys0 + o.U_sys1) / 2;
    for (auto i = 4u; i < U.size(); i++) {
        o.U_usr.push_back(U[i]);
        o.U_usr_avg += U[i];
    }
    o.U_usr_avg = o.U_usr_avg / m_number_of_discs;

    o.X_cpu0 = X[0];
    o.X_cpu1 = X[1];
    o.X_cpu_avg = (o.X_cpu0 + o.X_cpu1) / 2;
    o.X_sys0 = X[2];
    o.X_sys1 = X[3];
    o.X_sys_avg = (o.X_sys0 + o.X_sys1) / 2;
    for (auto i = 4u; i < X.size(); i++) {
        o.X_usr.push_back(X[i]);
        o.X_usr_avg += X[i];
    }
    o.X_usr_avg = o.X_usr_avg / m_number_of_discs;

    o.J_cpu0 = J[0];
    o.J_cpu1 = J[1];
    o.J_cpu_avg = (o.J_cpu0 + o.J_cpu1) / 2;
    o.J_sys0 = J[2];
    o.J_sys1 = J[3];
    o.J_sys_avg = (o.J_sys0 + o.J_sys1) / 2;
    for (auto i = 4u; i < J.size(); i++) {
        o.J_usr.push_back(J[i]);
        o.J_usr_avg += J[i];
    }
    o.J_usr_avg = o.J_usr_avg / m_number_of_discs;
    o.T = J[0] * s[0] * 1000.0 / U[0];
    return o;
}
