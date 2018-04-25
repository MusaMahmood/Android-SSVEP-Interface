//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: tf_timedomain_preprocess.cpp
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 25-Apr-2018 13:54:32
//

// Include Files
#include "rt_nonfinite.h"
#include "tf_timedomain_preprocess.h"

// Function Declarations
static void filter(const double x_data[], const int x_size[1], const double zi[3],
                   double y_data[], int y_size[1]);
static void filtfilt(const double x_in_data[], const int x_in_size[1], double
y_out_data[], int y_out_size[2]);
static void flipud(double x_data[], int x_size[1]);
static void rescale_minmax(const double X_data[], const int X_size[2], double
Y_data[], int Y_size[2]);

// Function Definitions

//
// Arguments    : const double x_data[]
//                const int x_size[1]
//                const double zi[3]
//                double y_data[]
//                int y_size[1]
// Return Type  : void
//
static void filter(const double x_data[], const int x_size[1], const double zi[3],
                   double y_data[], int y_size[1])
{
  int nx;
  int niccp;
  int k;
  int j;
  static const double dv0[4] = { 0.90431873448479, -2.7129562034543691,
                                 2.7129562034543691, -0.90431873448479 };

  double as;
  static const double dv1[4] = { 1.0, -2.7990220146733029, 2.6177355009222332,
                                 -0.81779236028278 };

  y_size[0] = (short)x_size[0];
  nx = x_size[0];
  niccp = x_size[0];
  if (!(niccp < 3)) {
    niccp = 3;
  }

  for (k = 0; k + 1 <= niccp; k++) {
    y_data[k] = zi[k];
  }

  while (niccp + 1 <= nx) {
    y_data[niccp] = 0.0;
    niccp++;
  }

  for (k = 1; k <= nx; k++) {
    niccp = (nx - k) + 1;
    if (!(niccp < 4)) {
      niccp = 4;
    }

    for (j = -1; j + 2 <= niccp; j++) {
      y_data[k + j] += x_data[k - 1] * dv0[j + 1];
    }

    niccp = nx - k;
    if (!(niccp < 3)) {
      niccp = 3;
    }

    as = -y_data[k - 1];
    for (j = 1; j <= niccp; j++) {
      y_data[(k + j) - 1] += as * dv1[j];
    }
  }
}

//
// Arguments    : const double x_in_data[]
//                const int x_in_size[1]
//                double y_out_data[]
//                int y_out_size[2]
// Return Type  : void
//
static void filtfilt(const double x_in_data[], const int x_in_size[1], double
y_out_data[], int y_out_size[2])
{
  double d0;
  double d1;
  int i1;
  int loop_ub;
  double y_data[530];
  int y_size[1];
  double a[3];
  static const double b_a[3] = { -0.90431873448472977, 1.8086374689694706,
                                 -0.90431873448474087 };

  double b_y_data[530];
  int b_y_size[1];
  int c_y_size[1];
  int tmp_data[512];
  d0 = 2.0 * x_in_data[0];
  d1 = 2.0 * x_in_data[x_in_size[0] - 1];
  for (i1 = 0; i1 < 9; i1++) {
    y_data[i1] = d0 - x_in_data[9 - i1];
  }

  loop_ub = x_in_size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    y_data[i1 + 9] = x_in_data[i1];
  }

  for (i1 = 0; i1 < 9; i1++) {
    y_data[(i1 + x_in_size[0]) + 9] = d1 - x_in_data[(x_in_size[0] - i1) - 2];
  }

  for (i1 = 0; i1 < 3; i1++) {
    a[i1] = b_a[i1] * y_data[0];
  }

  y_size[0] = 18 + x_in_size[0];
  loop_ub = 18 + x_in_size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    b_y_data[i1] = y_data[i1];
  }

  filter(b_y_data, y_size, a, y_data, b_y_size);
  flipud(y_data, b_y_size);
  for (i1 = 0; i1 < 3; i1++) {
    a[i1] = b_a[i1] * y_data[0];
  }

  c_y_size[0] = b_y_size[0];
  loop_ub = b_y_size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    b_y_data[i1] = y_data[i1];
  }

  filter(b_y_data, c_y_size, a, y_data, b_y_size);
  flipud(y_data, b_y_size);
  y_out_size[0] = x_in_size[0];
  y_out_size[1] = 1;
  loop_ub = x_in_size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    tmp_data[i1] = 10 + i1;
  }

  loop_ub = x_in_size[0];
  for (i1 = 0; i1 < loop_ub; i1++) {
    y_out_data[i1] = y_data[tmp_data[i1] - 1];
  }
}

//
// Arguments    : double x_data[]
//                int x_size[1]
// Return Type  : void
//
static void flipud(double x_data[], int x_size[1])
{
  int m;
  int md2;
  int i;
  double xtmp;
  m = x_size[0];
  md2 = x_size[0] >> 1;
  for (i = 1; i <= md2; i++) {
    xtmp = x_data[i - 1];
    x_data[i - 1] = x_data[m - i];
    x_data[m - i] = xtmp;
  }
}

//
// Arguments    : const double X_data[]
//                const int X_size[2]
//                double Y_data[]
//                int Y_size[2]
// Return Type  : void
//
static void rescale_minmax(const double X_data[], const int X_size[2], double
Y_data[], int Y_size[2])
{
  int ixstart;
  int n;
  double mtmp;
  int ix;
  boolean_T exitg1;
  double b_mtmp;
  ixstart = 1;
  n = X_size[0];
  mtmp = X_data[0];
  if (rtIsNaN(X_data[0])) {
    ix = 2;
    exitg1 = false;
    while ((!exitg1) && (ix <= n)) {
      ixstart = ix;
      if (!rtIsNaN(X_data[ix - 1])) {
        mtmp = X_data[ix - 1];
        exitg1 = true;
      } else {
        ix++;
      }
    }
  }

  if (ixstart < X_size[0]) {
    while (ixstart + 1 <= n) {
      if (X_data[ixstart] < mtmp) {
        mtmp = X_data[ixstart];
      }

      ixstart++;
    }
  }

  ixstart = 1;
  n = X_size[0];
  b_mtmp = X_data[0];
  if (rtIsNaN(X_data[0])) {
    ix = 2;
    exitg1 = false;
    while ((!exitg1) && (ix <= n)) {
      ixstart = ix;
      if (!rtIsNaN(X_data[ix - 1])) {
        b_mtmp = X_data[ix - 1];
        exitg1 = true;
      } else {
        ix++;
      }
    }
  }

  if (ixstart < X_size[0]) {
    while (ixstart + 1 <= n) {
      if (X_data[ixstart] > b_mtmp) {
        b_mtmp = X_data[ixstart];
      }

      ixstart++;
    }
  }

  b_mtmp -= mtmp;
  Y_size[0] = X_size[0];
  Y_size[1] = 1;
  ixstart = X_size[0] * X_size[1];
  for (n = 0; n < ixstart; n++) {
    Y_data[n] = (X_data[n] - mtmp) / b_mtmp;
  }
}

//
// Preprocessing for time domain data classification.
// Arguments    : const double X_data[]
//                const int X_size[1]
//                float Y_data[]
//                int Y_size[2]
// Return Type  : void
//
void tf_timedomain_preprocess(const double X_data[], const int X_size[1], float
Y_data[], int Y_size[2])
{
  double Y1_data[512];
  int Y1_size[2];
  double Y2_data[512];
  int Y2_size[2];
  int loop_ub;
  int i0;
  if (X_size[0] == 128) {
    filtfilt(X_data, X_size, Y1_data, Y1_size);
    rescale_minmax(Y1_data, Y1_size, Y2_data, Y2_size);
    Y_size[0] = Y2_size[0];
    Y_size[1] = 1;
    loop_ub = Y2_size[0] * Y2_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      Y_data[i0] = (float)Y2_data[i0];
    }
  } else if (X_size[0] == 192) {
    filtfilt(X_data, X_size, Y1_data, Y1_size);
    rescale_minmax(Y1_data, Y1_size, Y2_data, Y2_size);
    Y_size[0] = Y2_size[0];
    Y_size[1] = 1;
    loop_ub = Y2_size[0] * Y2_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      Y_data[i0] = (float)Y2_data[i0];
    }
  } else if (X_size[0] == 256) {
    filtfilt(X_data, X_size, Y1_data, Y1_size);
    rescale_minmax(Y1_data, Y1_size, Y2_data, Y2_size);
    Y_size[0] = Y2_size[0];
    Y_size[1] = 1;
    loop_ub = Y2_size[0] * Y2_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      Y_data[i0] = (float)Y2_data[i0];
    }
  } else if (X_size[0] == 384) {
    filtfilt(X_data, X_size, Y1_data, Y1_size);
    rescale_minmax(Y1_data, Y1_size, Y2_data, Y2_size);
    Y_size[0] = Y2_size[0];
    Y_size[1] = 1;
    loop_ub = Y2_size[0] * Y2_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      Y_data[i0] = (float)Y2_data[i0];
    }
  } else if (X_size[0] == 512) {
    filtfilt(X_data, X_size, Y1_data, Y1_size);
    rescale_minmax(Y1_data, Y1_size, Y2_data, Y2_size);
    Y_size[0] = Y2_size[0];
    Y_size[1] = 1;
    loop_ub = Y2_size[0] * Y2_size[1];
    for (i0 = 0; i0 < loop_ub; i0++) {
      Y_data[i0] = (float)Y2_data[i0];
    }
  } else {
    Y_size[0] = 128;
    Y_size[1] = 1;
    memset(&Y_data[0], 0, sizeof(float) << 7);
  }
}

//
// Arguments    : void
// Return Type  : void
//
void tf_timedomain_preprocess_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void tf_timedomain_preprocess_terminate()
{
  // (no terminate code required)
}

//
// File trailer for tf_timedomain_preprocess.cpp
//
// [EOF]
//
