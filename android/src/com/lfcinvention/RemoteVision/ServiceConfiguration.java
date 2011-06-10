package com.lfcinvention.RemoteVision;

public class ServiceConfiguration {
	public static enum NetworkMode {
		SERVER,
		RELAY_PROVIDER_CLIENT
	}
	
	public static enum NetworkType {
		WIFI_ONLY,
		GSM_ONLY,
		WIFI_PREFERED,
	}
	
	public NetworkMode networkMode = NetworkMode.SERVER;
	public NetworkType networkType = NetworkType.WIFI_PREFERED;
	public boolean     permitGSM   = false;
	public int         serverPort  = 0;
	public String      relayServerHost = null;
	public int         relayServerPort = -1;
}
