//
//  RTKOperationWaitor.h
//  RTKLEFoundation
//
//  Created by jerome_gu on 2021/10/26.
//  Copyright © 2022 Realtek. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * A general purpose object that you use to wait for an asynchronous task to be completed.
 *
 * @discussion Once you have an @c RTKOperationWaitor object, call @c -wait or @c -waitWithTimeout: to begin waiting, then call @c -cancel to stop waiting. The completion handler you pass to @c -initWithToken:completionHandler: will be invoked when either the waitting task is complete or time out occurs. You call @c -fulfillWithUserInfo: to tell the waitor task completion and the waitor then calls completion handler with @c success set to @c YES and @c error set to nil. If time out occurs, the completion handler is invoked with @c success set to @c NO and @c error set to a valid error object. A completed (fulfulled or time out) waitor can not receive @c -wait method, nor @c -cancel and @c -fulfill methods.
 *
 * When you call @c -fulfillWithUserInfo: with a non-nil pointer as @c userInfo, this pointer will be pass to @c userInfo parameter of the completion handler.
 *
 * You can use @c token property to associate any value to a waitor. If you pass an object as token parameter to @c -initWithToken:completionHandler: , the token is assigned the the object pointer value without retain it.
 */
@interface RTKOperationWaitor : NSObject

/**
 * A object assigned by app.
 *
 * @discussion You can use this property to save value that identifies some entity.
 */
@property (nullable) id token;

/**
 * Initializes a waitor with arbitrary value and completion handler.
 *
 * @param token Arbitrary value assigned to waitor's @c token property.
 * @param handler A completion handler that's invoked after this waitor has finished.
 */
- (instancetype)initWithToken:(nullable id)token completionHandler:(void(^)(BOOL success, NSError *_Nullable error, _Nullable id userInfo))handler;

/**
 * Start waiting for waitor completion.
 *
 * @discussion The interval for wait is 10 seconds.
 */
- (void)wait;

/**
 * Start waiting for waitor to complete within a specified time.
 *
 * @param interval The number of seconds within which the associated task must be fulfilled.
 */
- (void)waitWithTimeout:(NSTimeInterval)interval;

/**
 * Cancel waiting when waitor is waiting.
 */
- (void)cancel;

/**
 * Tell this waitor the associated task has completed.
 *
 * @param userInfo An arbitrarily value which is passed to @c userInfo parameter of completion handler.
 *
 * @discussion Once this method get called, the waitor call completion handler.
 */
- (void)fulfillWithUserInfo:(nullable id)userInfo;

@end

NS_ASSUME_NONNULL_END
