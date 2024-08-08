# Getting started with RTKOTASDK

Create a DFU Upgrade and start upgrade with appropriate images.

> Note: This guide includes sample code written with the new APIs. The legacy APIs (i.e. RTKOTAProfile and RTKOTAPeripheral) are deprecated and not recommended to be used.

## Overview

RTKOTASDK is a library you can use to upgrade an Realtek Bluetooth implementation based (RTL87xx ICs) device.

To upgrade a connected device, you create an RTKDFUUpgrade object. Before start upgrade, you should make the RTKDFUUpgrade prepared. When start upgrade, you can provide a list of images or a file that containing images for upgrade. If upgrade is started, you wait for ``-DFUUpgrade:didFinishUpgradeWithError:`` to be called on upgrade completion.


## Topics

### Link and embed framework

In Xcode, add framework file to your project. Open project editor, add this framework (and RTKLEFoundation) to a target and make it embedded in target bundle.

![Embed framework](./images/embed_framework.png)

> RTKOTASDK is dependent on RTKLEFoundation, so RTKLEFoundation shall be linked and embedded meanwhile.

### Provide Privacy Data Usage Description

Add the following Usage Decription key to your Information Property List.

- NSBluetoothPeripheralUsageDescription (earlier than iOS 13)
- NSBluetoothAlwaysUsageDescription

### Background mode

Add *bluetooth-central* to the Information Property List, to enable background mode running capability.

### Create an RTKDFUUpgrade

To create an RTKDFUUpgrade to upgrade a GATT connected device, you provide a CBPeripheral object.

```objective-c
    CBPeripheral *aKnownPeripheral;
    RTKDFUUpgrade *upgrade = [[RTKDFUUpgrade alloc] initWithPeripheral:aKnownPeripheral];
```

To create a RTKDFUUpgrade to upgrade a iAP connected device, you provide a EAAccessory object.

```objective-c
    EAAccessory *aKnownAccessory;
    RTKDFUUpgrade *upgrade = [[RTKDFUUpgrade alloc] initWithAccessory:aKnownAccessory];
```

### Specify delegate

To receive events during upgrade, assign a ``RTKDFUUpgradeDelegate`` conformed object to delegate.

```objective-c
    id <RTKDFUUpgradeDelegate> upgradeDelegate;
    RTKDFUUpgrade *upgrade;
    upgrade.delegate = upgradeDelegate;
```

### Prepare for upgrade

Before start upgrade, a ``RTKDFUUpgrade`` should be prepared.

```objective-c
    RTKDFUUpgrade *upgrade;
    [upgrade prepareForUpgrade];    
```

When RTKDFUUpgrade prepares succeessfully, it calls ``-DFUUpgradeDidReadyForUpgrade:`` on delegate object. If not, calls ``-DFUUpgrade:couldNotUpgradeWithError:`` on delegate object.

### Access device information

When a RTKDFUUpgrade is prepared successfully, you can access informations about upgrade by RTKDFUUpgrade object.

```objective-c
    RTKDFUUpgrade *upgrade;
    RTKOTADeviceInfo *info = upgrade.deviceInfo;
    NSLog(BDAddressString(info.bdAddress));
```

### Extract images from file

You call extract methods on ``RTKOTAUpgradeBin`` class to obtain images for upgrade.

The file should be one of the following format.
* Realtek MP binary file (containing one binary)
* Realtek MP pack file (containing multiple binaries)
* Realtek MP combine file for RWS pair (containing multiple binaries for both devices)

> To learn more about file format that OTASDK accept, please refer to <UpgradeFiles>.

To obtain a list of images for upgrade a single device, write code like.

```objective-c
    NSString *filePath;
    NSError *error;
    NSArray <RTKOTAUpgradeBin*> *bins = [RTKOTAUpgradeBin imagesExtractedFromMPPackFilePath:filePath error:&error];
```

If you upgrade a RWS pair devices, use combine file extracting method, the file should be of Realtek MP combine file.

```objective-c
    NSString *filePath;
    NSArray <RTKOTAUpgradeBin*> *imagesForPrimaryBud, *imagesForSecondaryBud;
    NSError *error = [RTKOTAUpgradeBin extractCombinePackFileWithFilePath:filePath toPrimaryBudBins:&imagesForPrimaryBud secondaryBudBins:&imagesForSecondaryBud];
```


### Upgrade devices

If a RTKDFUUpgrade is prepared and you have images in hand, you call one of upgrade methods to start upgrade.

```objective-c
    RTKDFUUpgrade *upgrade;
    NSArray <RTKOTAUpgradeBin*> *bins
    [upgrade upgradeWithImages:bins];
```

If the connected device is a member of rws pair, you should provide images for primary bud and secondary bud in the same time.

```objective-c
    RTKDFUUpgrade *upgrade;
    NSArray <RTKOTAUpgradeBin*> *imagesForPrimaryBud, *imagesForSecondaryBud;
    [upgrade upgradeWithImagesForPrimaryBud:imagesForPrimaryBud imagesForSecondaryBud:imagesForSecondaryBud];
```

During upgrading, the RTKDFUUpgrade call RTKDFUUpgradeDelegate methods to report progress events.

```objective-c
// MyDFUUpgradeDelegate
- (void)DFUUpgrade:(RTKDFUUpgrade *)task
        withDevice:(RTKProfileConnection *)connection
 didSendBytesCount:(NSUInteger)length
           ofImage:(RTKOTAUpgradeBin *)image {
    // did send some bytes
}

- (void)DFUUpgrade:(RTKDFUUpgrade *)task
        withDevice:(RTKProfileConnection *)connection
     willSendImage:(RTKOTAUpgradeBin *)image {
    // will send a image bytes
}

- (void)DFUUpgrade:(RTKDFUUpgrade *)task
        withDevice:(RTKProfileConnection *)connection
didCompleteSendImage:(RTKOTAUpgradeBin *)image {
    // did send a image
}

- (void)DFUUpgrade:(RTKDFUUpgrade *)task
        withDevice:(RTKProfileConnection *)connection
   didActivateImages:(NSSet<RTKOTAUpgradeBin*>*)images {
    // did activate some images
}

- (void)DFUUpgradeDidFinishFirstDeviceAndPrepareCompanionDevice:(RTKDFUUpgrade *)task {
    // did finish upgrade first device and will upgrade the companion device
}

- (void)DFUUpgrade:(RTKDFUUpgrade *)task
willUpgradeCompanionDevice:(RTKProfileConnection *)connection
        deviceInfo:(RTKOTADeviceInfo *)companionInfo {
    // is about to upgrade the companion device
}

- (void)DFUUpgrade:(RTKDFUUpgrade *)task didFinishUpgradeWithError:(nullable NSError *)error {
    if (error) {
    // upgrade failed.
    } else {
    // upgrade succeed.
    }
}
```
### Update Connection parameter

If you want to change LE ACL Connection Parameter, probably because you want a faster thoughput. You should call ``-updateConnectionParameterWithMinInterval:maxInterval:latency:suspervisionTimeout:completionHandler`` on an `RTKDFUConnectionUponGATT` object at the right time.

```objective-c
- (void)DFUUpgrade:(RTKDFUUpgrade *)task
isAboutToSendImageBytesTo:(RTKProfileConnection *)connection
withContinuationHandler:(void(^)(void))continuationHandler {
    if ([connection isKindOfClass:RTKDFUConnectionUponGATT.class]) {
        RTKDFUConnectionUponGATT *dfuConnection = (RTKDFUConnectionUponGATT*)connection;
        
        [dfuConnection updateConnectionParameterWithMinInterval:12 maxInterval:24 latency:10 suspervisionTimeout:100 completionHandler:^(BOOL success, NSError * _Nullable error) {
            continuationHandler();
        }];
    } else {
        continuationHandler();
    }
}
```
