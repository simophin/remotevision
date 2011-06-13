package com.lfcinvention.RemoteVision;

import com.lfcinvention.RemoteVision.ServiceConfiguration.NetworkMode;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.Layout;
import android.text.TextWatcher;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnKeyListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TableRow;
import android.widget.TextView;
import android.widget.TableLayout.LayoutParams;
import android.widget.TextView.OnEditorActionListener;

public class SettingActivity extends Activity {

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
	}



	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.setting);
		setTitle(getString(R.string.setting_text));
		mConfiguration = ServiceConfiguration.getCurrentConfiguration();
		
		mSpinner = (Spinner) findViewById(R.id.spinConnectMode);
		mRowContent = (LinearLayout) findViewById(R.id.rowSettingContent);
		mBtnSave = (Button) findViewById(R.id.btnSave);
		mBtnCancel = (Button) findViewById(R.id.btnCancel);
		
		mBtnSave.setOnClickListener(onBtnSaveClicked);
		
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
		
		EditText portView = (EditText) findViewById(R.id.serverModePort);
		portView.addTextChangedListener(new onValidateNeededTextWatcher());
		if (conf.serverPort >= 0) {
			portView.setText(String.valueOf(conf.serverPort));
		}
	}
	
	private void setupRelayMode (ServiceConfiguration conf) {
		LayoutInflater inflater = (LayoutInflater) getSystemService(LAYOUT_INFLATER_SERVICE);
		View v = inflater.inflate(R.layout.setting_relay_mode, null);
		mRowContent.addView(v,0,new LayoutParams(LayoutParams.FILL_PARENT, LayoutParams.FILL_PARENT));
		
		EditText serverAddr = (EditText) findViewById(R.id.relayServerHostEdit);
		serverAddr.setText(conf.relayServerHost);
		serverAddr.addTextChangedListener(new onValidateNeededTextWatcher());
		
		EditText port = (EditText) findViewById(R.id.relayServerPortEdit);
		port.addTextChangedListener(new onValidateNeededTextWatcher());
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
				mConfiguration.networkMode = NetworkMode.SERVER;
				clearMainSettingContent();
				setupServerMode(mConfiguration);				
				break;
			}
			
			case 1: // RELAY MODE
			{
				mConfiguration.networkMode = NetworkMode.RELAY_PROVIDER_CLIENT;
				clearMainSettingContent();
				setupRelayMode(mConfiguration);
				break;
			}
			}
			
			mBtnSave.setEnabled(validate());
		}

		public void onNothingSelected(AdapterView<?> arg0) {
			
		}
	};
	
	private class onValidateNeededTextWatcher implements TextWatcher {
		public void afterTextChanged(Editable s) {
		
		}

		public void beforeTextChanged(CharSequence s, int start, int count,
				int after) {
			
		}

		public void onTextChanged(CharSequence s, int start, int before,
				int count) {
			mBtnSave.setEnabled(validate());	
		}
	}
	
	private OnClickListener onBtnSaveClicked = new OnClickListener() {
		public void onClick(View v) {
			
		}
	};
	
	private boolean validate () {
		boolean ret = true;
		switch( mConfiguration.networkMode) {
		case NetworkMode.SERVER: {
			break;
		}
		
		case NetworkMode.RELAY_PROVIDER_CLIENT: {
			TextView hostEdit = (TextView) findViewById(R.id.relayServerHostEdit);
			TextView portEdit = (TextView) findViewById(R.id.relayServerPortEdit);
			if (hostEdit.getText().toString().trim().length() < 1) {
				return false;
			}
			if (portEdit.getText().toString().trim().length() < 1) {
				return false;
			}
			if ( Integer.parseInt(portEdit.getText().toString()) <= 0) {
				return false;
			}
			break;
		}
		
		default: ret = false; break;
		}
		
		
		
		return ret;
	}

	
	private Button  mBtnSave = null, mBtnCancel = null;
	private Spinner mSpinner = null;
	private LinearLayout mRowContent = null;
	private ServiceConfiguration mConfiguration = null;
}
