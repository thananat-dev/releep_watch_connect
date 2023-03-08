package com.achatsocial.releep_watch_connect;

import android.content.SharedPreferences;
import android.os.Handler;
import android.os.Looper;
import android.preference.PreferenceManager;

import com.google.gson.Gson;
import com.yucheng.ycbtsdk.Constants;
import com.yucheng.ycbtsdk.YCBTClient;
import com.yucheng.ycbtsdk.response.BleConnectResponse;
import com.yucheng.ycbtsdk.response.BleDataResponse;
import com.yucheng.ycbtsdk.utils.YCBTLog;

import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CompletableFuture;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class UtilReleepWatch {
    public static final int TIME_OUT = 0x01;
    public static final int NOT_OPEN = 0x02;
    public static final int DISCONNECT = 0x03;
    public static final int DISCONNECTING = 0x04;
    public static final int CONNECTING = 0x05;
    public static final int CONNECTED = 0x06;
    public static final int SERVICES_DISCOVERED = 0x07;
    public static final int CHARACTERISTIC_DISCOVERED = 0x08;
    public static final int CHARACTERISTIC_NOTIFICATION = 0x09;
    public static final int READ_WRITE_OK = 0x0A;

    public static String getStatusName(int status) {
        switch (status) {
            case TIME_OUT:
                return "TimeOut";
            case NOT_OPEN:
                return "NotOpen";
            case DISCONNECT:
                return "Disconnect";
            case DISCONNECTING:
                return "Disconnecting";
            case CONNECTING:
                return "Connecting";
            case CONNECTED:
                return "Connected";
            case SERVICES_DISCOVERED:
                return "ServicesDiscovered";
            case CHARACTERISTIC_DISCOVERED:
                return "CharacteristicDiscovered";
            case CHARACTERISTIC_NOTIFICATION:
                return "CharacteristicNotification";
            case READ_WRITE_OK:
                return "Connected(RW)";
            default:
                return "Unknown";
        }
    }

    public ScanBLEResponse findWatchByMac(String macAddress, ArrayList<ScanBLEResponse> listWatch){
        for (ScanBLEResponse response : listWatch) {
            if (response.getMacAddress().equals(macAddress)) {
                return response;
            }
        }
        return null;
    }

    public static int connectWatchBleWithMac(String macAddress) {
        final int[] res_code = { 0 };
        YCBTClient.stopScanBle();
        YCBTClient.connectBle(macAddress, new BleConnectResponse() {
            @Override
            public void onConnectResponse(final int code) {

                YCBTLog.e("connectBle code " + code);
                if (code == Constants.CODE.Code_OK) {
                    baseOrderSet();
                    res_code[0] = code;
                    // syncHealthDataByDataType(Constants.DATATYPE.Health_HistoryHeart);
                    YCBTLog.e("connectBle success ");
                } else if (code == Constants.CODE.Code_Failed) {
                    YCBTLog.e("connectBle fail ");
                    res_code[0] = code;
                }
            }
        });
        return res_code[0];
    }

    public static void baseOrderSet() {
        /***
         * 语言设置
         *
         * @param langType     0x00:English 0x01: Chinese 0x02: Russian 0x03: German
         *                     0x04:French
         *                     0x05: Japanese 0x06: Spanish 0x07: Italian 0x08:
         *                     Portuguese 0x09: Korean
         *                     0x0A: Polish 0x0B: Malay 0x0C: Traditional Chinese
         *                     0xFF:other
         * @param dataResponse
         */
        YCBTClient.settingLanguage(0x00, new BleDataResponse() {
            @Override
            public void onDataResponse(int i, float v, HashMap hashMap) {
                android.util.Log.e("device", "同步语言结束");
            }
        });

        // 心率采集
        YCBTClient.settingHeartMonitor(0x01, 10, new BleDataResponse() {
            @Override
            public void onDataResponse(int i, float v, HashMap hashMap) {
                android.util.Log.e("device", "设置10分钟间隔采集心率");
            }
        });
    }

    public static void syncHealthData(String macAddress,String userLoginToken,String serverIP){
        ArrayList lists = new ArrayList();
        YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistoryAll, new BleDataResponse() {
            @Override
            public void onDataResponse(int i, float v, HashMap hashMap) {
                if (hashMap != null) {
                    lists.addAll((ArrayList) hashMap.get("data"));
                    android.util.Log.e("Health_HistoryAll", "hashMap=" + hashMap.toString());

                    String jsonData = getHealthDataJson(lists,macAddress);
                    saveReleepHealthData(jsonData, userLoginToken, serverIP);
                } else {
                    android.util.Log.e("Health_HistoryAll", "no ..health All..data....");
                }
            }
        });
    }

    private static String getHealthDataJson(ArrayList healthDataList, String macAddress) {
        Map<String, Object> data = new HashMap<>();
        data.put("watchMacAddress", macAddress);
        data.put("watchHealthData", healthDataList);
        return new Gson().toJson(data);
    }

    public static void saveReleepHealthData(String jsonData,String userLoginToken,String serverIP) {
        String token = userLoginToken;
        String url = serverIP + "/api/ReleepWatchAPI/saveReleepHealthData/";

        OkHttpClient client = new OkHttpClient();

        MediaType mediaType = MediaType.parse("application/json");
        RequestBody body = RequestBody.create(jsonData, mediaType);
        Request request = new Request.Builder()
                .url(url)
                .post(body)
                .addHeader("Content-Type", "application/json")
                .addHeader("Authorization", "Bearer " + token)
                .build();

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                android.util.Log.e("saveReleepHealthData", "Unexpected response Error", e);
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (response.isSuccessful()) {
                    String jsonData = response.body().string();
                    android.util.Log.i("saveReleepHealthData", jsonData);
                } else {
                    if (response.code() == 401) {
                        //checkValidateToken();
                    }
                    android.util.Log.e("saveReleepHealthData", "Unexpected response code: " + response.code());
                }
            }
        });
    }

    public static void syncSleepData(String macAddress,String userLoginToken,String serverIP){

        ArrayList lists = new ArrayList();
        YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistorySleep, new BleDataResponse() {
            @Override
            public void onDataResponse(int i, float v, HashMap hashMap) {
                if (hashMap != null) {
                    lists.addAll((ArrayList) hashMap.get("data"));
                    android.util.Log.e("syncSleep", "hashMap=" + hashMap.toString());

                    String jsonData = getSleepDataJson(lists,macAddress);
                    saveReleepHealthSleep(jsonData, userLoginToken, serverIP);
                } else {
                    android.util.Log.e("syncSleep", "no ..sleep..data....");
                }
            }
        });
    }

    private static String getSleepDataJson(ArrayList sleepDataList, String macAddress) {
        Map<String, Object> data = new HashMap<>();
        data.put("watchMacAddress", macAddress);
        data.put("watchHealthSleep", sleepDataList);
        return new Gson().toJson(data);
    }

    public static void saveReleepHealthSleep(String jsonData,String userLoginToken,String serverIP) {
        String token = userLoginToken;
        String url = serverIP + "/api/ReleepWatchAPI/saveReleepHealthSleep/";

        OkHttpClient client = new OkHttpClient();

        MediaType mediaType = MediaType.parse("application/json");
        RequestBody body = RequestBody.create(jsonData, mediaType);
        Request request = new Request.Builder()
                .url(url)
                .post(body)
                .addHeader("Content-Type", "application/json")
                .addHeader("Authorization", "Bearer " + token)
                .build();

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                android.util.Log.e("saveReleepHealthSleep", "Unexpected response Error", e);
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (response.isSuccessful()) {
                    String jsonData = response.body().string();
                    android.util.Log.i("saveReleepHealthSleep", jsonData);
                } else {
                    if (response.code() == 401) {
                        //checkValidateToken();
                    }
                    android.util.Log.e("saveReleepHealthSleep", "Unexpected response code: " + response.code());
                }
            }
        });
    }

    public static void syncSportData(String macAddress,String userLoginToken,String serverIP){
        YCBTClient.healthHistoryData(Constants.DATATYPE.Health_HistorySport, new BleDataResponse() {
            @Override
            public void onDataResponse(int i, float v, HashMap hashMap) {
                if (hashMap != null) {
                    HashMap sportData = hashMap;
                    android.util.Log.e("syncSport", "hashMap=" + hashMap.toString());
                    String jsonData = getSportDataJson(sportData,macAddress);
                    saveReleepHealthSport(jsonData, userLoginToken, serverIP);
                } else {
                    android.util.Log.e("syncSport", "no ..sport..data....");
                }
            }
        });
    }

    private static String getSportDataJson(HashMap sportDataList, String macAddress) {
        Map<String, Object> data = new HashMap<>();
        data.put("watchMacAddress", macAddress);
        data.put("watchHealthSport", sportDataList);
        return new Gson().toJson(data);
    }

    public static void saveReleepHealthSport(String jsonData,String userLoginToken,String serverIP) {
        String token = userLoginToken;
        String url = serverIP + "/api/ReleepWatchAPI/saveReleepHealthSport/";

        OkHttpClient client = new OkHttpClient();

        MediaType mediaType = MediaType.parse("application/json");
        RequestBody body = RequestBody.create(jsonData, mediaType);
        Request request = new Request.Builder()
                .url(url)
                .post(body)
                .addHeader("Content-Type", "application/json")
                .addHeader("Authorization", "Bearer " + token)
                .build();

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(Call call, IOException e) {
                android.util.Log.e("saveReleepHealthSport", "Unexpected response Error", e);
            }

            @Override
            public void onResponse(Call call, Response response) throws IOException {
                if (response.isSuccessful()) {
                    String jsonData = response.body().string();
                    android.util.Log.i("saveReleepHealthSport", jsonData);
                } else {
                    if (response.code() == 401) {
                        //checkValidateToken();
                    }
                    android.util.Log.e("saveReleepHealthSport", "Unexpected response code: " + response.code());
                }
            }
        });
    }
}

