//
//  RTKPacketTransport.h
//  RTKLEFoundation
//
//  Created by jerome_gu on 2021/12/7.
//  Copyright © 2022 Realtek. All rights reserved.
//

#import <Foundation/Foundation.h>

#ifdef RTK_SDK_IS_STATIC_LIBRARY
#import "RTKBTGeneralDefines.h"
#import "RTKPacket.h"
#else
#import <RTKLEFoundation/RTKBTGeneralDefines.h>
#import <RTKLEFoundation/RTKPacket.h>
#endif

/**
 * A block to be called when a transport finishes packet sending.
 *
 * @param success Indicates whether send is successful or unsuccessful.
 * @param error A nullable object that capsulates failure cause.
 * @param supplement A data object which is generated during sending, providing more inforamtion.
 */
typedef void (^RTKTransportSendResult)(BOOL success, NSError *_Nullable error, NSData*_Nullable supplement);

/**
 * A block to be called when a transport finishes request sending.
 *
 * @param success Indicates whether send is successful or unsuccessful.
 * @param error A nullable object that capsulates failure cause.
 * @param response A data object containing response content.
 */
typedef void (^RTKTransportRequestResult)(BOOL success, NSError *_Nullable error, NSData*_Nullable response);


/**
 * Values that indicates the transport status.
 */
typedef NS_ENUM(NSUInteger, RTKPacketTransportStatus) {
    RTKPacketTransportStatusNotOpen,        ///< The transport is not open.
    RTKPacketTransportStatusOpening,        ///< The transport is opening.
    RTKPacketTransportStatusOpen,           ///< The transport is open.
    RTKPacketTransportStatusClosing,        ///< The transport is closing.
    RTKPacketTransportStatusClosed,         ///< The transport is closed.
    RTKPacketTransportStatusErrorOccurred,  ///< Error occured in this transport.
};


NS_ASSUME_NONNULL_BEGIN

@class RTKPacketTransport;

/**
 * Methods that RTKPacketTransport objects call on their delegates to handle state events and receive data.
 */
@protocol RTKPacketTransportClient <NSObject>

/**
 * Tells the delegate that transport state did change.
 *
 * @param transport The object that reports change event.
 */
- (void)transportDidChangeState:(RTKPacketTransport *)transport;

/**
 * Tells the delegate that new data is received.
 *
 * @param transport The object that receive new data.
 * @param data The new received data.
 */
- (void)transport:(RTKPacketTransport *)transport didReceive:(NSData *)data;

@end


/**
 * An abstract class that you can use to send and receive packet data.
 *
 * @discussion An concrete RTKPacketTransport object sends packets to remote device and receive packets from remote device. You call @c -send:completionHandler: to send a data. When transport object receives a packet, it call @c -transport:didReceive: on its delegate object.
 *
 * To send or receive packets, a transport object's state should be @c RTKPacketTransportStatusOpen . If it is not, you call @c open method to initiates opening operation.
 *
 * RTKPacketTransport conforms to RTKPacketTransportClient protocol, which means a transport can be served by another transport. An RTKPacketTransport object may base on another transport object, becoming a client of this transport.
 *
 * An transport has a list of clients which it serves, it calls @c RTKPacketTransportClient defined methods to report state events and received data on each clients. You can add more objects into this list or remove a object from this list.
 */
@interface RTKPacketTransport : NSObject <RTKPacketTransportClient>

/**
 * Returns the state of this transport.
 */
@property (readonly) RTKPacketTransportStatus state;

/**
 * A transport that this transport base on.
 *
 * @discussion Returns nil if no transport based on.
 */
@property (readonly, nullable) RTKPacketTransport *basingOnTransport;

/**
 * Initializes this transport with a transport for base on.
 */
- (nullable instancetype)initWithBasingOnTransport:(nullable RTKPacketTransport *)transport;

/**
 * A list of RTKPacketTransportClient conformed objects this transport serves.
 */
@property (readonly) NSArray <RTKPacketTransportClient> *clients;

/**
 * Add an RTKPacketTransportClient conformed object into clients list to serve.
 *
 * @discussion The client is not retained by this transport.
 */
- (void)addClient:(id <RTKPacketTransportClient>)client;

/**
 * Remove a RTKPacketTransportClient conformed object from clients list.
 */
- (void)removeClient:(id <RTKPacketTransportClient>)client;


// MARK: -
/**
 * Opens the transport for subsequent data sending and receiving.
 *
 * @param handler A block which is called when open complete successfully or unsuccessfully.
 */
- (void)openWithCompletionHandler:(nullable RTKLECompletionBlock)handler;


/**
 * Closes the transport to stop data sending and receiving.
 *
 * @param handler A block which is called when close complete successfully or unsuccessfully.
 */
- (void)closeWithCompletionHandler:(nullable RTKLECompletionBlock)handler;


// MARK: - Packet send
/**
 * Returns the maxium length of the data to send when call @c -send: method.
 */
@property (readonly) NSUInteger MTUSize;

/**
 * Sends a buffer of data to the receiver.
 *
 * @param data The data to be send. Should not exceed MTUSize.
 * @param handler A block to be called when send complete successfully or unsuccessfully.
 * @discussion If a transport is not able to determine if data is really send to remote device, the completion handler is not called.
 */
- (nullable RTKPacket *)send:(NSData *)data withCompletionHandler:(nullable RTKTransportSendResult)handler;

/**
 * Cancels a pending data sending.
 *
 * @discussion Notes that the completion handler when call send method is not called.
 */
- (void)cancelSending:(RTKPacket *)packet;

@end


/**
 * An RTKPacketTransport subclass which has acknowledge mechanism.
 *
 * @discussion When an RTKPacketACKTransport object send data, it wait for the acknowledge of this data before call the completion handler. This property and methods this class defines is protected for used in subclass.
 */
@interface RTKPacketACKTransport : RTKPacketTransport

/**
 * Sends a packet data and wait for acknowledge.
 *
 * @param data The data object containing packet contents.
 * @param interval The time in seconds when wait for ACK.
 * @param count The try count before a send is assumed fail.
 * @param handler The block which is called when ACK is received or send is fail.
 */
- (nullable RTKPacket *)send:(NSData *)data
                 withTimeout:(NSTimeInterval)interval
                  retryCount:(NSUInteger)count
       withCompletionHandler:(nullable RTKTransportSendResult)handler;

@end


/**
 * A RTKPacketTransport subclass that implements a request-response pattern.
 */
@interface RTKPacketRequestTransport : RTKPacketTransport

/**
 * Sends a request with data and wait for response.
 *
 * @param reqData The data object that represents the request content.
 * @param handler The block which is called when response is received or request send failed. If request is successful, the response param of completion handler represents the content of response.
 * @discussion The time interval for wait the response is 3 seconds and retry count is 0.
 */
- (nullable RTKRequestPacket *)sendRequest:(NSData *)reqData withCompletionHandler:(nullable RTKTransportRequestResult)handler;

/**
 * Sends a request with data and wait for response with specified wait interval and retry count.
 *
 * @param reqData The data object that represents the request content.
 * @param interval The time in seconds when wait for response.
 * @param count The try count before a request is assumed fail.
 * @param handler The block which is called when response is received or request send failed. If request is successful, the response param of completion handler represents the content of response.
 * @discussion The time interval for wait the response and retry count are implementation specific.
 */
- (nullable RTKRequestPacket *)sendRequest:(NSData *)reqData
                               withTimeout:(NSTimeInterval)interval
                                retryCount:(NSUInteger)count
                         completionHandler:(nullable RTKTransportRequestResult)handler;

@end

NS_ASSUME_NONNULL_END
