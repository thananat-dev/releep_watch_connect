#import "ReleepWatchConnectPlugin.h"
#if __has_include(<releep_watch_connect/releep_watch_connect-Swift.h>)
#import <releep_watch_connect/releep_watch_connect-Swift.h>
#else
// Support project import fallback if the generated compatibility header
// is not copied when this plugin is created as a library.
// https://forums.swift.org/t/swift-static-libraries-dont-copy-generated-objective-c-header/19816
#import "releep_watch_connect-Swift.h"
#endif

@implementation ReleepWatchConnectPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar {
  [SwiftReleepWatchConnectPlugin registerWithRegistrar:registrar];
    
}
@end
