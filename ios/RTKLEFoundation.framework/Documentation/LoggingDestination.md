#  Log SDK text to a specific destination

Control what log SDK prints and obtain the log content.

## Overview

The library Realtek provides will print log messages during executing. You can control what logs to be printed and obtain the log message content by ``RTKLog`` class.

## Set log level

The log level control what log is printed. Only the log message has higher level than the level ``RTKLog`` current used will be printed.

```objective-c
[RTKLog setLoggerLevel:RTKLogLevelInfo];
```

## Obtain log messages

By default, ``RTKLog`` use `NSLog` function to log message, which means log messages is printed to Console. You can obtain the log messages and save it in some persistent store for further analysis.

```objective-c
void myLogFunc(NSString* logMsg) {
  NSLog(@"Obtained log: %@", logMsg);
}
```

```objective-c

[RTKLog setFacility:RTKLogFacilityCustom];
[RTKLog setLogger:myLogFunc];
```
