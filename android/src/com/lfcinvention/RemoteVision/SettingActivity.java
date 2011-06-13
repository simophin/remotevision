package com.lfcinvention.RemoteVision;

import android.os.Bundle;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceActivity;

public class SettingActivity extends PreferenceActivity implements OnPreferenceChangeListener{

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		
		
		if (mHasPreferenceChanged) setResult(RESULT_OK);
		else setResult(RESULT_CANCELED);
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		addPreferencesFromResource(R.xml.preferences);
		
		mNetworkModePreference = (ListPreference) findPreference("network_mode");
		mNetworkModePreference.setOnPreferenceChangeListener(mOnNetworkModeChanged);
		
		clearPreferences();
		setEnablePreference(mNetworkModePreference.getValue() == "0" ? "server_mode" : "relay_mode");
	}
	
	
	private OnPreferenceChangeListener mOnNetworkModeChanged = new Preference.OnPreferenceChangeListener() {
		public boolean onPreferenceChange(Preference preference, Object newValue) {
			String v = newValue.toString();
			if (v.equals(String.valueOf(0))) {
				clearPreferences();
				setEnablePreference("server_mode_group");
			}else {
				clearPreferences();
				setEnablePreference("relay_mode_group");
			}
			return true;
		}
	};
	
	private void clearPreferences() {
		Preference sp = findPreference("server_mode");
		Preference rp = findPreference("relay_mode");
		sp.setEnabled(false);
		rp.setEnabled(false);
	}
	
	private void setEnablePreference (String key) {
		Preference s = findPreference(key);
		s.setEnabled(true);
	}

	
	ListPreference mNetworkModePreference = null;
	boolean        mHasPreferenceChanged = false;

	public boolean onPreferenceChange(Preference preference, Object newValue) {
		mHasPreferenceChanged = true;
		return true;
	}
}
