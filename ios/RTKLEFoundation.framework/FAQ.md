#  FAQ

## How to set up Xcode project to use Realtek libraries ?

Typically, Realtek libraries are distributed as framework bundles. When you want to use them in your project, you shall link the framework with your target, and embed it in your target application bundle.

![Link frameworks](./images/Link_embed_frameworks.png)

If app can not run and a message(as below screenshot) is printed in Console, this means the framework is not embedded in your app bundle.

![Frameworks not embedded](./images/not_embed_error.png)

Import the framework module header file in your source code in where you want to call library APIs.

```objective-c
#import <RTKLEFoundation/RTKLEFoundation.h>
```

> If library is distributed as static library, you add and link it directly. Set the Header Search Path to the library public header directory path.

If you want to communicate with GATT peripherals, you should provide Privacy Data Usage Description in app info property list. If you want to communicate with iAP accessory, include the protocol in Supported external accessory protocols.


## How to browse API document ?

Realtek library API document is provided as Symbol Document Comments in header source, not a separate document file. Although Xcode can parse it and show formatted content, it is not friendly for browsing. You could use a document tool such as Doxgen to generate documentation more convenient.
