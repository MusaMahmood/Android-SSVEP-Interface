package com.yeolabgt.mahmoodms.androidssvepinterfacetf;

import android.app.ActionBar;
import android.content.Context;
import android.os.Bundle;
import android.preference.PreferenceFragment;
import android.preference.PreferenceManager;
import android.support.annotation.Nullable;
import android.support.v4.app.NavUtils;
import android.view.MenuItem;

/**
 * Created by mmahmood31 on 11/2/2017.
 *
 */

public class PreferencesFragment extends PreferenceFragment {
    //Key Association:
    public static String SETTINGS_CH_SELECT = "switch_ch";
    public static String SETTINGS_SHOW_PSDA = "psda";
    public static String SETTINGS_PSD_RANGE = "psda_long";
    public static String SETTINGS_SHOW_BUTTONS = "showUIElements";

    public static boolean channelSelect(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_CH_SELECT, true);
    }

    public static boolean showPSDA(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_SHOW_PSDA, true);
    }

    public static boolean psdaWideRange(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_PSD_RANGE, false);
    }

    public static boolean showUIElements(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getBoolean(SETTINGS_SHOW_BUTTONS, false);
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.prefs);
    }
}
