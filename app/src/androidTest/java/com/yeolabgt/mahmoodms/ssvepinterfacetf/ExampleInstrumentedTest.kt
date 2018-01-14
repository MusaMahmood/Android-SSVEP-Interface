package com.yeolabgt.mahmoodms.ssvepinterfacetf

import android.support.test.InstrumentationRegistry
import android.support.test.runner.AndroidJUnit4

import org.junit.Test
import org.junit.runner.RunWith

import org.junit.Assert.*
import java.util.*

/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class ExampleInstrumentedTest {
    @Test
    fun useAppContext() {
        // Context of the app under test.
        val appContext = InstrumentationRegistry.getTargetContext()
        assertEquals("com.yeolabgt.mahmoodms.ssvepinterfacetf", appContext.packageName)
    }

    @Test
    fun jClassifyWorks() {
        val a = DoubleArray(1000)
        Arrays.fill(a, 0.0)
        val b = DoubleArray(1000)
        Arrays.fill(b, 0.0)
        val c = jClassifySSVEP(a, b, 1.1)
        assertEquals(c[0], 0.0, 0.1) //c[0] because c is array of size 2
    }

    private external fun jClassifySSVEP(a: DoubleArray, b: DoubleArray, c: Double): DoubleArray

    companion object {
        init {
            System.loadLibrary("ssvep-lib")
        }
    }
}
