//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: tf_csm_welch_w512.cpp
//
// MATLAB Coder version            : 3.3
// C/C++ source code generated on  : 29-Mar-2018 10:44:45
//

// Include Files
#include "rt_nonfinite.h"
#include "tf_csm_welch_w512.h"

// Function Declarations
static void b_repmat(const double a[2], double b[512]);
static void fft(const double x[512], creal_T y[512]);
static void filter(double b[4], double a[4], const double x[530], const double
zi[3], double y[530]);
static void filtfilt(const double x_in[512], double y_out[512]);
static void flipud(double x[530]);
static void mean(const double x[512], double y[2]);
static void power(const double a[256], double y[256]);
static void r2br_r2dit_trig_impl(const double x[512], int xoffInit, const double
costab[129], const double sintab[129], creal_T y[256]);
static void repmat(const double a[256], double b[512]);
static double sum(const double x[256]);
static void tf_csm_welch(const double signals[1024], double fs, const double
window[256], double CSM[384]);

// Function Definitions

//
// Arguments    : const double a[2]
//                double b[512]
// Return Type  : void
//
static void b_repmat(const double a[2], double b[512])
{
  int jcol;
  int ibmat;
  int itilerow;
  for (jcol = 0; jcol < 2; jcol++) {
    ibmat = jcol << 8;
    for (itilerow = 0; itilerow < 256; itilerow++) {
      b[ibmat + itilerow] = a[jcol];
    }
  }
}

//
// Arguments    : const double x[512]
//                creal_T y[512]
// Return Type  : void
//
static void fft(const double x[512], creal_T y[512])
{
  int k;
  static const double costab[129] = { 1.0, 0.99969881869620425,
                                      0.99879545620517241, 0.99729045667869021, 0.99518472667219693,
                                      0.99247953459871, 0.989176509964781, 0.98527764238894122,
                                      0.98078528040323043, 0.97570213003852857, 0.970031253194544,
                                      0.96377606579543984, 0.95694033573220882, 0.94952818059303667,
                                      0.94154406518302081, 0.932992798834739, 0.92387953251128674,
                                      0.91420975570353069, 0.90398929312344334, 0.89322430119551532,
                                      0.881921264348355, 0.87008699110871146, 0.85772861000027212,
                                      0.84485356524970712, 0.83146961230254524, 0.81758481315158371,
                                      0.80320753148064494, 0.78834642762660634, 0.773010453362737,
                                      0.75720884650648457, 0.74095112535495922, 0.724247082951467,
                                      0.70710678118654757, 0.68954054473706683, 0.67155895484701833,
                                      0.65317284295377676, 0.63439328416364549, 0.61523159058062682,
                                      0.59569930449243336, 0.57580819141784534, 0.55557023301960218,
                                      0.53499761988709715, 0.51410274419322166, 0.49289819222978404,
                                      0.47139673682599764, 0.44961132965460654, 0.42755509343028208,
                                      0.40524131400498986, 0.38268343236508978, 0.35989503653498811,
                                      0.33688985339222005, 0.31368174039889152, 0.29028467725446233,
                                      0.26671275747489837, 0.24298017990326387, 0.2191012401568698,
                                      0.19509032201612825, 0.17096188876030122, 0.14673047445536175,
                                      0.1224106751992162, 0.0980171403295606, 0.073564563599667426,
                                      0.049067674327418015, 0.024541228522912288, 0.0, -0.024541228522912288,
                                      -0.049067674327418015, -0.073564563599667426, -0.0980171403295606,
                                      -0.1224106751992162, -0.14673047445536175, -0.17096188876030122,
                                      -0.19509032201612825, -0.2191012401568698, -0.24298017990326387,
                                      -0.26671275747489837, -0.29028467725446233, -0.31368174039889152,
                                      -0.33688985339222005, -0.35989503653498811, -0.38268343236508978,
                                      -0.40524131400498986, -0.42755509343028208, -0.44961132965460654,
                                      -0.47139673682599764, -0.49289819222978404, -0.51410274419322166,
                                      -0.53499761988709715, -0.55557023301960218, -0.57580819141784534,
                                      -0.59569930449243336, -0.61523159058062682, -0.63439328416364549,
                                      -0.65317284295377676, -0.67155895484701833, -0.68954054473706683,
                                      -0.70710678118654757, -0.724247082951467, -0.74095112535495922,
                                      -0.75720884650648457, -0.773010453362737, -0.78834642762660634,
                                      -0.80320753148064494, -0.81758481315158371, -0.83146961230254524,
                                      -0.84485356524970712, -0.85772861000027212, -0.87008699110871146,
                                      -0.881921264348355, -0.89322430119551532, -0.90398929312344334,
                                      -0.91420975570353069, -0.92387953251128674, -0.932992798834739,
                                      -0.94154406518302081, -0.94952818059303667, -0.95694033573220882,
                                      -0.96377606579543984, -0.970031253194544, -0.97570213003852857,
                                      -0.98078528040323043, -0.98527764238894122, -0.989176509964781,
                                      -0.99247953459871, -0.99518472667219693, -0.99729045667869021,
                                      -0.99879545620517241, -0.99969881869620425, -1.0 };

  static const double sintab[129] = { 0.0, -0.024541228522912288,
                                      -0.049067674327418015, -0.073564563599667426, -0.0980171403295606,
                                      -0.1224106751992162, -0.14673047445536175, -0.17096188876030122,
                                      -0.19509032201612825, -0.2191012401568698, -0.24298017990326387,
                                      -0.26671275747489837, -0.29028467725446233, -0.31368174039889152,
                                      -0.33688985339222005, -0.35989503653498811, -0.38268343236508978,
                                      -0.40524131400498986, -0.42755509343028208, -0.44961132965460654,
                                      -0.47139673682599764, -0.49289819222978404, -0.51410274419322166,
                                      -0.53499761988709715, -0.55557023301960218, -0.57580819141784534,
                                      -0.59569930449243336, -0.61523159058062682, -0.63439328416364549,
                                      -0.65317284295377676, -0.67155895484701833, -0.68954054473706683,
                                      -0.70710678118654757, -0.724247082951467, -0.74095112535495922,
                                      -0.75720884650648457, -0.773010453362737, -0.78834642762660634,
                                      -0.80320753148064494, -0.81758481315158371, -0.83146961230254524,
                                      -0.84485356524970712, -0.85772861000027212, -0.87008699110871146,
                                      -0.881921264348355, -0.89322430119551532, -0.90398929312344334,
                                      -0.91420975570353069, -0.92387953251128674, -0.932992798834739,
                                      -0.94154406518302081, -0.94952818059303667, -0.95694033573220882,
                                      -0.96377606579543984, -0.970031253194544, -0.97570213003852857,
                                      -0.98078528040323043, -0.98527764238894122, -0.989176509964781,
                                      -0.99247953459871, -0.99518472667219693, -0.99729045667869021,
                                      -0.99879545620517241, -0.99969881869620425, -1.0, -0.99969881869620425,
                                      -0.99879545620517241, -0.99729045667869021, -0.99518472667219693,
                                      -0.99247953459871, -0.989176509964781, -0.98527764238894122,
                                      -0.98078528040323043, -0.97570213003852857, -0.970031253194544,
                                      -0.96377606579543984, -0.95694033573220882, -0.94952818059303667,
                                      -0.94154406518302081, -0.932992798834739, -0.92387953251128674,
                                      -0.91420975570353069, -0.90398929312344334, -0.89322430119551532,
                                      -0.881921264348355, -0.87008699110871146, -0.85772861000027212,
                                      -0.84485356524970712, -0.83146961230254524, -0.81758481315158371,
                                      -0.80320753148064494, -0.78834642762660634, -0.773010453362737,
                                      -0.75720884650648457, -0.74095112535495922, -0.724247082951467,
                                      -0.70710678118654757, -0.68954054473706683, -0.67155895484701833,
                                      -0.65317284295377676, -0.63439328416364549, -0.61523159058062682,
                                      -0.59569930449243336, -0.57580819141784534, -0.55557023301960218,
                                      -0.53499761988709715, -0.51410274419322166, -0.49289819222978404,
                                      -0.47139673682599764, -0.44961132965460654, -0.42755509343028208,
                                      -0.40524131400498986, -0.38268343236508978, -0.35989503653498811,
                                      -0.33688985339222005, -0.31368174039889152, -0.29028467725446233,
                                      -0.26671275747489837, -0.24298017990326387, -0.2191012401568698,
                                      -0.19509032201612825, -0.17096188876030122, -0.14673047445536175,
                                      -0.1224106751992162, -0.0980171403295606, -0.073564563599667426,
                                      -0.049067674327418015, -0.024541228522912288, -0.0 };

  creal_T rwork[256];
  for (k = 0; k < 2; k++) {
    r2br_r2dit_trig_impl(x, k << 8, costab, sintab, rwork);
    memcpy(&y[k << 8], &rwork[0], sizeof(creal_T) << 8);
  }
}

//
// Arguments    : double b[4]
//                double a[4]
//                const double x[530]
//                const double zi[3]
//                double y[530]
// Return Type  : void
//
static void filter(double b[4], double a[4], const double x[530], const double
zi[3], double y[530])
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

  memset(&y[3], 0, 527U * sizeof(double));
  for (k = 0; k < 530; k++) {
    naxpy = 530 - k;
    if (!(naxpy < 4)) {
      naxpy = 4;
    }

    for (j = 0; j + 1 <= naxpy; j++) {
      y[k + j] += x[k] * b[j];
    }

    naxpy = 529 - k;
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
// Arguments    : const double x_in[512]
//                double y_out[512]
// Return Type  : void
//
static void filtfilt(const double x_in[512], double y_out[512])
{
  double d0;
  double d1;
  int i;
  double y[530];
  double dv2[4];
  static const double dv3[4] = { 0.90431873448479, -2.71295620345437,
                                 2.71295620345437, -0.90431873448479 };

  double dv4[4];
  static const double dv5[4] = { 1.0, -2.7990220146733, 2.61773550092223,
                                 -0.81779236028278 };

  double b_y[530];
  double a[3];
  static const double b_a[3] = { -0.90431873448438438, 1.80863746896885,
                                 -0.90431873448445832 };

  d0 = 2.0 * x_in[0];
  d1 = 2.0 * x_in[511];
  for (i = 0; i < 9; i++) {
    y[i] = d0 - x_in[9 - i];
  }

  memcpy(&y[9], &x_in[0], sizeof(double) << 9);
  for (i = 0; i < 9; i++) {
    y[i + 521] = d1 - x_in[510 - i];
  }

  for (i = 0; i < 4; i++) {
    dv2[i] = dv3[i];
    dv4[i] = dv5[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 530U * sizeof(double));
  filter(dv2, dv4, b_y, a, y);
  flipud(y);
  for (i = 0; i < 4; i++) {
    dv2[i] = dv3[i];
    dv4[i] = dv5[i];
  }

  for (i = 0; i < 3; i++) {
    a[i] = b_a[i] * y[0];
  }

  memcpy(&b_y[0], &y[0], 530U * sizeof(double));
  filter(dv2, dv4, b_y, a, y);
  flipud(y);
  memcpy(&y_out[0], &y[9], sizeof(double) << 9);
}

//
// Arguments    : double x[530]
// Return Type  : void
//
static void flipud(double x[530])
{
  int i;
  double xtmp;
  for (i = 0; i < 265; i++) {
    xtmp = x[i];
    x[i] = x[529 - i];
    x[529 - i] = xtmp;
  }
}

//
// Arguments    : const double x[512]
//                double y[2]
// Return Type  : void
//
static void mean(const double x[512], double y[2])
{
  int i;
  int xoffset;
  double s;
  int k;
  for (i = 0; i < 2; i++) {
    xoffset = i << 8;
    s = x[xoffset];
    for (k = 0; k < 255; k++) {
      s += x[(xoffset + k) + 1];
    }

    y[i] = s / 256.0;
  }
}

//
// Arguments    : const double a[256]
//                double y[256]
// Return Type  : void
//
static void power(const double a[256], double y[256])
{
  int k;
  for (k = 0; k < 256; k++) {
    y[k] = a[k] * a[k];
  }
}

//
// Arguments    : const double x[512]
//                int xoffInit
//                const double costab[129]
//                const double sintab[129]
//                creal_T y[256]
// Return Type  : void
//
static void r2br_r2dit_trig_impl(const double x[512], int xoffInit, const double
costab[129], const double sintab[129], creal_T y[256])
{
  int ix;
  int ju;
  int iy;
  int i;
  boolean_T tst;
  double temp_re;
  double temp_im;
  int iheight;
  int istart;
  int j;
  double twid_re;
  double twid_im;
  int ihi;
  ix = xoffInit;
  ju = 0;
  iy = 0;
  for (i = 0; i < 255; i++) {
    y[iy].re = x[ix];
    y[iy].im = 0.0;
    iy = 256;
    tst = true;
    while (tst) {
      iy >>= 1;
      ju ^= iy;
      tst = ((ju & iy) == 0);
    }

    iy = ju;
    ix++;
  }

  y[iy].re = x[ix];
  y[iy].im = 0.0;
  for (i = 0; i <= 255; i += 2) {
    temp_re = y[i + 1].re;
    temp_im = y[i + 1].im;
    y[i + 1].re = y[i].re - y[i + 1].re;
    y[i + 1].im = y[i].im - y[i + 1].im;
    y[i].re += temp_re;
    y[i].im += temp_im;
  }

  iy = 2;
  ix = 4;
  ju = 64;
  iheight = 253;
  while (ju > 0) {
    for (i = 0; i < iheight; i += ix) {
      temp_re = y[i + iy].re;
      temp_im = y[i + iy].im;
      y[i + iy].re = y[i].re - temp_re;
      y[i + iy].im = y[i].im - temp_im;
      y[i].re += temp_re;
      y[i].im += temp_im;
    }

    istart = 1;
    for (j = ju; j < 128; j += ju) {
      twid_re = costab[j];
      twid_im = sintab[j];
      i = istart;
      ihi = istart + iheight;
      while (i < ihi) {
        temp_re = twid_re * y[i + iy].re - twid_im * y[i + iy].im;
        temp_im = twid_re * y[i + iy].im + twid_im * y[i + iy].re;
        y[i + iy].re = y[i].re - temp_re;
        y[i + iy].im = y[i].im - temp_im;
        y[i].re += temp_re;
        y[i].im += temp_im;
        i += ix;
      }

      istart++;
    }

    ju /= 2;
    iy = ix;
    ix += ix;
    iheight -= iy;
  }
}

//
// Arguments    : const double a[256]
//                double b[512]
// Return Type  : void
//
static void repmat(const double a[256], double b[512])
{
  int jtilecol;
  int ibtile;
  for (jtilecol = 0; jtilecol < 2; jtilecol++) {
    ibtile = jtilecol << 8;
    memcpy(&b[ibtile], &a[0], sizeof(double) << 8);
  }
}

//
// Arguments    : const double x[256]
// Return Type  : double
//
static double sum(const double x[256])
{
  double y;
  int k;
  y = x[0];
  for (k = 0; k < 255; k++) {
    y += x[k + 1];
  }

  return y;
}

//
// Arguments    : const double signals[1024]
//                double fs
//                const double window[256]
//                double CSM[384]
// Return Type  : void
//
static void tf_csm_welch(const double signals[1024], double fs, const double
window[256], double CSM[384])
{
  double data_taper[512];
  int a;
  double dv6[256];
  double B;
  int c;
  int i0;
  double Data_Block[512];
  double dv7[2];
  double dv8[512];
  creal_T b_Data_Block[512];
  double P[384];
  int aa;
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
  repmat(window, data_taper);

  //  Data segmentation into blocks of size block_samples:
  memset(&CSM[0], 0, 384U * sizeof(double));

  // ORIGINAL
  for (a = 0; a < 3; a++) {
    //  Retrieve current data block
    c = a << 7;
    for (i0 = 0; i0 < 2; i0++) {
      memcpy(&Data_Block[i0 << 8], &signals[(i0 << 9) + c], sizeof(double) << 8);
    }

    mean(Data_Block, dv7);
    b_repmat(dv7, dv8);

    //  Mean-shift (zeroing)
    for (i0 = 0; i0 < 512; i0++) {
      Data_Block[i0] = (Data_Block[i0] - dv8[i0]) * data_taper[i0];
    }

    // Taper it
    fft(Data_Block, b_Data_Block);

    // FFT it,
    //  bilateral DFT
    //  viii
    // ORIGINAL
    //  Data_Block = Data_Block(1:ceil(block_samples/2),:);
    // All spectral combinations:
    memset(&P[0], 0, 384U * sizeof(double));

    // ORIGINAL
    c = 0;
    for (aa = 0; aa < 2; aa++) {
      for (b = 0; b <= 1 - aa; b++) {
        //  P(:,c) = Data_Block(:,b).*conj(Data_Block(:,aa)); % THIS
        //  IS FOR WIND TUNNEL EESC-USP BEAMFORMING CODE
        //              P(:,c) = Data_Block(:,aa).*conj(Data_Block(:,b)); % THIS IS THE ORIGINAL
        //  LINE
        b_aa = aa + b;
        for (i0 = 0; i0 < 128; i0++) {
          P[i0 + (c << 7)] = b_Data_Block[i0 + (aa << 8)].re * b_Data_Block[i0 +
                                                                            (b_aa << 8)].re - b_Data_Block[i0 + (aa << 8)].im * -b_Data_Block[i0
                                                                                                                                              + (b_aa << 8)].im;
        }

        //              IS FOR FAN RIG BEAMFORMING CODE
        c++;
      }
    }

    //  Sum the spectrums up ...
    for (i0 = 0; i0 < 384; i0++) {
      CSM[i0] += P[i0];
    }
  }

  power(window, dv6);
  B = sum(dv6) * fs * 3.0;
  for (i0 = 0; i0 < 384; i0++) {
    CSM[i0] = CSM[i0] * 2.0 / B;
  }

  //  Average them out
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
// FEATURE EXTRACTION FOR CROSS-SPECTRAL MATRICES
//  CONSTANTS:
// Arguments    : double X[1024]
//                float Y[384]
// Return Type  : void
//
void tf_csm_welch_w512(double X[1024], float Y[384])
{
  double b_X[512];
  static const double dv0[256] = { 0.0, 0.00015177401106419852,
                                   0.00060700390285500783, 0.0013654133071059893, 0.0024265417964677849,
                                   0.0037897451640321322, 0.0054541958144270208, 0.0074188832662467341,
                                   0.0096826147655111328, 0.0122440160097817, 0.015101531982495253,
                                   0.018253427897008456, 0.02169779024977958, 0.025432527982049369,
                                   0.029455373749314229, 0.033763885297822094, 0.038355446947255289,
                                   0.043227271178699567, 0.048376400326935576, 0.0537997083760261,
                                   0.059493902857107761, 0.065455526847235823, 0.071680961068068649,
                                   0.0781664260831168, 0.08490798459222465, 0.091901543821889142,
                                   0.099142858009966639, 0.10662753098325839, 0.11435101882640963,
                                   0.12230863264050312, 0.13049554138967046, 0.13890677483399533,
                                   0.14753722654692641, 0.15638165701536866, 0.16543469682057088,
                                   0.17469084989787897, 0.18414449687337447, 0.19378989847537503,
                                   0.20362119901872255, 0.21363242995974741, 0.2238175135197471,
                                   0.23417026637478194, 0.2446844034095465, 0.25535354153303824,
                                   0.2661712035537066, 0.27713082211173085, 0.28822574366603781,
                                   0.29944923253364064, 0.31079447497884483, 0.32225458334984103,
                                   0.33382260026017019, 0.34549150281252627, 0.35725420686232889,
                                   0.36910357131847987, 0.38103240247869058, 0.3930334583967513,
                                   0.40509945327908703, 0.417223061907935, 0.42939692408845426,
                                   0.44161364911707185, 0.453865820268349, 0.46614599929764622,
                                   0.47844673095685214, 0.490760547520435, 0.50307997331906928,
                                   0.51539752927808513, 0.527705737457985, 0.53999712559427082,
                                   0.55226423163382665, 0.56449960826510148, 0.57669582743934278,
                                   0.58884548488013433, 0.600941204578505, 0.61297564327087373,
                                   0.62494149489711548, 0.63683149503604142, 0.64863842531560134,
                                   0.66035511779512746, 0.67197445931696409, 0.683489395824836,
                                   0.69489293664633967, 0.70617815873695167, 0.71733821088298244,
                                   0.72836631786092032, 0.73925578455064322, 0.74999999999999989,
                                   0.76059244143829252, 0.77102667823622473, 0.78129637580991163,
                                   0.79139529946658049, 0.80131731818962815, 0.81105640836073689,
                                   0.82060665741678906, 0.82996226743936119, 0.839117558674617,
                                   0.84806697298146327, 0.85680507720587618, 0.86532656647934658,
                                   0.87362626743944549, 0.8816991413705515, 0.88954028726283529,
                                   0.89714494478764306, 0.90450849718747373, 0.9116264740787936,
                                   0.9184945541659888, 0.925108567864807, 0.93146449983369484,
                                   0.93755849141149628, 0.943386842960031, 0.94894601611012908,
                                   0.95423263590976193, 0.95924349287296151, 0.96397554492828719,
                                   0.96842591926565524, 0.97259191408040979, 0.97647100021357824,
                                   0.980060822687314, 0.98335920213459371, 0.98636413612230234,
                                   0.98907380036690284, 0.991486549841951, 0.99360091977678455,
                                   0.99541562654578009, 0.9969295684476368, 0.99814182637421478,
                                   0.99905166436852211, 0.99965853007151151, 0.99996205505741531,
                                   0.99996205505741531, 0.99965853007151151, 0.99905166436852211,
                                   0.99814182637421478, 0.9969295684476368, 0.99541562654578009,
                                   0.99360091977678455, 0.991486549841951, 0.98907380036690284,
                                   0.98636413612230234, 0.98335920213459371, 0.980060822687314,
                                   0.97647100021357824, 0.97259191408040979, 0.96842591926565524,
                                   0.96397554492828719, 0.95924349287296151, 0.95423263590976193,
                                   0.94894601611012908, 0.943386842960031, 0.93755849141149628,
                                   0.93146449983369484, 0.925108567864807, 0.9184945541659888,
                                   0.9116264740787936, 0.90450849718747373, 0.89714494478764306,
                                   0.88954028726283529, 0.8816991413705515, 0.87362626743944549,
                                   0.86532656647934658, 0.85680507720587618, 0.84806697298146327,
                                   0.839117558674617, 0.82996226743936119, 0.82060665741678906,
                                   0.81105640836073689, 0.80131731818962815, 0.79139529946658049,
                                   0.78129637580991163, 0.77102667823622473, 0.76059244143829252,
                                   0.74999999999999989, 0.73925578455064322, 0.72836631786092032,
                                   0.71733821088298244, 0.70617815873695167, 0.69489293664633967,
                                   0.683489395824836, 0.67197445931696409, 0.66035511779512746,
                                   0.64863842531560134, 0.63683149503604142, 0.62494149489711548,
                                   0.61297564327087373, 0.600941204578505, 0.58884548488013433,
                                   0.57669582743934278, 0.56449960826510148, 0.55226423163382665,
                                   0.53999712559427082, 0.527705737457985, 0.51539752927808513,
                                   0.50307997331906928, 0.490760547520435, 0.47844673095685214,
                                   0.46614599929764622, 0.453865820268349, 0.44161364911707185,
                                   0.42939692408845426, 0.417223061907935, 0.40509945327908703,
                                   0.3930334583967513, 0.38103240247869058, 0.36910357131847987,
                                   0.35725420686232889, 0.34549150281252627, 0.33382260026017019,
                                   0.32225458334984103, 0.31079447497884483, 0.29944923253364064,
                                   0.28822574366603781, 0.27713082211173085, 0.2661712035537066,
                                   0.25535354153303824, 0.2446844034095465, 0.23417026637478194,
                                   0.2238175135197471, 0.21363242995974741, 0.20362119901872255,
                                   0.19378989847537503, 0.18414449687337447, 0.17469084989787897,
                                   0.16543469682057088, 0.15638165701536866, 0.14753722654692641,
                                   0.13890677483399533, 0.13049554138967046, 0.12230863264050312,
                                   0.11435101882640963, 0.10662753098325839, 0.099142858009966639,
                                   0.091901543821889142, 0.08490798459222465, 0.0781664260831168,
                                   0.071680961068068649, 0.065455526847235823, 0.059493902857107761,
                                   0.0537997083760261, 0.048376400326935576, 0.043227271178699567,
                                   0.038355446947255289, 0.033763885297822094, 0.029455373749314229,
                                   0.025432527982049369, 0.02169779024977958, 0.018253427897008456,
                                   0.015101531982495253, 0.0122440160097817, 0.0096826147655111328,
                                   0.0074188832662467341, 0.0054541958144270208, 0.0037897451640321322,
                                   0.0024265417964677849, 0.0013654133071059893, 0.00060700390285500783,
                                   0.00015177401106419852, 0.0 };

  double dv1[384];
  int ixstart;
  int i;
  float mtmp;
  int ix;
  boolean_T exitg1;
  float b_mtmp;

  //  High Pass Filter Coefficients at 4Hz, Butter; N = 3
  //  Filter:
  memcpy(&b_X[0], &X[0], sizeof(double) << 9);
  filtfilt(b_X, *(double (*)[512])&X[0]);
  memcpy(&b_X[0], &X[512], sizeof(double) << 9);
  filtfilt(b_X, *(double (*)[512])&X[512]);
  tf_csm_welch(X, 250.0, dv0, dv1);
  for (ixstart = 0; ixstart < 384; ixstart++) {
    Y[ixstart] = (float)dv1[ixstart];
  }

  for (i = 0; i < 3; i++) {
    ixstart = 1;
    mtmp = Y[i << 7];
    if (rtIsNaNF(Y[i << 7])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix < 129)) {
        ixstart = ix;
        if (!rtIsNaNF(Y[(ix + (i << 7)) - 1])) {
          mtmp = Y[(ix + (i << 7)) - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < 128) {
      while (ixstart + 1 < 129) {
        if (Y[ixstart + (i << 7)] < mtmp) {
          mtmp = Y[ixstart + (i << 7)];
        }

        ixstart++;
      }
    }

    ixstart = 1;
    b_mtmp = Y[i << 7];
    if (rtIsNaNF(Y[i << 7])) {
      ix = 2;
      exitg1 = false;
      while ((!exitg1) && (ix < 129)) {
        ixstart = ix;
        if (!rtIsNaNF(Y[(ix + (i << 7)) - 1])) {
          b_mtmp = Y[(ix + (i << 7)) - 1];
          exitg1 = true;
        } else {
          ix++;
        }
      }
    }

    if (ixstart < 128) {
      while (ixstart + 1 < 129) {
        if (Y[ixstart + (i << 7)] > b_mtmp) {
          b_mtmp = Y[ixstart + (i << 7)];
        }

        ixstart++;
      }
    }

    b_mtmp -= mtmp;
    for (ixstart = 0; ixstart < 128; ixstart++) {
      Y[ixstart + (i << 7)] = (Y[ixstart + (i << 7)] - mtmp) / b_mtmp;
    }
  }
}

//
// Arguments    : void
// Return Type  : void
//
void tf_csm_welch_w512_initialize()
{
  rt_InitInfAndNaN(8U);
}

//
// Arguments    : void
// Return Type  : void
//
void tf_csm_welch_w512_terminate()
{
  // (no terminate code required)
}

//
// File trailer for tf_csm_welch_w512.cpp
//
// [EOF]
//
