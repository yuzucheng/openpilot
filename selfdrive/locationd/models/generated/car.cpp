#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                       Code generated with SymPy 1.12                       *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_6865464546625223) {
   out_6865464546625223[0] = delta_x[0] + nom_x[0];
   out_6865464546625223[1] = delta_x[1] + nom_x[1];
   out_6865464546625223[2] = delta_x[2] + nom_x[2];
   out_6865464546625223[3] = delta_x[3] + nom_x[3];
   out_6865464546625223[4] = delta_x[4] + nom_x[4];
   out_6865464546625223[5] = delta_x[5] + nom_x[5];
   out_6865464546625223[6] = delta_x[6] + nom_x[6];
   out_6865464546625223[7] = delta_x[7] + nom_x[7];
   out_6865464546625223[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2712695235619417681) {
   out_2712695235619417681[0] = -nom_x[0] + true_x[0];
   out_2712695235619417681[1] = -nom_x[1] + true_x[1];
   out_2712695235619417681[2] = -nom_x[2] + true_x[2];
   out_2712695235619417681[3] = -nom_x[3] + true_x[3];
   out_2712695235619417681[4] = -nom_x[4] + true_x[4];
   out_2712695235619417681[5] = -nom_x[5] + true_x[5];
   out_2712695235619417681[6] = -nom_x[6] + true_x[6];
   out_2712695235619417681[7] = -nom_x[7] + true_x[7];
   out_2712695235619417681[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_4683216400954849983) {
   out_4683216400954849983[0] = 1.0;
   out_4683216400954849983[1] = 0;
   out_4683216400954849983[2] = 0;
   out_4683216400954849983[3] = 0;
   out_4683216400954849983[4] = 0;
   out_4683216400954849983[5] = 0;
   out_4683216400954849983[6] = 0;
   out_4683216400954849983[7] = 0;
   out_4683216400954849983[8] = 0;
   out_4683216400954849983[9] = 0;
   out_4683216400954849983[10] = 1.0;
   out_4683216400954849983[11] = 0;
   out_4683216400954849983[12] = 0;
   out_4683216400954849983[13] = 0;
   out_4683216400954849983[14] = 0;
   out_4683216400954849983[15] = 0;
   out_4683216400954849983[16] = 0;
   out_4683216400954849983[17] = 0;
   out_4683216400954849983[18] = 0;
   out_4683216400954849983[19] = 0;
   out_4683216400954849983[20] = 1.0;
   out_4683216400954849983[21] = 0;
   out_4683216400954849983[22] = 0;
   out_4683216400954849983[23] = 0;
   out_4683216400954849983[24] = 0;
   out_4683216400954849983[25] = 0;
   out_4683216400954849983[26] = 0;
   out_4683216400954849983[27] = 0;
   out_4683216400954849983[28] = 0;
   out_4683216400954849983[29] = 0;
   out_4683216400954849983[30] = 1.0;
   out_4683216400954849983[31] = 0;
   out_4683216400954849983[32] = 0;
   out_4683216400954849983[33] = 0;
   out_4683216400954849983[34] = 0;
   out_4683216400954849983[35] = 0;
   out_4683216400954849983[36] = 0;
   out_4683216400954849983[37] = 0;
   out_4683216400954849983[38] = 0;
   out_4683216400954849983[39] = 0;
   out_4683216400954849983[40] = 1.0;
   out_4683216400954849983[41] = 0;
   out_4683216400954849983[42] = 0;
   out_4683216400954849983[43] = 0;
   out_4683216400954849983[44] = 0;
   out_4683216400954849983[45] = 0;
   out_4683216400954849983[46] = 0;
   out_4683216400954849983[47] = 0;
   out_4683216400954849983[48] = 0;
   out_4683216400954849983[49] = 0;
   out_4683216400954849983[50] = 1.0;
   out_4683216400954849983[51] = 0;
   out_4683216400954849983[52] = 0;
   out_4683216400954849983[53] = 0;
   out_4683216400954849983[54] = 0;
   out_4683216400954849983[55] = 0;
   out_4683216400954849983[56] = 0;
   out_4683216400954849983[57] = 0;
   out_4683216400954849983[58] = 0;
   out_4683216400954849983[59] = 0;
   out_4683216400954849983[60] = 1.0;
   out_4683216400954849983[61] = 0;
   out_4683216400954849983[62] = 0;
   out_4683216400954849983[63] = 0;
   out_4683216400954849983[64] = 0;
   out_4683216400954849983[65] = 0;
   out_4683216400954849983[66] = 0;
   out_4683216400954849983[67] = 0;
   out_4683216400954849983[68] = 0;
   out_4683216400954849983[69] = 0;
   out_4683216400954849983[70] = 1.0;
   out_4683216400954849983[71] = 0;
   out_4683216400954849983[72] = 0;
   out_4683216400954849983[73] = 0;
   out_4683216400954849983[74] = 0;
   out_4683216400954849983[75] = 0;
   out_4683216400954849983[76] = 0;
   out_4683216400954849983[77] = 0;
   out_4683216400954849983[78] = 0;
   out_4683216400954849983[79] = 0;
   out_4683216400954849983[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_5192486936404132338) {
   out_5192486936404132338[0] = state[0];
   out_5192486936404132338[1] = state[1];
   out_5192486936404132338[2] = state[2];
   out_5192486936404132338[3] = state[3];
   out_5192486936404132338[4] = state[4];
   out_5192486936404132338[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_5192486936404132338[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_5192486936404132338[7] = state[7];
   out_5192486936404132338[8] = state[8];
}
void F_fun(double *state, double dt, double *out_4060946651797247268) {
   out_4060946651797247268[0] = 1;
   out_4060946651797247268[1] = 0;
   out_4060946651797247268[2] = 0;
   out_4060946651797247268[3] = 0;
   out_4060946651797247268[4] = 0;
   out_4060946651797247268[5] = 0;
   out_4060946651797247268[6] = 0;
   out_4060946651797247268[7] = 0;
   out_4060946651797247268[8] = 0;
   out_4060946651797247268[9] = 0;
   out_4060946651797247268[10] = 1;
   out_4060946651797247268[11] = 0;
   out_4060946651797247268[12] = 0;
   out_4060946651797247268[13] = 0;
   out_4060946651797247268[14] = 0;
   out_4060946651797247268[15] = 0;
   out_4060946651797247268[16] = 0;
   out_4060946651797247268[17] = 0;
   out_4060946651797247268[18] = 0;
   out_4060946651797247268[19] = 0;
   out_4060946651797247268[20] = 1;
   out_4060946651797247268[21] = 0;
   out_4060946651797247268[22] = 0;
   out_4060946651797247268[23] = 0;
   out_4060946651797247268[24] = 0;
   out_4060946651797247268[25] = 0;
   out_4060946651797247268[26] = 0;
   out_4060946651797247268[27] = 0;
   out_4060946651797247268[28] = 0;
   out_4060946651797247268[29] = 0;
   out_4060946651797247268[30] = 1;
   out_4060946651797247268[31] = 0;
   out_4060946651797247268[32] = 0;
   out_4060946651797247268[33] = 0;
   out_4060946651797247268[34] = 0;
   out_4060946651797247268[35] = 0;
   out_4060946651797247268[36] = 0;
   out_4060946651797247268[37] = 0;
   out_4060946651797247268[38] = 0;
   out_4060946651797247268[39] = 0;
   out_4060946651797247268[40] = 1;
   out_4060946651797247268[41] = 0;
   out_4060946651797247268[42] = 0;
   out_4060946651797247268[43] = 0;
   out_4060946651797247268[44] = 0;
   out_4060946651797247268[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_4060946651797247268[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_4060946651797247268[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4060946651797247268[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_4060946651797247268[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_4060946651797247268[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_4060946651797247268[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_4060946651797247268[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_4060946651797247268[53] = -9.8000000000000007*dt;
   out_4060946651797247268[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_4060946651797247268[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_4060946651797247268[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4060946651797247268[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4060946651797247268[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_4060946651797247268[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_4060946651797247268[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_4060946651797247268[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_4060946651797247268[62] = 0;
   out_4060946651797247268[63] = 0;
   out_4060946651797247268[64] = 0;
   out_4060946651797247268[65] = 0;
   out_4060946651797247268[66] = 0;
   out_4060946651797247268[67] = 0;
   out_4060946651797247268[68] = 0;
   out_4060946651797247268[69] = 0;
   out_4060946651797247268[70] = 1;
   out_4060946651797247268[71] = 0;
   out_4060946651797247268[72] = 0;
   out_4060946651797247268[73] = 0;
   out_4060946651797247268[74] = 0;
   out_4060946651797247268[75] = 0;
   out_4060946651797247268[76] = 0;
   out_4060946651797247268[77] = 0;
   out_4060946651797247268[78] = 0;
   out_4060946651797247268[79] = 0;
   out_4060946651797247268[80] = 1;
}
void h_25(double *state, double *unused, double *out_2343901854903859463) {
   out_2343901854903859463[0] = state[6];
}
void H_25(double *state, double *unused, double *out_1869804445345075207) {
   out_1869804445345075207[0] = 0;
   out_1869804445345075207[1] = 0;
   out_1869804445345075207[2] = 0;
   out_1869804445345075207[3] = 0;
   out_1869804445345075207[4] = 0;
   out_1869804445345075207[5] = 0;
   out_1869804445345075207[6] = 1;
   out_1869804445345075207[7] = 0;
   out_1869804445345075207[8] = 0;
}
void h_24(double *state, double *unused, double *out_5252802887467904730) {
   out_5252802887467904730[0] = state[4];
   out_5252802887467904730[1] = state[5];
}
void H_24(double *state, double *unused, double *out_1688121153457247929) {
   out_1688121153457247929[0] = 0;
   out_1688121153457247929[1] = 0;
   out_1688121153457247929[2] = 0;
   out_1688121153457247929[3] = 0;
   out_1688121153457247929[4] = 1;
   out_1688121153457247929[5] = 0;
   out_1688121153457247929[6] = 0;
   out_1688121153457247929[7] = 0;
   out_1688121153457247929[8] = 0;
   out_1688121153457247929[9] = 0;
   out_1688121153457247929[10] = 0;
   out_1688121153457247929[11] = 0;
   out_1688121153457247929[12] = 0;
   out_1688121153457247929[13] = 0;
   out_1688121153457247929[14] = 1;
   out_1688121153457247929[15] = 0;
   out_1688121153457247929[16] = 0;
   out_1688121153457247929[17] = 0;
}
void h_30(double *state, double *unused, double *out_4426933371446491473) {
   out_4426933371446491473[0] = state[4];
}
void H_30(double *state, double *unused, double *out_648528513162173420) {
   out_648528513162173420[0] = 0;
   out_648528513162173420[1] = 0;
   out_648528513162173420[2] = 0;
   out_648528513162173420[3] = 0;
   out_648528513162173420[4] = 1;
   out_648528513162173420[5] = 0;
   out_648528513162173420[6] = 0;
   out_648528513162173420[7] = 0;
   out_648528513162173420[8] = 0;
}
void h_26(double *state, double *unused, double *out_9018137434602214498) {
   out_9018137434602214498[0] = state[7];
}
void H_26(double *state, double *unused, double *out_1434721524415725394) {
   out_1434721524415725394[0] = 0;
   out_1434721524415725394[1] = 0;
   out_1434721524415725394[2] = 0;
   out_1434721524415725394[3] = 0;
   out_1434721524415725394[4] = 0;
   out_1434721524415725394[5] = 0;
   out_1434721524415725394[6] = 0;
   out_1434721524415725394[7] = 1;
   out_1434721524415725394[8] = 0;
}
void h_27(double *state, double *unused, double *out_2997965380982171100) {
   out_2997965380982171100[0] = state[3];
}
void H_27(double *state, double *unused, double *out_2872122584346116637) {
   out_2872122584346116637[0] = 0;
   out_2872122584346116637[1] = 0;
   out_2872122584346116637[2] = 0;
   out_2872122584346116637[3] = 1;
   out_2872122584346116637[4] = 0;
   out_2872122584346116637[5] = 0;
   out_2872122584346116637[6] = 0;
   out_2872122584346116637[7] = 0;
   out_2872122584346116637[8] = 0;
}
void h_29(double *state, double *unused, double *out_5370443224348153908) {
   out_5370443224348153908[0] = state[1];
}
void H_29(double *state, double *unused, double *out_1158759857476565604) {
   out_1158759857476565604[0] = 0;
   out_1158759857476565604[1] = 1;
   out_1158759857476565604[2] = 0;
   out_1158759857476565604[3] = 0;
   out_1158759857476565604[4] = 0;
   out_1158759857476565604[5] = 0;
   out_1158759857476565604[6] = 0;
   out_1158759857476565604[7] = 0;
   out_1158759857476565604[8] = 0;
}
void h_28(double *state, double *unused, double *out_5798344083283826040) {
   out_5798344083283826040[0] = state[0];
}
void H_28(double *state, double *unused, double *out_3923639159592964970) {
   out_3923639159592964970[0] = 1;
   out_3923639159592964970[1] = 0;
   out_3923639159592964970[2] = 0;
   out_3923639159592964970[3] = 0;
   out_3923639159592964970[4] = 0;
   out_3923639159592964970[5] = 0;
   out_3923639159592964970[6] = 0;
   out_3923639159592964970[7] = 0;
   out_3923639159592964970[8] = 0;
}
void h_31(double *state, double *unused, double *out_6549007256014133578) {
   out_6549007256014133578[0] = state[8];
}
void H_31(double *state, double *unused, double *out_5206870805166742046) {
   out_5206870805166742046[0] = 0;
   out_5206870805166742046[1] = 0;
   out_5206870805166742046[2] = 0;
   out_5206870805166742046[3] = 0;
   out_5206870805166742046[4] = 0;
   out_5206870805166742046[5] = 0;
   out_5206870805166742046[6] = 0;
   out_5206870805166742046[7] = 0;
   out_5206870805166742046[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_6865464546625223) {
  err_fun(nom_x, delta_x, out_6865464546625223);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2712695235619417681) {
  inv_err_fun(nom_x, true_x, out_2712695235619417681);
}
void car_H_mod_fun(double *state, double *out_4683216400954849983) {
  H_mod_fun(state, out_4683216400954849983);
}
void car_f_fun(double *state, double dt, double *out_5192486936404132338) {
  f_fun(state,  dt, out_5192486936404132338);
}
void car_F_fun(double *state, double dt, double *out_4060946651797247268) {
  F_fun(state,  dt, out_4060946651797247268);
}
void car_h_25(double *state, double *unused, double *out_2343901854903859463) {
  h_25(state, unused, out_2343901854903859463);
}
void car_H_25(double *state, double *unused, double *out_1869804445345075207) {
  H_25(state, unused, out_1869804445345075207);
}
void car_h_24(double *state, double *unused, double *out_5252802887467904730) {
  h_24(state, unused, out_5252802887467904730);
}
void car_H_24(double *state, double *unused, double *out_1688121153457247929) {
  H_24(state, unused, out_1688121153457247929);
}
void car_h_30(double *state, double *unused, double *out_4426933371446491473) {
  h_30(state, unused, out_4426933371446491473);
}
void car_H_30(double *state, double *unused, double *out_648528513162173420) {
  H_30(state, unused, out_648528513162173420);
}
void car_h_26(double *state, double *unused, double *out_9018137434602214498) {
  h_26(state, unused, out_9018137434602214498);
}
void car_H_26(double *state, double *unused, double *out_1434721524415725394) {
  H_26(state, unused, out_1434721524415725394);
}
void car_h_27(double *state, double *unused, double *out_2997965380982171100) {
  h_27(state, unused, out_2997965380982171100);
}
void car_H_27(double *state, double *unused, double *out_2872122584346116637) {
  H_27(state, unused, out_2872122584346116637);
}
void car_h_29(double *state, double *unused, double *out_5370443224348153908) {
  h_29(state, unused, out_5370443224348153908);
}
void car_H_29(double *state, double *unused, double *out_1158759857476565604) {
  H_29(state, unused, out_1158759857476565604);
}
void car_h_28(double *state, double *unused, double *out_5798344083283826040) {
  h_28(state, unused, out_5798344083283826040);
}
void car_H_28(double *state, double *unused, double *out_3923639159592964970) {
  H_28(state, unused, out_3923639159592964970);
}
void car_h_31(double *state, double *unused, double *out_6549007256014133578) {
  h_31(state, unused, out_6549007256014133578);
}
void car_H_31(double *state, double *unused, double *out_5206870805166742046) {
  H_31(state, unused, out_5206870805166742046);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
