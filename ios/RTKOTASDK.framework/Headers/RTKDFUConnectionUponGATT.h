//
//  RTKDFURoutineGATT.h
//  RTKOTASDK
//
//  Created by jerome_gu on 2021/10/19.
//  Copyright © 2022 Realtek. All rights reserved.
//

#ifdef RTK_SDK_IS_STATIC_LIBRARY
#import "libRTKLEFoundation.h"
#import "RTKDFURoutine.h"
#else
#import <RTKLEFoundation/RTKLEFoundation.h>
#import <RTKOTASDK/RTKDFURoutine.h>
#endif


NS_ASSUME_NONNULL_BEGIN

/**
 * In terms of upgrading, the possilble mode that a device runs.
 */
typedef NS_ENUM(NSUInteger, RTKDFUPeripheralMode) {
    RTKDFUPeripheralMode_normal,        ///< The mode that serve normal functionality.
    RTKDFUPeripheralMode_ota,           ///< The mode that serve upgrade task.
};


/**
 * A concrete subclass of @c RTKConnectionUponGATT which communicate with a remote LE device to perform upgrade related procedure.
 *
 * @discussion The @c RTKDFUConnectionUponGATT conforms to @c RTKDFURoutine protocol, providing interfaces to interact with device for upgrade. In addition, @c RTKDFUConnectionUponGATT defines properties and method to manage entering OTA mode and update LE connection parameter. When be requested to switching to OTA mode, the device will reboot to a dedicated mode, and be treated as another @c RTKDFUConnectionUponGATT instance.
 *
 * You can tell if the device can switching to OTA mode by @c canEnterOTAMode property, and tell if device can be started upgrade by @c canUpgrade property.
 */
@interface RTKDFUConnectionUponGATT : RTKConnectionUponGATT <RTKDFURoutine>

/**
 * Returns a boolean value that indicates whether the connected device can switch to a mode dedicated for upgrade.
 *
 * @discussion When this property is @c YES, you can call @c -switchToOTAModeWithCompletionHandler: to make device switching to OTA mode.
 */
@property (nonatomic, readonly) BOOL canEnterOTAMode;


/**
 * Return a boolean value that determines whether connected device can upgrade without switching to a mode dedicated for upgrade.
 *
 * @discussion If this property is @c NO, you can not call any image upgrade action methods defined in @c RTKDFURoutine .
 */
@property (nonatomic, readonly) BOOL canUpgrade;


/**
 * Request peripheral to upgrade VP image.
 *
 */
@property (nonatomic) BOOL upgradeVP;

/**
 * Request the connected device to initiate a connection parameter update procedure.
 *
 * @param minInterval Used to calculate the minimum value for the connection interval. Range from 6 to 3200.
 * @param maxInterval Used to calculate the maximum value for the connection interval. Range from 6 to 3200.
 * @param latency The slave latency parameter.
 * @param timeout Used for calculate connection timeout parameter.
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion You may call this method to update connection parameter for a faster upgrade speed. The connection parameters you passed may not be accepted by remote device.
 * Refers to SIG Bluetooth Spec for the connection parameter requirements.
 */
- (void)updateConnectionParameterWithMinInterval:(NSUInteger)minInterval
                                     maxInterval:(NSUInteger)maxInterval
                                         latency:(NSUInteger)latency
                             suspervisionTimeout:(NSUInteger)timeout
                               completionHandler:(nullable RTKLECompletionBlock)handler;

/**
 * Send the image version to the device.
 *
 * @param imageInfo The imageID and imageVersion to be send.
 * @param handler The completion handler to call when the request is complete.
 */
- (void)sendImageVersionOfImageInfo:(RTKImageVersionInfo_t)imageInfo withCompletionHandler:(nullable RTKLECompletionBlock)handler;

/**
 * Request the connected device to activate received images and reset to a specified runing mode.
 *
 * @param mode The device mode used for device next boot.
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion This method is expected be called by @c RTKDFUUpgrade.
 */
- (void)activateImagesAndResetToMode:(RTKDFUPeripheralMode)mode ofDeviceInfo:(RTKOTADeviceInfo *)deviceInfo withCompletionHandler:(nullable RTKLECompletionBlock)handler;

/**
 * Request the receiver to switching to a mode dedicated for upgrade.
 *
 * @param handler The completion handler to call when the request is complete.
 * @discussion You can call this method only if @c canEnterOTAMode return @c YES. You should not call this method when receiver represents a device already in OTA mode.
 *
 * A success switching will result in current connection disconnected, you typically call @c RTKDFUManager @c -scanForOTAModeDeviceConnectionOfDeviceConnection:withDeviceInfo:completionHandler: to scan for the device in OTA mode.
 */
- (void)switchToOTAModeWithCompletionHandler:(nullable RTKLECompletionBlock)handler;

- (void)reportImageID:(RTKImageId)imageID currentImageNumber:(uint8_t)currentNumber totalImageNumber:(uint8_t)totalNumber withCompletionHandler:(nullable RTKLECompletionBlock)handler;

- (void)checkImagesKeyOfCount:(uint16_t)imageNum andKeyInfo:(RTKImageKeyInfo_t *)imagesKey withCompletionHandler:(nullable RTKLECompletionBlock)handler;

@end

NS_ASSUME_NONNULL_END
