//
//  ScanBLEResponse.swift
//  releep_watch_connect
//
//  Created by EP&IT on 3/6/2565 BE.
//

import Foundation

class ScanBLEResponse : Codable {
    var DeviceName:String
    var MacAddress:String

    init(deviceName: String, macAddress: String) {
        self.DeviceName = deviceName
        self.MacAddress = macAddress
    }

}
