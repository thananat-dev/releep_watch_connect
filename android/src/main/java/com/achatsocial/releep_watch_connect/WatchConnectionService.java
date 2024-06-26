package com.achatsocial.releep_watch_connect;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Handler;
import android.os.IBinder;
import android.preference.PreferenceManager;

import androidx.annotation.RequiresApi;

import com.yucheng.ycbtsdk.YCBTClient;
import com.yucheng.ycbtsdk.response.BleDataResponse;

import java.util.HashMap;

public class WatchConnectionService extends Service {
    private static final String CHANNEL_ID = "ForegroundServiceChannel";
    private static final int NOTIFICATION_ID = 1;

    private NotificationManager mNotificationManager;
    private Handler mHandler;
    private Handler mWatchHandler;
    private int mCount;
    private String status;
    private int watchBatt;
    String watchMac;
    String userLoginToken;
    String serverIP;
    int DelayHour = 1000 * 60 * 60;
    private static boolean isStoppedByApp = false;

    @Override
    public void onCreate() {
        super.onCreate();
        mNotificationManager = getSystemService(NotificationManager.class);
        mHandler = new Handler();
        mWatchHandler = new Handler();
        mCount = 0;
        watchMac = "Loading...";
        status = "initial...";
        watchBatt = 0;
        userLoginToken = "";
        serverIP = "";
        isStoppedByApp = false;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationChannel channel = new NotificationChannel(CHANNEL_ID, "Foreground Service Channel",
                    NotificationManager.IMPORTANCE_LOW);
            mNotificationManager.createNotificationChannel(channel);

            Notification notification = buildNotification();

            startForeground(NOTIFICATION_ID, notification);
        } else {
            startForeground(NOTIFICATION_ID, new Notification());
        }

        SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(this);
        watchMac = sharedPref.getString("KEY_BLE_WATCH", "");
        userLoginToken = sharedPref.getString("USER_LOGIN_TOKEN", "");
        serverIP = sharedPref.getString("SERVER_IP", "");

        mHandler.postDelayed(mUpdateNotificationRunnable, 10000); // update every 10 seconds
        mWatchHandler.postDelayed(mUpdateWatchStatus, DelayHour); // update every 1 hour
        // do some background work here...

        return START_STICKY;
    }

//    private Runnable mUpdateNotificationRunnable = new Runnable() {
//        @Override
//        public void run() {
//            int bleState = YCBTClient.connectState();
//            String status = UtilReleepWatch.getStatusName(bleState);
//            mCount++;
//
//
//            Notification updatedNotification = new Notification.Builder(WatchConnectionService.this, CHANNEL_ID)
//                    .setContentTitle("Foreground Service")
//                    .setContentText("Service is running in foreground (" + mCount + ") "+ status)
//                    .setSmallIcon(R.drawable.abc_vector_test)
//                    .build();
//            mNotificationManager.notify(NOTIFICATION_ID, updatedNotification);
//            mHandler.postDelayed(this, 10000); // schedule the next update
//        }
//    };


    long maxDuration = 60000;
    Long startTime = null;
    long currentTime = System.currentTimeMillis();
    private Runnable mUpdateNotificationRunnable = new Runnable() {
        @RequiresApi(api = Build.VERSION_CODES.O)
        @Override
        public void run() {int bleState = YCBTClient.connectState();
            status = UtilReleepWatch.getStatusName(bleState);
            if(bleState != UtilReleepWatch.CONNECTED && bleState != UtilReleepWatch.CONNECTING && bleState != UtilReleepWatch.READ_WRITE_OK){
               if(!watchMac.equals("")) {
                   long currentTime = System.currentTimeMillis();
                   if (startTime == null) {
                       startTime = currentTime;
                   }
                   if (currentTime - startTime <= maxDuration) {
                       int code = UtilReleepWatch.connectWatchBleWithMac(watchMac);
                   }
               }
            }else{
                startTime = null;
            }

            mCount++;
            Notification updatedNotification = buildNotification();
            mNotificationManager.notify(NOTIFICATION_ID, updatedNotification);
            if (mCount == 1082) { // when 3 hour restart service
                isStoppedByApp = true;
                YCBTClient.disconnectBle();
                stopSelf();
                startService(new Intent(getApplicationContext(), WatchConnectionService.class));
            } else {
                mHandler.postDelayed(this, 10000); // schedule the next update
            }
        }
    };

    @RequiresApi(api = Build.VERSION_CODES.O)
    private Notification buildNotification() {
        return new Notification.Builder(WatchConnectionService.this, CHANNEL_ID)
                .setContentTitle("Watch : "+ watchMac)
                .setContentText(" (" + mCount + ") "+ status)
                .setSmallIcon(R.drawable.watch_67)
                .setLargeIcon(BitmapFactory.decodeResource(getResources(), R.drawable.watch_67))
                .build();
    }

    private Runnable mUpdateWatchStatus = new Runnable() {
        @Override
        public void run() {

            if(!serverIP.equals("") && !userLoginToken.equals("") && !watchMac.equals("")){
                UtilReleepWatch.syncHealthData(watchMac,userLoginToken,serverIP);
            }

//            YCBTClient.getDeviceInfo(new BleDataResponse() {
//                @Override
//                public void onDataResponse(int code, float ratio, HashMap resultMap) {
//                    if (resultMap != null){
////                    tDataMap.put("deviceBatteryValue", tBatteryNum);
//                    }
//                }
//            });

//            Notification updatedNotification = new Notification.Builder(WatchConnectionService.this, CHANNEL_ID)
//                    .setContentTitle("Watch : "+ watchMac)
//                    .setContentText(""+ status)
//                    .setSmallIcon(R.drawable.watch_67)
//                    .setLargeIcon(BitmapFactory.decodeResource(getResources(), R.drawable.watch_67))
//                    .build();
//            mNotificationManager.notify(NOTIFICATION_ID, updatedNotification);

            mWatchHandler.postDelayed(this, DelayHour); // schedule the next update
        }
    };


    @Override
    public void onDestroy() {
        super.onDestroy();
        mHandler.removeCallbacks(mUpdateNotificationRunnable);
        mWatchHandler.removeCallbacks(mUpdateWatchStatus);
        if (!isStoppedByApp) {
            Intent broadcastIntent = new Intent(this, RestartServiceBroadcastReceiver.class);
            sendBroadcast(broadcastIntent);
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    public static void setIsStoppedByApp(Boolean status){
        isStoppedByApp = status;
    }
}
