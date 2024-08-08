//
//  RTKPacket.h
//  RTKLEFoundation
//
//  Created by jerome_gu on 2021/12/7.
//  Copyright © 2022 Realtek. All rights reserved.
//

#import <Foundation/Foundation.h>

#define RTK_PACKET_ID_NULL -1

NS_ASSUME_NONNULL_BEGIN

/**
 * Represents a generic packet that can be carry out on a packet transport.
 *
 * @discussion An object that encapsulates message bytes and metadata about a message. A packet has `ID` and `subID` which you use to identify a specific packet.
 */
@interface RTKPacket : NSObject

/**
 * The primary identifier number of a packet.
 */
@property (readonly) NSInteger ID;

/**
 * The secondary identifier number of a packet.
 */
@property (readonly) NSInteger subID;

/**
 * The content that is used by a packet transport for transmission.
 */
@property (readonly, nullable) NSData *payload;

/**
 * The time when packet is created.
 */
@property (readonly) NSDate *time;

/**
 * Initializes a packet with a primary ID and payload data.
 *
 * @param ID The primary identifier number.
 * @param payload A data object.
 *
 * @discussion The returned packet has subID set to RTK_PACKET_ID_NULL .
 */
- (instancetype)initWithID:(NSInteger)ID payload:(nullable NSData *)payload;

/**
 * Initializes receiver with IDs and payload data.
 *
 * @param ID The primary identifier number.
 * @param subID The secondary identifier number.
 * @param payload A data object that containing meaningful data for upper layer app.
 */
- (instancetype)initWithID:(NSInteger)ID subID:(NSInteger)subID payload:(nullable NSData *)payload;

@end


/**
 * Represents a special packet that can acknowledge a packet.
 *
 * @discussion Within an acknowledge implemented transport, an ACK packet is used to acknowledge a normal packet.
 */
@interface RTKACKPacket : RTKPacket

/**
 * The identifier value that is used to match a normal packet ID.
 */
@property (readonly) NSInteger ACKID;

/**
 * A data object that containing extra bytes associated with this packet.
 *
 * @disucssion An ACK packet may or may not have a supplement object.
 */
@property (readonly, nullable) NSData *supplement;

/**
 * Initializes an ACK packet with ACK ID and extra supplement bytes.
 */
- (instancetype)initWithACKID:(NSInteger)ID supplement:(nullable NSData *)data;


/**
 * Returns a boolean value that indicates whether this packet can acknownledge a given packet.
 */
- (BOOL)canAcknowledgePacket:(RTKPacket *)packet;

@end


/**
 * A packet that represents a request.
 */
@interface RTKRequestPacket : RTKPacket

@end

NS_ASSUME_NONNULL_END
