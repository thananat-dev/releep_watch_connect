//
//  RTKCharacteristicTRXTransport.h
//  RTKBTFoundation
//
//  Created by jerome_gu on 2021/11/2.
//  Copyright © 2022 Realtek. All rights reserved.
//

#ifdef RTK_SDK_IS_STATIC_LIBRARY
#import "RTKPacketTransport.h"
#import "RTKCharacteristicOperate.h"
#import "RTKConnectionUponGATT.h"
#else
#import <RTKLEFoundation/RTKPacketTransport.h>
#import <RTKLEFoundation/RTKCharacteristicOperate.h>
#import <RTKLEFoundation/RTKConnectionUponGATT.h>
#endif

NS_ASSUME_NONNULL_BEGIN

/**
 * A concrete Packet Transport that allows sending packets by writing charactertistic value and receiving packets by characteristic value notification.
 *
 * @discussion The transport uses MTU size as `-[CBPeripheral maximumWriteValueLengthForType:]` returned. The transport does not have a basingOnTransport.
 */
@interface RTKCharacteristicTRXTransport : RTKPacketTransport <RTKCharacteristicNotificationRecept, RTKCharacteristicWrite>

/**
 * Initializes and returns a Transport with device connection and characteristics for send and receive packet.
 *
 * @param connection The connection that indicates the peripheral having characteristics to write and read. Must be a member of `RTKConnectionUponGATT` class.
 * @param RXCharacteristic The characteristic which value is read as received data, should permit notification (CBCharacteristicPropertyNotify or CBCharacteristicPropertyIndicate).
 * @param TXCharacteristic The characteristic to use for send data. The characteristic should have property of CBCharacteristicPropertyWrite or CBCharacteristicPropertyWriteWithoutResponse, depending on `+writeReliably` property.
 * @return An initialized transport object or nil if the parameter is invalid.
 *
 * @discussion You can not set RXCharacteristic and TXCharacteristic to nil at the same time, otherwise `RTKInvalidCallException` is throw.
 */
- (instancetype)initWithGATTConnection:(RTKConnectionUponGATT*)connection
                  characteristicForRX:(nullable CBCharacteristic *)RXCharacteristic
                 characteristicForTX:(nullable CBCharacteristic *)TXCharacteristic;

/**
 * Returns a boolean value that indicates whether the transport send packet reliably.
 *
 * @discussion When YES, packet is confirmed to send to remote when the completion block is called successfully. When NO, data is considered but is not sure to send to remote. The TXCharacteristic should have CBCharacteristicPropertyWrite property if this property return YES, should have CBCharacteristicPropertyWriteWithoutResponse property if this property return NO. The default value is YES.
 */
@property (class, readonly) BOOL writeReliably;

/// Specifies how long a packet is assumed to be send to remote.
///
/// This value is only used for unreliably send. The default value is 0.02 in seconds.
@property NSTimeInterval elapsedTime;

@end


/// An RTKCharacteristicTRXTransport subclass that sends packet unreliably.
///
/// @discussion This class overrides +writeReliably method and returns NO.
@interface RTKCharacteristicArbitrarilyTRX : RTKCharacteristicTRXTransport

@end

NS_ASSUME_NONNULL_END
