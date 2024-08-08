#  Errors

Describes the conditions that an Realtek error may occur and provides recovery suggestions corresponding to those errors.

## Defined in
`RTKLEFoundation/RTKError.h`

## Exception

If you call a method incorrectly, Realtek SDK may throw `RTKInvalidCallException` exception.


## Domain
`RTKBTErrorDomain`

## Error code

| error code |  condition  |  recovery suggestion |
| ------ | ---- | ----|
| `RTKErrorAttemptTimeout` | An `RTKActionAttempt` reports this error when the attempt wait for completion time out | A high layer action should complete attempt before timeout |
| `RTKErrorInvalidOperation` | The operation is not valid to be performed currently,  e.g., open a closed transport | Be sure that the condition of this operation is meet |
| `RTKErrorTransportBusy` | The operation can not be performed because system is busy in some other process | Wait until the process is not busy |
| `RTKErrorDeviceConnectionFail`  | Connect to peripheral fail | |
| `RTKErrorDeviceValidation` | The device is not assumed correct |
| `RTKErrorGATTAttributeDiscoveryBusy` | The peripheral is busy in discover attribute | Wait until the pervious discovery completes |
| `RTKErrorGATTAttributeDiscoveryTimeout` | Discover for the expected GATT attribute timeout | The expected attribute does not exist or the time of discovery is too short |
| `RTKErrorGATTAttributeStale` | A GATT attribute object is invalid because it is discovered during previous connection session | You should discover the attribute again |
| `RTKErrorIAPProtocolNotSupport` | The communication protocol is not supported by accessory | Double check the communication protocol for iAP accessory |
| `RTKErrorConnectionActivateBusy` | The connection object is busy in activation process | Wait until previous process completes |
| `RTKErrorConnectionNotActive` | The connection is not active | The connection object should be active before perform any tasks |
| `RTKErrorOperationWaitTimeout` | An `RTKOperationWaitor` reports this error when timeout occurs |
| `RTKErrorCharacteristicWriteBusy` | A previous writing action is still on | Wait until previous action completes |  
| `RTKErrorDeviceNotConnected` | Task fails due to device is not connected or can not connect |
| `RTKErrorDataLengthExceed` | The date to write has a length exceed MTU size |
| `RTKErrorNoMoreEvent` | An `RTKBatchDataSendReception` reports this if it can not obtain outside event to make progress |
| `RTKErrorBatchDataSendReceptionBusy` | There is already a task in progress | Start task after the previous task is done |
| `RTKErrorAccessorySessionNotOpen` | Accessory session is not open | The accessory session should be open before communicate with accessory |
| `RTKErrorOperationNotSupport` | The operation is not supported |
| `RTKErrorAccessoryDisconnected` | The connection with iAP accessory is disconnected unexpectedly |
