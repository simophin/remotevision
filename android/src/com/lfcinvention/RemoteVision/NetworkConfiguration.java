package com.lfcinvention.RemoteVision;

import java.io.Serializable;

public class NetworkConfiguration implements Serializable{
	public boolean permitNonWifi = false;
	public int port = -1;
	
	private static final long serialVersionUID = -6104476854994271926L;
}
