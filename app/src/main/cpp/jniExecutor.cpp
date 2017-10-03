//
// Created by mahmoodms on 4/3/2017.
//

#include "rt_nonfinite.h"
#include "classifySSVEP.h"
#include "extractPowerSpectrum2ch.h"
/*Additional Includes*/
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG "jniExecutor-cpp"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Function Definitions

extern "C" {
JNIEXPORT jdoubleArray JNICALL
Java_com_yeolabgt_mahmoodms_androidssvepinterfacetf_DeviceControlActivity_jClassifySSVEP(
        JNIEnv *env, jobject jobject1, jdoubleArray ch1, jdoubleArray ch2, jdouble threshold) {

    jdouble *X1 = env->GetDoubleArrayElements(ch1, NULL);
    jdouble *X2 = env->GetDoubleArrayElements(ch2, NULL);
    double Y[501]; // First two values = Y; last 499 = cPSD
    if (X1 == NULL) LOGE("ERROR - C_ARRAY IS NULL");
    if (X2 == NULL) LOGE("ERROR - C_ARRAY IS NULL");
    jdoubleArray m_result = env->NewDoubleArray(501);
    classifySSVEP(X1, X2, threshold, &Y[0], &Y[2]);
    env->SetDoubleArrayRegion(m_result, 0, 501, Y);
    return m_result;
}
}

extern "C" {
JNIEXPORT jdoubleArray JNICALL
Java_com_yeolabgt_mahmoodms_androidssvepinterfacetf_DeviceControlActivity_jPSDExtraction2ch(
        JNIEnv *env, jobject jobject1, jdoubleArray ch1, jdoubleArray ch2) {
    jdouble *X1 = env->GetDoubleArrayElements(ch1, NULL);
    jdouble *X2 = env->GetDoubleArrayElements(ch2, NULL);
    if (X1 == NULL) LOGE("ERROR - C_ARRAY IS NULL");
    if (X2 == NULL) LOGE("ERROR - C_ARRAY IS NULL");
    jdoubleArray m_result = env->NewDoubleArray(500);
    double Y[500];
    extractPowerSpectrum2ch(X1, X2, Y);
    env->SetDoubleArrayRegion(m_result, 0, 500, Y);
    return m_result;
}
}

extern "C" {
JNIEXPORT jdoubleArray JNICALL
Java_com_yeolabgt_mahmoodms_androidssvepinterfacetf_DeviceControlActivity_jLoadfPSD(
        JNIEnv *env, jobject jobject1) {
    jdoubleArray m_result = env->NewDoubleArray(499);
    double fPSD[499];
    for (int i = 0; i < 499; i++) {
        fPSD[i] = (double)i * 250.0 / 998.0;
    }
    env->SetDoubleArrayRegion(m_result, 0, 499, fPSD);
    return m_result;
}
}

extern "C" {
JNIEXPORT jdoubleArray JNICALL
/**
 *
 * @param env
 * @param jobject1
 * @return array of frequencies (Hz) corresponding to a raw input signal.
 */
Java_com_yeolabgt_mahmoodms_androidssvepinterfacetf_DeviceControlActivity_jLoadfPSD2ch(
        JNIEnv *env, jobject jobject1) {
    jdoubleArray m_result = env->NewDoubleArray(250);
    double fPSD[250];
    for (int i = 0; i < 250; i++) {
        fPSD[i] = (double)i * 250.0 / 500;
    }
    env->SetDoubleArrayRegion(m_result, 0, 250, fPSD);
    return m_result;
}
}

extern "C" {
JNIEXPORT jint JNICALL
Java_com_yeolabgt_mahmoodms_androidssvepinterfacetf_DeviceControlActivity_jmainInitialization(
        JNIEnv *env, jobject obj, jboolean terminate) {
    if (!(bool) terminate) {
        classifySSVEP_initialize();
        extractPowerSpectrum2ch_initialize();
        return 0;
    } else {
        return -1;
    }
}
}
