#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_7574567282840688442);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2438187676425805374);
void car_H_mod_fun(double *state, double *out_9042486626090707833);
void car_f_fun(double *state, double dt, double *out_4710937725663970664);
void car_F_fun(double *state, double dt, double *out_3422292361914265762);
void car_h_25(double *state, double *unused, double *out_7516217026234525530);
void car_H_25(double *state, double *unused, double *out_681753427287297713);
void car_h_24(double *state, double *unused, double *out_1686163153073000199);
void car_H_24(double *state, double *unused, double *out_4239679026089620849);
void car_h_30(double *state, double *unused, double *out_7791411088519031419);
void car_H_30(double *state, double *unused, double *out_3200086385794546340);
void car_h_26(double *state, double *unused, double *out_8050927664381822546);
void car_H_26(double *state, double *unused, double *out_3059749891586758511);
void car_h_27(double *state, double *unused, double *out_6572707173332863095);
void car_H_27(double *state, double *unused, double *out_1025323073994121429);
void car_h_29(double *state, double *unused, double *out_6991832857344348062);
void car_H_29(double *state, double *unused, double *out_3710317730108938524);
void car_h_28(double *state, double *unused, double *out_6872363632715842876);
void car_H_28(double *state, double *unused, double *out_1372081286960592050);
void car_h_31(double *state, double *unused, double *out_2290649570756484202);
void car_H_31(double *state, double *unused, double *out_3685957993820109987);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}