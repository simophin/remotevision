package com.lfcinvention.RemoteVision;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

public class Notification {
	public static int NOTIFY_ID = 1;
	
	public static void doNotify (Context ctx,CharSequence seq, 
			CharSequence title, CharSequence msg) {
		NotificationManager notifyMgr = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
		int icon = R.drawable.icon;
		long when = System.currentTimeMillis();
		
		android.app.Notification notification = new android.app.Notification(icon, seq, when);
		Intent intent = new Intent(ctx.getApplicationContext(), InfoActivity.class);
		PendingIntent pendingIntent = PendingIntent.getActivity(ctx, 0, intent, 
				android.app.Notification.FLAG_NO_CLEAR);
		notification.setLatestEventInfo(ctx, title, msg, pendingIntent);
		notifyMgr.notify(NOTIFY_ID, notification);
	}
	
	public static void cancelNotify (Context ctx) {
		NotificationManager notifyMgr = (NotificationManager) ctx.getSystemService(Context.NOTIFICATION_SERVICE);
		notifyMgr.cancel(NOTIFY_ID);
	}
	
}
