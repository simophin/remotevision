package com.lfcinvention.RemoteVision;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.util.Enumeration;

import android.app.Service;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.IBinder;

import com.lfcinvention.RemoteVision.ServiceConfiguration.NetworkMode;
import com.lfcinvention.RemoteVision.ServiceConfiguration.ServerType;


public class VideoService extends Service {
	@Override
	public boolean stopService(Intent name) {
		if (mState == State.STATE_IN_SERVICE) {
			try {
				stopVideoService();
			} catch (StateException e) {	
			} catch (NativeException e) {
			}
		}
		if (mState == State.STATE_READY) {
			nativeDestroyServerInstance(mNativeServer);
		}
		INSTANCE = null;
		return super.stopService(name);
	}

	public static VideoService INSTANCE = null;
	
	public static enum State {
		STATE_UNINITIALIZED,
		STATE_READY,
		STATE_IN_SERVICE
	}
	
	public class Channel extends Binder {
		public VideoService getService() {
			return VideoService.this; 
		}
	}
	
	public class NoInterfaceAvailable extends Exception {
		private static final long serialVersionUID = -4751554369144548163L;
	}
	
	public class StateException extends Exception {
		private static final long serialVersionUID = -8414417754405215499L;
	}
	
	public class NativeException extends Exception {
		public NativeException(NativeError err) {
			mError = err;
		}
		
		@Override
		public String getMessage() {
			return mError.getErrorString();
		}
		
		public NativeError getNativeError() {
			return mError;
		}
			
		private static final long serialVersionUID = 8732958897546506120L;
		private NativeError mError = null;
	}

	@Override
	public IBinder onBind(Intent intent) {
		return mChannel;
	}
	
	@Override
	public void onCreate() {
		mChannel = new Channel();
		INSTANCE = this;
		super.onCreate();
	}

	@Override
	public void onDestroy() {
		
		super.onDestroy();
	}

	@Override
	public void onLowMemory() {
		// TODO Auto-generated method stub
		super.onLowMemory();
	}
	
	public State getState() {
		return mState;
	}
	
	public void initService (ServiceConfiguration config) throws SocketException, NoInterfaceAvailable, StateException, NativeException {
		String boundAddr = null;
		int port = -1;
		int serviceType = 0;
		
		if (mState != State.STATE_UNINITIALIZED) {
			throw new StateException();
		}
		
		if (config.networkMode == NetworkMode.SERVER) {
			boundAddr = getAvailableAddress(config.serverType);
			if (boundAddr == null) {
				throw new NoInterfaceAvailable();
			}
			port = config.serverPort;
			serviceType = 0;
		}else /* if (config.networkMode == NetworkMode.RELAY_PROVIDER_CLIENT) */ {
			boundAddr = config.relayServerHost;
			port = config.relayServerPort;
			serviceType = 1;
		}
		
		try {
			InetAddress addr = InetAddress.getByName(boundAddr);
			boundAddr = addr.getHostAddress();
		} catch (UnknownHostException e) {
			throw new NoInterfaceAvailable();
		}
		
		NativeError error = new NativeError();
		int server = nativeCreateServerInstance(error, serviceType, boundAddr, port);
		if (error.isError()) {
			throw new NativeException(error);
		}
		mNativeServer = server;
		mState = State.STATE_READY;
	}
	
	public void startVideoService () throws StateException, NativeException {
		if (mState != State.STATE_READY) {
			throw new StateException();
		}
		
		NativeError error =  nativeStartService(mNativeServer);
		if (error.isError()) {
			throw new NativeException(error);
		}
		
		mState = State.STATE_IN_SERVICE;
		
	}
	
	public void stopVideoService() throws StateException, NativeException {
		if (mState != State.STATE_IN_SERVICE) {
			throw new StateException();
		}
		
		NativeError error =  nativeStopService(mNativeServer);
		if (error.isError()) {
			throw new NativeException(error);
		}
		mState = State.STATE_READY;
	}
	
	public String getBoundAddress () throws StateException {
		if (mState == State.STATE_READY) {
			throw new StateException();
		}
		return nativeGetBoundAddress(mNativeServer);
	}
	
	public Bitmap getPreviewImage (int ms) throws com.lfcinvention.RemoteVision.VideoService.NativeException {
		Bitmap ret = null;
		if (mPreviewData == null) return ret;
		
		NativeError error = new NativeError();
		NativePreviewImage img = new NativePreviewImage();
		img.buf = mPreviewData;
		img.size = mPreviewData.length;
		error = nativeGetPreviewImage(mNativeServer, img);
		if (error.isError()) {
			throw new NativeException(error);
		}
		if (img.usedSize > 0) {
			ret = BitmapFactory.decodeByteArray(mPreviewData, 0, img.usedSize);
		}
		return ret;
	}

	public void setPreviewingImage (boolean on) {
		if (on) {
			if (mPreviewData == null) {
				mPreviewData = new byte[1024000];
			}
		}else{
			if (mPreviewData != null) {
				mPreviewData = null;
			}
		}
	}
	
	private String getAvailableAddress (int ntype) throws SocketException {
		String addr = null;
		
		// Find a wifi network
		if (ntype == ServerType.WIFI_ONLY || 
				ntype == ServerType.WIFI_PREFERED) {
			WifiManager wifiMgr = (WifiManager) getSystemService(WIFI_SERVICE);
			if (wifiMgr.isWifiEnabled()){
				WifiInfo info = wifiMgr.getConnectionInfo();
				addr = intToIp(info.getIpAddress());
			}
		}
		
		if (ntype == ServerType.WIFI_ONLY || 
				(ntype == ServerType.WIFI_PREFERED && (addr != null)) ) return addr;
		
		
		// Find a gsm network
		Enumeration<NetworkInterface> interfaces = NetworkInterface.getNetworkInterfaces();
		InetAddress foundAddr = null;
		while(interfaces.hasMoreElements() && (foundAddr == null)) {
			NetworkInterface iface = interfaces.nextElement();
			Enumeration<InetAddress> addresses = iface.getInetAddresses();
			while (addresses.hasMoreElements() && (foundAddr == null)) {
				InetAddress inetAddr = addresses.nextElement();
				if (!inetAddr.isLinkLocalAddress() && !inetAddr.isLoopbackAddress() && 
						!inetAddr.isMulticastAddress()) {
					foundAddr = inetAddr;
					break;
				}
			}
		}
		
		if (foundAddr == null) return addr;
		addr = foundAddr.getHostAddress();
		
		return addr;
	}
	
	private String intToIp(int i)
	{ 
		return ( i & 0xFF)+ "." + ((i >> 8 ) & 0xFF)  + "." + ((i >> 16 ) & 0xFF) +"."+((i >> 24 ) & 0xFF ) ; 
	}
	
	private ServiceConfiguration mConfiguration = null;
	private State   mState = State.STATE_UNINITIALIZED;
	private Channel mChannel = null;
	private byte[]  mPreviewData = null;
	
	private int mNativeServer;
	private native int nativeCreateServerInstance (NativeError error, int server_type, String host,int port);
	private native NativeError nativeGetPreviewImage (int server,NativePreviewImage img);
	private native NativeError nativeDestroyServerInstance (int server);
	private native NativeError nativeStartService(int server);
	private native NativeError nativeStopService(int server);
	private native String nativeGetBoundAddress(int server);
}