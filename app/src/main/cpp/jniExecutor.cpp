//
// Created by mahmoodms on 4/3/2017.
//

#include "rt_nonfinite.h"
#include "get_hr_rr.h"

/*Additional Includes*/
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG "jniExecutor-cpp"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Function Definitions
extern "C" {
JNIEXPORT jdoubleArray JNICALL
Java_com_yeolabgt_mahmoodms_ssvepinterfacetf_NativeInterfaceClass_jGetHRRR(
        JNIEnv *env, jobject jobject1, jdoubleArray ch1, jdoubleArray ch2) {
    jdouble *X1 = env->GetDoubleArrayElements(ch1, NULL);
    jdouble *X2 = env->GetDoubleArrayElements(ch2, NULL);
    double Y[2]; // First two values = Y; last 499 = cPSD
    if (X1 == NULL) LOGE("ERROR - C_ARRAY IS NULL");
    if (X2 == NULL) LOGE("ERROR - C_ARRAY IS NULL");
    get_hr_rr(X1, X2, &Y[0], &Y[1]);
    jdoubleArray m_result = env->NewDoubleArray(2);
    env->SetDoubleArrayRegion(m_result, 0, 2, Y);
    return m_result;
}
}

extern "C" {
JNIEXPORT jint JNICALL
Java_com_yeolabgt_mahmoodms_ssvepinterfacetf_NativeInterfaceClass_jmainInitialization(
        JNIEnv *env, jobject obj, jboolean terminate) {
    if (!(bool) terminate) {
        get_hr_rr_initialize();
        return 0;
    } else {
        return -1;
    }
}
}
