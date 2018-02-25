//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: ssvep_filter_f32.cpp
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 25-Feb-2018 09:44:24
//

// Include Files
#include "rt_nonfinite.h"
#include "ssvep_filter_f32.h"

// Function Declarations
static void filter(double b[4], double a[4], const double x[1018], const double
                   zi[3], double y[1018]);
static void flipud(double x[1018]);

// Function Definitions

//
// Arguments    : double b[4]
//                double a[4]
//                const double x[1018]
//                const double zi[3]
//                double y[1018]
// Return Type  : void
//
static void filter(double b[4], double a[4], const double x[1018], const double
                   zi[3], double y[1018])
{
  double a1;
  int k;
  int naxpy;
  int j;
  a1 = a[0];
  if ((!rtIsInf(a[0])) && (!rtIsNaN(a[0])) && (!(a[0] == 0.0)) && (a[0] != 1.0))
  {
    for (k = 0; k < 4; k++) {
      b[k] /= a1;
    }

    for (k = 0; k < 3; k++) {
      a[k + 1] /= a1;
    }

    a[0] = 1.0;
  }

  for (k = 0; k < 3; k++) {
    y[k] = zi[k];
  }

  memset(&y[3], 0, 1015U * sizeof(double));
  for (k = 0; k < 1018; k++) {
    naxpy = 1018 - k;
    if (!(naxpy < 4)) {
      naxpy = 4;
    }

    for (j = 0; j + 1 <= naxpy; j++) {
      y[k + j] += x[k] * b[j];
    }

    naxpy = 1017 - k;
    if (!(naxpy < 3)) {
      naxpy = 3;
    }

    a1 = -y[k];
    for (j = 1; j <= naxpy; j++) {
      y[k + j] += a1 * a[j];
    }
  }
}

//
// Arguments    : double x[1018]
// Return Type  : void
//
static void flipud(double x[1018])
{
  int i;
  double xtmp;
  for (i = 0; i < 509; i++) {
    xtmp = x[i];
    x[i] = x[1017 - i];
    x[1017 - i] = xtmp;
  }
}

//
// [5 40] bandpass butterworth N=3; 250Hz
//  X = X(:);
//  Fs = 250 (DON'T USE OTHERWISE)
// Arguments    : const double X[1000]
//                float Y[1000]
// Return Type  : void
//
void ssvep_filter_f32(const double X[1000], float Y[1000])
{
  double d0;
  double d1;
  int i;
  double y[1018];
  double dv0[4];
  static const double dv1[4] = { 0.90431873448479, -2.71295620345437,
    2.71295620345437, -0.90431873448479 };

  double dv2[4];
  static const double dv3[4] = { 1.0, -2.7990220146733, 2.61773550092223,
    -0.81779236028278 };

  double b_y[1018];
  double a[3];
  static const double b_a[3] = { -0.90431873448438438, 1.80863746896885,
    -0.90431873448445832 };

  //  Optimized SSVEP C Filter:
  d0 = 2.0 * X[0];
  d1 = 2.0 * X[999];
  for (i = 0; i < 9; i++) {
    y[i] = d0 - X[9 - i];
  }

  memcpy(&y[9], &X[0], 1000U * sizeof(double));
  for (i = 0; i < 9; i++) {
    y[i + 1009] = d1 - X[998 - i];
  }

  for (i = 0; i < 4; i++) {
    dv0[i] = dv1[i];
    dv2[i] = dv3[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1018U * sizeof(double));
  filter(dv0, dv2, b_y, a, y);
  flipud(y);
  for (i = 0; i < 4; i++) {
    dv0[i] = dv1[i];
    dv2[i] = dv3[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1018U * sizeof(double));
  filter(dv0, dv2, b_y, a, y);
  flipud(y);
  for (i = 0; i < 1000; i++) {
    Y[i] = (float)y[9 + i];
  }
}

//
// Arguments    : void
// Return Type  : void
//
void ssvep_filter_f32_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void ssvep_filter_f32_terminate()
{
  // (no terminate code required)
}

//
// File trailer for ssvep_filter_f32.cpp
//
// [EOF]
//
