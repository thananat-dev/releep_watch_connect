import 'dart:async';

import 'package:flutter/services.dart';

class ReleepWatchConnect {
  static const MethodChannel _channel = MethodChannel('releep_watch_connect');
  static const stream = EventChannel('scan_releep_watch');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  static Future<int> connectWatch(macAddress) async {
    // code : 0 = OK 1 = Fail 2 = Time out
    final int code = await _channel
        .invokeMethod('connectReleepWatch', {'releepWatchMac': macAddress});
    return code;
  }

  static Future<int> getConnectionState() async {
    // public static final int TimeOut = 1;
    // public static final int NotOpen = 2;
    // public static final int Disconnect = 3;
    // public static final int Disconnecting = 4;
    // public static final int Connecting = 5;
    // public static final int Connected = 6;
    final int code = await _channel.invokeMethod('getConnectionState');
    return code;
  }

  static Future<dynamic> syncHealthAll() async {
    var healthDataList = await _channel.invokeMethod('syncHealthData');
    return healthDataList;
  }

  static Future<dynamic> syncHealthHeartRate() async {
    var dataHeartRateList = await _channel.invokeMethod('syncHeartRate');
    return dataHeartRateList;
  }

  static Future<dynamic> syncHealthBlood() async {
    var dataBloodList = await _channel.invokeMethod('syncBlood');
    return dataBloodList;
  }

  static Future<dynamic> syncHealthSPO2() async {
    var dataSPO2List = await _channel.invokeMethod('syncSPO2');
    return dataSPO2List;
  }

  static Future<dynamic> syncHealthTemp() async {
    var dataTempList = await _channel.invokeMethod('syncTemp');
    return dataTempList;
  }

  static Future<dynamic> syncHealthStep() async {
    var dataStep = await _channel.invokeMethod('syncStep');
    return dataStep;
  }

  static Future<dynamic> syncHealthSport() async {
    var dataStep = await _channel.invokeMethod('syncSport');
    return dataStep;
  }

  static Future<dynamic> syncHealthSleep() async {
    var dataSleep = await _channel.invokeMethod('syncSleep');
    return dataSleep;
  }

  static Future<dynamic> syncHealthECG() async {
    var dataECG = await _channel.invokeMethod('syncECG');
    return dataECG;
  }

  static Future<dynamic> getCurrentSystemMode() async {
    //0x00: Normal working mode 0x01: Caring working mode 0x02: Power saving working mode 0x03: Custom working mode
    var mode = await _channel.invokeMethod('getCurrentSystemMode');
    return mode;
  }

  static Future<dynamic> getDeviceInfo() async {
    //0x00: Normal working mode 0x01: Caring working mode 0x02: Power saving working mode 0x03: Custom working mode
    var deviceInfo = await _channel.invokeMethod('getDeviceInfo');
    return deviceInfo;
  }

  static Future<dynamic> settingWorkingMode(mode) async {
    //0x00: set to normal working mode 0x01: set to care working mode 0x02: set to power saving working mode 0x03: set to custom working mode
    var res_mode =
        await _channel.invokeMethod('settingWokingMode', {'workingMode': mode});
    return res_mode;
  }

  static Future<dynamic> settingLanguage(lang) async {
    //0x00:English 0x01: Chinese 0x02: Russian 0x03: German 0x04: French 0x05: Japanese 0x06: Spanish 0x07: Italian 0x08: Portuguese 0x09: Korean 0x0A: Polish 0x0B: Malay 0x0C: Traditional Chinese 0xFF:other
    var res_mode =
    await _channel.invokeMethod('settingLang', {'langCode': lang});
    return res_mode;
  }

  static Future<dynamic> settingBloodOxygenAlarm(value) async {
    // value is number of bloodOxygen Minimum blood oxygen alarm threshold 80-95
    var res_mode =
    await _channel.invokeMethod('settingBloodOxygenAlarm', {'value': value});
    return res_mode;
  }

  static Future<dynamic> settingTemperatureAlarm(value) async {
    // Temperature alarm upper limit (0-127)
    var res_mode =
    await _channel.invokeMethod('settingTemperatureAlarm', {'value': value});
    return res_mode;
  }

  static Future<dynamic> settingHeartAlarm(highHeart,lowHeart) async {
   // * @param highHeart Maximum heart rate alarm 100 – 240
   // * @param lowHeart Minimum heart rate alarm 30 - 60
    var res_mode =
    await _channel.invokeMethod('settingHeartAlarm', {'highHeart': highHeart,'lowHeart': lowHeart});
    return res_mode;
  }

  static Future<dynamic> deleteHealthAll() async {
    var dataHealthDel = await _channel.invokeMethod('deleteHistoryHealth');
    return dataHealthDel;
  }

  static Future<dynamic> disconectReleepWatch() async {
    var res = await _channel.invokeMethod('disconnectReleepWatch');
    return res;
  }

  static Stream get scanReleepWatch => stream.receiveBroadcastStream("scan");

  //static Stream get syncReleepWatch => stream.receiveBroadcastStream("sync");
}
