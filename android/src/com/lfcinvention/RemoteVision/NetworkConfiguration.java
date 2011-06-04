package com.lfcinvention.RemoteVision;

import java.io.Serializable;

public class NetworkConfiguration implements Serializable{
	public static final int NT_WIFI = 0;
	public static final int NT_GSM  = 1;
	
	public int networkType = NT_GSM;
	public int port = -1;
	
	private static final long serialVersionUID = -6104476854994271926L;
}
