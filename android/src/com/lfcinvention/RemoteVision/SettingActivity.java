package com.lfcinvention.RemoteVision;

import java.util.Vector;

import android.app.AlertDialog;
import android.app.PendingIntent;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.Preference;
import android.preference.Preference.OnPreferenceChangeListener;
import android.preference.PreferenceActivity;
import android.preference.PreferenceScreen;
import android.view.KeyEvent;

public class SettingActivity extends PreferenceActivity{
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) {
			if (mHasPreferenceChanged) {
				Builder builder = new AlertDialog.Builder(this);
				builder.setMessage("有设置已经被修改，确定要修改吗？")
						.setPositiveButton("是", mOnExitSaveDialogClick)
						.setNegativeButton("否", mOnExitSaveDialogClick)
						.setTitle("确认退出")
						.show();
				return false;
			}else{
				setResult(RESULT_CANCELED);
				finish();
				return true;
			}
			
		}else{
			return super.onKeyDown(keyCode, event);
		}
	}
	
	private DialogInterface.OnClickListener mOnExitSaveDialogClick = new DialogInterface.OnClickListener() {
		
		public void onClick(DialogInterface dialog, int which) {
			switch (which) {
			case DialogInterface.BUTTON_POSITIVE: {
				StringBuffer error = new StringBuffer();
				if (!validate(error)) {
					Builder builder = new AlertDialog.Builder(SettingActivity.this);
					builder.setTitle("错误");
					builder.setMessage(error.toString());
					builder.setCancelable(true);
					builder.create().show();
				}else{
					for (int i=0; i<mChangedPreferences.size(); i++) {
						SharedPreferences.Editor editor =  mChangedPreferences.elementAt(i).getEditor();
						if (editor != null) {
							editor.commit();
						}
					}
					setResult(RESULT_OK);
					finish();
				}
				break;
			}
			
			case DialogInterface.BUTTON_NEGATIVE: {
				setResult(RESULT_CANCELED);
				finish();
				break;
			}
			}
		}
	};

	
	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		getPreferenceManager().setSharedPreferencesName(ServiceConfiguration.SERVICE_CONF_KEY);
		
		addPreferencesFromResource(R.xml.preferences);
		
		mNetworkModePreference = (ListPreference) findPreference(ServiceConfiguration.NETWORK_MODE_CONF_KEY);
		mNetworkModePreference.setOnPreferenceChangeListener(mOnNetworkModeChanged);
		
		mServerTypePreference = findPreference(ServiceConfiguration.SERVER_TYPE_CONF_KEY);
		mServerPortPreference = findPreference(ServiceConfiguration.SERVER_PORT_CONF_KEY);
		mRelayServerHostPreference = findPreference(ServiceConfiguration.RELAY_SERVER_HOST_CONF_KEY);
		mRelayServerPortPreference = findPreference(ServiceConfiguration.RELAY_SERVER_PORT_CONF_KEY);
		
		mServerTypePreference.setOnPreferenceChangeListener(new onPreferenceChanged());
		mServerPortPreference.setOnPreferenceChangeListener(new onPreferenceChanged());
		mRelayServerHostPreference.setOnPreferenceChangeListener(new onPreferenceChanged());
		mRelayServerPortPreference.setOnPreferenceChangeListener(new onPreferenceChanged());
	
		clearPreferences();
		setEnablePreference(mNetworkModePreference.getValue().equals(String.valueOf(0)) ? SERVER_GROUP_KEY : RELAY_GROUP_KEY);
	}
	
	
	private OnPreferenceChangeListener mOnNetworkModeChanged = new Preference.OnPreferenceChangeListener() {
		public boolean onPreferenceChange(Preference preference, Object newValue) {
			String v = newValue.toString();
			if (v.equals(String.valueOf(0))) {
				clearPreferences();
				setEnablePreference(SERVER_GROUP_KEY);
			}else {
				clearPreferences();
				setEnablePreference(RELAY_GROUP_KEY);
			}
			mHasPreferenceChanged = true;
			mChangedPreferences.add(preference);
			return true;
		}
	};
	
	private void clearPreferences() {
		Preference sp = findPreference(SERVER_GROUP_KEY);
		Preference rp = findPreference(RELAY_GROUP_KEY);
		sp.setEnabled(false);
		rp.setEnabled(false);
	}
	
	private void setEnablePreference (String key) {
		Preference s = findPreference(key);
		s.setEnabled(true);
	}
	
	private class onPreferenceChanged implements OnPreferenceChangeListener {
		public boolean onPreferenceChange(Preference preference, Object newValue) {
			mHasPreferenceChanged = true;
			mChangedPreferences.add(preference);
			return true;
		}
	}; 
	
	private boolean validate (StringBuffer error) {
		boolean ret = true;
		
		if (mNetworkModePreference.getValue().equals(String.valueOf(ServiceConfiguration.NetworkMode.RELAY_PROVIDER_CLIENT))) {
			if ( ((EditTextPreference)mRelayServerHostPreference).getText().trim().equals("") ) {
				ret = false;
				error.append("中继服务器地址不能为空\n");
			}
			if ( ((EditTextPreference)mRelayServerPortPreference).getText().trim().equals("") ) {
				ret = false;
				error.append("中继服务器端口不能为空\n");
			}
		}else{
			if ( ((EditTextPreference)mServerPortPreference).getText().trim().equals("") ) {
				ret = false;
				error.append("服务器端口必须设置");
			}
		}
		
		return ret;
	}

	
	PreferenceScreen mPreferenceScreen = null;
	ListPreference mNetworkModePreference = null;
	boolean        mHasPreferenceChanged = false;
	Vector<Preference> mChangedPreferences = new Vector<Preference>();
	Preference mServerTypePreference = null,
			mServerPortPreference = null,
			mRelayServerHostPreference = null,
			mRelayServerPortPreference = null;

	
	public static final String SERVER_GROUP_KEY = "server_mode_group";
	public static final String RELAY_GROUP_KEY  = "relay_mode_group";
}
