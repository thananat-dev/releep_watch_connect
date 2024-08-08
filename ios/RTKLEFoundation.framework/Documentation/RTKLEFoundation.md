#  ``RTKLEFoundation``

Provides abstract class that describing Object Models for Bluetooth communication and utilities that other frameworks or app depend on.

## Overview

``RTKLEFoundation`` provides 2 key concepts: Profile Connection Manager and Profile Connection. A Profile Connection represents a connection with device that using a specific app profile. A Profile Connection can operate at GATT profile or iAP profile. A Profile Connection Manager discover and monitor eligible device and create a Profile Connection instance to represent it.

``RTKLEFoundation`` provides several utilities can be used for convenience.


## Topics

### Bluetooth communication model

- <doc:ProfileConnectionModel>

Abstract class ``RTKProfileConnectionUponGATT`` instance represents a device connection that operates upon GATT profile. Abstract class ``RTKProfileConnectionUponiAP`` instance represents a device connection that operates upon iAP profile.

You perform app layer task by calling methods of a Profile Connection. Before perform any app layer task, a Profile Connection should be active.

- ``RTKProfileConnection``
- ``RTKConnectionUponGATT``
- ``RTKConnectionUponiAP``
- ``RTKProfileConnectionManager``


### Transport model

For exchanging message, LEFoundation defines 2 class: `RTKPacket` represents a single packet that is send or received, `RTKPacketTransport` represents a unit that send packet or receive packet.

- ``RTKPacket``
- ``RTKPacketTransport``

`RTKCharacteristicTRXTransport` is provided to you for writing GATT characteristic to send message or receiving message by GATT characteristic notification. You use `RTKAccessorySessionTransport` if you want to communicate with a accessory with a protocol. 

- ``RTKCharacteristicTRXTransport``
- ``RTKAccessorySessionTransport``

### Attempt

- ``RTKOperationWaitor``
- ``RTKActionAttempt``

### Error

- ``RTKError``
- <doc:ErroCode>

### Logging

- ``RTKLog``
- ``RTKBTLogMacros``

### Data encryption and check value

- ``RTKData+KKAES``
- ``RTKData+CRC16``
