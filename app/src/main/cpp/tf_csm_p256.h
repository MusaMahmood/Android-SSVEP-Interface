//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: tf_csm_p256.h
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 25-Apr-2018 15:11:05
//
#ifndef TF_CSM_P256_H
#define TF_CSM_P256_H

// Include Files
#include <cmath>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "tf_csm_p256_types.h"

// Function Declarations
extern void tf_csm_p256(const double X_data[], const int X_size[2], float Y[384]);
extern void tf_csm_p256_initialize();
extern void tf_csm_p256_terminate();

#endif

//
// File trailer for tf_csm_p256.h
//
// [EOF]
//
