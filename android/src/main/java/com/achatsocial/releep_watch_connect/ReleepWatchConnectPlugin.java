package com.achatsocial.releep_watch_connect;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Handler;
import android.os.Looper;
import android.preference.PreferenceManager;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;

import com.google.gson.Gson;
import com.yucheng.ycbtsdk.Constants;
import com.yucheng.ycbtsdk.YCBTClient;
import com.yucheng.ycbtsdk.bean.ScanDeviceBean;
import com.yucheng.ycbtsdk.response.BleConnectResponse;
import com.yucheng.ycbtsdk.response.BleDataResponse;
import com.yucheng.ycbtsdk.response.BleDeviceToAppDataResponse;
import com.yucheng.ycbtsdk.response.BleRealDataResponse;
import com.yucheng.ycbtsdk.response.BleScanResponse;


import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;


import io.flutter.Log;
import io.flutter.embedding.android.FlutterActivity;
import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.embedding.engine.plugins.activity.ActivityAware;
import io.flutter.embedding.engine.plugins.activity.ActivityPluginBinding;
import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;

/** ReleepWatchConnectPlugin */
public class ReleepWatchConnectPlugin
    implements FlutterPlugin, MethodCallHandler, EventChannel.StreamHandler, ActivityAware {
  /// The MethodChannel that will the communication between Flutter and native
  /// Android
  ///
  /// This local reference serves to register the plugin with the Flutter Engine
  /// and unregister it
  /// when the Flutter Engine is detached from the Activity
  private FlutterActivity activity;
  private MethodChannel channel;
  private EventChannel stream_chanel;

  private List<String> listVal = new ArrayList<>();
  ArrayList<ScanBLEResponse> listWatch = new ArrayList<ScanBLEResponse>();
  private Gson gson = new Gson();
  boolean  isStop = false;

  private String[] permissionArray = new String[] {
      Manifest.permission.READ_EXTERNAL_STORAGE,
      Manifest.permission.ACCESS_COARSE_LOCATION,
      Manifest.permission.WRITE_EXTERNAL_STORAGE,
      Manifest.permission.READ_PHONE_STATE,
      Manifest.permission.ACCESS_FINE_LOCATION,
      Manifest.permission.READ_CONTACTS,
      Manifest.permission.WRITE_CONTACTS,
      Manifest.permission.CALL_PHONE,
      Manifest.permission.BLUETOOTH_SCAN,
      Manifest.permission.BLUETOOTH_CONNECT
  };

  @Override
  public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {

    YCBTClient.initClient(flutterPluginBinding.getApplicationContext(), true);
    YCBTClient.registerBleStateChange(bleConnectResponse);
    YCBTClient.deviceToApp(toAppDataResponse);

    channel = new MethodChannel(flutterPluginBinding.getBinaryMessenger(), "releep_watch_connect");
    channel.setMethodCallHandler(this);
    stream_chanel = new EventChannel(flutterPluginBinding.getBinaryMessenger(), "scan_releep_watch");
    stream_chanel.setStreamHandler(this);

  }



  @Override
  public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
//    SharedPreferences sharedPref = activity.getActivity().getPreferences(Context.MODE_PRIVATE);
    SharedPreferences sharedPref = PreferenceManager.getDefaultSharedPreferences(activity.getApplicationContext());

    if (call.method.equals("getPlatformVersion")) {
      result.success("Android " + android.os.Build.VERSION.RELEASE);
    }else if (call.method.equals("initData")) {
      String userLoginToken = call.argument("userToken");
      String serverIP = call.argument("serverIP");

      SharedPreferences.Editor editor = sharedPref.edit();
      editor.putString("USER_LOGIN_TOKEN", userLoginToken);
      editor.putString("SERVER_IP", serverIP);
      editor.apply();
      result.success(0);
    }else if (call.method.equals("connectReleepWatch")) {
      String macAddress = call.argument("releepWatchMac");
      SharedPreferences.Editor editor = sharedPref.edit();
      editor.putString("KEY_BLE_WATCH", macAddress);
      editor.apply();
      int code = UtilReleepWatch.connectWatchBleWithMac(macAddress);
      result.success(code);
    }else if (call.method.equals("getConnectionState")) {
      int bleState = YCBTClient.connectState();
      if (bleState == Constants.BLEState.ReadWriteOK){  //Connected successfully
        android.util.Log.e("BLEState", "Connected");
      }
      result.success(bleState);
    } else if (call.method.equals("settingTime")) {
      String timeNow = call.argument("timeNow");
      int code = settingWatchTime(timeNow);
      result.success(code);
    } else if (call.method.equals("syncHealthData")) {
      ArrayList lists = new ArrayList();
      YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistoryAll, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            lists.addAll((ArrayList) hashMap.get("data"));
            android.util.Log.e("Health_HistoryAll", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(lists); });
          } else {
            android.util.Log.e("Health_HistoryAll", "no ..health All..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }else if (call.method.equals("syncHeartRate")) {
      ArrayList lists = new ArrayList();
      YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistoryHeart, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            lists.addAll((ArrayList) hashMap.get("data"));
            android.util.Log.e("syncHeartRate", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(lists); });
          } else {
            android.util.Log.e("syncHeartRate", "no ..hr..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }else if (call.method.equals("syncBlood")) {
      ArrayList lists = new ArrayList();
      YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistoryBlood, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            lists.addAll((ArrayList) hashMap.get("data"));
            android.util.Log.e("syncBlood", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(lists); });
          } else {
            android.util.Log.e("syncBlood", "no ..bloodpresure..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    } else if (call.method.equals("syncSPO2")) {
      ArrayList lists = new ArrayList();

      YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistoryBloodOxygen, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            lists.addAll((ArrayList) hashMap.get("data"));
            android.util.Log.e("syncSPO2", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(lists); });
          } else {
            android.util.Log.e("syncSPO2", "no ..spo2..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    }else if (call.method.equals("syncTemp")) {
      ArrayList lists = new ArrayList();
      YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistoryTemp, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            lists.addAll((ArrayList) hashMap.get("data"));
            android.util.Log.e("syncTemp", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(lists); });
          } else {
            android.util.Log.e("syncTemp", "no ..temp..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    }else if (call.method.equals("syncStep")) {

//      YCBTClient.healthHistoryData(Constants.DATATYPE.GetNowStep, new BleDataResponse() {
//        @Override
//        public void onDataResponse(int i, float v, HashMap hashMap) {
//          if (hashMap != null) {
//            HashMap stepData = hashMap;
//            android.util.Log.e("syncStep", "hashMap=" + hashMap.toString());
//            new Handler(Looper.getMainLooper()).post(() -> { result.success(stepData); });
//          } else {
//            android.util.Log.e("syncStep", "no ..step..data....");
//            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
//          }
//        }
//      });
      YCBTClient.appRealSportFromDevice(Constants.DATATYPE.GetNowStep, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            HashMap stepData = hashMap;
            android.util.Log.e("syncStep", "hashMap=" + hashMap.toString());
//            new Handler(Looper.getMainLooper()).post(() -> { result.success(stepData); });
          } else {
            android.util.Log.e("syncStep", "no ..step..data....");
//            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
      YCBTClient.appRegisterRealDataCallBack(new BleRealDataResponse() {
        @Override
        public void onRealDataResponse(int i, HashMap hashMap) {
//          if (i == Constants.DATATYPE.Real_UploadSport) { if (hashMap != null && hashMap.size() > 0) {
//             hashMap.get("sportStep");//step count sportDistance = (int) hashMap.get("sportDistance");//distance sportCalorie = (int) hashMap.get("sportCalorie");//calories
//          } }
          if (hashMap != null) {
            HashMap stepData = hashMap;
            android.util.Log.e("syncStep", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(stepData); });
          } else {
            android.util.Log.e("syncStep", "no ..step..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        } });

    }else if (call.method.equals("syncFitness")) {

      YCBTClient.appRealSportFromDevice(Constants.SportType.FITNESS, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            HashMap stepData = hashMap;
            android.util.Log.e("syncFitness", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(stepData); });
          } else {
            android.util.Log.e("syncFitness", "no ..step..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    }else if (call.method.equals("startSport")) {

//      isStop = false;
      int typeSport = call.argument("typeSport");
//      YCBTClient.appRegisterRealDataCallBack(new BleRealDataResponse() {
//        @Override
//        public void onRealDataResponse(int dataType, HashMap dataMap) {
//          if(Boolean.TRUE.equals(isStop)){
//            if (dataMap != null ) {
//              isStop = false;
//              HashMap startSport = dataMap;
//              new Handler(Looper.getMainLooper()).post(() -> { result.success(startSport);
//              });
//            } else {
//              android.util.Log.e("syncFitness", "no ..step..data....");
//              new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
//            }
//          }
//          android.util.Log.d("mainactivity","chong-------" + dataMap.toString());
//        }
//      });
      YCBTClient.appRunMode(Constants.SportState.Start, typeSport, new BleDataResponse() {
        @Override
        public void onDataResponse(int code, float ratio, HashMap resultMap) {
          if (code == 0) {
            android.util.Log.d("mainactivity","chong------开启成功");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
        }
      });
    }else if (call.method.equals("stopSport")) {
      isStop = true;
      int typeSport = call.argument("typeSport");
      YCBTClient.appRunMode(Constants.SportState.Stop,typeSport, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
//          if (hashMap != null) {
//            HashMap stopSport = hashMap;
//          } else {
//            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
//          }
        }
      });
    }else if (call.method.equals("syncRUN")) {

      YCBTClient.appRealSportFromDevice(Constants.SportType.RUN, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            HashMap stepData = hashMap;
            android.util.Log.e("syncRUN", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(stepData); });
          } else {
            android.util.Log.e("syncRUN", "no ..step..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    }else if (call.method.equals("syncRunIndoors")) {

      YCBTClient.appRealSportFromDevice(Constants.SportType.RUN_INDOORS, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            HashMap stepData = hashMap;
            android.util.Log.e("syncRunIndoors", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(stepData); });
          } else {
            android.util.Log.e("syncRunIndoors", "no ..step..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    }else if (call.method.equals("syncRunOutSide")) {
      YCBTClient.appRealSportFromDevice(Constants.SportType.RUN_OUTSIDE, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            HashMap stepData = hashMap;
            android.util.Log.e("syncRunOutSide", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(stepData); });
          } else {
            android.util.Log.e("syncRunOutSide", "no ..step..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    }else if (call.method.equals("syncSport")) {
      ArrayList listSportTest = new ArrayList();

      YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistorySport, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            android.util.Log.e("test",  listSportTest.toString());
            HashMap sportData = hashMap;
            android.util.Log.e("syncSport", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> {
              result.success(sportData);
            });
          } else {
            android.util.Log.e("syncSport", "no ..sport..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    }
    else if (call.method.equals("syncSleep")) {
      ArrayList lists = new ArrayList();

      YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistorySleep, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            lists.addAll((ArrayList) hashMap.get("data"));
            android.util.Log.e("syncSleep", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(lists); });
          }else {
            android.util.Log.e("syncSleep", "no ..sleep..data....");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });


//      YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistorySleep, new BleDataResponse() {
//        @Override
//        public void onDataResponse(int i, float v, HashMap hashMap) {
//          if (hashMap != null) {
//            lists.addAll((ArrayList) hashMap.get("data"));
//            android.util.Log.e("syncSleep", "hashMap=" + hashMap.toString());
//            new Handler(Looper.getMainLooper()).post(() -> { result.success(lists); });
//          } else {
//            android.util.Log.e("syncSleep", "no ..sleep..data....");
//            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
//          }
//        }
//      });
    }else if (call.method.equals("syncECG")) {
      YCBTClient.collectHistoryListData(0x00, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (hashMap != null) {
            List data = (List) hashMap.get("data");
            android.util.Log.e("syncECG", "hashMap=" + hashMap.toString());
            new Handler(Looper.getMainLooper()).post(() -> { result.success(data); });
          } else {
            Log.e("syncECG ", "nothing");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    }else if (call.method.equals("disconnectReleepWatch")) {
      YCBTClient.disconnectBle();
      result.success(0);
    }else if(call.method.equals("getCurrentSystemMode")){
      YCBTClient.getCurrentSystemWorkingMode(new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if(i == 0){
//0x00: Normal working mode 0x01: Caring working mode 0x02: Power saving working mode 0x03: Custom working mode
            int currentSystemWorkingMode = (int) hashMap.get("currentSystemWorkingMode");
            new Handler(Looper.getMainLooper()).post(() -> { result.success(currentSystemWorkingMode); });
          }else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });
          }
        }
      });
    }else if(call.method.equals("getDeviceInfo")){
      YCBTClient.getDeviceInfo(new BleDataResponse() {
        @Override
        public void onDataResponse(int code, float ratio, HashMap resultMap) {
          if (resultMap != null){
            HashMap tDataMap = new HashMap();
            new Handler(Looper.getMainLooper()).post(() -> { result.success(tDataMap); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }else if(call.method.equals("settingWokingMode")){
      int mode = call.argument("workingMode");
      YCBTClient.settingWorkingMode(mode, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {// success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }else if(call.method.equals("settingHeartMonitor")){
      int time = call.argument("intervalTime");
      YCBTClient.settingHeartMonitor(0x01,time, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {// success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }else if(call.method.equals("settingTemperatureMonitor")){
      int time = call.argument("intervalTime");
      YCBTClient.settingTemperatureMonitor(true,time, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {// success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }else if(call.method.equals("settingBloodOxygenModeMonitor")){
      int time = call.argument("intervalTime");
      YCBTClient.settingBloodOxygenModeMonitor(true,time, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {// success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }else if (call.method.equals("settingLang")) {
      int langCode = call.argument("langCode");
      YCBTClient.settingLanguage(langCode, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {//settingBloodOxygenAlarm success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }
    else if(call.method.equals("settingBloodOxygenAlarm")){
      int value = call.argument("value");
      YCBTClient.settingBloodOxygenAlarm(0x01,value, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {//settingBloodOxygenAlarm success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }
    else if(call.method.equals("settingTemperatureAlarm")){
      int value = call.argument("value");
      YCBTClient.settingTemperatureAlarm(true,value, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {//settingBloodOxygenAlarm success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }
    else if(call.method.equals("settingHeartAlarm")){
      int highHeart = call.argument("highHeart");
      int lowHeart = call.argument("lowHeart");
      YCBTClient.settingHeartAlarm(0x01,highHeart,lowHeart, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {//settingBloodOxygenAlarm success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }
    else if(call.method.equals("settingBloodPressAlarm")){
      int maxSBP = call.argument("maxSBP");
      int maxDBP = call.argument("maxDBP");
      int minSBP = call.argument("minSBP");
      int minDBP = call.argument("minDBP");
      YCBTClient.settingBloodAlarm(0x01,maxSBP,maxDBP,minSBP,minDBP, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {//settingBloodOxygenAlarm success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }
    else if (call.method.equals("deleteHistoryHealth")) {
      YCBTClient.deleteHealthHistoryData(Constants.DATATYPE.Health_DeleteAll, new BleDataResponse() {
        @Override
        public void onDataResponse(int i, float v, HashMap hashMap) {
          if (i == 0) {//delete success
            new Handler(Looper.getMainLooper()).post(() -> { result.success(0); });
          }
          else {
            new Handler(Looper.getMainLooper()).post(() -> { result.success(null); });

          }
        }
      });
    }
    else if (call.method.equals("startService")) {
      String watchMac = sharedPref.getString("KEY_BLE_WATCH", "");
      if(!watchMac.equals("")) {
        startForegroundService();
      }
      result.success(true);
    }else if (call.method.equals("stopService")) {
      stopForegroundService();
      result.success(true);
    }
    else if (call.method.equals("removeMacAddress")) {
      SharedPreferences.Editor editor = sharedPref.edit();
      editor.putString("KEY_BLE_WATCH", "");
      editor.apply();
      result.success(true);
    }
    else {
      result.notImplemented();
    }
  }

  private int settingLanguage(int langCode) {

    return 0;
  }

  private int settingWatchTime(String timeNow) {
    // Settings 2020-2-20 12:12:12 Thursday
    // YCBTClient.settingTime(2020, 2, 20, 12, 12, 12, 3, new BleDataResponse() {
    // @Override
    // public void onDataResponse(int code, float ratio, HashMap resultMap) {
    //
    // if (code == Constants.CODE.Code_OK){
    //
    // }
    // }
    // });
    return 0;
  }

  public void startForegroundService() {
    Intent serviceIntent = new Intent(activity.getApplicationContext(), WatchConnectionService.class);
    ContextCompat.startForegroundService(activity.getApplicationContext(), serviceIntent);
  }

  public void stopForegroundService() {
    YCBTClient.disconnectBle();
    WatchConnectionService.setIsStoppedByApp(true);
    Context context = activity.getApplicationContext();
    Intent serviceIntent = new Intent(context, WatchConnectionService.class);
    context.stopService(serviceIntent);
  }

//  private int connectWatchBleWithMac(String macAddress) {
//    final int[] res_code = { 0 };
//    YCBTClient.stopScanBle();
//    YCBTClient.connectBle(macAddress, new BleConnectResponse() {
//      @Override
//      public void onConnectResponse(final int code) {
//
//        YCBTLog.e("connectBle code " + code);
//        if (code == Constants.CODE.Code_OK) {
//         UtilReleepWatch.baseOrderSet();
//          res_code[0] = code;
//          // syncHealthDataByDataType(Constants.DATATYPE.Health_HistoryHeart);
//          YCBTLog.e("connectBle success ");
//        } else if (code == Constants.CODE.Code_Failed) {
//          YCBTLog.e("connectBle fail ");
//          res_code[0] = code;
//        }
//      }
//    });
//    return res_code[0];
//  }

//  private void baseOrderSet() {
//    /***
//     * 语言设置
//     *
//     * @param langType     0x00:English 0x01: Chinese 0x02: Russian 0x03: German
//     *                     0x04:French
//     *                     0x05: Japanese 0x06: Spanish 0x07: Italian 0x08:
//     *                     Portuguese 0x09: Korean
//     *                     0x0A: Polish 0x0B: Malay 0x0C: Traditional Chinese
//     *                     0xFF:other
//     * @param dataResponse
//     */
//    YCBTClient.settingLanguage(0x00, new BleDataResponse() {
//      @Override
//      public void onDataResponse(int i, float v, HashMap hashMap) {
//        android.util.Log.e("device", "同步语言结束");
//      }
//    });
//
//    // 心率采集
//    YCBTClient.settingHeartMonitor(0x01, 10, new BleDataResponse() {
//      @Override
//      public void onDataResponse(int i, float v, HashMap hashMap) {
//        android.util.Log.e("device", "设置10分钟间隔采集心率");
//      }
//    });
//
//    // Heatrate Sync
//    // syncHealthDataByDataType(Constants.DATATYPE.Health_HistoryHeart);
//
//    /*
//     * //无感检测
//     * YCBTClient.settingPpgCollect(0x01, 60, 60, new BleDataResponse() {
//     *
//     * @Override
//     * public void onDataResponse(int i, float v, HashMap hashMap) {
//     * Log.e("device", "设置无感数据采集");
//     * }
//     * });
//     *
//     *
//     * //同步心率
//     * syncHisHr();
//     * //同步睡眠
//     * syncHisSleep();
//     *
//     * syncHisStep();
//     */
//  }

//  private ArrayList syncHealthDataByDataType(int health_historyHeart) {
//
//    ArrayList lists = new ArrayList();
//    YCBTClient.healthHistoryData(health_historyHeart, new BleDataResponse() {
//      @Override
//      public void onDataResponse(int i, float v, HashMap hashMap) {
//
//        if (hashMap != null) {
//          lists.addAll((ArrayList) hashMap.get("data"));
//          android.util.Log.e("history", "hashMap=" + hashMap.toString());
//          // android.util.Log.e("history", "hr time=" + hashMap.get("heartStartTime"));
//          // android.util.Log.e("history", "hr val=" + hashMap.get("heartValue"));
//        } else {
//          android.util.Log.e("history", "no ..hr..data....");
//        }
//      }
//    });
//    return lists;
//  }

  @Override
  public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
    channel.setMethodCallHandler(null);
  }

  @Override
  public void onListen(Object arguments, EventChannel.EventSink events) {
    if (arguments.equals("scan")) {
      listVal = new ArrayList();
      listWatch = new ArrayList<ScanBLEResponse>();
      YCBTClient.startScanBle(new BleScanResponse() {
        @Override
        public void onScanResponse(int i, ScanDeviceBean scanDeviceBean) {

          if (scanDeviceBean != null) {
            if (!listVal.contains(scanDeviceBean.getDeviceMac())) {
              listVal.add(scanDeviceBean.getDeviceMac());
              listWatch.add(new ScanBLEResponse(scanDeviceBean.getDeviceName(),scanDeviceBean.getDeviceMac()));
            }
            String json = gson.toJson(listWatch);
            events.success(json);

            Log.e("device", "mac=" + scanDeviceBean.getDeviceMac() + ";name=" + scanDeviceBean.getDeviceName() + "rssi="
                + scanDeviceBean.getDeviceRssi());

          }
        }
      }, 6);
    }

    if(arguments.equals("sportStart")){
      YCBTClient.appRegisterRealDataCallBack(new BleRealDataResponse() {
        @Override
        public void onRealDataResponse(int dataType, HashMap dataMap) {
          HashMap startSport = dataMap;
          new Handler(Looper.getMainLooper()).post(() -> { events.success(startSport); });
        }
      });
    }
//    if(arguments.equals("sync")){
//      List<Object> lists = new ArrayList<>();
//      YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistoryHeart, new BleDataResponse() {
//        @Override
//        public void onDataResponse(int i, float v, HashMap hashMap) {
//
//          if (hashMap != null) {
//            lists.addAll((ArrayList) hashMap.get("data"));
//            android.util.Log.e("history", "hashMap=" + hashMap.toString());
//            // android.util.Log.e("history", "hr time=" + hashMap.get("heartStartTime"));
//            // android.util.Log.e("history", "hr val=" + hashMap.get("heartValue"));
//            //String json = gson.toJson(lists);
//
//          } else {
//            android.util.Log.e("history", "no ..hr..data....");
//            //String json = gson.toJson(lists);
//          }
//          events.success(lists);
//        }
//      });
//    }
  }

  @Override
  public void onCancel(Object arguments) {

  }



  boolean isActiveDisconnect = false;
  BleConnectResponse bleConnectResponse = new BleConnectResponse() {
    @Override
    public void onConnectResponse(int code) {
      // Toast.makeText(MyApplication.this, "i222=" + var1,
      // Toast.LENGTH_SHORT).show();

      android.util.Log.e("deviceconnect", "全局监听返回=" + code);

      if (code == com.yucheng.ycbtsdk.Constants.BLEState.Disconnect) {
        // thirdConnect = false;
        // BangleUtil.getInstance().SDK_VERSIONS = -1;
        // EventBus.getDefault().post(new BlueConnectFailEvent());
        /*
         * if(SPUtil.getBindedDeviceMac() != null &&
         * !"".equals(SPUtil.getBindedDeviceMac())){
         * YCBTClient.connectBle(SPUtil.getBindedDeviceMac(), new BleConnectResponse() {
         * 
         * @Override
         * public void onConnectResponse(int code) {
         * 
         * }
         * });
         * }
         */
      } else if (code == com.yucheng.ycbtsdk.Constants.BLEState.Connected) {

      } else if (code == com.yucheng.ycbtsdk.Constants.BLEState.ReadWriteOK) {

        // thirdConnect = true;
        // BangleUtil.getInstance().SDK_VERSIONS = 3;
        // Log.e("deviceconnect", "蓝牙连接成功，全局监听");
        // setBaseOrder();
        // EventBus.getDefault().post(new ConnectEvent());
      } else {
        // code == Constants.BLEState.Disconnect
        // thirdConnect = false;
        // BangleUtil.getInstance().SDK_VERSIONS = -1;
        // EventBus.getDefault().post(new ConnectEvent());
      }
    }
  };

  BleDeviceToAppDataResponse toAppDataResponse = new BleDeviceToAppDataResponse() {

    @Override
    public void onDataResponse(int dataType, HashMap dataMap) {

      android.util.Log.e("TimeSetActivity", "被动回传数据。。。");
      android.util.Log.e("TimeSetActivity", dataMap.toString());

    }
  };

  @Override
  public void onAttachedToActivity(@NonNull ActivityPluginBinding binding) {
    activity = (FlutterActivity) binding.getActivity();
    boolean backBoolean = PermissionUtils.checkPermissionArray(activity, permissionArray, 3);

  }

  @Override
  public void onDetachedFromActivityForConfigChanges() {

  }

  @Override
  public void onReattachedToActivityForConfigChanges(@NonNull ActivityPluginBinding binding) {

  }

  @Override
  public void onDetachedFromActivity() {

  }
}
