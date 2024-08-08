//
//  RTKDFURoutine.h
//  RTKOTASDK
//
//  Created by jerome_gu on 2020/3/9.
//  Copyright © 2022 Realtek. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef RTK_SDK_IS_STATIC_LIBRARY
#import "RTKOTADeviceInfo.h"
#else
#import <RTKOTASDK/RTKOTADeviceInfo.h>
#endif


typedef uint16_t RTKImageId;        ///< An integer value that represents image identifier.
typedef uint32_t RTKImageVersion;   ///< An integer value that represents image version number.
typedef uint32_t RTKImageKey;

/// Represents an invalid @c RTKImageVersion value.
#define RTKImageVersionInvalid  0xffffffff

/**
 * Constants that represents upgrade method.
 */
typedef enum : uint8_t {
    RTKOTAUpgradeMode_default =   0x00,       ///< Using normal upgrade method
    RTKOTAUpgradeMode_OTATempSection,         ///< Use OTA temp section for upgrade
    RTKOTAUpgradeMode_updateVPData,           ///< Upgrade Voice Prompt data
} RTKOTAUpgradeMode;


/**
 * Constants that determine which bank a image exist.
 */
typedef enum : uint8_t {
    RTKOTABinBankExistState_notExist,           ///< Binary not exist at device
    RTKOTABinBankExistState_existAtFreeBank,    ///< Binary exist at free bank
    RTKOTABinBankExistState_existAtActiveBank,  ///< Binary exist at active bank
    RTKOTABinBankExistState_exist,              ///< Binary exist at both bank
} RTKOTABinBankExistState;

/**
 * Constants that indicate result of a buffer check request.
 */
typedef enum : uint8_t {
    RTKOTAImageBufferCheckResult_invalid,       ///< Buffer check result unkown, this is a invalid value.
    RTKOTAImageBufferCheckResult_success,       ///< Buffer check passed
    RTKOTAImageBufferCheckResult_crcError,      ///< Buffer check failed for CRC check failure.
    RTKOTAImageBufferCheckResult_lengthError,   ///< Buffer check failed for length error.
    RTKOTAImageBufferCheckResult_otherError,    ///< Buffer check failed for other errors.
} RTKOTAImageBufferCheckResult;

/**
 * Constants that indicate result of a validate image request.
 */
typedef enum : uint8_t {
    RTKOTAImageValidateResult_invalid,          ///< Image validate result invalid.
    RTKOTAImageValidateResult_success,          ///< Image validation result is success.
    RTKOTAImageValidateResult_checkFail,        ///< Image validation failed for image check failure.
    RTKOTAImageValidateResult_otherFail,        ///< Image validation failed for other errors.
} RTKOTAImageValidateResult;


/**
 * Structure containing a image id and check value.
 */
typedef struct __attribute__((packed)) {
    RTKImageId imageId;
    uint8_t checkBytes[32];
} RTKImageInfo_t;

/**
 * Structure containing a image id and version value.
 */
typedef struct __attribute__((packed)) {
    RTKImageId imageId;
    RTKImageVersion version;
} RTKImageVersionInfo_t;

/**
 * Structure containing a image id and key hash value.
 */
typedef struct __attribute__((packed)) {
    RTKImageId imageId;
    RTKImageKey key;
} RTKImageKeyInfo_t;

/**
 * A unsigned integer value represents battery charge level of one device.
 *
 * @discussion The value ranges from 0 to 100. Besides, @c RTKDFUBatteryLevelInvalid is a special value which indicates level value is not valid.
 */
typedef uint8_t RTKDFUBatteryLevel;

/// A constant value describes battery level is not vald.
extern const RTKDFUBatteryLevel RTKDFUBatteryLevelInvalid;


NS_ASSUME_NONNULL_BEGIN

/**
 * An protocol that defines methods to retrieve informations about upgrade of a device and send images to upgrade a device.
 *
 * @discussion This protocol defines common methods for upgrade, @c RTKDFUConnectionUponGATT and @c RTKDFUConnectionUponiAP class conform to it and implement all those methods. Those methods are expected to be used internal in SDK.
 *
 * All those methods execute asynchronously and receive a block parameter as completion handler to be called when request is complete. The completion handler takes at least two parameters, @c success indicates whether this request is successful or fail, @c error indicates why the request failed.
 *
 * You can pass nil to completion handler if you dont care about completion.
 */
@protocol RTKDFURoutine <NSObject>

#pragma mark - Information retrieve

/**
 * Request to retrieve upgrade related information of the connected device.
 *
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion If the request completes successfully, the info parameter of the completion handler block contains the information about upgrade of the connected device.
 */
- (void)getOTAInformationWithCompletionHandler:(nullable void(^)(BOOL success, NSError *__nullable error, RTKOTABaseDeviceInfo *__nullable info))handler;

/**
 * Request to retrieve the protocol number of the connected device.
 *
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion If the request completes successfully, the protocol parameter of the completion handler block indicates the protocol number of the connected device.
 */
- (void)getProtocolTypeWithCompletionHandler:(nullable void(^)(BOOL success, NSError *__nullable error, RTKOTAProtocolType protocol))handler;

/**
 * Request to retrieve bluetooth device address of the connected device and companion device.
 *
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion If the request completes successfully, the addr parameter of the completion handler block indicates the BDAddress of connected device, the companionAddr parameter indicates the BDAddress of the companion device if it has one.
 */
- (void)getBDAddressWithCompletionHandler:(nullable void(^)(BOOL success, NSError *error, BDAddressType addr, BDAddressType companionAddr))handler;


/**
 * Request device to get battery charge level.
 *
 * @param handler The completion handler to call when the request is complete.
 *
 */
- (void)getBatteryLevelWithCompletionHandler:(nullable void(^)(BOOL success, NSError *error, RTKDFUBatteryLevel battery, RTKDFUBatteryLevel secondaryBattery))handler;

// TODO: describe the relationship of -getImageVersionsWithCompletionHandler: and -getImageVersionsOfActiveBank:withCompletionHandler:
/**
 * Request to retrieve image versions installed on the connected device.
 *
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion If the request completes successfully, the count parameter of the completion handler block indicates count of @c infos array, the @c infos is a pointer to an array containing image version informations.
 */
- (void)getImageVersionsWithCompletionHandler:(nullable void(^)(BOOL success, NSError *error, NSUInteger count, RTKImageVersionInfo_t infos[_Nullable]))handler;

/**
 * Request to retrieve image versions installed on the connected device.
 *
 * @param isActiveBank A boolean value indicates if images is current active.
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion If the request completes successfully, the count parameter of the completion handler block indicates count of @c infos array, the @c infos is a pointer to an array containing image version informations.
 */
- (void)getImageVersionsOfActiveBank:(BOOL)isActiveBank withCompletionHandler:(nullable void(^)(BOOL success, NSError *error, NSUInteger count, RTKImageVersionInfo_t infos[_Nullable]))handler;


#pragma mark - DFU procedure operations

/**
 * Request to retrieve the upgrade state of a given image on the connected device.
 *
 * @param imageId The identifier of a image.
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion If the request completes successfully, the @c FWVersion parameter of the completion handler block indicates firmware version, the @c totalOffset indicates how much bytes did received by device, the @c bufferOffset indicates how much bytes did received on a buffer.
 */
- (void)getImageStateWithId:(RTKImageId)imageId withCompletionHandler:(nullable void(^)(BOOL success, NSError *__nullable error, NSUInteger FWVersion, NSUInteger totalOffset, NSUInteger bufferOffset))handler;

/**
 * Request the connected device to enable Buffer Check functionality for upgrade images.
 *
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion If the request completes successfully, the @c maxBufferSize parameter of the completion handler block indicates the size of the Buffer, the @c MTU indicates the maximum transmission size for send image bytes.
 */
- (void)enableBufferCheckWithCompletionHandler:(nullable void(^)(BOOL success, NSError *__nullable error, NSUInteger maxBufferSize, NSUInteger MTU))handler;

/**
 * Request to upgrade a specific image to connected device.
 *
 * @param headerData The header bytes of the specific image to send.
 * @param mode The mode to used for upgarde.
 * @param encrypt Whether encrypt parameter data.
 * @param key The key for encryption.
 * @param handler The completion handler to call when the request complete.
 */
- (void)startUpgradeWithImageHeaderData:(NSData *)headerData
                          upgradeMode:(RTKOTAUpgradeMode)mode
                       encryptParameter:(BOOL)encrypt
                          encryptionKey:(nullable NSData *)key
                  completionHandler:(nullable RTKLECompletionBlock)handler;

/**
 * Notify device to receive image bytes.
 *
 * @param imageId The image which is about to be send.
 * @param len The size of this image already sended.
 * @param handler The completion handler to call when the request is complete.
 */
- (void)beginReceiveImage:(RTKImageId)imageId withSendedLength:(NSUInteger)len completionHandler:(nullable RTKLECompletionBlock)handler;

/// Returns the maximum length in byte for sending image bytes.
@property (readonly) NSUInteger maximumImageSliceSendSize;

/**
 * Send a fragment of image bytes to connected device.
 *
 * @param dataSlice The bytes fragment which is about to be send. This data should have length not larger than maximumImageSliceSendSize.
 * @param handler The completion handler to call when the request is complete.
 */
- (void)sendImageSlice:(NSData *)dataSlice withCompletionHandler:(nullable RTKLECompletionBlock)handler;


/**
 * Check the sended data in current buffer.
 *
 * @param bufferData The bytes in current buffer to be check.
 * @param handler The completion handler to call when the request is complete.
 */
- (void)checkBufferCRCOf:(NSData *)bufferData withCompletionHandler:(nullable void(^)(BOOL success, NSError *__nullable error, RTKOTAImageBufferCheckResult result))handler;

/**
 * Validate the image just sended.
 *
 * @param imageId The image which is validated.
 * @param interval The time out interval when wait for this method completion.
 * @param handler The completion handler to call when the request is complete.
 */
- (void)validateImage:(RTKImageId)imageId timeoutInterval:(NSTimeInterval)interval withCompletionHandler:(nullable void(^)(BOOL success, NSError *__nullable error, RTKOTAImageValidateResult result))handler;

/**
 * Send a fragment of image bytes to connected device.
 *
 * @param imageId The image which is validated.
 * @param isLast A boolean value indicates if the last send image is the last one.
 * @param interval The time out interval when wait for this method completion.
 * @param handler The completion handler to call when the request is complete.
 */
- (void)validateImage:(RTKImageId)imageId isLastImage:(BOOL)isLast timeoutInterval:(NSTimeInterval)interval withCompletionHandler:(nullable void(^)(BOOL success, NSError *__nullable error, RTKOTAImageValidateResult result))handler;

/**
 * Activate sended images and make device reboot.
 *
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion Typically, the device will reboot and make current connection disconnected.
 */
- (void)activateImagesAndResetWithCompletionHandler:(nullable RTKLECompletionBlock)handler;

/**
 * Stop upgrade procedure.
 *
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion The connected device may reboot to make overal reset.
 */
- (void)stopDFUAndResetWithCompletionHandler:(nullable RTKLECompletionBlock)handler;

/**
 * Request to retrieve existence state of given images on the connected device.
 *
 * @param count The count of the @c images array.
 * @param images A array containing image identifier and check value.
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion This method may fail if remote device does not support this request.
 */
- (void)getExistenceOfImagesCount:(NSUInteger)count imageInfos:(RTKImageInfo_t *)images withCompletionHandler:(nullable void(^)(BOOL success, NSError *error, RTKOTABinBankExistState stateArr[_Nullable]))handler;

/**
 * Request to copy the specific image to free bank on the connected device.
 *
 * @param imageId The identifier of a image.
 * @param handler The completion handler to call when the request is complete.
 *
 * @discussion You use this method only when connected device supports "Dual Bank" feature.
 */
- (void)copyExistImageOfId:(RTKImageId)imageId toFreeBankWithCompletionHandler:(nullable void(^)(BOOL success, NSError *err))handler;

@end

NS_ASSUME_NONNULL_END
