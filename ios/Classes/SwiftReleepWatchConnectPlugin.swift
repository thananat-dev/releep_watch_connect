import Flutter
import BackgroundTasks
import UIKit
import YCProductSDK
import CoreBluetooth
import SwiftyJSON

public var devicesList = [CBPeripheral]()

public class SwiftReleepWatchConnectPlugin: NSObject, FlutterPlugin {
    
    var backgroundTaskID: UIBackgroundTaskIdentifier = .invalid
    var timer: Timer?
    
    public static func register(with registrar: FlutterPluginRegistrar) {
        // 初始化
        YCProduct.setLogLevel(.normal)
        _ = YCProduct.shared
        
        // 增加通知
        //      NotificationCenter.default.addObserver(
        //          self, selector: #selector(deviceStateChange(_:)), name: YCProduct.deviceStateNotification, object: nil
        //      )
        
        let channel = FlutterMethodChannel(name: "releep_watch_connect", binaryMessenger: registrar.messenger())
        let instance = SwiftReleepWatchConnectPlugin()
        
        registrar.addMethodCallDelegate(instance, channel: channel)
        
        let eventChannel = FlutterEventChannel(name: "scan_releep_watch", binaryMessenger: registrar.messenger())
        eventChannel.setStreamHandler(SwiftStreamHandler())
        
        // Register MyAppBackgroundTask as the app delegate
        if let appDelegateClass = NSClassFromString("MyAppBackgroundTask") as? NSObject.Type {
            let appDelegate = appDelegateClass.init() as! UIResponder
            UIApplication.shared.delegate = appDelegate as? any UIApplicationDelegate
        }
    }
    
    public func handle(_ call: FlutterMethodCall, result: @escaping FlutterResult) {
        if call.method == "getPlatformVersion" {
            result("iOS " + UIDevice.current.systemVersion)
        }
        else if call.method == "connectReleepWatch"{
            let args = call.arguments as? Dictionary<String, Any>
            let _macAddress = args?["releepWatchMac"] as? String
            let _uuidString = args?["releepUUIDString"] as? String
            let index = devicesList.firstIndex(where: { $0.macAddress == _macAddress || $0.identifier.uuidString == _uuidString})
            if index != nil {
                let device = devicesList[index ?? 0]
                
                YCProduct.connectDevice(device) { state, error in
                    
                    if state == .connected {
                        result(0)
                    }
                    else{
                        result(state.rawValue)
                    }
                    
                }
            }
            else {
                let deviceInfo = YCProduct.shared.currentPeripheral
                if deviceInfo == nil {
                    YCProduct.scanningDevice { devices, error in
                        var _ : [ScanBLEResponse] = [ScanBLEResponse]()
                        devicesList = devices
                        for device in devices {
                            print(device.name ?? "", device.macAddress, device.identifier.uuidString)
                        }
                        let index = devicesList.firstIndex(where: { $0.macAddress == _macAddress || $0.identifier.uuidString == _uuidString})
                        if index != nil {
                            let device = devicesList[index ?? 0]
                            
                            YCProduct.connectDevice(device) { state, error in
                                
                                if state == .connected {
                                    result(0)
                                }
                                else{
                                    result(state.rawValue)
                                }
                                
                            }
                        }
                        else{
                            result(1)
                        }
                    }
                }
                else {
                    YCProduct.connectDevice(deviceInfo!) { state, error in
                        
                        if state == .connected {
                            result(0)
                        }
                        else{
                            result(state.rawValue)
                        }
                        
                    }
                }
            }
        }
        else if call.method == "getConnectionState" {
            let device = YCProduct.shared.currentPeripheral
            print("getConnectionState" ,device as Any )
            if device == nil{
                result(3)
            }
            else{
                YCProduct.connectDevice(device!) { state, error in
                if state == .connected {
                print("connected")
                }
                    result(state.rawValue)
                }
                
            }
        }
        else if call.method == "settingTime" {
            let device = YCProduct.shared.currentPeripheral
            print("getConnectionState" ,device as Any )
        }
        else if call.method == "settingLang" {
            let args = call.arguments as! Dictionary<String, Any>
            let langCode = args["langCode"] as! UInt8
            YCProduct.setDeviceLanguage(language: YCDeviceLanguageType(rawValue: langCode) ?? .english) { state, response in
                if state == .succeed {
                    print("success")
                } else {
                    print("fail")
                }
                result(state)
            }
        }
        else if call.method == "syncHealthData" {
            YCProduct.queryHealthData(dataType: YCQueryHealthDataType.combinedData) { state, response in
                
                if state == .succeed, let datas = response as? [YCHealthDataCombinedData] {
                    
                    //                  for info in datas {
                    //                      print(info.startTimeStamp, info.bloodOxygen, info.respirationRate,
                    //                            info.temperature,
                    //                            info.fat
                    //                      )
                    //                  }
                    var arrayList = Array<Any>()
                    
                    for info in datas {
                        let tempFloatStr = String(info.temperature).components(separatedBy: ".")[1]
                        arrayList.append(
                            [
                                "heartValue": info.heartRate,
                                "hrvValue":info.hrv,
                                "cvrrValue":info.cvrr,
                                "stepValue": info.step,
                                "DBPValue": info.diastolicBloodPressure,
                                "bodyFatFloatValue": 0,
                                "OOValue": info.bloodOxygen,
                                "bodyFatIntValue": Int(info.fat),
                                "tempIntValue": Int(info.temperature),
                                "tempFloatValue": Int(tempFloatStr) ?? 0,
                                "startTime": info.startTimeStamp,
                                "SBPValue": info.systolicBloodPressure,
                                "respiratoryRateValue": info.respirationRate,
                            ]
                        )
                    }
                    print("syncHealthData",datas)
                    
                    result(arrayList)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "syncHeartRate" {
            YCProduct.queryHealthData(dataType: YCQueryHealthDataType.heartRate) { state, response in
                
                if state == .succeed, let datas = response as? [YCHealthDataHeartRate] {
                    var arrayList = Array<Any>()
                    for info in datas {
                        //print(info.startTimeStamp, info.heartRate)
                        arrayList.append(
                            ["heartStartTime":info.startTimeStamp,
                             "heartValue":info.heartRate]
                        )
                    }
                    print("syncHeartRate",datas)
                    //let json = JSON(datas)
                    result(arrayList)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "syncBlood" {
            YCProduct.queryHealthData(dataType: YCQueryHealthDataType.bloodPressure) { state, response in
                
                if state == .succeed, let datas = response as? [YCHealthDataBloodPressure] {
                    
                    var arrayList = Array<Any>()
                    for info in datas {
                        //print(info.startTimeStamp, info.heartRate)
                        arrayList.append(
                            [
                                "bloodStartTime":info.startTimeStamp,
                                "bloodDBP":info.diastolicBloodPressure,
                                "bloodSBP":info.systolicBloodPressure,
                                "islnflated": 1
                            ]
                        )
                    }
                    print("syncBlood",datas)
                    result(arrayList)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "syncSPO2" {
            YCProduct.queryHealthData(dataType: YCQueryHealthDataType.bloodOxygen) { state, response in
                
                if state == .succeed, let datas = response as? [YCHealthDataBloodOxygen] {
                    
                    //                  for info in datas {
                    //                      print(info.startTimeStamp, info.bloodOxygen)
                    //                  }
                    print("syncSpo2",datas)
                    result(datas)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "syncTemp" {
            YCProduct.queryHealthData(dataType: YCQueryHealthDataType.bodyTemperature) { state, response in
                
                if state == .succeed, let datas = response as? [YCHealthDataBodyTemperature] {
                    
                    //                  for info in datas {
                    //                      print(info.startTimeStamp, info.temperature)
                    //                  }
                    print("syncTemp",datas)
                    result(datas)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "syncStep" {
            YCProduct.queryHealthData(dataType: YCQueryHealthDataType.step) { state, response in
                
                if state == .succeed, let datas = response as? [YCHealthDataStep] {
                    
                    //                  for info in datas {
                    //                      print(info.startTimeStamp, info.step)
                    //                  }
                    var arrayList = Array<Any>()
                    for info in datas {
                        arrayList.append(
                            [
                                "code": 0,
                                "nowCalorie":info.calories,
                                "nowDistance":info.distance,
                                "nowStep": info.step,
                                "dataType": 524,
                                "supportOk": 1
                            ]
                        )
                    }
                    print("syncStep",datas)
                    result(arrayList)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "syncSport" {
            YCProduct.queryHealthData(dataType: YCQueryHealthDataType.sportModeHistoryData) { state, response in
                
                if state == .succeed, let datas = response as? [YCHealthDataSportModeHistory] {
                    var arrayList = Array<Any>()
                    for info in datas {
                        //print(info.startTimeStamp, info.heartRate)
                        arrayList.append(
                            [
                                "sportStartTime":info.startTimeStamp,
                                "sportEndTime":info.endTimeStamp,
                                "sportSrep":info.step,
                                "sportDistance": info.distance,
                                "sportCalorie": info.calories
                            ]
                        )
                    }
                    print("syncSport",datas)
                    result(arrayList)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "syncSleep" {
            YCProduct.queryHealthData(dataType: YCQueryHealthDataType.sleep) { state, response in
                
                if state == .succeed, let datas = response as? [YCHealthDataSleep] {
                    var arrayList = Array<Any>()
                    for info in datas {
                        var sleepDataList = Array<Any>()
                        //print(info.startTimeStamp, info.heartRate)
                        for sleepData in info.sleepDetailDatas {
                            sleepDataList.append(
                                [
                                    "sleepStartTime":sleepData.startTimeStamp,
                                    "sleepLen":sleepData.duration,
                                    "sleepType":sleepData.sleepType.rawValue,
                                ]
                            )
                        }
                        arrayList.append(
                            [
                                "deepSleepCount":info.deepSleepCount,
                                "lightSleepCount":info.lightSleepCount,
                                "sleepData":sleepDataList,
                                "startTime": info.startTimeStamp,
                                "lightSleepTotal": info.lightSleepMinutes,
                                "deepSleepTotal": info.deepSleepMinutes,
                                "endTime": info.endTimeStamp
                            ]
                        )
                    }
                    print("syncSleep",datas)
                    result(arrayList)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "syncECG" {
            YCProduct.queryDeviceElectrodePosition { state, response in
                if state == .succeed,
                   let info = response as? YCDeviceElectrodePosition {
                    print(info.rawValue)
                    result(info)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "disconnectReleepWatch" {
            //            YCProduct.disconnectDevice(device)
            YCProduct.scanningDevice { devices, error in
                var _ : [ScanBLEResponse] = [ScanBLEResponse]()
                devicesList = devices
                for device in devices {
                    print(device.name ?? "", device.macAddress)
                    YCProduct.disconnectDevice(device) { state, _ in
                        print(state.rawValue)
                    }
                }
            }
            result(0)
        }
        else if call.method == "getCurrentSystemMode" {
            YCProduct.queryDeviceWorkMode { state, response in
                if state == .succeed,
                   let info = response as? YCDeviceWorkModeType {
                    print(info)
                    result(info)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "getDeviceInfo" {
            result(0)
        }
        else if call.method == "settingWokingMode" {
            let args = call.arguments as! Dictionary<String, Any>
            let setmode = args["workingMode"] as! UInt8
            YCProduct.setDeviceWorkMode(mode: YCDeviceWorkModeType(rawValue: setmode) ?? .normal) { state, response in
                if state == .succeed {
                    print("success")
                    result(0)
                } else {
                    print("fail")
                    result(nil)
                }
            }
        }
        else if call.method == "settingHeartMonitor" {
            let args = call.arguments as! Dictionary<String, Any>
            let time = args["intervalTime"] as! UInt8
            YCProduct.setDeviceHeartRateMonitoringMode(isEnable: true, interval: time) { state, response in
                if state == .succeed {
                    print("success")
                    result(0)
                } else {
                    print("fail")
                    result(nil)
                }
            }
        }
        else if call.method == "settingTemperatureMonitor" {
            let args = call.arguments as! Dictionary<String, Any>
            let time = args["intervalTime"] as! UInt8
            YCProduct.setDeviceTemperatureMonitoringMode(isEnable: true, interval: time) { state, response in
                if state == .succeed {
                    print("success")
                    result(0)
                } else {
                    print("fail")
                    result(nil)
                }
            }
        }
        else if call.method == "settingBloodOxygenModeMonitor" {
            let args = call.arguments as! Dictionary<String, Any>
            let time = args["intervalTime"] as! UInt8
            YCProduct.setDeviceBloodOxygenMonitoringMode(isEnable: true, interval: time) { state, response in
                if state == .succeed {
                    print("success")
                    result(0)
                } else {
                    print("fail")
                    result(nil)
                }
            }
        }
        else if call.method == "settingBloodOxygenAlarm" {
            let args = call.arguments as! Dictionary<String, Any>
            let value = args["value"] as! UInt8
            YCProduct.setDeviceBloodOxygenAlarm(isEnable: true, minimum: value) { state, response in
                if state == .succeed {
                    print("success")
                    result(0)
                } else {
                    print("fail")
                    result(nil)
                }
            }
        }
        else if call.method == "settingTemperatureAlarm" {
            let args = call.arguments as! Dictionary<String, Any>
            let value = args["value"] as! UInt8
            YCProduct.setDeviceTemperatureAlarm(isEnable: true,
                                                highTemperatureIntegerValue: value,
                                                highTemperatureDecimalValue: 0,
                                                lowTemperatureIntegerValue: 35,
                                                lowTemperatureDecimalValue: 5) { state, response in
                if state == .succeed {
                    print("success")
                    result(0)
                } else {
                    print("fail")
                    result(nil)
                }
            }
        }
        else if call.method == "settingHeartAlarm" {
            let args = call.arguments as! Dictionary<String, Any>
            let highHeart = args["highHeart"] as! UInt8
            let lowHeart = args["lowHeart"] as! UInt8
            YCProduct.setDeviceHeartRateAlarm(isEnable: true,
                                              maxHeartRate: highHeart,
                                              minHeartRate: lowHeart) { state, response in
                if state == .succeed {
                    print("success")
                    result(0)
                } else {
                    print("fail")
                    result(nil)
                }
            }
        }
        else if call.method == "settingHeartAlarm" {
            let args = call.arguments as! Dictionary<String, Any>
            let maxSBP = args["maxSBP"] as! UInt8
            let maxDBP = args["maxDBP"] as! UInt8
            let minSBP = args["minSBP"] as! UInt8
            let minDBP = args["minDBP"] as! UInt8
            YCProduct.setDeviceBloodPressureAlarm(isEnable: true,
                                                  maximumSystolicBloodPressure: maxSBP,
                                                  maximumDiastolicBloodPressure:maxDBP,
                                                  minimumSystolicBloodPressure: minSBP,
                                                  minimumDiastolicBloodPressure: minDBP) { state, response in
                if state == .succeed {
                    print("success")
                    result(0)
                } else {
                    print("fail")
                    result(nil)
                }
            }
        }
        else if call.method == "disbleWatchNotification" {
            YCProduct.setDeviceInfoPush(isEnable: false, infoPushType:[.call,.email,.line,.facebook,.instagram,.linkedIn,.messenger,.sms,.telegram,.twitter,.skype,.whatsAPP,.snapchat,.qq]) { state, response in
                if state == .succeed {
                    print("disbleWatchNotification succeed")
                    result(0)
                }
                else{
                    result(nil)
                }
            }
        }
        else if call.method == "deleteHistoryHealth" {
            YCProduct.deleteHealthData(dataType:
                                        YCDeleteHealthDataType.combinedData) { state, response in
                if state == .succeed {
                    print("Delete succeed")
                    result(0)
                }
                else{
                    result(nil)
                }
            }
        }
        else if(call.method == "startService") {
           // startService()
            result(true)
        } else if(call.method == "stopService") {
           // stopService()
            result(true)
        } else if(call.method == "removeMacAddress") {
            UserDefaults.standard.set("", forKey: "KEY_BLE_WATCH")
            result(true)
        }
    }
    

//    private func startService() {
//        stopService()
//        let center = UNUserNotificationCenter.current()
//        center.requestAuthorization(options: [.alert, .sound]) { _, _ in }
//
//        let content = UNMutableNotificationContent()
//        content.title = "Foreground Service"
//        content.body = "Running background task..."
//
//        let trigger = UNTimeIntervalNotificationTrigger(timeInterval: 60, repeats: true)
//        let request = UNNotificationRequest(identifier: "foreground_service", content: content, trigger: trigger)
//
//        center.add(request)
//
//        timer = Timer.scheduledTimer(withTimeInterval: 60, repeats: true) { _ in
//            print("Running background task...")
//        }
//    }
    
    private func startService() {
      let center = UNUserNotificationCenter.current()
      center.requestAuthorization(options: [.alert, .sound]) { _, _ in }

      let content = UNMutableNotificationContent()
      content.title = "Foreground Service"
      content.body = "Running background task..."
      
      // Schedule the initial notification
      let initialTrigger = UNTimeIntervalNotificationTrigger(timeInterval: 0.1, repeats: false)
      let initialRequest = UNNotificationRequest(identifier: "foreground_service_init", content: content, trigger: initialTrigger)
      center.add(initialRequest)

      // Schedule the update notification
      let updateTrigger = UNTimeIntervalNotificationTrigger(timeInterval: 60, repeats: true)
      let updateRequest = UNNotificationRequest(identifier: "foreground_service_update", content: content, trigger: updateTrigger)
      center.add(updateRequest)

      // Register for remote notifications
      let notificationCenter = NotificationCenter.default
      notificationCenter.addObserver(self, selector: #selector(didReceiveRemoteNotification(_:)), name: Notification.Name("didReceiveRemoteNotification"), object: nil)
      
      // Start the timer to execute the task every 60 seconds
      timer = Timer.scheduledTimer(withTimeInterval: 60, repeats: true) { _ in
        // Update the notification content
        content.body = "Running background task...\nLast updated: \(Date())"
        let updateRequest = UNNotificationRequest(identifier: "foreground_service_update", content: content, trigger: updateTrigger)
        center.add(updateRequest)

        // Perform the background task
        print("Running background task...")

        // Schedule a local notification to update the notification content when the app is minimized
        let notificationContent = UNMutableNotificationContent()
        notificationContent.title = "Foreground Service"
        notificationContent.body = "Background task is running..."
        let notificationTrigger = UNTimeIntervalNotificationTrigger(timeInterval: 1, repeats: false)
        let notificationRequest = UNNotificationRequest(identifier: "foreground_service_notification", content: notificationContent, trigger: notificationTrigger)
        center.add(notificationRequest)
      }
    }

    @objc private func didReceiveRemoteNotification(_ notification: Notification) {
      // This method is called when a remote notification is received
      // You can perform any background tasks here
      print("Received remote notification")
    }


    private func stopService() {
      let center = UNUserNotificationCenter.current()
      center.removePendingNotificationRequests(withIdentifiers: ["foreground_service"])

      timer?.invalidate()
      timer = nil
    }
    
    
    /// 连接状态的变化
    @objc private func deviceStateChange(_ ntf: Notification) {
        
        guard let info = ntf.userInfo as? [String: Any],
              let state = info[YCProduct.connecteStateKey] as? YCProductState else {
                  return
              }
        
        print("=== 状态变化 \(state.rawValue)")
        
    }
}

class SwiftStreamHandler: NSObject, FlutterStreamHandler {
    public func onListen(withArguments arguments: Any?, eventSink events: @escaping FlutterEventSink) -> FlutterError? {
        if arguments as! String == "scan"{
            
            YCProduct.scanningDevice { devices, error in
                var scanBLEResponse : [ScanBLEResponse] = [ScanBLEResponse]()
                devicesList = devices
                for device in devices {
                    print(device.name ?? "", device.macAddress, device.identifier.uuidString)
                    let BLEObject : ScanBLEResponse = ScanBLEResponse(deviceName: device.name ?? "", macAddress:device.macAddress,uuidString: device.identifier.uuidString)
                    scanBLEResponse.append(BLEObject)
                    
                }
                do {
                    let jsonData = try JSONEncoder().encode(scanBLEResponse)
                    let jsonString = String(data: jsonData, encoding: .utf8)!
                    events(jsonString)
                } catch {
                    print(error)
                }
                //                let jsonData = self.json(from: scanBLEResponse)
                //                events(jsonData)
            }
            
        }
        
        //        events(FlutterError(code: "ERROR_CODE",
        //                             message: "Detailed message",
        //                             details: nil)) // in case of errors
        //        events(FlutterEndOfEventStream) // when stream is over
        return nil
    }
    
    public func onCancel(withArguments arguments: Any?) -> FlutterError? {
        return nil
    }
    
    func json(from object:Any) -> String? {
        guard let data = try? JSONSerialization.data(withJSONObject: object, options: []) else {
            return nil
        }
        return String(data: data, encoding: String.Encoding.utf8)
    }
    
   
    
}
