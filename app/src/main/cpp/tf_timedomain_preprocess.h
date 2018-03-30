//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: tf_timedomain_preprocess.h
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 29-Mar-2018 12:23:41
//
#ifndef TF_TIMEDOMAIN_PREPROCESS_H
#define TF_TIMEDOMAIN_PREPROCESS_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "tf_timedomain_preprocess_types.h"

// Function Declarations
extern void tf_timedomain_preprocess(const double X_data[], const int X_size[1],
  float Y_data[], int Y_size[2]);
extern void tf_timedomain_preprocess_initialize();
extern void tf_timedomain_preprocess_terminate();

#endif

//
// File trailer for tf_timedomain_preprocess.h
//
// [EOF]
//
