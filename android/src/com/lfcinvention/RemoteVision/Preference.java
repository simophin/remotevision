package com.lfcinvention.RemoteVision;

import java.io.Serializable;


public class Preference {
	public static final int CONNTYPE_NETWORK = 0;
	public static final int CONNTYPE_USB	 =  1;
	public static final int CONNTYPE_BLUETOOTH = 2;

	public static final String name = "RV_PREF";
	public static final String connTypeKey = "RV_PREF_CONNTYPE"; 
	public static final String networkConfigKey = "RV_PREF_NETWORK";
}
