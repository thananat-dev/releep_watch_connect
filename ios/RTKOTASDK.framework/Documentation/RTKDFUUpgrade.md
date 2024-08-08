# ``RTKDFUUpgrade``

## Topics

### Initializing a DFU Upgrade 

- ``-initWithPeripheral:``
- ``-initWithAccessory:``
- ``-initWithAccessory:existedMessageTransport:``
- ``-initWithDeviceConnection:profileManager:``

### Preparing for upgrade

- ``-prepareForUpgrade``


### Starting upgrade

- ``-upgradeWithImages:``
- ``-upgradeWithImagesForPrimaryBud:imagesForSecondaryBud:``
- ``-upgradeWithBinaryFileAtPath:``

### Canceling upgrade

- ``-cancelUpgrade``

### Monitoring upgrade progress

- ``delegate``
- ``RTKDFUUpgradeDelegate``

### Controlling upgrade behavior

- ``retryCountWhenBufferCheckFail``
- ``shouldWaitForDisconnectionBeforeReportCompletion``
- ``olderImageAllowed``
- ``-setEncryptionKey:``
