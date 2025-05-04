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
void car_err_fun(double *nom_x, double *delta_x, double *out_6865464546625223);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2712695235619417681);
void car_H_mod_fun(double *state, double *out_4683216400954849983);
void car_f_fun(double *state, double dt, double *out_5192486936404132338);
void car_F_fun(double *state, double dt, double *out_4060946651797247268);
void car_h_25(double *state, double *unused, double *out_2343901854903859463);
void car_H_25(double *state, double *unused, double *out_1869804445345075207);
void car_h_24(double *state, double *unused, double *out_5252802887467904730);
void car_H_24(double *state, double *unused, double *out_1688121153457247929);
void car_h_30(double *state, double *unused, double *out_4426933371446491473);
void car_H_30(double *state, double *unused, double *out_648528513162173420);
void car_h_26(double *state, double *unused, double *out_9018137434602214498);
void car_H_26(double *state, double *unused, double *out_1434721524415725394);
void car_h_27(double *state, double *unused, double *out_2997965380982171100);
void car_H_27(double *state, double *unused, double *out_2872122584346116637);
void car_h_29(double *state, double *unused, double *out_5370443224348153908);
void car_H_29(double *state, double *unused, double *out_1158759857476565604);
void car_h_28(double *state, double *unused, double *out_5798344083283826040);
void car_H_28(double *state, double *unused, double *out_3923639159592964970);
void car_h_31(double *state, double *unused, double *out_6549007256014133578);
void car_H_31(double *state, double *unused, double *out_5206870805166742046);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}