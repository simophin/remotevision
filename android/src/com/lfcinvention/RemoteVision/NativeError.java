package com.lfcinvention.RemoteVision;

public class NativeError {
	private boolean success;
	private String  errorString;
	
	public boolean isError() {
		return (success == false);
	}
	
	public boolean isSuccess() {
		return !isError();
	}
	
	public String getErrorString() {
		return errorString;
	}
}
