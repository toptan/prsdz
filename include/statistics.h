#ifndef _STATISTICS_H_
#define _STATISTICS_H_

/// \brief Struktura koja sadrži sve parametre koje je potrebno odrediti.
struct statistics {
    statistics()
        : T(0.0),
          U_cpu0(0.0),
          U_cpu1(0.0),
          U_cpu_avg(0.0),
          U_sys0(0.0),
          U_sys1(0.0),
          U_sys_avg(0.0),
          U_usr(),
          U_usr_avg(0.0),
          X_cpu0(0.0),
          X_cpu1(0.0),
          X_cpu_avg(0.0),
          X_sys0(0.0),
          X_sys1(0.0),
          X_sys_avg(0.0),
          X_usr(),
          X_usr_avg(0.0),
          J_cpu0(0.0),
          J_cpu1(0.0),
          J_cpu_avg(0.0),
          J_sys0(0.0),
          J_sys1(0.0),
          J_sys_avg(0.0),
          J_usr(),
          J_usr_avg(0.0) {}

    double T;
    double U_cpu0;
    double U_cpu1;
    double U_cpu_avg;
    double U_sys0;
    double U_sys1;
    double U_sys_avg;
    std::vector<double> U_usr;
    double U_usr_avg;
    double X_cpu0;
    double X_cpu1;
    double X_cpu_avg;
    double X_sys0;
    double X_sys1;
    double X_sys_avg;
    std::vector<double> X_usr;
    double X_usr_avg;
    double J_cpu0;
    double J_cpu1;
    double J_cpu_avg;
    double J_sys0;
    double J_sys1;
    double J_sys_avg;
    std::vector<double> J_usr;
    double J_usr_avg;
};

#endif  // _STATISTICS_H_
