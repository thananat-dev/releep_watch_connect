//
//  RTKConnectionUponiAP.h
//  RTKBTFoundation
//
//  Created by jerome_gu on 2021/10/18.
//  Copyright © 2022 Realtek. All rights reserved.
//

#import <ExternalAccessory/ExternalAccessory.h>

#ifdef RTK_SDK_IS_STATIC_LIBRARY
#import "RTKProfileConnection.h"
#else
#import <RTKLEFoundation/RTKProfileConnection.h>
#endif

NS_ASSUME_NONNULL_BEGIN

/**
 * A concrete device connection that communicates with a iAP accessory.
 *
 * @discussion A @c RTKConnectionUponiAP object specify the protocol by @c communicationProtocol property to use for communication with iAP accessory. The protocol should also be set to UISupportedExternalAccessoryProtocols value in your app's Info.plist.
 */
@interface RTKConnectionUponiAP : RTKProfileConnection

/**
 * Returns the iAP accessory this conneciton links.
 *
 * @discussion This property has the same value as @c device property.
 */
@property (nonatomic, readonly) EAAccessory *accessory;

/**
 * Initializes this connection object with a known iAP accessory.
 *
 * @discussion When this method is called, the accessory does not have to be in connected state. But when be called to activate, this accessory should be connected.
 */
- (instancetype)initWithAccessory:(EAAccessory *)accessory;

/**
 * Returns the protocol to use when communicating with the accessory.
 *
 * @discussion In addition, this protocol is also used as a condition when verify conformance of a specific profile.
 */
@property (class, readonly) NSString *communicationProtocol;

@end

NS_ASSUME_NONNULL_END
