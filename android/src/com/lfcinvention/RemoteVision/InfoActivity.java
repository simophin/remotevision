package com.lfcinvention.RemoteVision;

import com.lfcinvention.RemoteVision.VideoService.StateException;

import android.app.Activity;
import android.app.Service;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.text.InputFilter.LengthFilter;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class InfoActivity extends Activity {
    @Override
	protected void onDestroy() {
		
    	if (mBoundService != null) {
    		if (mBoundService.getState() != VideoService.State.STATE_IN_SERVICE) {
    			mBoundService.stopSelf();
    		}
    	}
    	
		super.onDestroy();
	}


	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        
        mBtnStart = (Button)findViewById(R.id.btnStart);
        mBtnStop  = (Button)findViewById(R.id.btnStop);
        mTextView = (TextView)findViewById(R.id.boundAddressText);
        mBtnStart.setOnClickListener(onBtnStartClicked);
        mBtnStop.setOnClickListener(onBtnStopClicked);

		bindService();
    }
    
    private VideoService mBoundService = null;
    private boolean mIsBound = false; 
    private TextView mTextView = null;
    private Button  mBtnStart = null;
    private Button mBtnStop;
    private ServiceConfiguration mServiceConfiguration = null;
    
    private ServiceConnection mServiceConnection = new ServiceConnection() {
		public void onServiceDisconnected(ComponentName name) {
			InfoActivity.this.mBoundService = null;
			InfoActivity.this.mIsBound = false;
		}
		
		public void onServiceConnected(ComponentName name, IBinder service) {
			VideoService s = ((VideoService.Channel)service).getService();
			
			InfoActivity.this.mBoundService = s;
			InfoActivity.this.mIsBound = true;
			
			updateServiceStatus();
		}
	};
	
	private void bindService() {
		try{
			Intent i = new Intent();
			i.setClass(getApplicationContext(), VideoService.class);

			if (VideoService.INSTANCE == null) {
				startService(i);
				VideoService.INSTANCE.initService(mServiceConfiguration);
			}

			if (!bindService(i, mServiceConnection, 0)) {
				Toast.makeText(this, "Can't bind service", Toast.LENGTH_LONG);
			}
		}catch(Exception e) {
			Toast.makeText(InfoActivity.this, e.getMessage(), Toast.LENGTH_LONG).show();
		}
	}
	
    
    private View.OnClickListener onBtnStartClicked = new View.OnClickListener() {
		public void onClick(View arg0) {
			try{
				mBoundService.startService();
				updateServiceStatus();
				if (mBoundService.getState() == VideoService.State.STATE_IN_SERVICE) {
					//mBoundService.notify("Listen on " + mBoundService.getBoundAddress());
				}
			}catch(Exception e) {
				Toast.makeText(InfoActivity.this, e.getMessage(), Toast.LENGTH_LONG);
			}
		}
	};
	
	private View.OnClickListener onBtnStopClicked = new View.OnClickListener() {
		
		public void onClick(View v) {
			try{
				mBoundService.stopService();
				updateServiceStatus();
			}catch(Exception e) {
				Toast.makeText(InfoActivity.this, e.getMessage(), Toast.LENGTH_LONG);
			}
		}
	};
	
	private void updateServiceStatus () {
		if (!mIsBound) return;
		VideoService.State state = mBoundService.getState() ;
		if (state ==  VideoService.State.STATE_IN_SERVICE) {
			mBtnStart.setEnabled(false);
			mBtnStop.setEnabled(true);
			try {
				String addr = mBoundService.getBoundAddress();	
				mTextView.setText(addr);
			} catch (StateException e) {
				e.printStackTrace();
			}
			
		}else if (state == VideoService.State.STATE_READY) {
			mBtnStart.setEnabled(true);
			mBtnStop.setEnabled(false);
			mTextView.setText("Ready to start");
		}
	}
	
	
	static {
		System.loadLibrary("RemoteVision");
	}
}