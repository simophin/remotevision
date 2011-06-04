package com.lfcinvention.RemoteVision;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.IBinder;

public class VideoService extends Service {
	
	static enum State {
		STATE_READY,
		STATE_IN_SERVICE,
		STATE_ERROR,
	}
	
	private SharedPreferences mPref = getSharedPreferences(Preference.name, 0);
	private ServiceChannel mChannel = new ServiceChannel();
	private State       mState      = State.STATE_ERROR;
	private String     mErrorString = "";
	private WifiManager mWifiManger = (WifiManager) getSystemService(WIFI_SERVICE);
	private NotificationManager mNotificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
	private NetworkConfiguration mNetworkConfig = null;
	private final int NOTIFY_ID = 1;

	
	@Override
	public void onCreate() {
		
		
		int connType = mPref.getInt(Preference.connTypeKey, Preference.CONNTYPE_NETWORK);
		
		try{
			switch (connType) {
			case Preference.CONNTYPE_NETWORK:
				String networkConfigStr = mPref.getString(Preference.networkConfigKey, "");
				if (networkConfigStr.trim().length() < 1) {
					mNetworkConfig = new NetworkConfiguration();
				}else{
					mNetworkConfig =  (com.lfcinvention.RemoteVision.NetworkConfiguration)Preference.unserialize(networkConfigStr);
				}
				
				switch(mNetworkConfig.networkType) {
				case NetworkConfiguration.NT_WIFI: {
					if (!mWifiManger.isWifiEnabled() || 
							mWifiManger.getConnectionInfo() == null) {
						updateState (State.STATE_ERROR, "Wifi is not enabled");
						break;
					}
					setUseWifi();
					updateState(State.STATE_READY, "Listen on " + nativeGetBoundAddress(mNativeServer));
					break;
				}
				
				case NetworkConfiguration.NT_GSM: {
					setUseGSM();
					updateState(State.STATE_READY, "Listen on " + nativeGetBoundAddress(mNativeServer));
					break;
				}
				}
			
				break;
				
			default:
				updateState(State.STATE_ERROR,"Unimplement connection type");
				break;
			}
		}catch (Exception e) {
			updateState(State.STATE_ERROR,e.getMessage());
		}
		
		super.onCreate();
	}


	@Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}


	@Override
	public void onLowMemory() {
		// TODO Auto-generated method stub
		super.onLowMemory();
	}
	
	@Override
	public IBinder onBind(Intent arg0) {
		return (IBinder) mChannel;
	}
	
	public class StateErrorException extends Exception {
		
		String errorString;
		
		public StateErrorException(String str) {
			errorString = str;
		}
		
		@Override
		public String getMessage() {
			return errorString;
		}
		private static final long serialVersionUID = 333838606534316252L;
	}
	
	public class CreateServerException extends Exception {
		private static final long serialVersionUID = 6397572426195074232L;
	}
	
	public class ServiceChannel {
		public String getBoundAddress() {
			return nativeGetBoundAddress(mNativeServer);
		}
		
		public State  getServiceState() {
			return mState;
		}
		
		public void startService() throws StateErrorException {
			if (mState != State.STATE_READY) {
				throw new StateErrorException("State should be ready");
			}
			nativeStartServer(mNativeServer, true);
		}
		
		public void stopService() throws StateErrorException{
			if (mState != State.STATE_IN_SERVICE) {
				throw new StateErrorException("State should be running");
			}
			nativeStartServer(mNativeServer, false);
		}
		
		public VideoService getService() {
			return VideoService.this;
		}
		
		public String getErrorString() {
			return mErrorString;
		}
	}

	
	public void setUseWifi () throws CreateServerException {
		WifiManager wm = (WifiManager) getSystemService(WIFI_SERVICE);
		WifiInfo info = wm.getConnectionInfo();
		
		String address = intToIp(info.getIpAddress());
		int ptr = nativeCreateServer(address, this.mNetworkConfig.port);
		if (ptr == 0) {
			throw new CreateServerException();
		}
		mNativeServer = ptr;
	}
	
	public void setUseGSM () {
		
	}
	
	private void updateState(State s, String str) {
		int icon = R.drawable.icon;
		CharSequence tickerText = str;
		long when = System.currentTimeMillis();
		mState = s;
		mErrorString = str;
		
		Notification notification = new Notification(icon, tickerText, when);
		CharSequence title = getResources().getText(R.string.app_name);
		CharSequence content = str;
		Intent notificationIntent = new Intent(this,InfoActivity.class);
		PendingIntent contentIntent = PendingIntent.getActivity(getApplicationContext(), 0, 
				notificationIntent, 0);
		notification.setLatestEventInfo(getApplicationContext(), title, content, contentIntent);
		
		mNotificationManager.notify(NOTIFY_ID, notification);
	}
	
	private String intToIp(int i)
	{ 
		return ( i & 0xFF)+ "." + ((i >> 8 ) & 0xFF)  + "." + ((i >> 16 ) & 0xFF) +"."+((i >> 24 ) & 0xFF ) ; 
	}

	private int mNativeServer = 0;
	private native int    nativeCreateServer(String addr, int port);
	private native void   nativeDestroyServer(int server);
	private native void   nativeStartServer(int server,boolean start);
	private native String nativeGetBoundAddress(int nativeServer);
}
