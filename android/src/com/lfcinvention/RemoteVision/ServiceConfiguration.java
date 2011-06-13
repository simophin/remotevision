package com.lfcinvention.RemoteVision;

import android.content.Context;
import android.content.SharedPreferences;

public class ServiceConfiguration {
	class NetworkMode {
		public static final int SERVER = 0;
		public static final int RELAY_PROVIDER_CLIENT = 1;
	}
	
	class ServerType {
		public static final int WIFI_ONLY=0;
		public static final int WIFI_PREFERED=1;
		public static final int GSM_ONLY=2;
	}
	
	public static ServiceConfiguration 
	getCurrentConfiguration () {
		ServiceConfiguration ret = new ServiceConfiguration();
		SharedPreferences p = mContext.getSharedPreferences("service_config", 0);
		ret.networkMode = p.getInt("network_mode", NetworkMode.SERVER);
		ret.serverType = p.getInt("network_type", ServerType.WIFI_PREFERED);
		ret.serverPort  = p.getInt("server_port", 0);
		ret.relayServerHost = p.getString("relay_server", new String());
		return ret;
	}
	
	public static boolean
	saveConfiguration (ServiceConfiguration conf) {
		SharedPreferences p = mContext.getSharedPreferences("service_config", 0);
		SharedPreferences.Editor editor = p.edit();
		editor.putInt("network_mode", conf.networkMode);
		editor.putInt("network_type", conf.serverType);
		editor.putString("relay_server", conf.relayServerHost);
		editor.putInt("server_port", conf.serverPort);
		return editor.commit();
	}
	
	public static void initConfiguration (Context p) {
		mContext = p;
	}
	
	public int networkMode = NetworkMode.SERVER;
	public int serverType = ServerType.WIFI_PREFERED;
	public int         serverPort  = 0;
	public String      relayServerHost = null;
	public int         relayServerPort = 15001;
	
	private static Context mContext = null;
}
