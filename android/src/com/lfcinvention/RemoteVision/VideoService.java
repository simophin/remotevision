package com.lfcinvention.RemoteVision;

import java.io.ObjectInputStream;
import java.io.StringBufferInputStream;

import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
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

	
	@SuppressWarnings("deprecation")
	@Override
	public void onCreate() {
		super.onCreate();
		
		int connType = mPref.getInt(Preference.connTypeKey, Preference.CONNTYPE_NETWORK);
		
		try{
			switch (connType) {
			case Preference.CONNTYPE_NETWORK:
				String networkConfigStr = mPref.getString(Preference.networkConfigKey, "");
				NetworkConfiguration networkConfig;
				if (networkConfigStr.trim().length() < 1) {
					networkConfig = new NetworkConfiguration();
				}else{
					ObjectInputStream os = new ObjectInputStream(new StringBufferInputStream(networkConfigStr));
					networkConfig =  (com.lfcinvention.RemoteVision.NetworkConfiguration) os.readObject();

				}
			}
		}catch (Exception e) {
			// TODO: handle exception
		}
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
		}
		
		public void stopService() throws StateErrorException{
			if (mState != State.STATE_IN_SERVICE) {
				throw new StateErrorException("State should be running");
			}
		}
		
		public String getErrorString() {
			return mErrorString;
		}
	}

	
	private int mNativeServer = 0;
	private native int    nativeCreateServer(String addr, int port);
	private native String nativeGetBoundAddress(int nativeServer);
}
