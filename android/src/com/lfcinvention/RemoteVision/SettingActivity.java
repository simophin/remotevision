package com.lfcinvention.RemoteVision;

import android.app.Activity;
import android.os.Bundle;
import android.text.Layout;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.TableLayout.LayoutParams;

public class SettingActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.setting);
		setTitle(getString(R.string.setting_text));
		mConfiguration = ServiceConfiguration.getCurrentConfiguration();
		
		mSpinner = (Spinner) findViewById(R.id.spinConnectMode);
		mRowContent = (LinearLayout) findViewById(R.id.rowSettingContent);
		
		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
				R.array.setting_connect_mode_string_array, android.R.layout.simple_spinner_item);
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		mSpinner.setAdapter(adapter);

		mSpinner.setOnItemSelectedListener(onNetworkModeSelected);
	}
	
	
	
	private void setupServerMode (ServiceConfiguration conf) {
		LayoutInflater inflater = (LayoutInflater) getSystemService(LAYOUT_INFLATER_SERVICE);
		View v = inflater.inflate(R.layout.setting_server_mode, null);
		mRowContent.addView(v,0,new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		
		ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
				R.array.setting_server_network_string_array, android.R.layout.simple_spinner_item);
		adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		
		Spinner networkSpinner = (Spinner) v.findViewById(R.id.spinnerServerNetworkMode);
		networkSpinner.setAdapter(adapter);
		networkSpinner.setSelection(conf.serverType);
		
		TextView portView = (TextView) findViewById(R.id.serverModePort);
		if (conf.serverPort >= 0) {
			portView.setText(String.valueOf(conf.serverPort));
		}
	}
	
	private void setupRelayMode (ServiceConfiguration conf) {
		LayoutInflater inflater = (LayoutInflater) getSystemService(LAYOUT_INFLATER_SERVICE);
		View v = inflater.inflate(R.layout.setting_relay_mode, null);
		mRowContent.addView(v,0,new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		
		TextView serverAddr = (TextView) findViewById(R.id.relayServerHostEdit);
		serverAddr.setText(conf.relayServerHost);
		
		TextView port = (TextView) findViewById(R.id.relayServerPortEdit);
		port.setText(String.valueOf(conf.relayServerPort));
	}
	
	private void clearMainSettingContent () {
		mRowContent.removeAllViews();
	}
	
	private OnItemSelectedListener onNetworkModeSelected = new OnItemSelectedListener() {

		public void onItemSelected(AdapterView<?> parent, View vew, int pos,
				long id) {
			switch (pos) {
			default:
			case 0: // SERVER MODE
			{
				clearMainSettingContent();
				setupServerMode(mConfiguration);
				break;
			}
			
			case 1: // RELAY MODE
			{
				clearMainSettingContent();
				setupRelayMode(mConfiguration);
				break;
			}
			}
		}

		public void onNothingSelected(AdapterView<?> arg0) {
			
		}
	};

	
	private Spinner mSpinner = null;
	private LinearLayout mRowContent = null;
	private ServiceConfiguration mConfiguration = null;
}
