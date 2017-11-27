package com.yeolabgt.mahmoodms.ssvepinterfacetf

import android.content.Context
import android.os.Bundle
import android.preference.PreferenceFragment
import android.preference.PreferenceManager

/**
 * Created by mmahmood31 on 11/2/2017.
 *
 */

class PreferencesFragment : PreferenceFragment() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        addPreferencesFromResource(R.xml.prefs)
    }

    companion object {
        //Key Association:
        private var SETTINGS_CH_SELECT = "switch_ch"
        private var SETTINGS_SHOW_PSDA = "psda"
        private var SETTINGS_PSD_RANGE = "psda_long"
        private var SETTINGS_SHOW_BUTTONS = "showUIElements"
        private var SETTINGS_SAVE_TIMESTAMPS = "timestamps"
        private var SETTINGS_SAVE_CLASS = "save_class"
        private var SETTINGS_BIT_PRECISION = "bit_precision"
        private var SETTINGS_FILTER_DATA = "filterData"

        fun channelSelect(context: Context): Boolean {
            return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_CH_SELECT, true)
        }

        fun showPSDA(context: Context): Boolean {
            return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_SHOW_PSDA, true)
        }

        fun psdaWideRange(context: Context): Boolean {
            return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_PSD_RANGE, false)
        }

        fun showUIElements(context: Context): Boolean {
            return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_SHOW_BUTTONS, false)
        }

        fun saveTimestamps(context: Context): Boolean {
            return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_SAVE_TIMESTAMPS, false)
        }

        fun saveClass(context: Context): Boolean {
            return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_SAVE_CLASS, true)
        }

        fun setFilterData(context: Context): Boolean {
            return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_FILTER_DATA, false)
        }

        fun setBitPrecision(context: Context): Boolean {
            return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_BIT_PRECISION, true)
        }

        fun setNumberChannelsEnabled(context: Context): Int {
            return PreferenceManager.getDefaultSharedPreferences(context).getString("channel_list_preference", "4").toInt()
        }

        fun setSampleRate(context: Context): Int {
            return PreferenceManager.getDefaultSharedPreferences(context).getString("sample_rate_list_preference", "1").toInt()
        }

        fun setSRB1(context: Context): Boolean {
            return PreferenceManager.getDefaultSharedPreferences(context).getBoolean("switch_srb1", false)
        }
    }
}
