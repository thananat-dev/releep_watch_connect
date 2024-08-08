#  ``RTKOTASDK``

Upgrade GATT or iAP connected devices using Realtek solution.

## Overview

The framework provides ``RTKDFUUpgrade`` class you use mostly to initiate upgrade and monitor upgrade progress. When upgrade a GATT connected device, ``RTKDFUUpgrade`` creates a ``RTKDFUConnectionUponGATT`` to make actual communication. When upgrade a iAP connected device, ``RTKDFUUpgrade`` creates a  ``RTKDFUConnectionUponiAP`` to make actual communication. Except of updating LE connection parameter, you rarely need to interact directly with ``RTKDFUConnectionUponGATT`` and ``RTKDFUConnectionUponiAP``.

When a ``RTKDFUUpgrade`` is prepared, you can access device informations by ``RTKOTADeviceInfo``.

When initialize a upgrade, you provides a list of ``RTKOTAUpgradeBin`` to ``RTKDFUUpgrade``.

## Topics

### Compatiblity

- <doc:Compatiblity>

### Essentials

- <doc:GettingStarted>
- <doc:UnderstandingRealtekDFUProcedure>
- ``RTKDFUUpgrade``

### Upgrading devices

- ``RTKDFUUpgrade``
- ``RTKDFUUpgradeDelegate``


### Accessing device information

- ``RTKOTADeviceInfo``


### Images for upgrade

- ``RTKOTAUpgradeBin``
- ``RTKOTABin``


### Interact with device

- ``RTKDFUConnectionUponGATT``
- ``RTKDFUConnectionUponiAP``
- ``RTKDFURoutine``


### Errors

- ``RTKOTAErrorDomain``
- ``RTKOTAErrorCode``
- <doc:ErrorCode>


### Deprecated
- ``RTKOTAProfile``
- ``RTKOTAPeripheral``
- ``RTKDFUPeripheral``
- ``RTKMultiDFUPeripheral``
