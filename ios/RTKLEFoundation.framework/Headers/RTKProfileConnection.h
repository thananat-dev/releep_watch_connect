//
//  RTKProfileConnection.h
//  RTKLEFoundation
//
//  Created by jerome_gu on 2021/10/18.
//  Copyright © 2022 Realtek. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ExternalAccessory/ExternalAccessory.h>
#import <CoreBluetooth/CoreBluetooth.h>

#ifdef RTK_SDK_IS_STATIC_LIBRARY
#import "RTKBTGeneralDefines.h"
#else
#import <RTKLEFoundation/RTKBTGeneralDefines.h>
#endif

NS_ASSUME_NONNULL_BEGIN

typedef enum : NSUInteger {
    RTKProfileConnectionStatusInactive,
    RTKProfileConnectionStatusActivating,
    RTKProfileConnectionStatusActive,
    RTKProfileConnectionStatusDeactivating,
} RTKProfileConnectionStatus;


@class RTKProfileConnection;

@protocol RTKProfileConnectionPerformer <NSObject>

- (void)requestConnectDeviceOf:(RTKProfileConnection *)connection
                   withTimeout:(NSTimeInterval)interval
             completionHandler:(nullable RTKLECompletionBlock)handler;

- (void)requestDisconnectDeviceOf:(RTKProfileConnection *)connection
                      withTimeout:(NSTimeInterval)interval
                completionHandler:(nullable RTKLECompletionBlock)handler;

@end


/**
 * A protocol that defines an optional method for receiving notifications when the profile connection open state changes.
 */
@protocol RTKProfileConnectionDelegate <NSObject>
@optional

/**
 * Tells the delegate object that the connection state of this connection is changed.
 */
- (void)profileConnection:(RTKProfileConnection *)connection deviceDidBeDisconnected:(nullable NSError *)error;

/**
 * Tells the delegate object that the connection did receive new message.
 */
- (void)profileConnection:(RTKProfileConnection *)connection didReceiveMessage:(NSData *)msgData;

@end


// Which term is better, Connection or Session or Communication ?
/**
 * The abstract base class that represents a application profile connection to a remote device which manage communication with it.
 *
 * @discussion A little different from meaning of a CBPeripheral connection, this class instance does not indicate a live connected state with a remote device. It represents a relationship between local app and remote device in the perspective of a application profile, whether the device is now connected to this phone or not.
 * Typically, a RTKProfileConnection subclass object has knowledge of a paricular profile and implements the profile client role. You use RTKProfileConnection (or subclass) objects for communicating with a connected device using particular profile protocol, such as obtaining device state, initiates device actions.
 *
 * The communication may takes place upon GATT profile which you uses @c RTKConnectionUponGATT , or upon iAP profile which you uses @c RTKConnectionUponiAP . Call @c +connectionWithPeripheral: to create a connection object which manages connection with a GATT peripheral. Call @c +connectionWithAccessory: to create a connection object which manages connection with a iAP accessory.
 *
 * You call @c -openWithComletionHandler: to make a connection to be open. A connection object should be open before receives any methods that making a real application communication. The device should be connected before begins open. When open method is called, the connection first verify profile compliance of the connected device.
 *
 * You can call @c -sendMessageData:withCompletionHandler: to send a custom data to remote device if the profile supports.
 *
 *  TODO: override notes
 */
@interface RTKProfileConnection : NSObject

/**
 * Creates and returns a connection object that links with a specified GATT peripheral.
 *
 * @discussion The peripheral does not need to be now connected. Should be override.
 */
+ (instancetype)connectionWithPeripheral:(CBPeripheral *)peripheral;

/**
 * Creates and returns a connection object that links with a known iAP accessory.
 *
 * @discussion The accessory does not need to be now connected. Should be override.
 */
+ (instancetype)connectionWithAccessory:(EAAccessory *)accessory;


/**
 * Returns the name of the linked remote device.
 */
@property (readonly, nullable) NSString *deviceName;

/**
 * Returns the remote device this connection object links.
 *
 * @discussion This property may be of CBPeripheral class or EAAccessory class.
 */
@property (readonly) id device;

/**
 * Returns a boolean value that indicates whether a connection is estabilished with remote device.
 */
@property (readonly) BOOL deviceIsConnected;


@property (weak, nullable) id<RTKProfileConnectionPerformer> connectionPerformer;

/**
 * The delegate object that listens for change notification of profile connection open state.
 *
 * @discussion Subclass may notifies more events through this property.
 */
@property (weak, nullable, nonatomic) id<RTKProfileConnectionDelegate> delegate;

/**
 * Indicates whether the device connection is ready for interact with.
 *
 * @discussion If you call method to initiates a communication while this property is NO, this call fails. If this method receive a completion handler, the handler is called to report failure.
 *
 * This property is KVO-applicable.
 */
@property (readonly) RTKProfileConnectionStatus status;


#define RTKDistantInterval 31536000.

/**
 * @discussion The default value is 10 seconds. You should not set this property while this connection is in period of activating.
 */
@property NSTimeInterval connectWaitInterval;

/**
 * Open the connection for make further communication.
 *
 * @param handler A nullable block that will be called when open completion successfully or unsuccessfully.
 * @discussion A connection manager calls this method as a substep when activating a profile connection.
 *
 * If the connection is already open, the handler block is called immediately.
 */
- (void)activateWithCompletionHandler:(nullable RTKLECompletionBlock)handler;

/**
 * Close this device connection to stop interacting with it.
 *
 * @param handler A nullable block that will be called when close completion successfully or unsuccessfully.
 * @discussion A connection manager may calls this method as a substep when activate a device connection.
 *
 * If a connection is already closed, the handler block is called immediately.
 */
- (void)deactivateWithCompletionHandler:(nullable RTKLECompletionBlock)handler;


/**
 * Send a custom data to the connected remote device.
 *
 * @param data The data to be send.
 * @param handler The block to be called when message send successfully or unsuccessfully.
 * @discussion The method is available only if a subclass object supports sending custom data, if not, the handler block is called with error code set to @c NotSupport .
 */
- (void)sendMessageData:(NSData *)data withCompletionHandler:(nullable void(^)(BOOL success, NSError *_Nullable error))handler;

@end

@interface RTKProfileConnection (Protect)

/**
 * Be notified that the device is disconnected.
 *
 * @discussion This is a override pointer for subclass to responds to connection change. Don't call this method directly. If you override this method in subclass, you need to call this method on super class.
 */
- (void)onDeviceDidDisconnectWithError:(nullable NSError *)error;

@end

NS_ASSUME_NONNULL_END
