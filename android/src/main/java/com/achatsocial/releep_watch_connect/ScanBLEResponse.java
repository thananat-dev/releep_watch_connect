package com.achatsocial.releep_watch_connect;
import com.google.gson.annotations.SerializedName;

public class ScanBLEResponse {
    @SerializedName("DeviceName")
    private String DeviceName = "";
    @SerializedName("MacAddress")
    private String MacAddress = "";

    public ScanBLEResponse(String deviceName,String macAddress){
        this.DeviceName = deviceName;
        this.MacAddress = macAddress;
    }
}
