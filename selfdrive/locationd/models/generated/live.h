#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void live_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_9(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_12(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_35(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_32(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_update_33(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void live_H(double *in_vec, double *out_742148225835129227);
void live_err_fun(double *nom_x, double *delta_x, double *out_4932625361128218531);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_5091719650809670856);
void live_H_mod_fun(double *state, double *out_1669533905452122654);
void live_f_fun(double *state, double dt, double *out_452407580724599486);
void live_F_fun(double *state, double dt, double *out_8363446884750752161);
void live_h_4(double *state, double *unused, double *out_1041271705608517819);
void live_H_4(double *state, double *unused, double *out_836312682658128159);
void live_h_9(double *state, double *unused, double *out_6978894764831227922);
void live_H_9(double *state, double *unused, double *out_8123531617922575629);
void live_h_10(double *state, double *unused, double *out_8669447785041758723);
void live_H_10(double *state, double *unused, double *out_2890494636352708251);
void live_h_12(double *state, double *unused, double *out_117476402947836119);
void live_H_12(double *state, double *unused, double *out_8503440996340578651);
void live_h_35(double *state, double *unused, double *out_3254898062564875699);
void live_H_35(double *state, double *unused, double *out_7197740045043959256);
void live_h_32(double *state, double *unused, double *out_374327625260950500);
void live_H_32(double *state, double *unused, double *out_4414081880625541663);
void live_h_13(double *state, double *unused, double *out_7213944346996647805);
void live_H_13(double *state, double *unused, double *out_8154555791718865638);
void live_h_14(double *state, double *unused, double *out_6978894764831227922);
void live_H_14(double *state, double *unused, double *out_8123531617922575629);
void live_h_33(double *state, double *unused, double *out_7792370730077755811);
void live_H_33(double *state, double *unused, double *out_4047183040405101652);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}