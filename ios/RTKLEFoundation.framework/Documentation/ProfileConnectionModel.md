#  Profile connection model

Learn about profile connection models and how to subclass it for concrete use case.

## Overview

![Architecture](./images/ProfileConnectionArchitecture.png)

``RTKLEFoundation`` provides an abstract class ``RTKProfileConnection`` to represent a logic app layer profile connection with a remote device. ``RTKConnectionUponGATT``  and ``RTKConnectionUponiAP`` are 2 subclasses that indicate communication occur upon GATT or iAP profile. GATT and iAP profiles are not assumed as app layer profiles.

``RTKLEFoundation`` provides an ``RTKProfileConnectionManager`` class to discover profile connection instances. A profile connection manager creates profile connection instance for a eligible device by:

* Scanning nearby GATT peripherals;
* Monitoring new connected GATT peripherals or iAP accessories;
* Obtaining peripherals or accessories connected to system currently.

A exist profile connection instance does not mean app is being connected with the device. A profile connection has a status that indicating if it is active. A profile connection should be active before perform any app profile interoperation with the corresponding device.

The ``RTKConnectionUponGATT`` and ``RTKConnectionUponiAP`` are intended to be subclassed for specific usage case. For example, *RTKOTASDK* provides  ``RTKDFUConnectionUponGATT`` and ``RTKDFUConnectionUponiAP`` for perform DFU procedures with device, and provides ``RTKDFUManger`` to discover devices that is able to upgrade. You can create your subclasses specially for your custom profile usage.


## Discover profile connections

Typically you use a concrete profile connection manager to discover profile connections.

1. Create profile connection manager

```objective-c
RTKProfileConnectionManager *myManager = [[RTKProfileConnectionManager alloc] initWithDelegate:self];
```

When a profile connection manager is available, it calls method on its delegate to report.

```objective-c
- (void)profileManagerDidUpdateGATTAvailability:(RTKProfileConnectionManager *)manager {
  if (manager.GATTAvailable) {
    // Now, you can call methods on manager to discover connections.
  }
}
```

2. Scanning nearby peripherals

You can scan devices nearby that is advertising LE ADV.

```objective-c
[myManager scanForPeripherals];
```
Once an eligible peripheral is scanned, an ``RTKConnectionUponGATT`` instance is created and passed to delegate object.

```objective-c
- (void)profileManager:(RTKProfileConnectionManager *)manager
didDiscoverPeripheralOfConnection:(RTKProfileConnection *)connection
     advertisementData:(NSDictionary<NSString *, id> *)advertisementData
                  RSSI:(NSNumber *)RSSI {
                    NSLog(@"A new peripheral is scanned.");
                  }
```

3. Retrieve connected peripherals or accessories

For peripherals or accessories that is already connected with system, you can retrieve those connections.

```objective-c
NSArray <RTKProfileConnection*> *peripheralConnections = [myManager retrieveConnectedPeripheralConnections];
NSArray <RTKProfileConnection*> *accessoryConnections = [myManager retrieveConnectedAccessoryConnections];
```

4.  Monitoring new connected peripherals or accessories

You can request Profile connection manager to monitor peripherals or accessories that are new connected with system.

```objective-c
[myManager startMonitoringNewConnectionWithAccessory];
[myManager startMonitoringNewConnectionWithPeripheral];
```
The profile connection manager creates a profile connection instance and reports to you when an eligible device connection is detected.

```objective-c
- (void)profileManager:(RTKProfileConnectionManager *)manager
didDetectNewConnectedConnection:(RTKProfileConnection *)connection {
  if ([connection isKindOf:RTKConnectionUponGATT.class]) {
    // new connected peripheral detected
  } else if ([connection isKindOf:RTKConnectionUponiAP.class]) {
    // new connected iAP accessory detected
  }
}
```

5. Instantiate profile connection

If you have a peripheral or accessory, you can let profile connection manager creates the corresponding connection for you.

```objective-c
CBPeripheral *aKnownPeripheral; // if peripheral is connected is not concerned.
RTKConnectionUponGATT *peripheralConnection = [myManager instantiateConnectionWithPeripheral:aKnownPeripheral];
```
```objective-c
EAAccessory *aKnownAccessory;
RTKConnectionUponGATT *accessoryConnection = [myManager instantiateConnectionWithAccessory:aKnownAccessory];
```

### Create profile connection manually

You can create profile connections without a profile connection manager.

```objective-c
CBPeripheral *aKnownPeripheral;
RTKConnectionUponGATT *peripheralConnection = [[RTKConnectionUponGATT alloc] initWithPeripheral:aKnownPeripheral];
```

```objective-c
EAAccessory *aKnownAccessory;
RTKConnectionUponiAP *accessoryConnection = [[RTKConnectionUponiAP alloc] initWithAccessory:aKnownAccessory];
```

The profile connection you create manually has the limit according to connection establishment during activation.

### Activate profile connection

If you have a profile connection in hand and want to interoperate with the device to perform some tasks. You should activate it first.

```objective-c
RTKProfileConnection *aConnection;
[aConnection activateWithCompletionHandler:^(BOOL success, NSError *error) {
  if (success) {
    // aConnection is active and be able to perform tasks.
  }
}];
```

A profile connection activation may perform connection establishment, service discovery, and others process before the connection is assumed to be active.

> A profile connection requests its `connectionPerformer` to perform connection establishment. A manually created profile connection does not have a connectionPerformer.
>
> So if a manually created profile connection having the device that is not connected, the activation may fail due to connection can not be established.


### Customize for a specific case

If you want to use Profile Connection model as object framework for your use case. You most focus on creating a ``RTKConnectionUponGATT`` subclass or ``RTKConnectionUponiAP`` subclass.

1. Providing identifier for match

In your ``RTKConnectionUponGATT`` subclass, override ``+serviceUUIDsInADV`` and ``-interestedServiceUUIDs`` to return service UUIDs that profile connection manager uses to determine if a peripheral is eligible.

```objective-c
+ (NSArray <CBUUID *> *)serviceUUIDsInADV {
    return @[];
}
```

```objective-c
- (NSArray <CBUUID *> *)interestedServiceUUIDs {
    return @[];
}
```

For iAP accessory, you provide the communication protocol used for communicate with iAP device.

```
+ (NSString *)communicationProtocol {
    return @"com.rtk.datapath";
}
```

2. Activation operation implement

You should override `-activateWithCompletionHandler:` method and do what ever things that is needed before a profile connection is assumed to be active.

As a rule, in your `-activateWithCompletionHandler:`, you may carry out following operations:

  - Check if connection is established, start connection establishment if it is not;
  - Verify if the device comply with the profile requirement;
  - Discover service and open communication channel;
  - Acquire some required information of device;

In the meantime, override `-deactivateWithCompletionHandler:` to perform some operations that is the reverse of activation.

3. Register your custom profile connection class

Finally, register your subclasses to the manager. The manager use your class to create connection instance.

```
RTKProfileConnectionManager *myManager;
[myManager registerConnectionClassForInstantiateGATTPeripheral:MyGATTProfileConnection.class];
[myManager registerConnectionClassForInstantiateiAPAccessory:MyiAPProfileConnection.class];
```
