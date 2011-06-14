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
	
	public static final String SERVICE_CONF_KEY = "service_config";
	public static final String NETWORK_MODE_CONF_KEY = "network_mode";
	public static final String SERVER_TYPE_CONF_KEY = "server_type";
	public static final String SERVER_PORT_CONF_KEY = "server_port";
	public static final String RELAY_SERVER_HOST_CONF_KEY = "relay_server_host";
	public static final String RELAY_SERVER_PORT_CONF_KEY = "relay_server_port";
	
	public static ServiceConfiguration 
	getCurrentConfiguration () {
		ServiceConfiguration ret = new ServiceConfiguration();
		SharedPreferences p = mContext.getSharedPreferences(SERVICE_CONF_KEY, 0);

		ret.networkMode = Integer.parseInt( p.getString(NETWORK_MODE_CONF_KEY, String.valueOf(NetworkMode.SERVER)));
		ret.serverType = Integer.parseInt(p.getString(SERVER_TYPE_CONF_KEY, String.valueOf(ServerType.WIFI_PREFERED)));
		ret.serverPort  = Integer.parseInt(p.getString(SERVER_PORT_CONF_KEY, String.valueOf(0)));
		ret.relayServerHost = p.getString(RELAY_SERVER_HOST_CONF_KEY, new String());
		ret.relayServerPort = Integer.parseInt(p.getString(RELAY_SERVER_PORT_CONF_KEY, String.valueOf(15000)));

		return ret;
	}
	
	public static boolean
	saveConfiguration (ServiceConfiguration conf) {
		SharedPreferences p = mContext.getSharedPreferences(SERVICE_CONF_KEY, 0);
		SharedPreferences.Editor editor = p.edit();
		editor.putString(NETWORK_MODE_CONF_KEY, String.valueOf(conf.networkMode));
		editor.putString(SERVER_TYPE_CONF_KEY, String.valueOf(conf.serverType));
		editor.putString(RELAY_SERVER_HOST_CONF_KEY, conf.relayServerHost);
		editor.putString(SERVER_PORT_CONF_KEY, String.valueOf(conf.serverPort));
		editor.putString(RELAY_SERVER_PORT_CONF_KEY, String.valueOf(conf.relayServerPort));
		return editor.commit();
	}
	
	public static boolean
	checkConfiguration (ServiceConfiguration conf,String err) {
		boolean ret = true;
		switch (conf.networkMode) {
		case NetworkMode.SERVER: {
			if (conf.serverPort < 0) {
				ret = false;
				err += "服务器绑定端口号错误";
			}
			break;
		}
		
		case NetworkMode.RELAY_PROVIDER_CLIENT: {
			if (conf.relayServerHost.trim().length() < 1) {
				ret = false;
				err += "中继服务器地址无效";
				break;
			}
			if (conf.relayServerPort <= 0) {
				ret = false;
				err += "中继服务器端口无效";
			}
			break;
		}
		}
		
		return ret;
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
