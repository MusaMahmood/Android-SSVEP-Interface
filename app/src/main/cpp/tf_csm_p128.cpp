//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: tf_csm_p128.cpp
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 25-Apr-2018 14:59:57
//

// Include Files
#include "rt_nonfinite.h"
#include "tf_csm_p128.h"

// Function Declarations
static void b_repmat(const double a[2], double b[256]);
static void fft(const double x[256], creal_T y[256]);
static void mean(const double x[256], double y[2]);
static void power(const double a[128], double y[128]);
static void repmat(const double a[128], double b[256]);
static void rescale_minmax(const float X[64], float Y[64]);
static double sum(const double x[128]);
static void tf_csm_welch(const double signals_data[], const int signals_size[2],
  const double window[128], float CSM[192]);

// Function Definitions

//
// Arguments    : const double a[2]
//                double b[256]
// Return Type  : void
//
static void b_repmat(const double a[2], double b[256])
{
  int jcol;
  int ibmat;
  int itilerow;
  for (jcol = 0; jcol < 2; jcol++) {
    ibmat = jcol << 7;
    for (itilerow = 0; itilerow < 128; itilerow++) {
      b[ibmat + itilerow] = a[jcol];
    }
  }
}

//
// Arguments    : const double x[256]
//                creal_T y[256]
// Return Type  : void
//
static void fft(const double x[256], creal_T y[256])
{
  int k;
  int ix;
  int ju;
  int iy;
  int i;
  creal_T rwork[128];
  boolean_T tst;
  double temp_re;
  double temp_im;
  int iheight;
  int istart;
  int j;
  double twid_re;
  static const double dv4[65] = { 1.0, 0.99879545620517241, 0.99518472667219693,
    0.989176509964781, 0.98078528040323043, 0.970031253194544,
    0.95694033573220882, 0.94154406518302081, 0.92387953251128674,
    0.90398929312344334, 0.881921264348355, 0.85772861000027212,
    0.83146961230254524, 0.80320753148064494, 0.773010453362737,
    0.74095112535495922, 0.70710678118654757, 0.67155895484701833,
    0.63439328416364549, 0.59569930449243336, 0.55557023301960218,
    0.51410274419322166, 0.47139673682599764, 0.42755509343028208,
    0.38268343236508978, 0.33688985339222005, 0.29028467725446233,
    0.24298017990326387, 0.19509032201612825, 0.14673047445536175,
    0.0980171403295606, 0.049067674327418015, 0.0, -0.049067674327418015,
    -0.0980171403295606, -0.14673047445536175, -0.19509032201612825,
    -0.24298017990326387, -0.29028467725446233, -0.33688985339222005,
    -0.38268343236508978, -0.42755509343028208, -0.47139673682599764,
    -0.51410274419322166, -0.55557023301960218, -0.59569930449243336,
    -0.63439328416364549, -0.67155895484701833, -0.70710678118654757,
    -0.74095112535495922, -0.773010453362737, -0.80320753148064494,
    -0.83146961230254524, -0.85772861000027212, -0.881921264348355,
    -0.90398929312344334, -0.92387953251128674, -0.94154406518302081,
    -0.95694033573220882, -0.970031253194544, -0.98078528040323043,
    -0.989176509964781, -0.99518472667219693, -0.99879545620517241, -1.0 };

  double twid_im;
  static const double dv5[65] = { 0.0, -0.049067674327418015,
    -0.0980171403295606, -0.14673047445536175, -0.19509032201612825,
    -0.24298017990326387, -0.29028467725446233, -0.33688985339222005,
    -0.38268343236508978, -0.42755509343028208, -0.47139673682599764,
    -0.51410274419322166, -0.55557023301960218, -0.59569930449243336,
    -0.63439328416364549, -0.67155895484701833, -0.70710678118654757,
    -0.74095112535495922, -0.773010453362737, -0.80320753148064494,
    -0.83146961230254524, -0.85772861000027212, -0.881921264348355,
    -0.90398929312344334, -0.92387953251128674, -0.94154406518302081,
    -0.95694033573220882, -0.970031253194544, -0.98078528040323043,
    -0.989176509964781, -0.99518472667219693, -0.99879545620517241, -1.0,
    -0.99879545620517241, -0.99518472667219693, -0.989176509964781,
    -0.98078528040323043, -0.970031253194544, -0.95694033573220882,
    -0.94154406518302081, -0.92387953251128674, -0.90398929312344334,
    -0.881921264348355, -0.85772861000027212, -0.83146961230254524,
    -0.80320753148064494, -0.773010453362737, -0.74095112535495922,
    -0.70710678118654757, -0.67155895484701833, -0.63439328416364549,
    -0.59569930449243336, -0.55557023301960218, -0.51410274419322166,
    -0.47139673682599764, -0.42755509343028208, -0.38268343236508978,
    -0.33688985339222005, -0.29028467725446233, -0.24298017990326387,
    -0.19509032201612825, -0.14673047445536175, -0.0980171403295606,
    -0.049067674327418015, -0.0 };

  int ihi;
  for (k = 0; k < 2; k++) {
    ix = k << 7;
    ju = 0;
    iy = 0;
    for (i = 0; i < 127; i++) {
      rwork[iy].re = x[ix];
      rwork[iy].im = 0.0;
      iy = 128;
      tst = true;
      while (tst) {
        iy >>= 1;
        ju ^= iy;
        tst = ((ju & iy) == 0);
      }

      iy = ju;
      ix++;
    }

    rwork[iy].re = x[ix];
    rwork[iy].im = 0.0;
    for (i = 0; i <= 127; i += 2) {
      temp_re = rwork[i + 1].re;
      temp_im = rwork[i + 1].im;
      rwork[i + 1].re = rwork[i].re - rwork[i + 1].re;
      rwork[i + 1].im = rwork[i].im - rwork[i + 1].im;
      rwork[i].re += temp_re;
      rwork[i].im += temp_im;
    }

    iy = 2;
    ix = 4;
    ju = 32;
    iheight = 125;
    while (ju > 0) {
      for (i = 0; i < iheight; i += ix) {
        temp_re = rwork[i + iy].re;
        temp_im = rwork[i + iy].im;
        rwork[i + iy].re = rwork[i].re - temp_re;
        rwork[i + iy].im = rwork[i].im - temp_im;
        rwork[i].re += temp_re;
        rwork[i].im += temp_im;
      }

      istart = 1;
      for (j = ju; j < 64; j += ju) {
        twid_re = dv4[j];
        twid_im = dv5[j];
        i = istart;
        ihi = istart + iheight;
        while (i < ihi) {
          temp_re = twid_re * rwork[i + iy].re - twid_im * rwork[i + iy].im;
          temp_im = twid_re * rwork[i + iy].im + twid_im * rwork[i + iy].re;
          rwork[i + iy].re = rwork[i].re - temp_re;
          rwork[i + iy].im = rwork[i].im - temp_im;
          rwork[i].re += temp_re;
          rwork[i].im += temp_im;
          i += ix;
        }

        istart++;
      }

      ju /= 2;
      iy = ix;
      ix += ix;
      iheight -= iy;
    }

    memcpy(&y[k << 7], &rwork[0], sizeof(creal_T) << 7);
  }
}

//
// Arguments    : const double x[256]
//                double y[2]
// Return Type  : void
//
static void mean(const double x[256], double y[2])
{
  int i;
  int xoffset;
  double s;
  int k;
  for (i = 0; i < 2; i++) {
    xoffset = i << 7;
    s = x[xoffset];
    for (k = 0; k < 127; k++) {
      s += x[(xoffset + k) + 1];
    }

    y[i] = s / 128.0;
  }
}

//
// Arguments    : const double a[128]
//                double y[128]
// Return Type  : void
//
static void power(const double a[128], double y[128])
{
  int k;
  for (k = 0; k < 128; k++) {
    y[k] = a[k] * a[k];
  }
}

//
// Arguments    : const double a[128]
//                double b[256]
// Return Type  : void
//
static void repmat(const double a[128], double b[256])
{
  int jtilecol;
  int ibtile;
  for (jtilecol = 0; jtilecol < 2; jtilecol++) {
    ibtile = jtilecol << 7;
    memcpy(&b[ibtile], &a[0], sizeof(double) << 7);
  }
}

//
// Arguments    : const float X[64]
//                float Y[64]
// Return Type  : void
//
static void rescale_minmax(const float X[64], float Y[64])
{
  int ixstart;
  float mtmp;
  int ix;
  boolean_T exitg1;
  float b_mtmp;
  ixstart = 1;
  mtmp = X[0];
  if (rtIsNaNF(X[0])) {
    ix = 2;
    exitg1 = false;
    while ((!exitg1) && (ix < 65)) {
      ixstart = ix;
      if (!rtIsNaNF(X[ix - 1])) {
        mtmp = X[ix - 1];
        exitg1 = true;
      } else {
        ix++;
      }
    }
  }

  if (ixstart < 64) {
    while (ixstart + 1 < 65) {
      if (X[ixstart] < mtmp) {
        mtmp = X[ixstart];
      }

      ixstart++;
    }
  }

  ixstart = 1;
  b_mtmp = X[0];
  if (rtIsNaNF(X[0])) {
    ix = 2;
    exitg1 = false;
    while ((!exitg1) && (ix < 65)) {
      ixstart = ix;
      if (!rtIsNaNF(X[ix - 1])) {
        b_mtmp = X[ix - 1];
        exitg1 = true;
      } else {
        ix++;
      }
    }
  }

  if (ixstart < 64) {
    while (ixstart + 1 < 65) {
      if (X[ixstart] > b_mtmp) {
        b_mtmp = X[ixstart];
      }

      ixstart++;
    }
  }

  b_mtmp -= mtmp;
  for (ixstart = 0; ixstart < 64; ixstart++) {
    Y[ixstart] = (X[ixstart] - mtmp) / b_mtmp;
  }
}

//
// Arguments    : const double x[128]
// Return Type  : double
//
static double sum(const double x[128])
{
  double y;
  int k;
  y = x[0];
  for (k = 0; k < 127; k++) {
    y += x[k + 1];
  }

  return y;
}

//
// if size(signals,2) > size(signals,1)
//      signals = signals.';
//  end
// Arguments    : const double signals_data[]
//                const int signals_size[2]
//                const double window[128]
//                float CSM[192]
// Return Type  : void
//
static void tf_csm_welch(const double signals_data[], const int signals_size[2],
  const double window[128], float CSM[192])
{
  double z;
  int number_of_blocks;
  double data_taper[256];
  double S[192];
  int a;
  double dv1[128];
  int c;
  int i0;
  double Data_Block[256];
  double dv2[2];
  int aa;
  double dv3[256];
  float b_CSM[64];
  creal_T b_Data_Block[256];
  double P[192];
  int b;
  int b_aa;

  //  Function for spectra estimation by Welch's method
  //  Developed by Luiz A. Baccala, Fl?vio Caduda and Luciano Caldas, all from
  //  Escola Polit?cnica - Poli-USP, with cooperation of Carlos Pagani and Felipe 
  //  Amaral from Escola de Engenharia de S?o Carlos - EESC-USP.
  //
  //  Cross-spectra matrix are estimated by Welch's method with 50% overlap and
  //  the window energy loss are compasated by a factor of 1/sum(Wi.^2) where
  //  Wi are the elements of the window [1]. Then, the spectra becomes:
  //  Sxy = fft(x)*conj(fft(y))/sum(Wi.^2)
  //
  //  Code was tested with a known- spectra signal from a white noise filtered
  //  by a filter. The variance (power) of the signal checks with the integral
  //  of the PSD estimated.
  //
  //  INPUT:
  //  -- signals: matrix of signals to perform the spectra estimatino. Size is
  //  [Samples x number of sensors];
  //  -- fs: samplerate in Hertz;
  //  -- window: data taper desired. Must be a vector. For best performance it
  //  should be a power of 2. For general applications do: window=hanning(1024); 
  //
  //  OUTPUT:
  //  -- CSM: Cross Spectral Matrix: Unilateral (0:fs/2) spectra. Welch's
  //  method is used with 50% overlap. Matrix size: sensors x sensors x
  //  windowsize/2
  //  -- frequencies: vector with all frequencies corresponding to each layer
  //  (3rd layer in depth) of CSM.
  //
  //  LAST REVISION: Aug - 18 - 2016
  //  ADDED 'fs' missing term in line 82, for calibration factor
  //  [1] Trobs,M.; Heinzel,G. "Improved spectrum estimation from digitized
  //  time series on a logarithmic frequency axis"
  //  doi:10.1016/j.measurement.2005.10.010
  // must be even, best if 2^n
  // ORIGINAL;
  z = 2.0 * (double)signals_size[0] / 128.0;
  number_of_blocks = (int)std::floor(z);
  repmat(window, data_taper);

  //  Data segmentation into blocks of size block_samples:
  memset(&S[0], 0, 192U * sizeof(double));

  // ORIGINAL
  for (a = 0; a <= number_of_blocks - 2; a++) {
    //  Retrieve current data block
    c = a << 6;
    for (i0 = 0; i0 < 2; i0++) {
      for (aa = 0; aa < 128; aa++) {
        Data_Block[aa + (i0 << 7)] = signals_data[(aa + c) + signals_size[0] *
          i0];
      }
    }

    mean(Data_Block, dv2);
    b_repmat(dv2, dv3);

    //  Mean-shift (zeroing)
    for (i0 = 0; i0 < 256; i0++) {
      Data_Block[i0] = (Data_Block[i0] - dv3[i0]) * data_taper[i0];
    }

    // Taper it
    fft(Data_Block, b_Data_Block);

    // FFT it,
    //  bilateral DFT
    //  viii
    // ORIGINAL
    //  Data_Block = Data_Block(1:ceil(block_samples/2),:);
    // All spectral combinations:
    memset(&P[0], 0, 192U * sizeof(double));

    // ORIGINAL
    c = 0;
    for (aa = 0; aa < 2; aa++) {
      for (b = 0; b <= 1 - aa; b++) {
        //  P(:,c) = Data_Block(:,b).*conj(Data_Block(:,aa)); % THIS
        //  IS FOR WIND TUNNEL EESC-USP BEAMFORMING CODE
        //              P(:,c) = Data_Block(:,aa).*conj(Data_Block(:,b)); % THIS IS THE ORIGINAL 
        //  LINE
        b_aa = aa + b;
        for (i0 = 0; i0 < 64; i0++) {
          P[i0 + (c << 6)] = b_Data_Block[i0 + (aa << 7)].re * b_Data_Block[i0 +
            (b_aa << 7)].re - b_Data_Block[i0 + (aa << 7)].im * -b_Data_Block[i0
            + (b_aa << 7)].im;
        }

        //              IS FOR FAN RIG BEAMFORMING CODE
        c++;
      }
    }

    //  Sum the spectrums up ...
    for (i0 = 0; i0 < 192; i0++) {
      S[i0] += P[i0];
    }
  }

  power(window, dv1);
  z = sum(dv1) * 250.0 * (std::floor(z) - 1.0);

  //  Average them out
  for (i0 = 0; i0 < 192; i0++) {
    CSM[i0] = (float)(S[i0] * 2.0 / z);
  }

  for (c = 0; c < 3; c++) {
    memcpy(&b_CSM[0], &CSM[c << 6], sizeof(float) << 6);
    rescale_minmax(b_CSM, *(float (*)[64])&CSM[c << 6]);
  }

  //  Sf = zeros(sensors,sensors,size(S,1));
  //  c=1;
  //  for a = 1:sensors
  //      for b = a:sensors
  //          Sf(a,b,:) = S(:,c);
  //          c = c+1;
  //      end
  //  end
  //  clear S
  //  CSM = Sf;
  //  Goes through n x n x : matrix and does:
  //  = ORIGINAL + TRANSPOSE - IDENTITY(ORIGINAL).
  //  for i = 1:size(CSM,3)
  //      CSM(:,:,i) = CSM(:,:,i) + CSM(:,:,i)' - eye(sensors).*CSM(:,:,i);
  //  end
  //
  //  CSM = reshape(CSM, [size(CSM, 1)*size(CSM,2), size(CSM, 3)]);
}

//
// Preprocessing for time domain data classification. MUST USE A FILTER BEFORE APPLYING THIS!!!
// Arguments    : const double X_data[]
//                const int X_size[2]
//                float Y[192]
// Return Type  : void
//
void tf_csm_p128(const double X_data[], const int X_size[2], float Y[192])
{
  static const double dv0[128] = { 0.0, 0.0006117918928693622,
    0.0024456704141966878, 0.0054971477559346282, 0.0097587564416873329,
    0.015220067600876686, 0.02186771648993624, 0.029685435198078036,
    0.038654092457596656, 0.048751740461286885, 0.059953668572405805,
    0.072232463795742, 0.08555807786180808, 0.099897900759991443,
    0.11521684054071507, 0.13147740919131806, 0.14863981437550489,
    0.1666620568118603, 0.18550003305312873, 0.20510764341474025,
    0.22543690478846373, 0.24643806806511348, 0.26805973987895532,
    0.29024900837588552, 0.31295157269760338, 0.33611187586491165,
    0.35967324073495344, 0.38357800869967928, 0.407767680786122,
    0.43218306081318719, 0.45676440025462711, 0.48145154445369737,
    0.50618407983168145, 0.530901481730042, 0.55554326252440223,
    0.58004911964789874, 0.60435908316166753, 0.6284136625113339,
    0.65215399211036806, 0.6755219753940388, 0.69846042699144351,
    0.72091321266769337, 0.74282538669379183, 0.76414332630804194,
    0.78481486293992853, 0.80478940987535386, 0.82401808605080262,
    0.84245383567349563, 0.86005154337480016, 0.87676814461509767,
    0.89256273106992745, 0.90739665073951215, 0.92123360253667874,
    0.93403972512170086, 0.94578367976667221, 0.95643672704662452,
    0.96597279716971718, 0.97436855377438547, 0.9816034510373286,
    0.98765978395258136, 0.99252273165863159, 0.99618039370755151,
    0.9986238191873873, 0.99984702862654151, 0.99984702862654151,
    0.9986238191873873, 0.99618039370755151, 0.99252273165863159,
    0.98765978395258136, 0.9816034510373286, 0.97436855377438547,
    0.96597279716971718, 0.95643672704662452, 0.94578367976667221,
    0.93403972512170086, 0.92123360253667874, 0.90739665073951215,
    0.89256273106992745, 0.87676814461509767, 0.86005154337480016,
    0.84245383567349563, 0.82401808605080262, 0.80478940987535386,
    0.78481486293992853, 0.76414332630804194, 0.74282538669379183,
    0.72091321266769337, 0.69846042699144351, 0.6755219753940388,
    0.65215399211036806, 0.6284136625113339, 0.60435908316166753,
    0.58004911964789874, 0.55554326252440223, 0.530901481730042,
    0.50618407983168145, 0.48145154445369737, 0.45676440025462711,
    0.43218306081318719, 0.407767680786122, 0.38357800869967928,
    0.35967324073495344, 0.33611187586491165, 0.31295157269760338,
    0.29024900837588552, 0.26805973987895532, 0.24643806806511348,
    0.22543690478846373, 0.20510764341474025, 0.18550003305312873,
    0.1666620568118603, 0.14863981437550489, 0.13147740919131806,
    0.11521684054071507, 0.099897900759991443, 0.08555807786180808,
    0.072232463795742, 0.059953668572405805, 0.048751740461286885,
    0.038654092457596656, 0.029685435198078036, 0.02186771648993624,
    0.015220067600876686, 0.0097587564416873329, 0.0054971477559346282,
    0.0024456704141966878, 0.0006117918928693622, 0.0 };

  //  X = filtfilt(b, a, X); % DO THIS SEPARATELY
  if (X_size[0] == 128) {
    tf_csm_welch(X_data, X_size, dv0, Y);
  } else if (X_size[0] == 192) {
    tf_csm_welch(X_data, X_size, dv0, Y);
  } else if (X_size[0] == 256) {
    tf_csm_welch(X_data, X_size, dv0, Y);
  } else {
    memset(&Y[0], 0, 192U * sizeof(float));
  }
}

//
// Arguments    : void
// Return Type  : void
//
void tf_csm_p128_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void tf_csm_p128_terminate()
{
  // (no terminate code required)
}

//
// File trailer for tf_csm_p128.cpp
//
// [EOF]
//
