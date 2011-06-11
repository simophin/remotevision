package com.lfcinvention.RemoteVision;

public class NativeError {
	private boolean isSuccess;
	private String  errorString;
	
	public boolean isError() {
		return (isSuccess == false);
	}
	
	public boolean isSuccess() {
		return !isError();
	}
	
	public String getErrorString() {
		return errorString;
	}
}
