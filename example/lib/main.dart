import 'dart:convert';

import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:releep_watch_connect/releep_watch_connect.dart';
import 'package:releep_watch_connect/util_releep_watch.dart';

void main() {
  runApp(
    MaterialApp(
      home: MyApp(),
      routes: <String, WidgetBuilder>{
        '/Fitness': (BuildContext context) => new FitnessPage(),
      },
    ),
  );
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
  late StreamSubscription _ReleepWatchScanSubscription;
  var _listWatch = [];
  final TextEditingController _resReleepWatch = TextEditingController();

  @override
  void initState() {
    super.initState();
    initPlatformState();
    _resReleepWatch.text = "no response";
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    try {
      platformVersion = await ReleepWatchConnect.platformVersion ??
          'Unknown platform version';
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
    });
  }

  Future<Null> _connectReleepWatch(watchMac, uuidStr) async {
    int code =
        await ReleepWatchConnect.connectWatch(watchMac, uuidString: uuidStr);
    _cancelWatchScan();
    debugPrint("connect Res ${code}");
    setState(() {
      _resReleepWatch.text = code.toString();
    });
  }

  Future<Null> _disconnectReleepWatch() async {
    int code = await ReleepWatchConnect.disconectReleepWatch();
    debugPrint("disconnect Res ${code}");
    setState(() {
      _resReleepWatch.text = code.toString();
    });
  }

  Future<Null> _syncDataReleepWatch() async {
    var res = await ReleepWatchConnect.syncHealthAll();
    debugPrint("syncHealthAll --> HealthAll ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  Future<Null> _syncHealthStep() async {
    var res = await ReleepWatchConnect.syncHealthStep();
    debugPrint("_syncHealthStep --> Step ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  //Sport
  Future<Null> _syncSportRun() async {
    var res = await ReleepWatchConnect.syncRUN();
    debugPrint("_syncHealthStep --> Run ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  Future<Null> _syncSportFitness() async {
    var res = await ReleepWatchConnect.syncFitness();
    debugPrint("_syncHealthStep --> Fitness ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  Future<Null> _syncHealthHR() async {
    var res = await ReleepWatchConnect.syncHealthHeartRate();
    debugPrint("_syncHealthHR --> HR ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  Future<Null> _syncHealthTemp() async {
    var res = await ReleepWatchConnect.syncHealthTemp();
    debugPrint("_syncHealthTemp --> Temp ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  Future<Null> _syncHealthSPO2() async {
    var res = await ReleepWatchConnect.syncHealthSPO2();
    debugPrint("_syncHealthSPO2 --> SPO2 ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  Future<Null> _syncHealthBloodPresure() async {
    var res = await ReleepWatchConnect.syncHealthBlood();
    debugPrint("_syncHealthBloodPresure --> BloodPresure ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  Future<Null> _syncHealthSport() async {
    var res = await ReleepWatchConnect.syncHealthSport();
    debugPrint("_syncHealthSport --> Sport ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  Future<Null> _syncHealthSleep() async {
    var res = await ReleepWatchConnect.syncHealthSleep();
    debugPrint("_syncHealthSleep --> Sleep ${res.toString()}");
    setState(() {
      _resReleepWatch.text = res.toString();
    });
  }

  Future<Null> _syncHealthECG() async {
    var res = await ReleepWatchConnect.syncHealthSleep();
    debugPrint("_syncHealthECG --> ECG ${res.toString()}");
    setState(() {
      setState(() {
        _resReleepWatch.text = res.toString();
      });
    });
  }

  Future<Null> _deleteHealthAll() async {
    var res = await ReleepWatchConnect.deleteHealthAll();
    debugPrint("deleteHealthAll --> HR ${res.toString()}");
    setState(() {
      setState(() {
        _resReleepWatch.text = res.toString();
      });
    });
  }

  Future<Null> _disableNotification() async {
    var res = await ReleepWatchConnect.disbleWatchNotification();
    debugPrint("disbleWatchNotification -->  ${res.toString()}");
    setState(() {
      setState(() {
        _resReleepWatch.text = res.toString();
      });
    });
  }

  Future<Null> _settingLangReleepWatch(lang) async {
    int code = await ReleepWatchConnect.settingLanguage(lang);
    debugPrint("_settingLangReleepWatch Res ${code}");
    setState(() {
      _resReleepWatch.text = "_settingLangReleepWatch :" + code.toString();
    });
  }

  Future<Null> _settingBloodOxygenAlarm(value) async {
    int code = await ReleepWatchConnect.settingBloodOxygenAlarm(value);
    debugPrint("_settingBloodOxygenAlarm Res ${code}");
    setState(() {
      _resReleepWatch.text = "_settingBloodOxygenAlarm :" + code.toString();
    });
  }

  Future<Null> _settingTemperatureAlarm(value) async {
    int code = await ReleepWatchConnect.settingTemperatureAlarm(value);
    debugPrint("settingTemperatureAlarm Res ${code}");
    setState(() {
      _resReleepWatch.text = "settingTemperatureAlarm :" + code.toString();
    });
  }

  Future<Null> _settingHeartAlarm(highValue, lowValue) async {
    int code = await ReleepWatchConnect.settingHeartAlarm(highValue, lowValue);
    debugPrint("settingHeartAlarm Res ${code}");
    setState(() {
      _resReleepWatch.text = "settingHeartAlarm :" + code.toString();
    });
  }

  Future<Null> _settingWorkingMode(mode) async {
    int code = await ReleepWatchConnect.settingWorkingMode(mode);
    debugPrint("_settingWorkingMode Res ${code}");
    setState(() {
      _resReleepWatch.text = "_settingWorkingMode :" + code.toString();
    });
  }

  Future<Null> _settingHeartMonitor(mode) async {
    int code = await ReleepWatchConnect.settingHeartMonitor(mode);
    debugPrint("_settingHeartMonitor Res ${code}");
    setState(() {
      _resReleepWatch.text = "_settingHeartMonitor :" + code.toString();
    });
  }

  Future<Null> _settingTemperatureMonitor(mode) async {
    int code = await ReleepWatchConnect.settingTemperatureMonitor(mode);
    debugPrint("_settingTemperatureMonitor Res ${code}");
    setState(() {
      _resReleepWatch.text = "_settingTemperatureMonitor :" + code.toString();
    });
  }

  Future<Null> _settingBloodOxygenModeMonitor(mode) async {
    int code = await ReleepWatchConnect.settingBloodOxygenModeMonitor(mode);
    debugPrint("_settingBloodOxygenModeMonitor Res ${code}");
    setState(() {
      _resReleepWatch.text =
          "_settingBloodOxygenModeMonitor :" + code.toString();
    });
  }

  //_getConnectState
  Future<Null> _getConnectState() async {
    int code = await ReleepWatchConnect.getConnectionState();
    debugPrint("getConnectionState Res ${code}");
    setState(() {
      _resReleepWatch.text =
          "getConnectionState :" + UtilReleepWatch.getStatusName(code);
    });
  }

  void _startWatchScan() {
    setState(() {
      _listWatch = [];
    });
    debugPrint("_startWatchScan");
    _ReleepWatchScanSubscription =
        ReleepWatchConnect.scanReleepWatch.listen((event) => {
              setState(() {
                var json = jsonDecode(event);
                _listWatch = json;
              })
            });
  }

  void _cancelWatchScan() {
    _ReleepWatchScanSubscription.cancel();
  }

  // void _syncHeartRate() {
  //   debugPrint("_syncHeartRate");
  //   _ReleepWatchScanSubscription =
  //       ReleepWatchConnect.syncReleepWatch.listen((event) => {
  //             debugPrint("_syncHeartRate Response ${event}"),
  //           });
  // }

  // void _scanWatchListener(event) {
  //   debugPrint("Find BLE :"); // 05:28:00:00:01:3A E300
  //   //debugPrint(event);
  // }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
          appBar: AppBar(
            title: const Text('Releep Watch Connect'),
          ),
          body: SingleChildScrollView(
            physics: const ScrollPhysics(),
            child: Column(
              children: [
                Wrap(
                  spacing: 10.0,
                  children: <Widget>[
                    ElevatedButton(
                        onPressed: _syncDataReleepWatch,
                        child: const Text("Sync Data")),
                    ElevatedButton(
                        onPressed: _syncHealthHR, child: const Text("Sync HR")),
                    ElevatedButton(
                        onPressed: _syncHealthBloodPresure,
                        child: const Text("Sync BP")),
                    ElevatedButton(
                        onPressed: _syncHealthSPO2,
                        child: const Text("Sync SPO2")),
                    ElevatedButton(
                        onPressed: _syncHealthSport,
                        child: const Text("Sync Sport")),
                    ElevatedButton(
                        onPressed: _syncHealthSleep,
                        child: const Text("Sync Sleep")),
                    ElevatedButton(
                        onPressed: _syncHealthECG,
                        child: const Text("Sync ECG")),
                    ElevatedButton(
                        onPressed: _syncHealthTemp,
                        child: const Text("Sync Temp")),
                    ElevatedButton(
                        onPressed: _syncHealthStep,
                        child: const Text("Sync Step")),
                    ElevatedButton(
                        onPressed: _deleteHealthAll,
                        child: const Text("Delete Health All")),
                    ElevatedButton(
                        onPressed: () => _settingLangReleepWatch(0x00),
                        child: const Text("Setting Lang EN")),
                    ElevatedButton(
                        onPressed: () => _settingLangReleepWatch(0x01),
                        child: const Text("Setting Lang CN")),
                    ElevatedButton(
                        onPressed: () => _settingLangReleepWatch(0x0D),
                        child: const Text("Setting Lang TH")),
                    ElevatedButton(
                        onPressed: () => _settingTemperatureAlarm(38),
                        child: const Text("Setting Temp 38")),
                    ElevatedButton(
                        onPressed: () => _settingBloodOxygenAlarm(95),
                        child: const Text("Setting Spo2 95")),
                    ElevatedButton(
                        onPressed: () => _settingHeartAlarm(100, 45),
                        child: const Text("Setting HR 100,45")),
                    ElevatedButton(
                        onPressed: () => _settingWorkingMode(0x00),
                        child: const Text("Setting Mode normal")),
                    ElevatedButton(
                        onPressed: () => _settingWorkingMode(0x01),
                        child: const Text("Setting Mode care")),
                    ElevatedButton(
                        onPressed: () => _settingWorkingMode(0x02),
                        child: const Text("Setting Mode Save Power")),
                    ElevatedButton(
                        onPressed: () => _settingHeartMonitor(5),
                        child: const Text("Setting Heart Monitor 5m")),
                    ElevatedButton(
                        onPressed: () => _disableNotification(),
                        child: const Text("Disable Push Notification")),
                    ElevatedButton(
                        onPressed: () => _getConnectState(),
                        child: const Text("Connect State")),
                    ElevatedButton(
                        onPressed: () => _connectReleepWatch(
                            "C0:AB:61:18:5B:22",
                            "CCEE2AF2-36D8-8DE2-8F2F-34F11801250B"),
                        child: const Text("Connect E66")),
                    ElevatedButton(
                        onPressed: () => _disconnectReleepWatch(),
                        child: const Text("Disconnect")),
                    ElevatedButton(
                        onPressed: () =>
                            ReleepWatchConnect.startForegroundTask(),
                        child: const Text("startTask")),
                    ElevatedButton(
                        onPressed: () => ReleepWatchConnect.syncFitness(),
                        child: const Text("syncFitness")),
                    ElevatedButton(
                        onPressed: () => ReleepWatchConnect.syncRUN(),
                        child: const Text("syncRUN")),
                    ElevatedButton(
                        onPressed: () => ReleepWatchConnect.syncRunIndoors(),
                        child: const Text("syncRunIndoors")),
                    ElevatedButton(
                        onPressed: () => ReleepWatchConnect.syncRunOutSide(),
                        child: const Text("syncRunOutSide")),
                    ElevatedButton(
                        onPressed: () =>
                            Navigator.of(context).pushNamed('/Fitness'),
                        child: const Text("ไปยังหน้าออกกำลังกาย")),
                  ],
                ),
                Wrap(
                  children: [
                    Text("res :"),
                  ],
                ),
                TextField(
                  maxLines: null,
                  keyboardType: TextInputType.multiline,
                  controller: _resReleepWatch,
                ),
                ListView.builder(
                  shrinkWrap: true,
                  itemCount: _listWatch.length,
                  itemBuilder: (context, index) {
                    return ListTile(
                      title: Text('${_listWatch[index]['DeviceName']}' +
                          ' | ' +
                          '${_listWatch[index]['MacAddress']}' +
                          ' ' +
                          '${_listWatch[index]['UUIDString'] ?? ""}'),
                      onTap: () => {
                        _connectReleepWatch(
                            '${_listWatch[index]['MacAddress']}',
                            '${_listWatch[index]['UUIDString'] ?? ""}'),
                      },
                    );
                  },
                ),
              ],
            ),
          ),
          floatingActionButton: FloatingActionButton(
            backgroundColor: Colors.green,
            onPressed: _startWatchScan,
            child: const Icon(Icons.search),
          )),
    );
  }
}

class FitnessPage extends StatefulWidget {
  FitnessPage({Key? key}) : super(key: key);

  @override
  State<FitnessPage> createState() => _FitnessPageState();
}

class _FitnessPageState extends State<FitnessPage> {
  bool isStart = false;
  String text = "";
  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: Text("ออกกำลังกาย"),
        ),
        body: Text(text),
        floatingActionButton: SizedBox(
          width: 90,
          height: 90,
          child: FloatingActionButton(
            backgroundColor: Colors.green,
            onPressed: () async {
              try{
                if (!isStart) {
                  setState(() {
                    isStart = true;
                    text = "";
                  });
                  var res = await ReleepWatchConnect.startSport(typeSport: SportType.RIDE);
                  if (res != null) {
                    text = res.toString();
                    setState(() {
                      isStart = false;
                    });
                  }
                } else {
                  var res = await ReleepWatchConnect.stopSport();

                }
              }catch(e){
                setState(() {
                  isStart = false;
                });
              }

            },
            child: Icon(isStart ? Icons.stop : Icons.play_arrow, size: 60),
          ),
        ));
  }
}


class SportType {
  static const int RESERVED = 0;
  static const int RUN = 1;
  static const int SWIMMING = 2;
  static const int RIDE = 3;
  static const int FITNESS = 4;
  static const int SCRAP = 5;
  static const int SKIPPING_ROPE = 6;
  static const int BASKETBALL = 7;
  static const int WALKING = 8;
  static const int BADMINTON = 9;
  static const int FOOTBALL = 10;
  static const int MOUNTAINEERING = 11;
  static const int PING_PONG = 12;
  static const int FREE_MODE = 13;
  static const int RUN_INDOORS = 14;
  static const int RUN_OUTSIDE = 15;
  static const int WALK_OUTDOOR = 16;
  static const int WALK_INDOOR = 17;
  static const int WALK_AND_RUN = 18;
  static const int INDOOR_CYCLING = 19;
  static const int STEPPER = 20;
  static const int ROWING_MACHINE = 21;
  static const int REAL_TIME_MODE = 22;
  static const int SIT_UPS = 23;
  static const int LEAPING_MOTION = 24;
  static const int WEIGHT_TRAINING = 25;
  static const int YOGA = 26;
  static const int ONFOOT = 27;
  static const int VOLLEYBALL = 28;
  static const int KAYAK = 29;
  static const int ROLLER_SKATING = 30;
  static const int TENNIS = 31;
  static const int GOLF = 32;
  static const int ELLIPTICAL_MACHINE = 33;
  static const int DANCE = 34;
  static const int ROCK_CLIMBING = 35;
  static const int AEROBICS = 36;
  static const int OTHERSPORTS = 37;
}
