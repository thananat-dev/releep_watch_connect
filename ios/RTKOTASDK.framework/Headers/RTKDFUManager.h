//
//  RTKDFUManager.h
//  RTKOTASDK
//
//  Created by jerome_gu on 2021/10/21.
//  Copyright © 2022 Realtek. All rights reserved.
//

#ifdef RTK_SDK_IS_STATIC_LIBRARY
#import "libRTKLEFoundation.h"
#import "RTKOTADeviceInfo.h"
#else
#import <RTKLEFoundation/RTKLEFoundation.h>
#import <RTKOTASDK/RTKOTADeviceInfo.h>
#endif

NS_ASSUME_NONNULL_BEGIN


/**
 * A profile manager that manage connection with devices that are about to be upgrade.
 *
 * @discussion The @c RTKDFUManager class provides two scan methods for scaning for device in OTA mode and scaning for companion device.
 * Dont call @c -scanForPeripherals: which is inherit from @c RTKProfileConnectionManager on this class instance to scan for neaby devices, this class only support scan for OTA mode device and companion device.
 */
@interface RTKDFUManager : RTKProfileConnectionManager

/**
 * Scan for device in OTA mode of the given device that is in normal mode.
 *
 * @param connection The connection with device for which to scan its in OTA mode device object.
 * @param deviceInfo The object containing information of this device when scan.
 * @param handler The completion handler to call when the expected device is found or time out occurs.
 *
 * @discussion Although there is only one physical device, SDK uses two @c RTKConnectionUponGATT objects to represents normal mode device and OTA mode device. When a device switches to OTA mode, call this method to discover the device in OTA mode. When the device in OTA mode is discovered, the @c handler block is invoked with @c otaModeDevice parameter set to object that represents in OTA mode device.
 *
 * The default interval of time out is 10 seconds.
 */
- (void)scanForOTAModeDeviceConnectionOfDeviceConnection:(RTKConnectionUponGATT *)connection
                                          withDeviceInfo:(RTKOTADeviceInfo *)deviceInfo
                                       completionHandler:(void (^)(BOOL success, NSError *__nullable error, RTKConnectionUponGATT *otaModeDevice))handler;

/**
 * Scan for companion device of the given device.
 *
 * @param connection The connection of a device of which to scan the companion device.
 * @param deviceInfo The object containing information of this given device.
 * @param handler The completion handler to call when the expected device is found or time out occurs.
 *
 * @discussion While upgrading a RWS pair devices, call this method to scan for the companion device when finish upgrade the first device.
 *
 * The default interval of time out is 10 seconds.
 */
- (void)scanForCompanionDeviceConnectionOfDeviceConnection:(RTKProfileConnection *)connection
                                            withDeviceInfo:(RTKOTADeviceInfo *)deviceInfo
                                         completionHandler:(void (^)(BOOL success, NSError *__nullable error, RTKProfileConnection *companion))handler;

@end

NS_ASSUME_NONNULL_END
