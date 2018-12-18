package com.yeolabgt.mahmoodms.ssvepinterfacetf

/**
 * Created by Musa Mahmood on 1/14/2018.
 *
 */

class NativeInterfaceClass {


    @Throws(IllegalArgumentException::class)
    external fun jGetHRRR(data: DoubleArray, data2: DoubleArray): DoubleArray
    @Throws(IllegalArgumentException::class)
    external fun jmainInitialization(b: Boolean): Int

    companion object {
        init {
            System.loadLibrary("ssvep-lib")
        }
    }
}