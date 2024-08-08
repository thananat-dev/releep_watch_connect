//
//  RTKProfileConnectionManager.h
//  RTKBTFoundation
//
//  Created by jerome_gu on 2021/10/18.
//  Copyright © 2022 Realtek. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreBluetooth/CoreBluetooth.h>
#import <ExternalAccessory/ExternalAccessory.h>

#ifdef RTK_SDK_IS_STATIC_LIBRARY
#import "RTKProfileConnection.h"
#else
#import <RTKLEFoundation/RTKProfileConnection.h>
#endif

NS_ASSUME_NONNULL_BEGIN

@class RTKProfileConnectionManager;

/**
 * A protocol defines methods that a delegate of a @c RTKProfileConnectionManager object must adopt to receive connection related events.
 */
@protocol RTKProfileConnectionManagerDelegate <NSObject>
@required
/**
 * Tells the delegate that GATT service availablity state did update.
 *
 * @param manager The manager that reports this event.
 * @discussion When this method get called, whether GATT service is available can be determined by access @c GATTAvailable property of the @c RTKProfileConnectionManager object.
 */
- (void)profileManagerDidUpdateGATTAvailability:(RTKProfileConnectionManager *)manager;

@optional
/**
 * Tells the delegate that a connection with a qualified device that is connected just now.
 *
 * @param manager The manager that reports this event.
 * @param connection The connection that a device is connected with manager.
 * @discussion This method will get called, if the manager is listening connection events, which is started by call @c -beginListeningForConnectionWithAccessory or @c -beginListeningForConnectionWithPeripherals . If a qualified GATT peripheral is connected, the connection is of @c RTKConnectionUponGATT class type. If a qualified iAP accessory is connected, the connection is of @c RTKConnectionUponiAP class type.
 */
- (void)profileManager:(RTKProfileConnectionManager *)manager
didDetectNewConnectedConnection:(RTKProfileConnection *)connection;


/**
 * Tells the delegate that a connection with a qualified device that is discovered just now while scanning.
 *
 * @param manager The manager that reports this event.
 * @param connection The connection that a device is connected with manager.
 * @discussion While the manager is scanning, it calls this method to report discovered device.
 */
- (void)profileManager:(RTKProfileConnectionManager *)manager
didDiscoverPeripheralOfConnection:(RTKProfileConnection *)connection
     advertisementData:(NSDictionary<NSString *, id> *)advertisementData
                  RSSI:(NSNumber *)RSSI;

@end


/**
 * @class RTKProfileConnectionManager
 *
 * A generic object that you use to scan, detect, activate connections that is linked with interested remote devices.
 *
 * @discussion @c RTKProfileConnectionManager objects create connection objects (represented by @c RTKProfileConnection objects) and report to you when it did discover GATT devices or detect new iAP connected devices if those deivce is interested by this class.
 * If you already have a @c EAAccessory instance or @c CBPeripheral instance, call @c -instantiateConnectionWithPeripheral: and @c -instantiateConnectionWithAccessory: respectively to obtain a connection object.
 * You must retain the device connection objects with which you want to interact with further.
 *
 * To communicate with remote device with which a connection object connect, call @c -activateProfileConnection: methods and wait till connection is active.
 *
 * Before call any methods to create or communicate with GATT devices, GATT shall be available, as indicated by @c GATTAvailable property .
 *
 * You typically use @c RTKProfileConnectionManager for a specific Profile usage scenario, which means that you actually use a @c RTKProfileConnectionManager subclass. When subclassing, you should override @c +concreteConnectionClassForGATTPeripheral to return a concrete @c RTKConnectionUponGATT subclass and @c +concreteConnectionClassForIAPAccessory: to return a concrete @c RTKConnectionUponiAP subclass.
 *
 * When the manager discovers new peripherals, detects new connections or finish activate connections, it calls methods defined by @c RTKProfileConnectionManagerDelegate on its delegate object.
 */
@interface RTKProfileConnectionManager : NSObject <CBCentralManagerDelegate>

/**
 * Initializes the profile manager with a specified delegate.
 *
 * @param delegate The delegate that receives connection-related events.
 * @discussion After this method returns, @c -profileManagerDidUpdateGATTAvailability: may be called on its delegate as the underlying central manager is power on.
 */
- (instancetype)initWithDelegate:(nullable id <RTKProfileConnectionManagerDelegate>)delegate NS_DESIGNATED_INITIALIZER;

/**
 * The delegate that will receive connection discovery and other events.
 */
@property (weak) id <RTKProfileConnectionManagerDelegate> delegate;

/**
 * Returns a central manager which used internal.
 *
 * @discussion Protected used in subclass.
 */
@property (readonly) CBCentralManager *centralManager;


/**
 * Returns a boolean value that indicates if GATT service is available.
 *
 * @discussion You call methods to interact with GATT devices only when this property value is YES.
 */
@property (readonly) BOOL GATTAvailable;


#pragma mark - Register class for connection
/**
 * The class type used to create a @c RTKDeviceConnection instances for GATT peripherals.
 *
 * @discussion The default class is RTKConnectionUponGATT.
 */
- (void)registerConnectionClassForInstantiateGATTPeripheral:(Class)connectionClass;

/**
 * The class type used to create a @c RTKDeviceConnection instances for iAP accessories.
 *
 * @discussion The default class is RTKConnectionUponiAP.
 */
- (void)registerConnectionClassForInstantiateiAPAccessory:(Class)connectionClass;


#pragma mark - LE Perpheral Scanning

/**
 * Indicates whether the manager is currently scanning.
 */
@property (readonly) BOOL isScaning;

/**
 * Scans for nearby LE GATT peripherals.
 *
 * @discussion When the manager discovers a LE GATT device with requirements meet, it call @c -profileManager:didDiscoverConnection: on its delegate.
 * The @c GATTAvailable should be YES when call this method.
 */
- (void)scanForPeripherals;

/**
 * Scans for nearby LE GATT peripherals without duplicate filtering.
 *
 * @discussion The profile manager report discovery events each time it receives an advertising packet from the peripheral. Use this method only if necessary.
 * The @c GATTAvailable should be YES when call this method.
 */
- (void)scanForPeripheralsAndReportRepeatedly;

/**
 * Stop scaning for LE GATT peripherals.
 *
 * @discussion The @c GATTAvailable should be YES when call this method.
 */
- (void)stopScan;


#pragma mark - New connected connection detect

/**
 * Begins to receive notifications of connection or disconnection events of qualified iAP accessories.
 *
 * @discussion The profile manager calls @c -profileManager:didDetectConnectedConnection: if a qualified accessory is connected, calls @c -profileManager:didDetectDisconnectedConnection: if a qualified accessory is diconnected on delegate object.
 */
- (void)startMonitoringNewConnectionWithAccessory;

/**
 * Stops receiving notifications of connection or disconnection events of qualified iAP accessories.
 */
- (void)stopMonitoringNewConnectionWithAccessory;

/**
 * Begins to receive notifications of connection or disconnection events of qualified GATT peripherals.
 *
 * @discussion The profile manager calls @c -profileManager:didDetectConnectedConnection: on delegate object if a qualified GATT peripheral is connected, calls @c -profileManager:didDetectDisconnectedConnection: on delegate object if a qualified GATT peripheral is diconnected.
 *
 * The @c GATTAvailable should be YES when call this method.
 */
- (void)startMonitoringNewConnectionWithPeripheral;

/**
 * Stops receiving notifications of connection or disconnection events of qualified GATT peripherals.
 *
 * @discussion The @c GATTAvailable should be YES when call this method.
 */
- (void)stopMonitoringNewConnectionWithPeripheral;



#pragma mark - Connection retrieve

/**
 * Returns an array of the device connection objects each represents connection with qualified GATT peripheral that is connected to the system.
 *
 * @discussion The @c GATTAvailable should be YES when call this method.
 */
- (NSArray <RTKProfileConnection*> *)retrieveConnectedPeripheralConnections;


/**
 * Returns an array of the device connection objects each represents connection with qualified iAP accessory that is connected to the system.
 */
- (NSArray <RTKProfileConnection*> *)retrieveConnectedAccessoryConnections;


#pragma mark - Instantiate connection

/**
 * Creates and returns a connection object which represents connection with the specific GATT peripheral.
 *
 * @param peripheral A CBPeripheral instance created somewhere else.
 * @return Returns a device connection that is managed by this manager. Returns nil if the device connection cannot be created.
 * @discussion The return peripheral object has class of @c RTKConnectionUponGATT or subclass.
 * The @c GATTAvailable should be YES when call this method.
 */
- (nullable RTKProfileConnection *)instantiateConnectionWithPeripheral:(CBPeripheral *)peripheral;

/**
 * Creates and returns a connection object which represents connection with the specific iAP accessory.
 *
 * @param accessory A EAAccessory instance created somewhere else.
 * @return Returns a device connection that is managed by this manager. Returns nil if the device connection cannot be created.
 * @discussion The return peripheral object has class of @c RTKConnectionUponiAP or subclass.
 */
- (nullable RTKProfileConnection *)instantiateConnectionWithAccessory:(EAAccessory *)accessory;



#pragma mark - Managed connection fetch

/**
 * Returns a boolean value that indicates whether the specific device connection is managed by this manager.
 */
- (BOOL)manageConnection:(RTKProfileConnection *)connection;

/**
 * Returns the @c RTKProfileConnection object which connected with a specific GATT peripheral.
 *
 * @return Returns nil if there is no profile connection with this peripheral that is managed by this manager.
 */
- (nullable RTKProfileConnection *)managedConnectionWithPeripheral:(CBPeripheral *)peripheral;

/**
 * Returns the @c RTKProfileConnection object which connected with a specific iAP accessory.
 *
 * @return Returns nil if there is no profile connection with this accessory that is managed by this manager.
 */
- (nullable RTKProfileConnection *)managedConnectionWithAccessory:(EAAccessory *)accessory;


// MARK: - Device connection state observation

/**
 * Waits for the device to be connected for up to the specified timeout.
 *
 * @param interval The amount of time within which device should be connected.
 * @param handler The block to be called when the device is connected within timeout, or timeout occurs.
 * @discussion Currently, this is only used for LE peripherals.
 */
- (void)waitForDeviceBeConnectedOf:(RTKProfileConnection *)connection
                           timeout:(NSTimeInterval)interval
                 completionHandler:(RTKLECompletionBlock)handler;

/**
 * Stops waiting for connection events if there is a outstanding connection waitting.
 */
- (void)cancelWaitForDeviceBeConnectedOf:(RTKProfileConnection *)connection;

/**
 * Waits for the device to be disconnected for up to the specified timeout.
 *
 * @param interval The amount of time within which device should be connected.
 * @param handler The block to be called when the device is disconnected within timeout, or timeout occurs.
 */
- (void)waitForDeviceBeDisconnectedOf:(RTKProfileConnection *)connection
                              timeout:(NSTimeInterval)interval
                    completionHandler:(RTKLECompletionBlock)handler;

/**
 * Stops waiting for connection events if there is a outstanding disconnection waitting.
 */
- (void)cancelWaitForDeviceBeDisconnectedOf:(RTKProfileConnection *)connection;

@end

NS_ASSUME_NONNULL_END
