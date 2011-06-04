package com.lfcinvention.RemoteVision;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.OptionalDataException;
import java.io.StringBufferInputStream;


@SuppressWarnings("deprecation")
public class Preference {
	public static final int CONNTYPE_NETWORK = 0;
	public static final int CONNTYPE_USB	 =  1;
	public static final int CONNTYPE_BLUETOOTH = 2;

	public static final String name = "RV_PREF";
	public static final String connTypeKey = "RV_PREF_CONNTYPE"; 
	public static final String networkConfigKey = "RV_PREF_NETWORK";
	
	@SuppressWarnings("deprecation")
	public static final Object unserialize(String str) throws OptionalDataException, ClassNotFoundException, IOException {
		ObjectInputStream os = new ObjectInputStream(new StringBufferInputStream(str));
		return os.readObject();
	}
	
	public static final String serialize(Object obj) throws IOException {
		ByteArrayOutputStream bs = new ByteArrayOutputStream();
		ObjectOutputStream os = new ObjectOutputStream(bs);
		os.writeObject(obj);
		return bs.toString();
	}
}
