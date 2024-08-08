//
//  RTKDFUUpgrade.h
//  RTKOTASDK
//
//  Created by jerome_gu on 2020/3/19.
//  Copyright © 2022 Realtek. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef RTK_SDK_IS_STATIC_LIBRARY
#import "libRTKLEFoundation.h"
#import "RTKOTAUpgradeBin.h"
#import "RTKOTADeviceInfo.h"
#import "RTKDFURoutine.h"
#else
#import <RTKLEFoundation/RTKLEFoundation.h>
#import <RTKOTASDK/RTKOTAUpgradeBin.h>
#import <RTKOTASDK/RTKOTADeviceInfo.h>
#import <RTKOTASDK/RTKDFURoutine.h>
#endif


NS_ASSUME_NONNULL_BEGIN

@class RTKDFUUpgrade;

/**
 * @protocol RTKDFUUpgradeDelegate
 *
 * @c RTKDFUUpgradeDelegate defines methods to be called by @c RTKDFUUpgrade object to report events during upgrading.
 *
 * @discussion You should set your  @c RTKDFUUpgradeDelegate conformed object to @c RTKDFUUpgrade delegate before call @c -prepareForUpgrade: or any upgrade methods.
 */
@protocol RTKDFUUpgradeDelegate <NSObject>
@optional

/**
 * Tells the delegate that upgrade is able to be started.
 *
 * @param task The upgrade task reporting this event.
 *
 * @discussion As a result of  @c -prepareForUpgrade: invocation,  a @c RTKDFUUpgrade object calls this method when preparation process succeed.
 * When this method get called, you can access device inforamtion related to upgrade by @c -RTKDFUUpgrade.deviceInfo, and you can call one of upgrade methods to start upgrade procedure.
 */
- (void)DFUUpgradeDidReadyForUpgrade:(RTKDFUUpgrade *)task;

/**
 * Tells the delegate that upgrade could not be started.
 *
 * @param task The upgrade task reporting this event.
 * @param error An error object containing information about the preparation failure.
 *
 * @discussion As a result of  @c -prepareForUpgrade: invocation,  @c RTKDFUUpgrade may call this method if preparation process fail.
 * You can not call any upgrade method to start upgrade procedure after this method get called.
 */
- (void)DFUUpgrade:(RTKDFUUpgrade *)task couldNotUpgradeWithError:(NSError *)error;

/**
 * Tells the delegate that upgrade is about to send image bytes.
 *
 * @param task The upgrade task reporting this event.
 * @param connection The device connection upon which upgrade is performing.
 * @param continuationHandler The block must be called when delegate finishs its work.
 *
 * @discussion A DFU delegate can use this method to do anything need just before Upgrade send image bytes. For example, delegate may perform updating connection parameter for a faster connection.
 */
- (void)DFUUpgrade:(RTKDFUUpgrade *)task
isAboutToSendImageBytesTo:(RTKProfileConnection *)connection
   withContinuationHandler:(void(^)(void))continuationHandler;

/**
 * Tell the delegate that the new progress of image byte sending.
 *
 * @param task The upgrade task reporting this event.
 * @param connection The device connection upon which upgrade is performing.
 * @param length The bytes count of image did send to remote device.
 * @param image The image that is current upgrading.
 *
 * @discussion To get the total bytes count of image, call @c image.data.length .
 */
- (void)DFUUpgrade:(RTKDFUUpgrade *)task
        withDevice:(RTKProfileConnection *)connection
 didSendBytesCount:(NSUInteger)length
           ofImage:(RTKOTAUpgradeBin *)image;


/**
 * Tell the delegate that @c RTKDFUUpgrade is about to send a new image.
 *
 * @param task The upgrade task reporting this event.
 * @param connection The device connection upon which upgrade is performing.
 * @param image The image that is about to be upgrade.
 */
- (void)DFUUpgrade:(RTKDFUUpgrade *)task
        withDevice:(RTKProfileConnection *)connection
     willSendImage:(RTKOTAUpgradeBin *)image;

/**
 * Tell the delegate that @c RTKDFUUpgrade did finish sending a image.
 *
 * @param task The upgrade task reporting this event.
 * @param connection The device connection upon which upgrade is performing.
 * @param image The image that just be sended.
 */
- (void)DFUUpgrade:(RTKDFUUpgrade *)task
        withDevice:(RTKProfileConnection *)connection
didCompleteSendImage:(RTKOTAUpgradeBin *)image;


/**
 * Tell the delegate that @c RTKDFUUpgrade did activate sended images.
 *
 * @param task The upgrade task reporting this event.
 * @param connection The device connection upon which upgrade is performing.
 * @param images Images that just be activated.
 *
 * @discussion Sended image works for next device boot only if be successfully activated .
 */
- (void)DFUUpgrade:(RTKDFUUpgrade *)task
        withDevice:(RTKProfileConnection *)connection
   didActivateImages:(NSSet<RTKOTAUpgradeBin*>*)images;


/**
 * Tell the delegate that @c RTKDFUUpgrade did complete upgrade one device and begin upgrade companion device.
 *
 * @param task The upgrade task reporting this event.
 *
 * @discussion If the remote device to be upgraded is a member of RWS bud pair, @c RTKDFUUpgrade will automatically discover and upgrade the companion device.
 */
- (void)DFUUpgradeDidFinishFirstDeviceAndPrepareCompanionDevice:(RTKDFUUpgrade *)task;

/**
 * Tell the delegate that @c RTKDFUUpgrade is about to upgrade the companion device.
 *
 * @param task The upgrade task reporting this event.
 * @param connection The device connection upon which upgrade is performing. The connection associate with the companion device.
 * @param companionInfo The device information related to upgrade of the companion device.
 *
 * @discussion The @c connection parameter typically is different from the @c RTKDFUUpgrade @c deviceConnection property value. Use the @c companionInfo parameter to get the information of companion device. Soon after this method get called, @c RTKDFUUpgrade will report progress of images send to the companion device.
 */
- (void)DFUUpgrade:(RTKDFUUpgrade *)task
willUpgradeCompanionDevice:(RTKProfileConnection *)connection
        deviceInfo:(RTKOTADeviceInfo *)companionInfo;


@required

/**
 * Tell the delegate that upgrade did complete successfully or unsuccessfully.
 *
 * @param task The upgrade task reporting this event.
 * @param error  An error object that indicating upgrade failure, or nil indicating upgrade success.
 *
 * @discussion Check if error parameter value is nil to determine if upgrade succeed. In general, device will reboot to use new images, result in a disconnection.
 */
- (void)DFUUpgrade:(RTKDFUUpgrade *)task didFinishUpgradeWithError:(nullable NSError *)error;

@end


/**
 * An object that manage device upgrade task.
 *
 * @discussion An (concreate) @c RTKDFUUpgrade object communicate with remote device to upgrade it. The device can be a GATT profile connected device (respresendted as @c CBPeripheral instance), or iAP profile connected device (represented as @c EAAccessory instance). When upgrade a GATT device, you call @c -initWithPeripheral: initializer, and the  @c RTKDFUUpgradeGATT subclass  object is actually returned. @c RTKDFUUpgradeIAP subclass object is actually returned when call @c -initWithAccessory: for upgrade a iAP device.
 *
 * If you have a @c RTKDFUUpgrade object at hand for upgrade, call @c -prepareForUpgrade prior to calling any  @c -upgrade: methods, and wait for @c -DFUUpgradeDidReadyForUpgrade: and @c -DFUUpgrade:couldNotUpgradeWithError: get called on your delegate object. You can call @c -upgrade: methods to start upgrade only after @c -DFUUpgradeDidReadyForUpgrade: get called. In principle, every @c -upgrade: invocation should be preceded by a @c -DFUUpgradeDidReadyForUpgrade: invocation.
 *
 * When the delegate @c -DFUUpgradeDidReadyForUpgrade: get called, you can access @c deviceInfo to fetch upgrade related information of the device.
 *
 * If the remote device to be upgraded is a member of RWS bud pair, @c RTKDFUUpgrade will automatically discover and upgrade the companion device if the companion device is online (engaged). You call @c -upgradeWithImages: to upgrade a single device, call @c -upgradeWithImagesForPrimaryBud:imagesForSecondaryBud: to upgrade a RWS pair devices. The @c images or @c primaryImages and @c secondaryImages should be appropriate for device, @c RTKDFUUpgrade may complain of image mismatch, or old images. You can also call @c -upgradeWithBinaryFileAtPath: with a image file path, @c RTKDFUUpgrade will parse and extract available images for upgrade.
 *
 * For device that support bank switch feature, @c RTKDFUUpgrade may select a subset of passed images applicable for next upgrade. You can call APIs of @c RTKOTADeviceInfo object to know this information.
 *
 * @c RTKDFUUpgrade will report upgrade progress and event by call @c RTKDFUUpgradeDelegate methods on your delegate while in upgrading. @c RTKDFUUpgrade conforms to @c NSProgressReporting protocol to return a overall progress object.
 *
 * Everytime upgrade complete successfully or unsuccessfully, @c RTKDFUUpgrade report by calling your delegate  @c -DFUUpgrade:didFinishUpgradeWithError: . When @c RTKDFUUpgrade object is upgrading in progress, you can call @c -cancelUpgrade to request cancel upgrade, your delegate @c -DFUUpgrade:didFinishUpgradeWithError: get called when canceling complete.
 *
 * You can create multiple @c RTKDFUUpgrade instances for different device, and upgrade them concurrently.
 */
@interface RTKDFUUpgrade : NSObject <NSProgressReporting>

/**
 * Initializes @c RTKDFUUpgrade object with a GATT peripheral.
 *
 * @param peripheral The device that is about to be upgraded.
 * @discussion Use this initializer for GATT device (represendted by @c CBPeripheral instance). The returned object is actually of @c RTKDFUUpgradeGATT type.
 */
- (instancetype)initWithPeripheral:(CBPeripheral *)peripheral;

/**
 * Initialize @c RTKDFUUpgrade object with a iAP device.
 *
 * @param accessory The device that is about to be upgraded.
 * @discussion Use this initializer for iAP device (represendted by @c EAAccessory instance). The returned object is actually of @c RTKDFUUpgradeIAP type.
 */
- (instancetype)initWithAccessory:(EAAccessory *)accessory;

/**
 * Initialize @c RTKDFUUpgrade object with a iAP device and existed message communication.
 *
 * @param accessory The device that is about to be upgraded.
 * @param msgTransport A already created message communication object.
 * @discussion Use this initializer for iAP device (represendted by @c EAAccessory instance). The returned object is actually of @c RTKDFUUpgradeIAP type.
 */
- (instancetype)initWithAccessory:(EAAccessory *)accessory existedMessageTransport:(RTKPacketTransport *)msgTransport;

/**
 * Initialize @c RTKDFUUpgrade object with a general device connection object and profile manager.
 *
 * @param connection The DFU profile connection which associate with device is about to be upgrade. Should be instance of @c RTKDFUConnectionUponGATT or @c RTKDFUConnectionUponiAP .
 * @param manager The manager object which manage the profile connection with remote device.
 * @discussion The returned object type is based on which connection class is used.
 */
- (instancetype)initWithDeviceConnection:(RTKProfileConnection *)connection profileManager:(RTKProfileConnectionManager *)manager;


/**
 * Return the profile connection object associated with the device which this @c RTKDFUUpgrade will upgrade.
 *
 * @discussion The returned object may be of @c RTKDFUConnectionUponGATT class or @c RTKDFUConnectionUponiAP class. You can make request manually with device by call methods on this returned instance.
 *
 * This property return a nil value when the underlying @c CBCentralManager is not available. It's sure to return a non-nil value when the delegate @c -DFUUpgradeDidReadyForUpgrade: get called.
 */
@property (readonly, nullable) RTKProfileConnection *deviceConnection;

/**
 * The assigned delegate object to receive upgrade progress and result.
 *
 * @discussion DFU upgrade calls @c RTKDFUUpgradeDelegate declared methods on this delegate using main queue.
 */
@property (nonatomic, weak) id<RTKDFUUpgradeDelegate> delegate;


/**
 * Start preparation process for next upgrade.
 *
 * @discussion While @c RTKDFUUpgrade perform preparation, it may make app connected with remote device if not connected. It calls the @c -DFUUpgradeDidReadyForUpgrade: on delegate if preparation succeed,  or @c -DFUUpgrade:couldNotUpgradeWithError: if preparation fail.
 *
 * You should call this method before call any upgrade methods.
 */
- (void)prepareForUpgrade;


// MARK: - Access upgrade state

/**
 * Return the related information about the device to upgrade.
 *
 * @discussion Once your delegate @c -DFUUpgradeDidReadyForUpgrade: get called, you can access this property to fetch upgrade-related information.
 *
 * For a RWS pair of devices, this property return the information of the device that is passed to initializer. Once @c -DFUUpgrade:willUpgradeCompanionDevice:deviceInfo: method get called, you can fetch the information of companion device  by the @c deviceInfo parameter.
 */
@property (nonatomic, nullable, readonly) RTKOTADeviceInfo *deviceInfo;


/**
 * Return the image which is being send currently.
 *
 * @discussion Return nil if no image is being send currently.
 */
@property (readonly, weak, nullable) RTKOTAUpgradeBin *upgradingImage;

// MARK: - Upgrade setting

/// The default value is RTKOTAUpgradeMode_default
@property RTKOTAUpgradeMode upgradeMode;

/**
 * Set a new key for encrypt the image bytes.
 *
 * @param encryptionKey The  new key used for bytes encryption. Should be of 32 byte in length.
 * @discussion If encyption is supported by the remote device which can be determined by @c RTKOTADeviceInfo.AESEnable , @c RTKDFUUpgrade will encrypt image bytes before send to device. The used key should be equal with device.  A default key is used if not set a new one. You should set this property value only when upgrade is not in progress.
 */
- (void)setEncryptionKey:(NSData * _Nonnull)encryptionKey;


/**
 * Set a new retry number when buffer check failure.
 *
 * @discussion When the device use "Buffer Check" manner to receive image bytes, @c RTKDFUUpgrade will retry sending the same bytes if previous buffer check failed, until the @c retryCountWhenBufferCheckFail reached. You should set this property value only when upgrade is not in progress.
 * The default value is 2.
 */
@property (nonatomic) NSUInteger retryCountWhenBufferCheckFail;

/**
 * A boolean value that control when upgrade completion method get called.
 *
 * @discussion Deivce usually reboot once be upgraded successfully or be reset, resulting in a disconnection. This property direct @c RTKDFUUpgrade whether to wait for disconnection when upgrade device successfully or reset, before call @c -DFUUpgrade:didFinishUpgradeWithError: on delegate. You should set this property value only when upgrade is not in progress.
 * The default value is YES.
 */
@property BOOL shouldWaitForDisconnectionBeforeReportCompletion;


/**
 * A boolean value that indicates if older version image is allowed for upgrade.
 *
 * @disucssion You should set this property value only when upgrade is not in progress. The default value is NO. (You could choose whether or not to use a strict check mechanism)
 */
@property BOOL olderImageAllowed;

/**
 * A boolean value that indicates whether all upgrade images should be newer than current images.
 *
 * @disucssion You should set this property value only when upgrade is not in progress. The default value is NO. （If @c olderImageAllowed is NO, @c usingStrictImageCheckMechanism is NO , it will compares image versions based on image priorities）
 */
@property BOOL usingStrictImageCheckMechanism;


/**
 * Set the minimum battery level for upgrade.
 *
 * @discussion When upgrading, the device should have battery level great than or equal to batteryLevelLimit could be upgraded. The default value is 30. The value @c RTKDFUBatteryLevelInvalid means not checking for battery level.
 */
@property RTKDFUBatteryLevel batteryLevelLimit;


// MARK: - Upgrade control
/**
 * A boolean value that indicating whether a upgrade task is in progress.
 */
@property (readonly) BOOL upgradeInProgress;

/**
 * Start upgrage with specified images.
 *
 * @param images An array that contain images available for upgrade. The images may contain images applying to Bank 0 and Bank 1 when the device support "Dual Bank".
 *
 * @discussion You should call @c -prepareForUpgrade and wait for the @c -DFUUpgradeDidReadyForUpgrade called on the delegate, before call this method.
 * This method returns immediately without waiting for tasks to finish. While upgrading, @c RTKDFUUpgrade call @c -DFUUpgrade:withDevice:didSendBytesCount:ofImage: on delegate to report progress and call @c -DFUUpgrade:didFinishUpgradeWithError: on delegate to report completion.
 *
 * For devices that support "Dual Bank" feature, only images applying to the current unused bank will be send to device. Which means, you can containing all images applying to both Bank 0 and Bank 1 in the @c images parameter, the actual available images is selected from all images and be send to device.
 *
 * @c RTKDFUUpgrade will verify applicability of @c images parameter. If the @c images contain any image not match the remote device, or if @c images is old, @c RTKDFUUpgrade will call @c -DFUUpgrade:didFinishUpgradeWithError: with a non-nil error on delegate.
 *
 * You should not call this method to upgrade a pair of RWS devices.
 */
- (void)upgradeWithImages:(NSArray <RTKOTAUpgradeBin*> *)images;

/**
 * Start upgrage with specified images for primary and secondary device.
 *
 * @param imagesForPrimary An array containing images available for upgrade to primary device. The images may contain images applying to Bank 0 and Bank 1 when the device support "Dual Bank".
 * @param imagesForSecondary An array containing images available for upgrade to secondary device. The images may contain images applying to Bank 0 and Bank 1 when the device support "Dual Bank".
 *
 * @discussion You should call @c -prepareForUpgrade and wait for the @c -DFUUpgradeDidReadyForUpgrade called on the delegate, before call this method.
 * This method returns immediately without waiting for tasks to finish. While upgrading, @c RTKDFUUpgrade call @c -DFUUpgrade:withDevice:didSendBytesCount:ofImage: on delegate to report progress and call @c -DFUUpgrade:didFinishUpgradeWithError: on delegate to report completion. @c RTKDFUUpgrade will call @c -DFUUpgradeDidFinishFirstDeviceAndPrepareCompanionDevice: on delegate once it complete upgrade first device and call @c -DFUUpgrade:willUpgradeCompanionDevice:deviceInfo: on delegate when it is just about to upgrade the secondary device.
 *
 * For devices that support "Dual Bank" feature, only images applying to the current unused bank will be send to device. Which means, you can containing all images applying to both Bank 0 and Bank 1 in the @c primaryImages and @c secondaryImages parameter, the actual available images is selected from all images and be send to device.
 *
 * @c RTKDFUUpgrade will verify applicability of @c primaryImages and @c secondaryImages parameter. If the  @c primaryImages and @c secondaryImages contain any image not match the remote device, or if  @c primaryImages and @c secondaryImages is old, @c RTKDFUUpgrade will call @c -DFUUpgrade:didFinishUpgradeWithError: with a non-nil error on delegate.
 *
 * You should call this method only to upgrade a RWS pair of devices.
 */
- (void)upgradeWithImagesForPrimaryBud:(NSArray <RTKOTAUpgradeBin*> *)imagesForPrimary imagesForSecondaryBud:(NSArray <RTKOTAUpgradeBin*> *)imagesForSecondary;


/**
 * Start upgrage using images extracted from the specifed file.
 *
 * @param path The path to a file which can read and extract image objects for upgrade.
 *
 * @discussion The file specified by @c path should be a valid MP binary format or MP Pack format. Images extracted from file should be applicable for device, which means, you can pass a file that containing images for single device when upgrade a single device, pass a file that containing primary and secondary images for RWS pair when upgrade a pair of RWS devices.
 *
 * Before call this method, you should call @c -prepareForUpgrade and wait for the @c -DFUUpgradeDidReadyForUpgrade called on the delegate.
 * Just like @c -upgradeWithImages: and @c -upgradeWithImagesForPrimaryBud:imagesForSecondaryBud:, @c RTKDFUUpgrade call methods on delegate to report progress and result.
 */
- (void)upgradeWithBinaryFileAtPath:(NSString *)path;


/**
 * Cancel an outstanding upgrade task.
 *
 * @discussion When there is a outstanding upgrade task, you can call this method to cancel upgrade. This method returns immediately, marking the task as being canceled. Once a task is marked as being canceled, @c -DFUUpgrade:didFinishUpgradeWithError: will be sent to the task delegate, passing an error in the domain @c RTKOTADomain with the code @c RTKOTAErrorUserCancelled.
 *
 * You should not call this method if there is no outstanding upgrade task.
 */
- (void)cancelUpgrade;

@end


/**
 * A concrete subclass of @c RTKDFUUpgrade which applying for GATT profile connected device.
 *
 * @discussion This class override @c deviceConnection and return a @c RTKDFUConnectionUponGATT instance.
 */
@interface RTKDFUUpgradeGATT : RTKDFUUpgrade

/**
 * A boolean value that represents whether user prefers to upgrade using OTA mode.
 *
 * @discussion When the connected device both support upgrading silently and switching to OTA mode, this value determines which method to be used for upgrade. This value is of no effect when device support either method.
 *
 * The default value is @c NO.
 */
@property BOOL prefersUpgradeUsingOTAMode;

@end


/**
 * A concrete @c RTKDFUUpgrade subclass  which applying for iAP profile connected device.
 *
 * @discussion This class override @c deviceConnection and return an @c RTKDFUConnectionUponiAP instance.
 */
@interface RTKDFUUpgradeIAP : RTKDFUUpgrade
@end

NS_ASSUME_NONNULL_END
