#include <buzen.h>
#include <cmath>
#include <iostream>

buzen::buzen(int number_of_discs, int number_of_processes)
    : m_number_of_discs(number_of_discs),
      m_number_of_processes(number_of_processes),
      x(),
      s(),
      G(),
      J(m_number_of_discs + 4, 0.0),
      U(),
      pcc(0.05),
      pcs(0.1),
      pcu(0.7 / number_of_discs),
      psc(0.2),
      psu(0.6 / number_of_discs),
      puc(0.5) {
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
    double xu = (s[4] / s[0]) * ((1 - 2 * pcc - 4 * psc * pcs) / (m_number_of_discs * puc)) * 1.0;
    for (auto i = 0; i < m_number_of_discs; i++) {
        x.push_back(xu);
    }
    G.push_back(1.0);
    for (auto i = 1; i <= m_number_of_processes; i++) {
        G.push_back(0.0);
    }
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
    for (auto i = 4; i < x.size(); i++) {
        U.push_back(x[i] * g);
    }
    for (auto i = 0; i < s.size(); i++) {
        X.push_back(U[i] / s[i]);
    }
    for (auto j = 1; j < m_number_of_processes; j++) {
        for (auto i = 0; i < J.size(); i++) {
            J[i] = J[i] + (G[m_number_of_processes - j] / G[m_number_of_processes]) * pow(x[i], j);
        }
    }
}

buzen::~buzen() {}

void buzen::print_results() const {
    for (auto temp: J) {
        std::cout << temp << std::endl;
    }
}
