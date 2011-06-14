package com.lfcinvention.RemoteVision;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.lfcinvention.RemoteVision.VideoService.State;
import com.lfcinvention.RemoteVision.VideoService.StateException;

public class InfoActivity extends Activity {
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == 0 && resultCode == RESULT_OK) {
			destroyVideoService();
			bindVideoService();
		}
	}


	@Override
	public boolean onMenuItemSelected(int featureId, MenuItem item) {
		switch (item.getItemId()) {
		case R.id.menuItemOption:{
			Intent i = new Intent(this, SettingActivity.class);
			startActivityForResult(i, 0);
			break;
		}
		
		case R.id.menuItemExit: {
			if (mBoundService != null) {
				try {
					mBoundService.stopVideoService();
				} catch (Exception e) {
					
				} 
			}
			finish();
			break;
		}
		
		default: return false; 
		}
		
		return true;
	}


	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.main, menu);
		return true;
	}


	@Override
	protected void onDestroy() {
    	if (mBoundService != null && mBoundService.getState() != VideoService.State.STATE_IN_SERVICE) {
    		destroyVideoService();
    	}
    	
		super.onDestroy();
	}

	
	private void destroyVideoService () {
		if (mBoundService != null) {
			Intent intent = new Intent(this, VideoService.class);
			try {
				mBoundService.stopVideoService();
			} catch (Exception e){
				
			}
			mBoundService.stopService(intent);
			updateServiceStatus();
		}
		
		Notification.cancelNotify(this);
	}
	
	private void stopVideoService() {
		if (mBoundService != null) {
			try {
				mBoundService.stopVideoService();
			}catch (Exception e) {
				
			}
			updateServiceStatus();
		}
		Notification.cancelNotify(this);
	}

	/** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        // Init the configuration
    	ServiceConfiguration.initConfiguration(getApplicationContext());
        
        setContentView(R.layout.main);
        
        mBtnStart = (Button)findViewById(R.id.btnStart);
        mBtnStop  = (Button)findViewById(R.id.btnStop);
        mTextView = (TextView)findViewById(R.id.boundAddressText);
        mBtnStart.setOnClickListener(onBtnStartClicked);
        mBtnStop.setOnClickListener(onBtnStopClicked);
        
        bindVideoService();

        return;
    }
    
    private VideoService mBoundService = null;
    private TextView mTextView = null;
    private Button  mBtnStart = null;
    private Button mBtnStop;
    private ServiceConfiguration mServiceConfiguration = null;
    
    private ServiceConnection mServiceConnection = new ServiceConnection() {
		public void onServiceDisconnected(ComponentName name) {
			InfoActivity.this.mBoundService = null;
		}
		
		public void onServiceConnected(ComponentName name, IBinder service) {
			VideoService s = ((VideoService.Channel)service).getService();
			
			if (s.getState() == State.STATE_UNINITIALIZED) {
				try {
					s.initService(mServiceConfiguration);
				} catch (Exception e) {
					return;
				}
			}
			
			InfoActivity.this.mBoundService = s;
		}
	};
	
	private void bindVideoService() {
		try{
			Intent i = new Intent();
			i.setClass(getApplicationContext(), VideoService.class);

			if (VideoService.INSTANCE == null) {
				loadServiceConfiguration();
				startService(i);
			}


			if (!bindService(i, mServiceConnection, 0)) {
				Toast.makeText(this, "Can't bind service", Toast.LENGTH_LONG);
			}
		}catch(Exception e) {
			Toast.makeText(InfoActivity.this, e.getMessage(), Toast.LENGTH_LONG).show();
		}
	}
	
	private void loadServiceConfiguration () {
		mServiceConfiguration = ServiceConfiguration.getCurrentConfiguration();
	}
	
    
    private View.OnClickListener onBtnStartClicked = new View.OnClickListener() {
		public void onClick(View arg0) {
			try{
				mBoundService.startVideoService();
				updateServiceStatus();
				if (mBoundService.getState() == VideoService.State.STATE_IN_SERVICE) {
					//mBoundService.notify("Listen on " + mBoundService.getBoundAddress());
				}
			}catch(Exception e) {
				Toast.makeText(InfoActivity.this, e.getMessage(), Toast.LENGTH_LONG);
				return;
			}
			
			CharSequence seq,title,msg;
			try {
				seq = "RemoteVision服务运行中。监听于：" + mBoundService.getBoundAddress();
				title = "RemoteVision";
				msg = "监听于："+ mBoundService.getBoundAddress();
			}catch (Exception e) {
				return;
			}
			
			Notification.doNotify(InfoActivity.this, seq, title, msg);
		}
	};
	
	private View.OnClickListener onBtnStopClicked = new View.OnClickListener() {
		
		public void onClick(View v) {
			stopVideoService();
		}
	};
	
	private void updateServiceStatus () {
		if (mBoundService == null) return;
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