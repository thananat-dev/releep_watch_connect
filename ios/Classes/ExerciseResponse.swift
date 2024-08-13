//
//  ExerciseResponse.swift
//  releep_watch_connect
//
//  Created by jenthum worakul on 13/8/2567 BE.
//

import Foundation

@objcMembers public class YCReceivedMonitoringModeInfo2: NSObject, Codable {
    public internal(set) var startTimeStamp: Int
    public internal(set) var heartRate: Int
    public internal(set) var systolicBloodPressure: Int
    public internal(set) var diastolicBloodPressure: Int
    public internal(set) var bloodOxygen: Int
    public internal(set) var respirationRate: Int
    public internal(set) var temperature: Double
    public internal(set) var realStep: Int
    public internal(set) var realDistance: UInt16
    public internal(set) var realCalories: UInt16
    public internal(set) var modeStep: Int
    public internal(set) var modeDistance: UInt16
    public internal(set) var modeCalories: UInt16
    public internal(set) var ppi: Int
    public internal(set) var vo2max: Int
    public internal(set) var isStop: Int

    public init(
        startTimeStamp: Int,
        heartRate: Int,
        systolicBloodPressure: Int,
        diastolicBloodPressure: Int,
        bloodOxygen: Int,
        respirationRate: Int,
        temperature: Double,
        realStep: Int,
        realDistance: UInt16,
        realCalories: UInt16,
        modeStep: Int,
        modeDistance: UInt16,
        modeCalories: UInt16,
        ppi: Int,
        vo2max: Int,
        isStop: Int
    ) {
        self.startTimeStamp = startTimeStamp
        self.heartRate = heartRate
        self.systolicBloodPressure = systolicBloodPressure
        self.diastolicBloodPressure = diastolicBloodPressure
        self.bloodOxygen = bloodOxygen
        self.respirationRate = respirationRate
        self.temperature = temperature
        self.realStep = realStep
        self.realDistance = realDistance
        self.realCalories = realCalories
        self.modeStep = modeStep
        self.modeDistance = modeDistance
        self.modeCalories = modeCalories
        self.ppi = ppi
        self.vo2max = vo2max
        self.isStop = isStop
    }
}
