//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: get_hr_rr.cpp
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 18-Dec-2018 13:23:01
//

// Include Files
#include "rt_nonfinite.h"
#include "get_hr_rr.h"
#include "get_hr_rr_emxutil.h"

// Named Constants
#define Fs                             (31.25)

// Function Declarations
static void b_do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
  emxArray_int32_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib);
static void b_filter(const double b[2], const double x[1006], double zi, double
                     y[1006]);
static void b_filtfilt(const double x_in[1000], double y_out[1000]);
static void b_flipud(double x[1006]);
static void b_getAllPeaksCodegen(const double y[1000], emxArray_int32_T *iPk,
  emxArray_int32_T *iInf, emxArray_int32_T *iInflect);
static double b_mean(const emxArray_real_T *x);
static void c_filtfilt(const double x_in[1000], double y_out[1000]);
static void combinePeaks(const emxArray_int32_T *iPk, const emxArray_int32_T
  *iInf, emxArray_int32_T *iPkOut);
static void do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
  emxArray_int32_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib);
static void ecg_filt_rescale(const double X[1000], float Y[1000]);
static void filter(double b[4], double a[4], const double x[1018], const double
                   zi[3], double y[1018]);
static void filtfilt(const double x_in[1000], double y_out[1000]);
static void findExtents(const double y[1000], emxArray_int32_T *iPk, const
  emxArray_int32_T *iFin, const emxArray_int32_T *iInf, const emxArray_int32_T
  *iInflect, double minP, emxArray_real_T *bPk, emxArray_real_T *bxPk,
  emxArray_real_T *byPk, emxArray_real_T *wxPk);
static void findLeftIntercept(const double y[1000], int *idx, int borderIdx,
  double refHeight);
static void findRightIntercept(const double y[1000], int *idx, int borderIdx,
  double refHeight);
static void findpeaks(const double Yin[1000], double varargin_2, emxArray_real_T
                      *Ypk, emxArray_real_T *Xpk);
static void flipud(double x[1018]);
static void getAllPeaksCodegen(const double y[996], emxArray_int32_T *iPk,
  emxArray_int32_T *iInf, emxArray_int32_T *iInflect);
static void getLeftBase(const double yTemp[1000], const emxArray_int32_T *iPeak,
  const emxArray_int32_T *iFinite, const emxArray_int32_T *iInflect,
  emxArray_int32_T *iBase, emxArray_int32_T *iSaddle);
static void getPeakBase(const double yTemp[1000], const emxArray_int32_T *iPk,
  const emxArray_int32_T *iFin, const emxArray_int32_T *iInflect,
  emxArray_real_T *peakBase, emxArray_int32_T *iLeftSaddle, emxArray_int32_T
  *iRightSaddle);
static double linterp(double xa, double xb, double ya, double yb, double yc,
                      double bc);
static double mean(const double x[1000]);
static void power(const double a[1000], double y[1000]);

// Function Definitions

//
// Arguments    : const emxArray_int32_T *a
//                const emxArray_int32_T *b
//                emxArray_int32_T *c
//                emxArray_int32_T *ia
//                emxArray_int32_T *ib
// Return Type  : void
//
static void b_do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
  emxArray_int32_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib)
{
  int iafirst;
  int ncmax;
  int nc;
  int ialast;
  int ibfirst;
  int iblast;
  int b_ialast;
  int ak;
  emxArray_int32_T *b_ia;
  int b_iblast;
  int bk;
  emxArray_int32_T *b_ib;
  emxArray_int32_T *b_c;
  iafirst = a->size[0];
  ncmax = b->size[0];
  if (iafirst < ncmax) {
    ncmax = iafirst;
  }

  iafirst = c->size[0];
  c->size[0] = ncmax;
  emxEnsureCapacity((emxArray__common *)c, iafirst, sizeof(int));
  iafirst = ia->size[0];
  ia->size[0] = ncmax;
  emxEnsureCapacity((emxArray__common *)ia, iafirst, sizeof(int));
  iafirst = ib->size[0];
  ib->size[0] = ncmax;
  emxEnsureCapacity((emxArray__common *)ib, iafirst, sizeof(int));
  nc = 0;
  iafirst = 0;
  ialast = 1;
  ibfirst = 0;
  iblast = 1;
  while ((ialast <= a->size[0]) && (iblast <= b->size[0])) {
    b_ialast = ialast;
    ak = a->data[ialast - 1];
    while ((b_ialast < a->size[0]) && (a->data[b_ialast] == ak)) {
      b_ialast++;
    }

    ialast = b_ialast;
    b_iblast = iblast;
    bk = b->data[iblast - 1];
    while ((b_iblast < b->size[0]) && (b->data[b_iblast] == bk)) {
      b_iblast++;
    }

    iblast = b_iblast;
    if (ak == bk) {
      nc++;
      c->data[nc - 1] = ak;
      ia->data[nc - 1] = iafirst + 1;
      ib->data[nc - 1] = ibfirst + 1;
      ialast = b_ialast + 1;
      iafirst = b_ialast;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    } else if (ak < bk) {
      ialast = b_ialast + 1;
      iafirst = b_ialast;
    } else {
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    }
  }

  if (ncmax > 0) {
    if (1 > nc) {
      ialast = 0;
    } else {
      ialast = nc;
    }

    emxInit_int32_T(&b_ia, 1);
    iafirst = b_ia->size[0];
    b_ia->size[0] = ialast;
    emxEnsureCapacity((emxArray__common *)b_ia, iafirst, sizeof(int));
    for (iafirst = 0; iafirst < ialast; iafirst++) {
      b_ia->data[iafirst] = ia->data[iafirst];
    }

    iafirst = ia->size[0];
    ia->size[0] = b_ia->size[0];
    emxEnsureCapacity((emxArray__common *)ia, iafirst, sizeof(int));
    ialast = b_ia->size[0];
    for (iafirst = 0; iafirst < ialast; iafirst++) {
      ia->data[iafirst] = b_ia->data[iafirst];
    }

    emxFree_int32_T(&b_ia);
    if (1 > nc) {
      ialast = 0;
    } else {
      ialast = nc;
    }

    emxInit_int32_T(&b_ib, 1);
    iafirst = b_ib->size[0];
    b_ib->size[0] = ialast;
    emxEnsureCapacity((emxArray__common *)b_ib, iafirst, sizeof(int));
    for (iafirst = 0; iafirst < ialast; iafirst++) {
      b_ib->data[iafirst] = ib->data[iafirst];
    }

    iafirst = ib->size[0];
    ib->size[0] = b_ib->size[0];
    emxEnsureCapacity((emxArray__common *)ib, iafirst, sizeof(int));
    ialast = b_ib->size[0];
    for (iafirst = 0; iafirst < ialast; iafirst++) {
      ib->data[iafirst] = b_ib->data[iafirst];
    }

    emxFree_int32_T(&b_ib);
    if (1 > nc) {
      ialast = 0;
    } else {
      ialast = nc;
    }

    emxInit_int32_T(&b_c, 1);
    iafirst = b_c->size[0];
    b_c->size[0] = ialast;
    emxEnsureCapacity((emxArray__common *)b_c, iafirst, sizeof(int));
    for (iafirst = 0; iafirst < ialast; iafirst++) {
      b_c->data[iafirst] = c->data[iafirst];
    }

    iafirst = c->size[0];
    c->size[0] = b_c->size[0];
    emxEnsureCapacity((emxArray__common *)c, iafirst, sizeof(int));
    ialast = b_c->size[0];
    for (iafirst = 0; iafirst < ialast; iafirst++) {
      c->data[iafirst] = b_c->data[iafirst];
    }

    emxFree_int32_T(&b_c);
  }
}

//
// Arguments    : const double b[2]
//                const double x[1006]
//                double zi
//                double y[1006]
// Return Type  : void
//
static void b_filter(const double b[2], const double x[1006], double zi, double
                     y[1006])
{
  int k;
  int j;
  y[0] = zi;
  memset(&y[1], 0, 1005U * sizeof(double));
  for (k = 0; k < 2; k++) {
    for (j = k; j + 1 < 1007; j++) {
      y[j] += b[k] * x[j - k];
    }
  }
}

//
// Arguments    : const double x_in[1000]
//                double y_out[1000]
// Return Type  : void
//
static void b_filtfilt(const double x_in[1000], double y_out[1000])
{
  double d2;
  double d3;
  int i;
  double y[1018];
  double dv10[4];
  static const double dv11[4] = { 0.667031236673441, -2.001093710020323,
    2.001093710020323, -0.667031236673441 };

  double dv12[4];
  static const double dv13[4] = { 1.0, -2.2006359767013279, 1.690715140792437,
    -0.444898775893761 };

  double b_y[1018];
  double a[3];
  static const double b_a[3] = { -0.66703123667343955, 1.3340624733468802,
    -0.66703123667344033 };

  d2 = 2.0 * x_in[0];
  d3 = 2.0 * x_in[999];
  for (i = 0; i < 9; i++) {
    y[i] = d2 - x_in[9 - i];
  }

  memcpy(&y[9], &x_in[0], 1000U * sizeof(double));
  for (i = 0; i < 9; i++) {
    y[i + 1009] = d3 - x_in[998 - i];
  }

  for (i = 0; i < 4; i++) {
    dv10[i] = dv11[i];
    dv12[i] = dv13[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1018U * sizeof(double));
  filter(dv10, dv12, b_y, a, y);
  flipud(y);
  for (i = 0; i < 4; i++) {
    dv10[i] = dv11[i];
    dv12[i] = dv13[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1018U * sizeof(double));
  filter(dv10, dv12, b_y, a, y);
  flipud(y);
  memcpy(&y_out[0], &y[9], 1000U * sizeof(double));
}

//
// Arguments    : double x[1006]
// Return Type  : void
//
static void b_flipud(double x[1006])
{
  int i;
  double xtmp;
  for (i = 0; i < 503; i++) {
    xtmp = x[i];
    x[i] = x[1005 - i];
    x[1005 - i] = xtmp;
  }
}

//
// Arguments    : const double y[1000]
//                emxArray_int32_T *iPk
//                emxArray_int32_T *iInf
//                emxArray_int32_T *iInflect
// Return Type  : void
//
static void b_getAllPeaksCodegen(const double y[1000], emxArray_int32_T *iPk,
  emxArray_int32_T *iInf, emxArray_int32_T *iInflect)
{
  int kfirst;
  int nPk;
  int nInf;
  int nInflect;
  char dir;
  double ykfirst;
  boolean_T isinfykfirst;
  int k;
  double yk;
  boolean_T isinfyk;
  char previousdir;
  int i2;
  kfirst = iPk->size[0];
  iPk->size[0] = 1000;
  emxEnsureCapacity((emxArray__common *)iPk, kfirst, sizeof(int));
  kfirst = iInf->size[0];
  iInf->size[0] = 1000;
  emxEnsureCapacity((emxArray__common *)iInf, kfirst, sizeof(int));
  kfirst = iInflect->size[0];
  iInflect->size[0] = 1000;
  emxEnsureCapacity((emxArray__common *)iInflect, kfirst, sizeof(int));
  nPk = 0;
  nInf = 0;
  nInflect = -1;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 0; k < 1000; k++) {
    yk = y[k];
    if (rtIsNaN(y[k])) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(y[k]) && (y[k] > 0.0)) {
      isinfyk = true;
      nInf++;
      iInf->data[nInf - 1] = k + 1;
    } else {
      isinfyk = false;
    }

    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
        if (kfirst >= 1) {
          nInflect++;
          iInflect->data[nInflect] = kfirst;
        }
      } else if (yk < ykfirst) {
        dir = 'd';
        if ('d' != previousdir) {
          nInflect++;
          iInflect->data[nInflect] = kfirst;
          if (previousdir == 'i') {
            nPk++;
            iPk->data[nPk - 1] = kfirst;
          }
        }
      } else {
        dir = 'i';
        if ('i' != previousdir) {
          nInflect++;
          iInflect->data[nInflect] = kfirst;
        }
      }

      ykfirst = yk;
      kfirst = k + 1;
      isinfykfirst = isinfyk;
    }
  }

  if ((!isinfykfirst) && ((nInflect + 1 == 0) || (iInflect->data[nInflect] <
        1000))) {
    nInflect++;
    iInflect->data[nInflect] = 1000;
  }

  kfirst = iPk->size[0];
  if (1 > nPk) {
    iPk->size[0] = 0;
  } else {
    iPk->size[0] = nPk;
  }

  emxEnsureCapacity((emxArray__common *)iPk, kfirst, sizeof(int));
  kfirst = iInf->size[0];
  if (1 > nInf) {
    iInf->size[0] = 0;
  } else {
    iInf->size[0] = nInf;
  }

  emxEnsureCapacity((emxArray__common *)iInf, kfirst, sizeof(int));
  kfirst = iInflect->size[0];
  if (1 > nInflect + 1) {
    i2 = -1;
  } else {
    i2 = nInflect;
  }

  iInflect->size[0] = i2 + 1;
  emxEnsureCapacity((emxArray__common *)iInflect, kfirst, sizeof(int));
}

//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
static double b_mean(const emxArray_real_T *x)
{
  double y;
  int k;
  if (x->size[1] == 0) {
    y = 0.0;
  } else {
    y = x->data[0];
    for (k = 2; k <= x->size[1]; k++) {
      y += x->data[k - 1];
    }
  }

  y /= (double)x->size[1];
  return y;
}

//
// Arguments    : const double x_in[1000]
//                double y_out[1000]
// Return Type  : void
//
static void c_filtfilt(const double x_in[1000], double y_out[1000])
{
  double d4;
  double d5;
  int i0;
  double y[1006];
  double b_y[1006];
  double dv14[2];
  d4 = 2.0 * x_in[0];
  d5 = 2.0 * x_in[999];
  for (i0 = 0; i0 < 3; i0++) {
    y[i0] = d4 - x_in[3 - i0];
  }

  memcpy(&y[3], &x_in[0], 1000U * sizeof(double));
  for (i0 = 0; i0 < 3; i0++) {
    y[i0 + 1003] = d5 - x_in[998 - i0];
  }

  for (i0 = 0; i0 < 2; i0++) {
    dv14[i0] = 7.8125 + -16.5625 * (double)i0;
  }

  memcpy(&b_y[0], &y[0], 1006U * sizeof(double));
  b_filter(dv14, b_y, -8.75 * y[0], y);
  b_flipud(y);
  for (i0 = 0; i0 < 2; i0++) {
    dv14[i0] = 7.8125 + -16.5625 * (double)i0;
  }

  memcpy(&b_y[0], &y[0], 1006U * sizeof(double));
  b_filter(dv14, b_y, -8.75 * y[0], y);
  b_flipud(y);
  memcpy(&y_out[0], &y[3], 1000U * sizeof(double));
}

//
// Arguments    : const emxArray_int32_T *iPk
//                const emxArray_int32_T *iInf
//                emxArray_int32_T *iPkOut
// Return Type  : void
//
static void combinePeaks(const emxArray_int32_T *iPk, const emxArray_int32_T
  *iInf, emxArray_int32_T *iPkOut)
{
  emxArray_int32_T *ia;
  emxArray_int32_T *ib;
  emxInit_int32_T(&ia, 1);
  emxInit_int32_T(&ib, 1);
  do_vectors(iPk, iInf, iPkOut, ia, ib);
  emxFree_int32_T(&ib);
  emxFree_int32_T(&ia);
}

//
// Arguments    : const emxArray_int32_T *a
//                const emxArray_int32_T *b
//                emxArray_int32_T *c
//                emxArray_int32_T *ia
//                emxArray_int32_T *ib
// Return Type  : void
//
static void do_vectors(const emxArray_int32_T *a, const emxArray_int32_T *b,
  emxArray_int32_T *c, emxArray_int32_T *ia, emxArray_int32_T *ib)
{
  int na;
  int nb;
  int ncmax;
  int iafirst;
  int nc;
  int nia;
  int nib;
  int ialast;
  int ibfirst;
  int iblast;
  int b_ialast;
  int ak;
  int b_iblast;
  int bk;
  emxArray_int32_T *b_ia;
  emxArray_int32_T *b_ib;
  emxArray_int32_T *b_c;
  na = a->size[0];
  nb = b->size[0];
  ncmax = a->size[0] + b->size[0];
  iafirst = c->size[0];
  c->size[0] = ncmax;
  emxEnsureCapacity((emxArray__common *)c, iafirst, sizeof(int));
  iafirst = ia->size[0];
  ia->size[0] = a->size[0];
  emxEnsureCapacity((emxArray__common *)ia, iafirst, sizeof(int));
  iafirst = ib->size[0];
  ib->size[0] = b->size[0];
  emxEnsureCapacity((emxArray__common *)ib, iafirst, sizeof(int));
  nc = -1;
  nia = -1;
  nib = 0;
  iafirst = 1;
  ialast = 1;
  ibfirst = 0;
  iblast = 1;
  while ((ialast <= na) && (iblast <= nb)) {
    b_ialast = ialast;
    ak = a->data[ialast - 1];
    while ((b_ialast < a->size[0]) && (a->data[b_ialast] == ak)) {
      b_ialast++;
    }

    ialast = b_ialast;
    b_iblast = iblast;
    bk = b->data[iblast - 1];
    while ((b_iblast < b->size[0]) && (b->data[b_iblast] == bk)) {
      b_iblast++;
    }

    iblast = b_iblast;
    if (ak == bk) {
      nc++;
      c->data[nc] = ak;
      nia++;
      ia->data[nia] = iafirst;
      ialast = b_ialast + 1;
      iafirst = b_ialast + 1;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    } else if (ak < bk) {
      nc++;
      nia++;
      c->data[nc] = ak;
      ia->data[nia] = iafirst;
      ialast = b_ialast + 1;
      iafirst = b_ialast + 1;
    } else {
      nc++;
      nib++;
      c->data[nc] = bk;
      ib->data[nib - 1] = ibfirst + 1;
      iblast = b_iblast + 1;
      ibfirst = b_iblast;
    }
  }

  while (ialast <= na) {
    b_ialast = ialast;
    while ((b_ialast < a->size[0]) && (a->data[b_ialast] == a->data[ialast - 1]))
    {
      b_ialast++;
    }

    nc++;
    nia++;
    c->data[nc] = a->data[ialast - 1];
    ia->data[nia] = ialast;
    ialast = b_ialast + 1;
  }

  while (iblast <= nb) {
    b_iblast = iblast;
    while ((b_iblast < b->size[0]) && (b->data[b_iblast] == b->data[iblast - 1]))
    {
      b_iblast++;
    }

    nc++;
    nib++;
    c->data[nc] = b->data[iblast - 1];
    ib->data[nib - 1] = iblast;
    iblast = b_iblast + 1;
  }

  if (a->size[0] > 0) {
    if (1 > nia + 1) {
      nia = -1;
    }

    emxInit_int32_T(&b_ia, 1);
    iafirst = b_ia->size[0];
    b_ia->size[0] = nia + 1;
    emxEnsureCapacity((emxArray__common *)b_ia, iafirst, sizeof(int));
    for (iafirst = 0; iafirst <= nia; iafirst++) {
      b_ia->data[iafirst] = ia->data[iafirst];
    }

    iafirst = ia->size[0];
    ia->size[0] = b_ia->size[0];
    emxEnsureCapacity((emxArray__common *)ia, iafirst, sizeof(int));
    nia = b_ia->size[0];
    for (iafirst = 0; iafirst < nia; iafirst++) {
      ia->data[iafirst] = b_ia->data[iafirst];
    }

    emxFree_int32_T(&b_ia);
  }

  if (b->size[0] > 0) {
    if (1 > nib) {
      nia = 0;
    } else {
      nia = nib;
    }

    emxInit_int32_T(&b_ib, 1);
    iafirst = b_ib->size[0];
    b_ib->size[0] = nia;
    emxEnsureCapacity((emxArray__common *)b_ib, iafirst, sizeof(int));
    for (iafirst = 0; iafirst < nia; iafirst++) {
      b_ib->data[iafirst] = ib->data[iafirst];
    }

    iafirst = ib->size[0];
    ib->size[0] = b_ib->size[0];
    emxEnsureCapacity((emxArray__common *)ib, iafirst, sizeof(int));
    nia = b_ib->size[0];
    for (iafirst = 0; iafirst < nia; iafirst++) {
      ib->data[iafirst] = b_ib->data[iafirst];
    }

    emxFree_int32_T(&b_ib);
  }

  if (ncmax > 0) {
    if (1 > nc + 1) {
      nia = -1;
    } else {
      nia = nc;
    }

    emxInit_int32_T(&b_c, 1);
    iafirst = b_c->size[0];
    b_c->size[0] = nia + 1;
    emxEnsureCapacity((emxArray__common *)b_c, iafirst, sizeof(int));
    for (iafirst = 0; iafirst <= nia; iafirst++) {
      b_c->data[iafirst] = c->data[iafirst];
    }

    iafirst = c->size[0];
    c->size[0] = b_c->size[0];
    emxEnsureCapacity((emxArray__common *)c, iafirst, sizeof(int));
    nia = b_c->size[0];
    for (iafirst = 0; iafirst < nia; iafirst++) {
      c->data[iafirst] = b_c->data[iafirst];
    }

    emxFree_int32_T(&b_c);
  }
}

//
// Input: doubles (2000, 1)
//  Output: Y, single (2000, 1)
//  Filter is order 3, HPF @ 1 Hz, butterworth. 31.25 Hz Fs.
//  CODE:
//  [b, a] = butter(3, 1*2/31.25, 'high');
// Arguments    : const double X[1000]
//                float Y[1000]
// Return Type  : void
//
static void ecg_filt_rescale(const double X[1000], float Y[1000])
{
  double b_X[1000];
  int ixstart;
  double mtmp;
  int ix;
  boolean_T exitg1;
  double b_mtmp;
  filtfilt(X, b_X);
  ixstart = 1;
  mtmp = b_X[0];
  if (rtIsNaN(b_X[0])) {
    ix = 2;
    exitg1 = false;
    while ((!exitg1) && (ix < 1001)) {
      ixstart = ix;
      if (!rtIsNaN(b_X[ix - 1])) {
        mtmp = b_X[ix - 1];
        exitg1 = true;
      } else {
        ix++;
      }
    }
  }

  if (ixstart < 1000) {
    while (ixstart + 1 < 1001) {
      if (b_X[ixstart] < mtmp) {
        mtmp = b_X[ixstart];
      }

      ixstart++;
    }
  }

  ixstart = 1;
  b_mtmp = b_X[0];
  if (rtIsNaN(b_X[0])) {
    ix = 2;
    exitg1 = false;
    while ((!exitg1) && (ix < 1001)) {
      ixstart = ix;
      if (!rtIsNaN(b_X[ix - 1])) {
        b_mtmp = b_X[ix - 1];
        exitg1 = true;
      } else {
        ix++;
      }
    }
  }

  if (ixstart < 1000) {
    while (ixstart + 1 < 1001) {
      if (b_X[ixstart] > b_mtmp) {
        b_mtmp = b_X[ixstart];
      }

      ixstart++;
    }
  }

  b_mtmp -= mtmp;
  for (ixstart = 0; ixstart < 1000; ixstart++) {
    Y[ixstart] = (float)((b_X[ixstart] - mtmp) / b_mtmp);
  }
}

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
// Arguments    : const double x_in[1000]
//                double y_out[1000]
// Return Type  : void
//
static void filtfilt(const double x_in[1000], double y_out[1000])
{
  double d0;
  double d1;
  int i;
  double y[1018];
  double dv6[4];
  static const double dv7[4] = { 0.817582402590499, -2.4527472077714969,
    2.4527472077714969, -0.817582402590499 };

  double dv8[4];
  static const double dv9[4] = { 1.0, -2.5985327986088551, 2.273686141233203,
    -0.668440280881935 };

  double b_y[1018];
  double a[3];
  static const double b_a[3] = { -0.81758240259053439, 1.6351648051810546,
    -0.81758240259052273 };

  d0 = 2.0 * x_in[0];
  d1 = 2.0 * x_in[999];
  for (i = 0; i < 9; i++) {
    y[i] = d0 - x_in[9 - i];
  }

  memcpy(&y[9], &x_in[0], 1000U * sizeof(double));
  for (i = 0; i < 9; i++) {
    y[i + 1009] = d1 - x_in[998 - i];
  }

  for (i = 0; i < 4; i++) {
    dv6[i] = dv7[i];
    dv8[i] = dv9[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1018U * sizeof(double));
  filter(dv6, dv8, b_y, a, y);
  flipud(y);
  for (i = 0; i < 4; i++) {
    dv6[i] = dv7[i];
    dv8[i] = dv9[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 1018U * sizeof(double));
  filter(dv6, dv8, b_y, a, y);
  flipud(y);
  memcpy(&y_out[0], &y[9], 1000U * sizeof(double));
}

//
// Arguments    : const double y[1000]
//                emxArray_int32_T *iPk
//                const emxArray_int32_T *iFin
//                const emxArray_int32_T *iInf
//                const emxArray_int32_T *iInflect
//                double minP
//                emxArray_real_T *bPk
//                emxArray_real_T *bxPk
//                emxArray_real_T *byPk
//                emxArray_real_T *wxPk
// Return Type  : void
//
static void findExtents(const double y[1000], emxArray_int32_T *iPk, const
  emxArray_int32_T *iFin, const emxArray_int32_T *iInf, const emxArray_int32_T
  *iInflect, double minP, emxArray_real_T *bPk, emxArray_real_T *bxPk,
  emxArray_real_T *byPk, emxArray_real_T *wxPk)
{
  double yFinite[1000];
  int iLeft;
  int i3;
  emxArray_real_T *b_bPk;
  emxArray_int32_T *iLB;
  emxArray_int32_T *iRB;
  emxArray_boolean_T *x;
  emxArray_int32_T *ii;
  int nx;
  int idx;
  boolean_T exitg1;
  emxArray_real_T *b_idx;
  emxArray_int32_T *b_iPk;
  emxArray_real_T *c_bPk;
  emxArray_int32_T *b_iLB;
  emxArray_int32_T *b_iRB;
  unsigned int unnamed_idx_0;
  emxArray_real_T *b_wxPk;
  emxArray_int32_T *iInfL;
  double refHeight;
  emxArray_int32_T *iInfR;
  emxArray_int32_T *c;
  emxArray_int32_T *iInfinite;
  emxArray_real_T *r1;
  emxArray_real_T *r2;
  memcpy(&yFinite[0], &y[0], 1000U * sizeof(double));
  iLeft = iInf->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    yFinite[iInf->data[i3] - 1] = rtNaN;
  }

  emxInit_real_T1(&b_bPk, 1);
  emxInit_int32_T(&iLB, 1);
  emxInit_int32_T(&iRB, 1);
  emxInit_boolean_T(&x, 1);
  getPeakBase(yFinite, iPk, iFin, iInflect, b_bPk, iLB, iRB);
  i3 = x->size[0];
  x->size[0] = iPk->size[0];
  emxEnsureCapacity((emxArray__common *)x, i3, sizeof(boolean_T));
  iLeft = iPk->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    x->data[i3] = (yFinite[iPk->data[i3] - 1] - b_bPk->data[i3] >= minP);
  }

  emxInit_int32_T(&ii, 1);
  nx = x->size[0];
  idx = 0;
  i3 = ii->size[0];
  ii->size[0] = x->size[0];
  emxEnsureCapacity((emxArray__common *)ii, i3, sizeof(int));
  iLeft = 1;
  exitg1 = false;
  while ((!exitg1) && (iLeft <= nx)) {
    if (x->data[iLeft - 1]) {
      idx++;
      ii->data[idx - 1] = iLeft;
      if (idx >= nx) {
        exitg1 = true;
      } else {
        iLeft++;
      }
    } else {
      iLeft++;
    }
  }

  if (x->size[0] == 1) {
    if (idx == 0) {
      i3 = ii->size[0];
      ii->size[0] = 0;
      emxEnsureCapacity((emxArray__common *)ii, i3, sizeof(int));
    }
  } else {
    i3 = ii->size[0];
    if (1 > idx) {
      ii->size[0] = 0;
    } else {
      ii->size[0] = idx;
    }

    emxEnsureCapacity((emxArray__common *)ii, i3, sizeof(int));
  }

  emxFree_boolean_T(&x);
  emxInit_real_T1(&b_idx, 1);
  i3 = b_idx->size[0];
  b_idx->size[0] = ii->size[0];
  emxEnsureCapacity((emxArray__common *)b_idx, i3, sizeof(double));
  iLeft = ii->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    b_idx->data[i3] = ii->data[i3];
  }

  emxInit_int32_T(&b_iPk, 1);
  i3 = b_iPk->size[0];
  b_iPk->size[0] = b_idx->size[0];
  emxEnsureCapacity((emxArray__common *)b_iPk, i3, sizeof(int));
  iLeft = b_idx->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    b_iPk->data[i3] = iPk->data[(int)b_idx->data[i3] - 1];
  }

  i3 = iPk->size[0];
  iPk->size[0] = b_iPk->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, i3, sizeof(int));
  iLeft = b_iPk->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    iPk->data[i3] = b_iPk->data[i3];
  }

  emxFree_int32_T(&b_iPk);
  emxInit_real_T1(&c_bPk, 1);
  i3 = c_bPk->size[0];
  c_bPk->size[0] = b_idx->size[0];
  emxEnsureCapacity((emxArray__common *)c_bPk, i3, sizeof(double));
  iLeft = b_idx->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    c_bPk->data[i3] = b_bPk->data[(int)b_idx->data[i3] - 1];
  }

  i3 = b_bPk->size[0];
  b_bPk->size[0] = c_bPk->size[0];
  emxEnsureCapacity((emxArray__common *)b_bPk, i3, sizeof(double));
  iLeft = c_bPk->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    b_bPk->data[i3] = c_bPk->data[i3];
  }

  emxFree_real_T(&c_bPk);
  emxInit_int32_T(&b_iLB, 1);
  i3 = b_iLB->size[0];
  b_iLB->size[0] = b_idx->size[0];
  emxEnsureCapacity((emxArray__common *)b_iLB, i3, sizeof(int));
  iLeft = b_idx->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    b_iLB->data[i3] = iLB->data[(int)b_idx->data[i3] - 1];
  }

  i3 = iLB->size[0];
  iLB->size[0] = b_iLB->size[0];
  emxEnsureCapacity((emxArray__common *)iLB, i3, sizeof(int));
  iLeft = b_iLB->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    iLB->data[i3] = b_iLB->data[i3];
  }

  emxFree_int32_T(&b_iLB);
  emxInit_int32_T(&b_iRB, 1);
  i3 = b_iRB->size[0];
  b_iRB->size[0] = b_idx->size[0];
  emxEnsureCapacity((emxArray__common *)b_iRB, i3, sizeof(int));
  iLeft = b_idx->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    b_iRB->data[i3] = iRB->data[(int)b_idx->data[i3] - 1];
  }

  i3 = iRB->size[0];
  iRB->size[0] = b_iRB->size[0];
  emxEnsureCapacity((emxArray__common *)iRB, i3, sizeof(int));
  iLeft = b_iRB->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    iRB->data[i3] = b_iRB->data[i3];
  }

  emxFree_int32_T(&b_iRB);
  if (iPk->size[0] == 0) {
    unnamed_idx_0 = (unsigned int)iPk->size[0];
    i3 = b_idx->size[0];
    b_idx->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)b_idx, i3, sizeof(double));
    iLeft = (int)unnamed_idx_0;
    for (i3 = 0; i3 < iLeft; i3++) {
      b_idx->data[i3] = 0.0;
    }

    unnamed_idx_0 = (unsigned int)iPk->size[0];
    i3 = iLB->size[0];
    iLB->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)iLB, i3, sizeof(int));
    iLeft = (int)unnamed_idx_0;
    for (i3 = 0; i3 < iLeft; i3++) {
      iLB->data[i3] = 0;
    }

    unnamed_idx_0 = (unsigned int)iPk->size[0];
    i3 = iRB->size[0];
    iRB->size[0] = (int)unnamed_idx_0;
    emxEnsureCapacity((emxArray__common *)iRB, i3, sizeof(int));
    iLeft = (int)unnamed_idx_0;
    for (i3 = 0; i3 < iLeft; i3++) {
      iRB->data[i3] = 0;
    }
  } else {
    i3 = b_idx->size[0];
    b_idx->size[0] = b_bPk->size[0];
    emxEnsureCapacity((emxArray__common *)b_idx, i3, sizeof(double));
    iLeft = b_bPk->size[0];
    for (i3 = 0; i3 < iLeft; i3++) {
      b_idx->data[i3] = b_bPk->data[i3];
    }
  }

  emxInit_real_T(&b_wxPk, 2);
  i3 = b_wxPk->size[0] * b_wxPk->size[1];
  b_wxPk->size[0] = iPk->size[0];
  b_wxPk->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)b_wxPk, i3, sizeof(double));
  iLeft = iPk->size[0] << 1;
  for (i3 = 0; i3 < iLeft; i3++) {
    b_wxPk->data[i3] = 0.0;
  }

  for (nx = 0; nx < iPk->size[0]; nx++) {
    refHeight = (yFinite[iPk->data[nx] - 1] + b_idx->data[nx]) / 2.0;
    iLeft = iPk->data[nx];
    findLeftIntercept(yFinite, &iLeft, iLB->data[nx], refHeight);
    if (iLeft < iLB->data[nx]) {
      b_wxPk->data[nx] = 1.0 + ((double)iLB->data[nx] - 1.0);
    } else {
      b_wxPk->data[nx] = linterp(1.0 + ((double)iLeft - 1.0), 1.0 + ((double)
        (iLeft + 1) - 1.0), yFinite[iLeft - 1], yFinite[iLeft], yFinite
        [iPk->data[nx] - 1], b_idx->data[nx]);
    }

    iLeft = iPk->data[nx];
    findRightIntercept(yFinite, &iLeft, iRB->data[nx], refHeight);
    if (iLeft > iRB->data[nx]) {
      b_wxPk->data[nx + b_wxPk->size[0]] = 1.0 + ((double)iRB->data[nx] - 1.0);
    } else {
      b_wxPk->data[nx + b_wxPk->size[0]] = linterp(1.0 + ((double)iLeft - 1.0),
        1.0 + ((double)(iLeft - 1) - 1.0), yFinite[iLeft - 1], yFinite[iLeft - 2],
        yFinite[iPk->data[nx] - 1], b_idx->data[nx]);
    }
  }

  emxInit_int32_T(&iInfL, 1);
  emxInit_int32_T(&iInfR, 1);
  emxInit_int32_T(&c, 1);
  do_vectors(iPk, iInf, c, ii, iInfR);
  b_do_vectors(c, iPk, iInfL, ii, iInfR);
  i3 = b_idx->size[0];
  b_idx->size[0] = ii->size[0];
  emxEnsureCapacity((emxArray__common *)b_idx, i3, sizeof(double));
  iLeft = ii->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    b_idx->data[i3] = ii->data[i3];
  }

  emxInit_int32_T(&iInfinite, 1);
  b_do_vectors(c, iInf, iInfL, ii, iInfR);
  i3 = iInfinite->size[0];
  iInfinite->size[0] = ii->size[0];
  emxEnsureCapacity((emxArray__common *)iInfinite, i3, sizeof(int));
  iLeft = ii->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    iInfinite->data[i3] = ii->data[i3];
  }

  iLeft = c->size[0];
  i3 = c->size[0];
  c->size[0] = iLeft;
  emxEnsureCapacity((emxArray__common *)c, i3, sizeof(int));
  unnamed_idx_0 = (unsigned int)c->size[0];
  i3 = bPk->size[0];
  bPk->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)bPk, i3, sizeof(double));
  iLeft = (int)unnamed_idx_0;
  for (i3 = 0; i3 < iLeft; i3++) {
    bPk->data[i3] = 0.0;
  }

  iLeft = b_bPk->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    bPk->data[(int)b_idx->data[i3] - 1] = b_bPk->data[i3];
  }

  emxFree_real_T(&b_bPk);
  i3 = ii->size[0];
  ii->size[0] = iInfinite->size[0];
  emxEnsureCapacity((emxArray__common *)ii, i3, sizeof(int));
  iLeft = iInfinite->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    ii->data[i3] = iInfinite->data[i3];
  }

  iLeft = ii->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    bPk->data[ii->data[i3] - 1] = 0.0;
  }

  i3 = ii->size[0];
  ii->size[0] = iInf->size[0];
  emxEnsureCapacity((emxArray__common *)ii, i3, sizeof(int));
  iLeft = iInf->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    ii->data[i3] = iInf->data[i3] - 1;
  }

  unnamed_idx_0 = (unsigned int)ii->size[0];
  i3 = iInfL->size[0];
  iInfL->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)iInfL, i3, sizeof(int));
  for (nx = 0; nx + 1 <= (int)unnamed_idx_0; nx++) {
    if (ii->data[nx] < 1) {
      iLeft = 1;
    } else {
      iLeft = ii->data[nx];
    }

    iInfL->data[nx] = iLeft;
  }

  i3 = ii->size[0];
  ii->size[0] = iInf->size[0];
  emxEnsureCapacity((emxArray__common *)ii, i3, sizeof(int));
  iLeft = iInf->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    ii->data[i3] = iInf->data[i3] + 1;
  }

  unnamed_idx_0 = (unsigned int)ii->size[0];
  i3 = iInfR->size[0];
  iInfR->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)iInfR, i3, sizeof(int));
  for (nx = 0; nx + 1 <= (int)unnamed_idx_0; nx++) {
    iLeft = ii->data[nx];
    if (!(iLeft < 1000)) {
      iLeft = 1000;
    }

    iInfR->data[nx] = iLeft;
  }

  emxFree_int32_T(&ii);
  i3 = bxPk->size[0] * bxPk->size[1];
  bxPk->size[0] = c->size[0];
  bxPk->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)bxPk, i3, sizeof(double));
  iLeft = c->size[0] << 1;
  for (i3 = 0; i3 < iLeft; i3++) {
    bxPk->data[i3] = 0.0;
  }

  emxInit_real_T1(&r1, 1);
  i3 = r1->size[0];
  r1->size[0] = iLB->size[0];
  emxEnsureCapacity((emxArray__common *)r1, i3, sizeof(double));
  iLeft = iLB->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    r1->data[i3] = (short)(1 + (short)(iLB->data[i3] - 1));
  }

  iLeft = r1->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    bxPk->data[(int)b_idx->data[i3] - 1] = r1->data[i3];
  }

  emxFree_real_T(&r1);
  emxInit_real_T1(&r2, 1);
  i3 = r2->size[0];
  r2->size[0] = iRB->size[0];
  emxEnsureCapacity((emxArray__common *)r2, i3, sizeof(double));
  iLeft = iRB->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    r2->data[i3] = (short)(1 + (short)(iRB->data[i3] - 1));
  }

  iLeft = r2->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    bxPk->data[((int)b_idx->data[i3] + bxPk->size[0]) - 1] = r2->data[i3];
  }

  emxFree_real_T(&r2);
  iLeft = iInf->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    bxPk->data[iInfinite->data[i3] - 1] = 0.5 * (double)(short)((short)((short)
      (iInf->data[i3] - 1) + (short)(iInfL->data[i3] - 1)) + 2);
  }

  iLeft = iInf->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    bxPk->data[(iInfinite->data[i3] + bxPk->size[0]) - 1] = 0.5 * (double)(short)
      ((short)((short)(iInf->data[i3] - 1) + (short)(iInfR->data[i3] - 1)) + 2);
  }

  i3 = byPk->size[0] * byPk->size[1];
  byPk->size[0] = c->size[0];
  byPk->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)byPk, i3, sizeof(double));
  iLeft = c->size[0] << 1;
  for (i3 = 0; i3 < iLeft; i3++) {
    byPk->data[i3] = 0.0;
  }

  iLeft = iLB->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    byPk->data[(int)b_idx->data[i3] - 1] = y[iLB->data[i3] - 1];
  }

  emxFree_int32_T(&iLB);
  iLeft = iRB->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    byPk->data[((int)b_idx->data[i3] + byPk->size[0]) - 1] = y[iRB->data[i3] - 1];
  }

  emxFree_int32_T(&iRB);
  iLeft = iInfL->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    byPk->data[iInfinite->data[i3] - 1] = y[iInfL->data[i3] - 1];
  }

  iLeft = iInfR->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    byPk->data[(iInfinite->data[i3] + byPk->size[0]) - 1] = y[iInfR->data[i3] -
      1];
  }

  i3 = wxPk->size[0] * wxPk->size[1];
  wxPk->size[0] = c->size[0];
  wxPk->size[1] = 2;
  emxEnsureCapacity((emxArray__common *)wxPk, i3, sizeof(double));
  iLeft = c->size[0] << 1;
  for (i3 = 0; i3 < iLeft; i3++) {
    wxPk->data[i3] = 0.0;
  }

  for (i3 = 0; i3 < 2; i3++) {
    iLeft = b_wxPk->size[0];
    for (nx = 0; nx < iLeft; nx++) {
      wxPk->data[((int)b_idx->data[nx] + wxPk->size[0] * i3) - 1] = b_wxPk->
        data[nx + b_wxPk->size[0] * i3];
    }
  }

  emxFree_real_T(&b_idx);
  emxFree_real_T(&b_wxPk);
  iLeft = iInf->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    wxPk->data[iInfinite->data[i3] - 1] = 0.5 * (double)(short)((short)((short)
      (iInf->data[i3] - 1) + (short)(iInfL->data[i3] - 1)) + 2);
  }

  emxFree_int32_T(&iInfL);
  iLeft = iInf->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    wxPk->data[(iInfinite->data[i3] + wxPk->size[0]) - 1] = 0.5 * (double)(short)
      ((short)((short)(iInf->data[i3] - 1) + (short)(iInfR->data[i3] - 1)) + 2);
  }

  emxFree_int32_T(&iInfinite);
  emxFree_int32_T(&iInfR);
  i3 = iPk->size[0];
  iPk->size[0] = c->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, i3, sizeof(int));
  iLeft = c->size[0];
  for (i3 = 0; i3 < iLeft; i3++) {
    iPk->data[i3] = c->data[i3];
  }

  emxFree_int32_T(&c);
}

//
// Arguments    : const double y[1000]
//                int *idx
//                int borderIdx
//                double refHeight
// Return Type  : void
//
static void findLeftIntercept(const double y[1000], int *idx, int borderIdx,
  double refHeight)
{
  while ((*idx >= borderIdx) && (y[*idx - 1] > refHeight)) {
    (*idx)--;
  }
}

//
// Arguments    : const double y[1000]
//                int *idx
//                int borderIdx
//                double refHeight
// Return Type  : void
//
static void findRightIntercept(const double y[1000], int *idx, int borderIdx,
  double refHeight)
{
  while ((*idx <= borderIdx) && (y[*idx - 1] > refHeight)) {
    (*idx)++;
  }
}

//
// Arguments    : const double Yin[1000]
//                double varargin_2
//                emxArray_real_T *Ypk
//                emxArray_real_T *Xpk
// Return Type  : void
//
static void findpeaks(const double Yin[1000], double varargin_2, emxArray_real_T
                      *Ypk, emxArray_real_T *Xpk)
{
  boolean_T needWidth;
  emxArray_int32_T *iPk;
  emxArray_int32_T *idx;
  emxArray_int32_T *iInfinite;
  emxArray_int32_T *iInflect;
  int k;
  int nPk;
  double base;
  emxArray_real_T *bPk;
  emxArray_int32_T *b_iPk;
  emxArray_real_T *bxPk;
  emxArray_real_T *byPk;
  emxArray_real_T *wxPk;
  emxArray_int32_T *y;
  int n;
  emxArray_int32_T *b_idx;
  emxArray_int32_T *c_iPk;
  emxArray_real_T *r0;
  needWidth = (varargin_2 > 0.0);
  emxInit_int32_T(&iPk, 1);
  emxInit_int32_T(&idx, 1);
  emxInit_int32_T(&iInfinite, 1);
  emxInit_int32_T(&iInflect, 1);
  b_getAllPeaksCodegen(Yin, idx, iInfinite, iInflect);
  k = iPk->size[0];
  iPk->size[0] = idx->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, k, sizeof(int));
  nPk = 0;
  for (k = 0; k + 1 <= idx->size[0]; k++) {
    if (Yin[idx->data[k] - 1] > rtMinusInf) {
      if ((Yin[idx->data[k] - 2] > Yin[idx->data[k]]) || rtIsNaN(Yin[idx->data[k]]))
      {
        base = Yin[idx->data[k] - 2];
      } else {
        base = Yin[idx->data[k]];
      }

      if (Yin[idx->data[k] - 1] - base >= 0.0) {
        nPk++;
        iPk->data[nPk - 1] = idx->data[k];
      }
    }
  }

  k = iPk->size[0];
  if (1 > nPk) {
    iPk->size[0] = 0;
  } else {
    iPk->size[0] = nPk;
  }

  emxEnsureCapacity((emxArray__common *)iPk, k, sizeof(int));
  emxInit_real_T1(&bPk, 1);
  if (needWidth) {
    emxInit_real_T(&bxPk, 2);
    emxInit_real_T(&byPk, 2);
    emxInit_real_T(&wxPk, 2);
    findExtents(Yin, iPk, idx, iInfinite, iInflect, varargin_2, bPk, bxPk, byPk,
                wxPk);
    emxFree_real_T(&wxPk);
    emxFree_real_T(&byPk);
    emxFree_real_T(&bxPk);
  } else {
    emxInit_int32_T(&b_iPk, 1);
    k = b_iPk->size[0];
    b_iPk->size[0] = iPk->size[0];
    emxEnsureCapacity((emxArray__common *)b_iPk, k, sizeof(int));
    nPk = iPk->size[0];
    for (k = 0; k < nPk; k++) {
      b_iPk->data[k] = iPk->data[k];
    }

    combinePeaks(b_iPk, iInfinite, iPk);
    emxFree_int32_T(&b_iPk);
  }

  emxFree_int32_T(&iInflect);
  emxFree_int32_T(&iInfinite);
  emxInit_int32_T1(&y, 2);
  nPk = iPk->size[0];
  if (nPk < 1) {
    n = 0;
  } else {
    n = nPk;
  }

  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = n;
  emxEnsureCapacity((emxArray__common *)y, k, sizeof(int));
  if (n > 0) {
    y->data[0] = 1;
    nPk = 1;
    for (k = 2; k <= n; k++) {
      nPk++;
      y->data[k - 1] = nPk;
    }
  }

  k = idx->size[0];
  idx->size[0] = y->size[1];
  emxEnsureCapacity((emxArray__common *)idx, k, sizeof(int));
  nPk = y->size[1];
  for (k = 0; k < nPk; k++) {
    idx->data[k] = y->data[y->size[0] * k];
  }

  emxFree_int32_T(&y);
  if (idx->size[0] > 1000) {
    emxInit_int32_T(&b_idx, 1);
    k = b_idx->size[0];
    b_idx->size[0] = 1000;
    emxEnsureCapacity((emxArray__common *)b_idx, k, sizeof(int));
    for (k = 0; k < 1000; k++) {
      b_idx->data[k] = idx->data[k];
    }

    k = idx->size[0];
    idx->size[0] = b_idx->size[0];
    emxEnsureCapacity((emxArray__common *)idx, k, sizeof(int));
    nPk = b_idx->size[0];
    for (k = 0; k < nPk; k++) {
      idx->data[k] = b_idx->data[k];
    }

    emxFree_int32_T(&b_idx);
  }

  emxInit_int32_T(&c_iPk, 1);
  k = c_iPk->size[0];
  c_iPk->size[0] = idx->size[0];
  emxEnsureCapacity((emxArray__common *)c_iPk, k, sizeof(int));
  nPk = idx->size[0];
  for (k = 0; k < nPk; k++) {
    c_iPk->data[k] = iPk->data[idx->data[k] - 1];
  }

  emxFree_int32_T(&idx);
  k = iPk->size[0];
  iPk->size[0] = c_iPk->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, k, sizeof(int));
  nPk = c_iPk->size[0];
  for (k = 0; k < nPk; k++) {
    iPk->data[k] = c_iPk->data[k];
  }

  emxFree_int32_T(&c_iPk);
  if (needWidth) {
    k = Ypk->size[0];
    Ypk->size[0] = iPk->size[0];
    emxEnsureCapacity((emxArray__common *)Ypk, k, sizeof(double));
    nPk = iPk->size[0];
    for (k = 0; k < nPk; k++) {
      Ypk->data[k] = Yin[iPk->data[k] - 1];
    }

    emxInit_real_T1(&r0, 1);
    k = r0->size[0];
    r0->size[0] = iPk->size[0];
    emxEnsureCapacity((emxArray__common *)r0, k, sizeof(double));
    nPk = iPk->size[0];
    for (k = 0; k < nPk; k++) {
      r0->data[k] = (short)(1 + (short)(iPk->data[k] - 1));
    }

    k = Xpk->size[0];
    Xpk->size[0] = r0->size[0];
    emxEnsureCapacity((emxArray__common *)Xpk, k, sizeof(double));
    nPk = r0->size[0];
    for (k = 0; k < nPk; k++) {
      Xpk->data[k] = r0->data[k];
    }

    emxFree_real_T(&r0);
  } else {
    k = Ypk->size[0];
    Ypk->size[0] = iPk->size[0];
    emxEnsureCapacity((emxArray__common *)Ypk, k, sizeof(double));
    nPk = iPk->size[0];
    for (k = 0; k < nPk; k++) {
      Ypk->data[k] = Yin[iPk->data[k] - 1];
    }

    emxInit_real_T1(&r0, 1);
    k = r0->size[0];
    r0->size[0] = iPk->size[0];
    emxEnsureCapacity((emxArray__common *)r0, k, sizeof(double));
    nPk = iPk->size[0];
    for (k = 0; k < nPk; k++) {
      r0->data[k] = (short)(1 + (short)(iPk->data[k] - 1));
    }

    k = Xpk->size[0];
    Xpk->size[0] = r0->size[0];
    emxEnsureCapacity((emxArray__common *)Xpk, k, sizeof(double));
    nPk = r0->size[0];
    for (k = 0; k < nPk; k++) {
      Xpk->data[k] = r0->data[k];
    }

    emxFree_real_T(&r0);
  }

  emxFree_real_T(&bPk);
  emxFree_int32_T(&iPk);
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
// Arguments    : const double y[996]
//                emxArray_int32_T *iPk
//                emxArray_int32_T *iInf
//                emxArray_int32_T *iInflect
// Return Type  : void
//
static void getAllPeaksCodegen(const double y[996], emxArray_int32_T *iPk,
  emxArray_int32_T *iInf, emxArray_int32_T *iInflect)
{
  int kfirst;
  int nPk;
  int nInf;
  int nInflect;
  char dir;
  double ykfirst;
  boolean_T isinfykfirst;
  int k;
  double yk;
  boolean_T isinfyk;
  char previousdir;
  int i1;
  kfirst = iPk->size[0];
  iPk->size[0] = 996;
  emxEnsureCapacity((emxArray__common *)iPk, kfirst, sizeof(int));
  kfirst = iInf->size[0];
  iInf->size[0] = 996;
  emxEnsureCapacity((emxArray__common *)iInf, kfirst, sizeof(int));
  kfirst = iInflect->size[0];
  iInflect->size[0] = 996;
  emxEnsureCapacity((emxArray__common *)iInflect, kfirst, sizeof(int));
  nPk = 0;
  nInf = 0;
  nInflect = -1;
  dir = 'n';
  kfirst = 0;
  ykfirst = rtInf;
  isinfykfirst = true;
  for (k = 0; k < 996; k++) {
    yk = y[k];
    if (rtIsNaN(y[k])) {
      yk = rtInf;
      isinfyk = true;
    } else if (rtIsInf(y[k]) && (y[k] > 0.0)) {
      isinfyk = true;
      nInf++;
      iInf->data[nInf - 1] = k + 1;
    } else {
      isinfyk = false;
    }

    if (yk != ykfirst) {
      previousdir = dir;
      if (isinfyk || isinfykfirst) {
        dir = 'n';
        if (kfirst >= 1) {
          nInflect++;
          iInflect->data[nInflect] = kfirst;
        }
      } else if (yk < ykfirst) {
        dir = 'd';
        if ('d' != previousdir) {
          nInflect++;
          iInflect->data[nInflect] = kfirst;
          if (previousdir == 'i') {
            nPk++;
            iPk->data[nPk - 1] = kfirst;
          }
        }
      } else {
        dir = 'i';
        if ('i' != previousdir) {
          nInflect++;
          iInflect->data[nInflect] = kfirst;
        }
      }

      ykfirst = yk;
      kfirst = k + 1;
      isinfykfirst = isinfyk;
    }
  }

  if ((!isinfykfirst) && ((nInflect + 1 == 0) || (iInflect->data[nInflect] < 996)))
  {
    nInflect++;
    iInflect->data[nInflect] = 996;
  }

  kfirst = iPk->size[0];
  if (1 > nPk) {
    iPk->size[0] = 0;
  } else {
    iPk->size[0] = nPk;
  }

  emxEnsureCapacity((emxArray__common *)iPk, kfirst, sizeof(int));
  kfirst = iInf->size[0];
  if (1 > nInf) {
    iInf->size[0] = 0;
  } else {
    iInf->size[0] = nInf;
  }

  emxEnsureCapacity((emxArray__common *)iInf, kfirst, sizeof(int));
  kfirst = iInflect->size[0];
  if (1 > nInflect + 1) {
    i1 = -1;
  } else {
    i1 = nInflect;
  }

  iInflect->size[0] = i1 + 1;
  emxEnsureCapacity((emxArray__common *)iInflect, kfirst, sizeof(int));
}

//
// Arguments    : const double yTemp[1000]
//                const emxArray_int32_T *iPeak
//                const emxArray_int32_T *iFinite
//                const emxArray_int32_T *iInflect
//                emxArray_int32_T *iBase
//                emxArray_int32_T *iSaddle
// Return Type  : void
//
static void getLeftBase(const double yTemp[1000], const emxArray_int32_T *iPeak,
  const emxArray_int32_T *iFinite, const emxArray_int32_T *iInflect,
  emxArray_int32_T *iBase, emxArray_int32_T *iSaddle)
{
  unsigned int unnamed_idx_0;
  int n;
  int i;
  emxArray_real_T *peak;
  emxArray_real_T *valley;
  emxArray_int32_T *iValley;
  int j;
  int k;
  double v;
  int iv;
  double p;
  int isv;
  unnamed_idx_0 = (unsigned int)iPeak->size[0];
  n = iBase->size[0];
  iBase->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)iBase, n, sizeof(int));
  i = (int)unnamed_idx_0;
  for (n = 0; n < i; n++) {
    iBase->data[n] = 0;
  }

  unnamed_idx_0 = (unsigned int)iPeak->size[0];
  n = iSaddle->size[0];
  iSaddle->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)iSaddle, n, sizeof(int));
  i = (int)unnamed_idx_0;
  for (n = 0; n < i; n++) {
    iSaddle->data[n] = 0;
  }

  emxInit_real_T1(&peak, 1);
  unnamed_idx_0 = (unsigned int)iFinite->size[0];
  n = peak->size[0];
  peak->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)peak, n, sizeof(double));
  i = (int)unnamed_idx_0;
  for (n = 0; n < i; n++) {
    peak->data[n] = 0.0;
  }

  emxInit_real_T1(&valley, 1);
  unnamed_idx_0 = (unsigned int)iFinite->size[0];
  n = valley->size[0];
  valley->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)valley, n, sizeof(double));
  i = (int)unnamed_idx_0;
  for (n = 0; n < i; n++) {
    valley->data[n] = 0.0;
  }

  emxInit_int32_T(&iValley, 1);
  unnamed_idx_0 = (unsigned int)iFinite->size[0];
  n = iValley->size[0];
  iValley->size[0] = (int)unnamed_idx_0;
  emxEnsureCapacity((emxArray__common *)iValley, n, sizeof(int));
  i = (int)unnamed_idx_0;
  for (n = 0; n < i; n++) {
    iValley->data[n] = 0;
  }

  n = -1;
  i = 0;
  j = 0;
  k = 0;
  v = rtNaN;
  iv = 1;
  while (k + 1 <= iPeak->size[0]) {
    while (iInflect->data[i] != iFinite->data[j]) {
      v = yTemp[iInflect->data[i] - 1];
      iv = iInflect->data[i];
      if (rtIsNaN(yTemp[iInflect->data[i] - 1])) {
        n = -1;
      } else {
        while ((n + 1 > 0) && (valley->data[n] > v)) {
          n--;
        }
      }

      i++;
    }

    p = yTemp[iInflect->data[i] - 1];
    while ((n + 1 > 0) && (peak->data[n] < p)) {
      if (valley->data[n] < v) {
        v = valley->data[n];
        iv = iValley->data[n];
      }

      n--;
    }

    isv = iv;
    while ((n + 1 > 0) && (peak->data[n] <= p)) {
      if (valley->data[n] < v) {
        v = valley->data[n];
        iv = iValley->data[n];
      }

      n--;
    }

    n++;
    peak->data[n] = yTemp[iInflect->data[i] - 1];
    valley->data[n] = v;
    iValley->data[n] = iv;
    if (iInflect->data[i] == iPeak->data[k]) {
      iBase->data[k] = iv;
      iSaddle->data[k] = isv;
      k++;
    }

    i++;
    j++;
  }

  emxFree_int32_T(&iValley);
  emxFree_real_T(&valley);
  emxFree_real_T(&peak);
}

//
// Arguments    : const double yTemp[1000]
//                const emxArray_int32_T *iPk
//                const emxArray_int32_T *iFin
//                const emxArray_int32_T *iInflect
//                emxArray_real_T *peakBase
//                emxArray_int32_T *iLeftSaddle
//                emxArray_int32_T *iRightSaddle
// Return Type  : void
//
static void getPeakBase(const double yTemp[1000], const emxArray_int32_T *iPk,
  const emxArray_int32_T *iFin, const emxArray_int32_T *iInflect,
  emxArray_real_T *peakBase, emxArray_int32_T *iLeftSaddle, emxArray_int32_T
  *iRightSaddle)
{
  emxArray_int32_T *iLeftBase;
  emxArray_int32_T *x;
  int md2;
  int m;
  int i;
  emxArray_int32_T *b_x;
  int xtmp;
  emxArray_int32_T *c_x;
  emxArray_int32_T *iRightBase;
  double extremum;
  emxInit_int32_T(&iLeftBase, 1);
  emxInit_int32_T(&x, 1);
  getLeftBase(yTemp, iPk, iFin, iInflect, iLeftBase, iLeftSaddle);
  md2 = x->size[0];
  x->size[0] = iPk->size[0];
  emxEnsureCapacity((emxArray__common *)x, md2, sizeof(int));
  m = iPk->size[0];
  for (md2 = 0; md2 < m; md2++) {
    x->data[md2] = iPk->data[md2];
  }

  m = iPk->size[0];
  md2 = iPk->size[0] >> 1;
  for (i = 1; i <= md2; i++) {
    xtmp = x->data[i - 1];
    x->data[i - 1] = x->data[m - i];
    x->data[m - i] = xtmp;
  }

  emxInit_int32_T(&b_x, 1);
  md2 = b_x->size[0];
  b_x->size[0] = iFin->size[0];
  emxEnsureCapacity((emxArray__common *)b_x, md2, sizeof(int));
  m = iFin->size[0];
  for (md2 = 0; md2 < m; md2++) {
    b_x->data[md2] = iFin->data[md2];
  }

  m = iFin->size[0];
  md2 = iFin->size[0] >> 1;
  for (i = 1; i <= md2; i++) {
    xtmp = b_x->data[i - 1];
    b_x->data[i - 1] = b_x->data[m - i];
    b_x->data[m - i] = xtmp;
  }

  emxInit_int32_T(&c_x, 1);
  md2 = c_x->size[0];
  c_x->size[0] = iInflect->size[0];
  emxEnsureCapacity((emxArray__common *)c_x, md2, sizeof(int));
  m = iInflect->size[0];
  for (md2 = 0; md2 < m; md2++) {
    c_x->data[md2] = iInflect->data[md2];
  }

  m = iInflect->size[0];
  md2 = iInflect->size[0] >> 1;
  for (i = 1; i <= md2; i++) {
    xtmp = c_x->data[i - 1];
    c_x->data[i - 1] = c_x->data[m - i];
    c_x->data[m - i] = xtmp;
  }

  emxInit_int32_T(&iRightBase, 1);
  getLeftBase(yTemp, x, b_x, c_x, iRightBase, iRightSaddle);
  m = iRightBase->size[0];
  md2 = iRightBase->size[0] >> 1;
  i = 1;
  emxFree_int32_T(&c_x);
  emxFree_int32_T(&b_x);
  emxFree_int32_T(&x);
  while (i <= md2) {
    xtmp = iRightBase->data[i - 1];
    iRightBase->data[i - 1] = iRightBase->data[m - i];
    iRightBase->data[m - i] = xtmp;
    i++;
  }

  m = iRightSaddle->size[0];
  md2 = iRightSaddle->size[0] >> 1;
  for (i = 1; i <= md2; i++) {
    xtmp = iRightSaddle->data[i - 1];
    iRightSaddle->data[i - 1] = iRightSaddle->data[m - i];
    iRightSaddle->data[m - i] = xtmp;
  }

  if (iLeftBase->size[0] <= iRightBase->size[0]) {
    i = iLeftBase->size[0];
  } else {
    i = iRightBase->size[0];
  }

  md2 = peakBase->size[0];
  peakBase->size[0] = i;
  emxEnsureCapacity((emxArray__common *)peakBase, md2, sizeof(double));
  for (m = 0; m + 1 <= i; m++) {
    if ((yTemp[iLeftBase->data[m] - 1] > yTemp[iRightBase->data[m] - 1]) ||
        rtIsNaN(yTemp[iRightBase->data[m] - 1])) {
      extremum = yTemp[iLeftBase->data[m] - 1];
    } else {
      extremum = yTemp[iRightBase->data[m] - 1];
    }

    peakBase->data[m] = extremum;
  }

  emxFree_int32_T(&iLeftBase);
  emxFree_int32_T(&iRightBase);
}

//
// Arguments    : double xa
//                double xb
//                double ya
//                double yb
//                double yc
//                double bc
// Return Type  : double
//
static double linterp(double xa, double xb, double ya, double yb, double yc,
                      double bc)
{
  double xc;
  xc = xa + (xb - xa) * (0.5 * (yc + bc) - ya) / (yb - ya);
  if (rtIsNaN(xc)) {
    if (rtIsInf(bc)) {
      xc = 0.5 * (xa + xb);
    } else {
      xc = xb;
    }
  }

  return xc;
}

//
// Arguments    : const double x[1000]
// Return Type  : double
//
static double mean(const double x[1000])
{
  double y;
  int k;
  y = x[0];
  for (k = 0; k < 999; k++) {
    y += x[k + 1];
  }

  y /= 1000.0;
  return y;
}

//
// Arguments    : const double a[1000]
//                double y[1000]
// Return Type  : void
//
static void power(const double a[1000], double y[1000])
{
  int k;
  for (k = 0; k < 1000; k++) {
    y[k] = a[k] * a[k];
  }
}

//
// get_hr_rr This function uses a variant of the Pan-Tompkins Algorithm to
// detect ECG QRS features from low frequency data:
//  Input: X (raw data, floats or doubles (preferred);
//  Outputs: Y .. (TODO);
//  0. Get X:
// Arguments    : const double X_raw[1000]
//                const double Z_resp[1000]
//                double *HR_mean
//                double *Respiratory_rate
// Return Type  : void
//
void get_hr_rr(const double X_raw[1000], const double Z_resp[1000], double
               *HR_mean, double *Respiratory_rate)
{
  float fv0[1000];
  int i;
  double X[1000];
  double mtmp;
  int k;
  double X0[1000];
  double X2[1000];
  double X4[996];
  int n;
  boolean_T exitg1;
  emxArray_int32_T *iPk;
  emxArray_int32_T *idx;
  emxArray_int32_T *iFinite;
  emxArray_int32_T *iInfinite;
  emxArray_int32_T *y;
  double base;
  emxArray_int32_T *b_idx;
  emxArray_real_T *dist;
  double b_y[1018];
  double dv0[4];
  static const double dv1[4] = { 0.980094256649532, -2.9402827699485949,
    2.9402827699485949, -0.980094256649532 };

  double dv2[4];
  static const double dv3[4] = { 1.0, -2.9597882911224671, 2.9203810101573811,
    -0.960584751916406 };

  double c_y[1018];
  double a[3];
  static const double b_a[3] = { -0.98009425669876293, 1.9601885133955457,
    -0.9800942566968226 };

  static const double dv4[4] = { 0.005647548524668, 0.016942645574005,
    0.016942645574005, 0.005647548524668 };

  static const double dv5[4] = { 1.0, -2.2006359767013288, 1.6907151407924379,
    -0.444898775893762 };

  static const double c_a[3] = { 0.99435245147530726, -1.2232261707999721,
    0.45054632441841885 };

  emxArray_real_T *P;
  emxArray_real_T *L;
  ecg_filt_rescale(X_raw, fv0);
  for (i = 0; i < 1000; i++) {
    X[i] = fv0[i];
  }

  //  1. Remove Mean:
  mtmp = mean(X);
  for (k = 0; k < 1000; k++) {
    X[k] -= mtmp;
  }

  //  2. Remove baseline drift (Butterworth, high pass: [b, a] = butter(3, 0.5*2/62.5, 'high');  
  //  @ 5 hz
  //  b = [0.601580928135908,-1.804742784407724,1.804742784407724,-0.601580928135908]; 
  //  a = [1,-2.003797477370017,1.447054019489380,-0.361795928227867];
  //  @ Equivalent to: [b, a] = butter(3, 2*2/31.25, 'high');
  b_filtfilt(X, X0);

  //  2.1 Divide by max value to acheive peak of 1
  //  X0 = X0/ max( abs( X0 ) );
  //  3. Derivative Filter:
  c_filtfilt(X0, X2);

  //  X2 = X2/max(X2);
  //  4. Square to accentuate ecg waveformsS
  power(X2, X);

  //  5. Convolution filtering to smooth peaks:
  //  conv_filter = ones(1, round(0.150*62.5)) / round(0.150*62.5);
  //  Custom: Use Hann Window as filter - better defined peaks.
  // hannWin
  memset(&X4[0], 0, 996U * sizeof(double));
  for (k = 0; k < 5; k++) {
    for (i = 0; i < 996; i++) {
      X4[i] += X[(i - k) + 4];
    }
  }

  // rescale_minmax
  //  Extract fiducial markers (peaks);
  i = 1;
  mtmp = X4[0];
  if (rtIsNaN(X4[0])) {
    n = 2;
    exitg1 = false;
    while ((!exitg1) && (n < 997)) {
      i = n;
      if (!rtIsNaN(X4[n - 1])) {
        mtmp = X4[n - 1];
        exitg1 = true;
      } else {
        n++;
      }
    }
  }

  if (i < 996) {
    while (i + 1 < 997) {
      if (X4[i] > mtmp) {
        mtmp = X4[i];
      }

      i++;
    }
  }

  emxInit_int32_T(&iPk, 1);
  emxInit_int32_T(&idx, 1);
  emxInit_int32_T(&iFinite, 1);
  emxInit_int32_T(&iInfinite, 1);

  //  [Peaks, Locations] = findpeaks(X4,'MinPeakProminence',0.3656836669*max_peak); 
  mtmp *= 0.1156836669;
  getAllPeaksCodegen(X4, iFinite, iInfinite, idx);
  k = iPk->size[0];
  iPk->size[0] = iFinite->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, k, sizeof(int));
  i = 0;
  for (k = 0; k + 1 <= iFinite->size[0]; k++) {
    if (X4[iFinite->data[k] - 1] > mtmp) {
      if ((X4[iFinite->data[k] - 2] > X4[iFinite->data[k]]) || rtIsNaN
          (X4[iFinite->data[k]])) {
        base = X4[iFinite->data[k] - 2];
      } else {
        base = X4[iFinite->data[k]];
      }

      if (X4[iFinite->data[k] - 1] - base >= 0.0) {
        i++;
        iPk->data[i - 1] = iFinite->data[k];
      }
    }
  }

  emxInit_int32_T1(&y, 2);
  k = iPk->size[0];
  if (1 > i) {
    iPk->size[0] = 0;
  } else {
    iPk->size[0] = i;
  }

  emxEnsureCapacity((emxArray__common *)iPk, k, sizeof(int));
  combinePeaks(iPk, iInfinite, iFinite);
  i = iFinite->size[0];
  emxFree_int32_T(&iInfinite);
  if (i < 1) {
    n = 0;
  } else {
    n = i;
  }

  k = y->size[0] * y->size[1];
  y->size[0] = 1;
  y->size[1] = n;
  emxEnsureCapacity((emxArray__common *)y, k, sizeof(int));
  if (n > 0) {
    y->data[0] = 1;
    i = 1;
    for (k = 2; k <= n; k++) {
      i++;
      y->data[k - 1] = i;
    }
  }

  k = idx->size[0];
  idx->size[0] = y->size[1];
  emxEnsureCapacity((emxArray__common *)idx, k, sizeof(int));
  i = y->size[1];
  for (k = 0; k < i; k++) {
    idx->data[k] = y->data[y->size[0] * k];
  }

  emxFree_int32_T(&y);
  if (idx->size[0] > 996) {
    emxInit_int32_T(&b_idx, 1);
    k = b_idx->size[0];
    b_idx->size[0] = 996;
    emxEnsureCapacity((emxArray__common *)b_idx, k, sizeof(int));
    for (k = 0; k < 996; k++) {
      b_idx->data[k] = idx->data[k];
    }

    k = idx->size[0];
    idx->size[0] = b_idx->size[0];
    emxEnsureCapacity((emxArray__common *)idx, k, sizeof(int));
    i = b_idx->size[0];
    for (k = 0; k < i; k++) {
      idx->data[k] = b_idx->data[k];
    }

    emxFree_int32_T(&b_idx);
  }

  k = iPk->size[0];
  iPk->size[0] = idx->size[0];
  emxEnsureCapacity((emxArray__common *)iPk, k, sizeof(int));
  i = idx->size[0];
  for (k = 0; k < i; k++) {
    iPk->data[k] = iFinite->data[idx->data[k] - 1];
  }

  emxFree_int32_T(&iFinite);
  emxInit_real_T(&dist, 2);

  //  Output = [Peaks; Locations];
  //  calculate distance between fiducial markers:
  n = idx->size[0] - 1;
  k = dist->size[0] * dist->size[1];
  dist->size[0] = 1;
  dist->size[1] = n;
  emxEnsureCapacity((emxArray__common *)dist, k, sizeof(double));
  for (k = 0; k < n; k++) {
    dist->data[k] = 0.0;
  }

  n = idx->size[0];
  i = 0;
  emxFree_int32_T(&idx);
  while (i <= n - 2) {
    dist->data[i] = iPk->data[i + 1] - iPk->data[i];
    i++;
  }

  emxFree_int32_T(&iPk);
  mtmp = b_mean(dist);

  //  average in seconds:
  *HR_mean = 60.0 / (mtmp / Fs);

  // %% RESPIRATORY RATE FROM IMPEDANCE %%%
  //  HPF: [b, a] = butter(3, 0.1*2/31.25, 'high');
  mtmp = 2.0 * Z_resp[0];
  base = 2.0 * Z_resp[999];
  for (k = 0; k < 9; k++) {
    b_y[k] = mtmp - Z_resp[9 - k];
  }

  memcpy(&b_y[9], &Z_resp[0], 1000U * sizeof(double));
  for (k = 0; k < 9; k++) {
    b_y[k + 1009] = base - Z_resp[998 - k];
  }

  for (k = 0; k < 4; k++) {
    dv0[k] = dv1[k];
    dv2[k] = dv3[k];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * b_y[0];
  }

  memcpy(&c_y[0], &b_y[0], 1018U * sizeof(double));
  filter(dv0, dv2, c_y, a, b_y);
  flipud(b_y);
  for (k = 0; k < 4; k++) {
    dv0[k] = dv1[k];
    dv2[k] = dv3[k];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * b_y[0];
  }

  memcpy(&c_y[0], &b_y[0], 1018U * sizeof(double));
  filter(dv0, dv2, c_y, a, b_y);
  flipud(b_y);

  //  LPF: [b, a] = butter(3, 2*2/31.25, 'low');
  mtmp = 2.0 * b_y[9];
  base = 2.0 * b_y[1008];
  for (k = 0; k < 9; k++) {
    c_y[k] = mtmp - b_y[18 - k];
  }

  memcpy(&c_y[9], &b_y[9], 1000U * sizeof(double));
  for (k = 0; k < 9; k++) {
    c_y[k + 1009] = base - b_y[1007 - k];
  }

  memcpy(&b_y[0], &c_y[0], 1018U * sizeof(double));
  for (k = 0; k < 4; k++) {
    dv0[k] = dv4[k];
    dv2[k] = dv5[k];
  }

  for (i = 0; i < 3; i++) {
    a[i] = c_a[i] * b_y[0];
  }

  memcpy(&c_y[0], &b_y[0], 1018U * sizeof(double));
  filter(dv0, dv2, c_y, a, b_y);
  flipud(b_y);
  for (k = 0; k < 4; k++) {
    dv0[k] = dv4[k];
    dv2[k] = dv5[k];
  }

  for (i = 0; i < 3; i++) {
    a[i] = c_a[i] * b_y[0];
  }

  memcpy(&c_y[0], &b_y[0], 1018U * sizeof(double));
  filter(dv0, dv2, c_y, a, b_y);
  flipud(b_y);

  //  Downsample
  i = 1;
  mtmp = b_y[9];
  if (rtIsNaN(b_y[9])) {
    n = 2;
    exitg1 = false;
    while ((!exitg1) && (n < 1001)) {
      i = n;
      if (!rtIsNaN(b_y[n + 8])) {
        mtmp = b_y[n + 8];
        exitg1 = true;
      } else {
        n++;
      }
    }
  }

  if (i < 1000) {
    for (n = i + 9; n - 8 < 1001; n++) {
      if (b_y[n] > mtmp) {
        mtmp = b_y[n];
      }
    }
  }

  emxInit_real_T1(&P, 1);
  emxInit_real_T1(&L, 1);
  findpeaks(*(double (*)[1000])&b_y[9], 0.5 * mtmp, P, L);
  k = dist->size[0] * dist->size[1];
  dist->size[0] = 1;
  dist->size[1] = P->size[0] - 1;
  emxEnsureCapacity((emxArray__common *)dist, k, sizeof(double));
  i = P->size[0] - 1;
  emxFree_real_T(&P);
  for (k = 0; k < i; k++) {
    dist->data[k] = 0.0;
  }

  for (i = 0; i <= L->size[0] - 2; i++) {
    dist->data[i] = L->data[i + 1] - L->data[i];
  }

  emxFree_real_T(&L);
  mtmp = b_mean(dist);

  //  average in seconds:
  *Respiratory_rate = 60.0 / (mtmp / Fs);
  emxFree_real_T(&dist);
}

//
// Arguments    : void
// Return Type  : void
//
void get_hr_rr_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void get_hr_rr_terminate()
{
  // (no terminate code required)
}

//
// File trailer for get_hr_rr.cpp
//
// [EOF]
//
