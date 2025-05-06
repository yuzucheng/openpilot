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
void live_H(double *in_vec, double *out_1345249788758015084);
void live_err_fun(double *nom_x, double *delta_x, double *out_7719634259818376092);
void live_inv_err_fun(double *nom_x, double *true_x, double *out_6741930509538719735);
void live_H_mod_fun(double *state, double *out_2627288097967823166);
void live_f_fun(double *state, double dt, double *out_8742394592132095515);
void live_F_fun(double *state, double dt, double *out_569786763108838867);
void live_h_4(double *state, double *unused, double *out_5603552778612686787);
void live_H_4(double *state, double *unused, double *out_1767891489218198864);
void live_h_9(double *state, double *unused, double *out_7135624971026263438);
void live_H_9(double *state, double *unused, double *out_9055110424482646334);
void live_h_10(double *state, double *unused, double *out_7405996803063147821);
void live_H_10(double *state, double *unused, double *out_4449009372014597764);
void live_h_12(double *state, double *unused, double *out_5169321085057836161);
void live_H_12(double *state, double *unused, double *out_6787347897250160659);
void live_h_35(double *state, double *unused, double *out_4323656818132815336);
void live_H_35(double *state, double *unused, double *out_8913833144134377248);
void live_h_32(double *state, double *unused, double *out_3228059702760003102);
void live_H_32(double *state, double *unused, double *out_6646429506726719872);
void live_h_13(double *state, double *unused, double *out_6305347991969589285);
void live_H_13(double *state, double *unused, double *out_3095891855353063491);
void live_h_14(double *state, double *unused, double *out_7135624971026263438);
void live_H_14(double *state, double *unused, double *out_9055110424482646334);
void live_h_33(double *state, double *unused, double *out_6801844409662643527);
void live_H_33(double *state, double *unused, double *out_8285110551229663844);
void live_predict(double *in_x, double *in_P, double *in_Q, double dt);
}