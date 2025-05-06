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
void err_fun(double *nom_x, double *delta_x, double *out_7574567282840688442) {
   out_7574567282840688442[0] = delta_x[0] + nom_x[0];
   out_7574567282840688442[1] = delta_x[1] + nom_x[1];
   out_7574567282840688442[2] = delta_x[2] + nom_x[2];
   out_7574567282840688442[3] = delta_x[3] + nom_x[3];
   out_7574567282840688442[4] = delta_x[4] + nom_x[4];
   out_7574567282840688442[5] = delta_x[5] + nom_x[5];
   out_7574567282840688442[6] = delta_x[6] + nom_x[6];
   out_7574567282840688442[7] = delta_x[7] + nom_x[7];
   out_7574567282840688442[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_2438187676425805374) {
   out_2438187676425805374[0] = -nom_x[0] + true_x[0];
   out_2438187676425805374[1] = -nom_x[1] + true_x[1];
   out_2438187676425805374[2] = -nom_x[2] + true_x[2];
   out_2438187676425805374[3] = -nom_x[3] + true_x[3];
   out_2438187676425805374[4] = -nom_x[4] + true_x[4];
   out_2438187676425805374[5] = -nom_x[5] + true_x[5];
   out_2438187676425805374[6] = -nom_x[6] + true_x[6];
   out_2438187676425805374[7] = -nom_x[7] + true_x[7];
   out_2438187676425805374[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_9042486626090707833) {
   out_9042486626090707833[0] = 1.0;
   out_9042486626090707833[1] = 0;
   out_9042486626090707833[2] = 0;
   out_9042486626090707833[3] = 0;
   out_9042486626090707833[4] = 0;
   out_9042486626090707833[5] = 0;
   out_9042486626090707833[6] = 0;
   out_9042486626090707833[7] = 0;
   out_9042486626090707833[8] = 0;
   out_9042486626090707833[9] = 0;
   out_9042486626090707833[10] = 1.0;
   out_9042486626090707833[11] = 0;
   out_9042486626090707833[12] = 0;
   out_9042486626090707833[13] = 0;
   out_9042486626090707833[14] = 0;
   out_9042486626090707833[15] = 0;
   out_9042486626090707833[16] = 0;
   out_9042486626090707833[17] = 0;
   out_9042486626090707833[18] = 0;
   out_9042486626090707833[19] = 0;
   out_9042486626090707833[20] = 1.0;
   out_9042486626090707833[21] = 0;
   out_9042486626090707833[22] = 0;
   out_9042486626090707833[23] = 0;
   out_9042486626090707833[24] = 0;
   out_9042486626090707833[25] = 0;
   out_9042486626090707833[26] = 0;
   out_9042486626090707833[27] = 0;
   out_9042486626090707833[28] = 0;
   out_9042486626090707833[29] = 0;
   out_9042486626090707833[30] = 1.0;
   out_9042486626090707833[31] = 0;
   out_9042486626090707833[32] = 0;
   out_9042486626090707833[33] = 0;
   out_9042486626090707833[34] = 0;
   out_9042486626090707833[35] = 0;
   out_9042486626090707833[36] = 0;
   out_9042486626090707833[37] = 0;
   out_9042486626090707833[38] = 0;
   out_9042486626090707833[39] = 0;
   out_9042486626090707833[40] = 1.0;
   out_9042486626090707833[41] = 0;
   out_9042486626090707833[42] = 0;
   out_9042486626090707833[43] = 0;
   out_9042486626090707833[44] = 0;
   out_9042486626090707833[45] = 0;
   out_9042486626090707833[46] = 0;
   out_9042486626090707833[47] = 0;
   out_9042486626090707833[48] = 0;
   out_9042486626090707833[49] = 0;
   out_9042486626090707833[50] = 1.0;
   out_9042486626090707833[51] = 0;
   out_9042486626090707833[52] = 0;
   out_9042486626090707833[53] = 0;
   out_9042486626090707833[54] = 0;
   out_9042486626090707833[55] = 0;
   out_9042486626090707833[56] = 0;
   out_9042486626090707833[57] = 0;
   out_9042486626090707833[58] = 0;
   out_9042486626090707833[59] = 0;
   out_9042486626090707833[60] = 1.0;
   out_9042486626090707833[61] = 0;
   out_9042486626090707833[62] = 0;
   out_9042486626090707833[63] = 0;
   out_9042486626090707833[64] = 0;
   out_9042486626090707833[65] = 0;
   out_9042486626090707833[66] = 0;
   out_9042486626090707833[67] = 0;
   out_9042486626090707833[68] = 0;
   out_9042486626090707833[69] = 0;
   out_9042486626090707833[70] = 1.0;
   out_9042486626090707833[71] = 0;
   out_9042486626090707833[72] = 0;
   out_9042486626090707833[73] = 0;
   out_9042486626090707833[74] = 0;
   out_9042486626090707833[75] = 0;
   out_9042486626090707833[76] = 0;
   out_9042486626090707833[77] = 0;
   out_9042486626090707833[78] = 0;
   out_9042486626090707833[79] = 0;
   out_9042486626090707833[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_4710937725663970664) {
   out_4710937725663970664[0] = state[0];
   out_4710937725663970664[1] = state[1];
   out_4710937725663970664[2] = state[2];
   out_4710937725663970664[3] = state[3];
   out_4710937725663970664[4] = state[4];
   out_4710937725663970664[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8000000000000007*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_4710937725663970664[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_4710937725663970664[7] = state[7];
   out_4710937725663970664[8] = state[8];
}
void F_fun(double *state, double dt, double *out_3422292361914265762) {
   out_3422292361914265762[0] = 1;
   out_3422292361914265762[1] = 0;
   out_3422292361914265762[2] = 0;
   out_3422292361914265762[3] = 0;
   out_3422292361914265762[4] = 0;
   out_3422292361914265762[5] = 0;
   out_3422292361914265762[6] = 0;
   out_3422292361914265762[7] = 0;
   out_3422292361914265762[8] = 0;
   out_3422292361914265762[9] = 0;
   out_3422292361914265762[10] = 1;
   out_3422292361914265762[11] = 0;
   out_3422292361914265762[12] = 0;
   out_3422292361914265762[13] = 0;
   out_3422292361914265762[14] = 0;
   out_3422292361914265762[15] = 0;
   out_3422292361914265762[16] = 0;
   out_3422292361914265762[17] = 0;
   out_3422292361914265762[18] = 0;
   out_3422292361914265762[19] = 0;
   out_3422292361914265762[20] = 1;
   out_3422292361914265762[21] = 0;
   out_3422292361914265762[22] = 0;
   out_3422292361914265762[23] = 0;
   out_3422292361914265762[24] = 0;
   out_3422292361914265762[25] = 0;
   out_3422292361914265762[26] = 0;
   out_3422292361914265762[27] = 0;
   out_3422292361914265762[28] = 0;
   out_3422292361914265762[29] = 0;
   out_3422292361914265762[30] = 1;
   out_3422292361914265762[31] = 0;
   out_3422292361914265762[32] = 0;
   out_3422292361914265762[33] = 0;
   out_3422292361914265762[34] = 0;
   out_3422292361914265762[35] = 0;
   out_3422292361914265762[36] = 0;
   out_3422292361914265762[37] = 0;
   out_3422292361914265762[38] = 0;
   out_3422292361914265762[39] = 0;
   out_3422292361914265762[40] = 1;
   out_3422292361914265762[41] = 0;
   out_3422292361914265762[42] = 0;
   out_3422292361914265762[43] = 0;
   out_3422292361914265762[44] = 0;
   out_3422292361914265762[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_3422292361914265762[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_3422292361914265762[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_3422292361914265762[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_3422292361914265762[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_3422292361914265762[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_3422292361914265762[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_3422292361914265762[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_3422292361914265762[53] = -9.8000000000000007*dt;
   out_3422292361914265762[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_3422292361914265762[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_3422292361914265762[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3422292361914265762[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3422292361914265762[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_3422292361914265762[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_3422292361914265762[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_3422292361914265762[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_3422292361914265762[62] = 0;
   out_3422292361914265762[63] = 0;
   out_3422292361914265762[64] = 0;
   out_3422292361914265762[65] = 0;
   out_3422292361914265762[66] = 0;
   out_3422292361914265762[67] = 0;
   out_3422292361914265762[68] = 0;
   out_3422292361914265762[69] = 0;
   out_3422292361914265762[70] = 1;
   out_3422292361914265762[71] = 0;
   out_3422292361914265762[72] = 0;
   out_3422292361914265762[73] = 0;
   out_3422292361914265762[74] = 0;
   out_3422292361914265762[75] = 0;
   out_3422292361914265762[76] = 0;
   out_3422292361914265762[77] = 0;
   out_3422292361914265762[78] = 0;
   out_3422292361914265762[79] = 0;
   out_3422292361914265762[80] = 1;
}
void h_25(double *state, double *unused, double *out_7516217026234525530) {
   out_7516217026234525530[0] = state[6];
}
void H_25(double *state, double *unused, double *out_681753427287297713) {
   out_681753427287297713[0] = 0;
   out_681753427287297713[1] = 0;
   out_681753427287297713[2] = 0;
   out_681753427287297713[3] = 0;
   out_681753427287297713[4] = 0;
   out_681753427287297713[5] = 0;
   out_681753427287297713[6] = 1;
   out_681753427287297713[7] = 0;
   out_681753427287297713[8] = 0;
}
void h_24(double *state, double *unused, double *out_1686163153073000199) {
   out_1686163153073000199[0] = state[4];
   out_1686163153073000199[1] = state[5];
}
void H_24(double *state, double *unused, double *out_4239679026089620849) {
   out_4239679026089620849[0] = 0;
   out_4239679026089620849[1] = 0;
   out_4239679026089620849[2] = 0;
   out_4239679026089620849[3] = 0;
   out_4239679026089620849[4] = 1;
   out_4239679026089620849[5] = 0;
   out_4239679026089620849[6] = 0;
   out_4239679026089620849[7] = 0;
   out_4239679026089620849[8] = 0;
   out_4239679026089620849[9] = 0;
   out_4239679026089620849[10] = 0;
   out_4239679026089620849[11] = 0;
   out_4239679026089620849[12] = 0;
   out_4239679026089620849[13] = 0;
   out_4239679026089620849[14] = 1;
   out_4239679026089620849[15] = 0;
   out_4239679026089620849[16] = 0;
   out_4239679026089620849[17] = 0;
}
void h_30(double *state, double *unused, double *out_7791411088519031419) {
   out_7791411088519031419[0] = state[4];
}
void H_30(double *state, double *unused, double *out_3200086385794546340) {
   out_3200086385794546340[0] = 0;
   out_3200086385794546340[1] = 0;
   out_3200086385794546340[2] = 0;
   out_3200086385794546340[3] = 0;
   out_3200086385794546340[4] = 1;
   out_3200086385794546340[5] = 0;
   out_3200086385794546340[6] = 0;
   out_3200086385794546340[7] = 0;
   out_3200086385794546340[8] = 0;
}
void h_26(double *state, double *unused, double *out_8050927664381822546) {
   out_8050927664381822546[0] = state[7];
}
void H_26(double *state, double *unused, double *out_3059749891586758511) {
   out_3059749891586758511[0] = 0;
   out_3059749891586758511[1] = 0;
   out_3059749891586758511[2] = 0;
   out_3059749891586758511[3] = 0;
   out_3059749891586758511[4] = 0;
   out_3059749891586758511[5] = 0;
   out_3059749891586758511[6] = 0;
   out_3059749891586758511[7] = 1;
   out_3059749891586758511[8] = 0;
}
void h_27(double *state, double *unused, double *out_6572707173332863095) {
   out_6572707173332863095[0] = state[3];
}
void H_27(double *state, double *unused, double *out_1025323073994121429) {
   out_1025323073994121429[0] = 0;
   out_1025323073994121429[1] = 0;
   out_1025323073994121429[2] = 0;
   out_1025323073994121429[3] = 1;
   out_1025323073994121429[4] = 0;
   out_1025323073994121429[5] = 0;
   out_1025323073994121429[6] = 0;
   out_1025323073994121429[7] = 0;
   out_1025323073994121429[8] = 0;
}
void h_29(double *state, double *unused, double *out_6991832857344348062) {
   out_6991832857344348062[0] = state[1];
}
void H_29(double *state, double *unused, double *out_3710317730108938524) {
   out_3710317730108938524[0] = 0;
   out_3710317730108938524[1] = 1;
   out_3710317730108938524[2] = 0;
   out_3710317730108938524[3] = 0;
   out_3710317730108938524[4] = 0;
   out_3710317730108938524[5] = 0;
   out_3710317730108938524[6] = 0;
   out_3710317730108938524[7] = 0;
   out_3710317730108938524[8] = 0;
}
void h_28(double *state, double *unused, double *out_6872363632715842876) {
   out_6872363632715842876[0] = state[0];
}
void H_28(double *state, double *unused, double *out_1372081286960592050) {
   out_1372081286960592050[0] = 1;
   out_1372081286960592050[1] = 0;
   out_1372081286960592050[2] = 0;
   out_1372081286960592050[3] = 0;
   out_1372081286960592050[4] = 0;
   out_1372081286960592050[5] = 0;
   out_1372081286960592050[6] = 0;
   out_1372081286960592050[7] = 0;
   out_1372081286960592050[8] = 0;
}
void h_31(double *state, double *unused, double *out_2290649570756484202) {
   out_2290649570756484202[0] = state[8];
}
void H_31(double *state, double *unused, double *out_3685957993820109987) {
   out_3685957993820109987[0] = 0;
   out_3685957993820109987[1] = 0;
   out_3685957993820109987[2] = 0;
   out_3685957993820109987[3] = 0;
   out_3685957993820109987[4] = 0;
   out_3685957993820109987[5] = 0;
   out_3685957993820109987[6] = 0;
   out_3685957993820109987[7] = 0;
   out_3685957993820109987[8] = 1;
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
void car_err_fun(double *nom_x, double *delta_x, double *out_7574567282840688442) {
  err_fun(nom_x, delta_x, out_7574567282840688442);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_2438187676425805374) {
  inv_err_fun(nom_x, true_x, out_2438187676425805374);
}
void car_H_mod_fun(double *state, double *out_9042486626090707833) {
  H_mod_fun(state, out_9042486626090707833);
}
void car_f_fun(double *state, double dt, double *out_4710937725663970664) {
  f_fun(state,  dt, out_4710937725663970664);
}
void car_F_fun(double *state, double dt, double *out_3422292361914265762) {
  F_fun(state,  dt, out_3422292361914265762);
}
void car_h_25(double *state, double *unused, double *out_7516217026234525530) {
  h_25(state, unused, out_7516217026234525530);
}
void car_H_25(double *state, double *unused, double *out_681753427287297713) {
  H_25(state, unused, out_681753427287297713);
}
void car_h_24(double *state, double *unused, double *out_1686163153073000199) {
  h_24(state, unused, out_1686163153073000199);
}
void car_H_24(double *state, double *unused, double *out_4239679026089620849) {
  H_24(state, unused, out_4239679026089620849);
}
void car_h_30(double *state, double *unused, double *out_7791411088519031419) {
  h_30(state, unused, out_7791411088519031419);
}
void car_H_30(double *state, double *unused, double *out_3200086385794546340) {
  H_30(state, unused, out_3200086385794546340);
}
void car_h_26(double *state, double *unused, double *out_8050927664381822546) {
  h_26(state, unused, out_8050927664381822546);
}
void car_H_26(double *state, double *unused, double *out_3059749891586758511) {
  H_26(state, unused, out_3059749891586758511);
}
void car_h_27(double *state, double *unused, double *out_6572707173332863095) {
  h_27(state, unused, out_6572707173332863095);
}
void car_H_27(double *state, double *unused, double *out_1025323073994121429) {
  H_27(state, unused, out_1025323073994121429);
}
void car_h_29(double *state, double *unused, double *out_6991832857344348062) {
  h_29(state, unused, out_6991832857344348062);
}
void car_H_29(double *state, double *unused, double *out_3710317730108938524) {
  H_29(state, unused, out_3710317730108938524);
}
void car_h_28(double *state, double *unused, double *out_6872363632715842876) {
  h_28(state, unused, out_6872363632715842876);
}
void car_H_28(double *state, double *unused, double *out_1372081286960592050) {
  H_28(state, unused, out_1372081286960592050);
}
void car_h_31(double *state, double *unused, double *out_2290649570756484202) {
  h_31(state, unused, out_2290649570756484202);
}
void car_H_31(double *state, double *unused, double *out_3685957993820109987) {
  H_31(state, unused, out_3685957993820109987);
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
