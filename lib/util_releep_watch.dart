import 'dart:io';

class UtilReleepWatch {
  static const int TimeOut = 0x01;
  static const int NotOpen = 0x02;
  static const int Disconnect = 0x03;
  static const int Disconnecting = 0x04;
  static const int Connecting = 0x05;
  static const int Connected = 0x06;
  static const int ServicesDiscovered = 0x07;
  static const int CharacteristicDiscovered = 0x08;
  static const int CharacteristicNotification = 0x09;
  static const int ReadWriteOK = 0x0A;

  static String getStatusName(int status) {
    if (Platform.isIOS) {
      return getYCProductStateString(YCProductState.values[status]);
    } else {
      switch (status) {
        case TimeOut:
          return "TimeOut";
        case NotOpen:
          return "NotOpen";
        case Disconnect:
          return "Disconnect";
        case Disconnecting:
          return "Disconnecting";
        case Connecting:
          return "Connecting";
        case Connected:
          return "Connected";
        case ServicesDiscovered:
          return "ServicesDiscovered";
        case CharacteristicDiscovered:
          return "CharacteristicDiscovered";
        case CharacteristicNotification:
          return "CharacteristicNotification";
        case ReadWriteOK:
          return "ReadWriteOK";
        default:
          return "Unknown";
      }
    }
  }

  static String getYCProductStateString(YCProductState state) {
    switch (state) {
      case YCProductState.unknown:
        return "Unknown";
      case YCProductState.resetting:
        return "Resetting";
      case YCProductState.unsupported:
        return "Unsupported";
      case YCProductState.unauthorized:
        return "Unauthorized";
      case YCProductState.poweredOff:
        return "Powered Off";
      case YCProductState.poweredOn:
        return "Powered On";
      case YCProductState.disconnected:
        return "Disconnected";
      case YCProductState.connected:
        return "Connected";
      case YCProductState.connectedFailed:
        return "Connected Failed";
      case YCProductState.succeed:
        return "Succeed";
      case YCProductState.failed:
        return "Failed";
      case YCProductState.unavailable:
        return "Unavailable";
      case YCProductState.timeout:
        return "Timeout";
      case YCProductState.dataError:
        return "Data Error";
      case YCProductState.crcError:
        return "CRC Error";
      case YCProductState.dataTypeError:
        return "Data Type Error";
      case YCProductState.noRecord:
        return "No Record";
      case YCProductState.parameterError:
        return "Parameter Error";
      case YCProductState.alarmNotExist:
        return "Alarm Does Not Exist";
      case YCProductState.alarmAlreadyExist:
        return "Alarm Already Exists";
      case YCProductState.alarmCountLimit:
        return "Alarm Count Limit";
      case YCProductState.alarmTypeN:
        return "Alarm Type N";
      default:
        return "Unknown";
    }
  }
}

enum YCProductState {
  unknown, // Bluetooth status is unknown
  resetting, // Bluetooth reset
  unsupported, // Does not support Bluetooth
  unauthorized, // Bluetooth is not authorized
  poweredOff, // Bluetooth off
  poweredOn, // Bluetooth is on
  disconnected, // Bluetooth disconnect
  connected, // Bluetooth is connected
  connectedFailed, // Bluetooth connection failed
  succeed, // Success
  failed, // Fail
  unavailable, // API is not available, device does not support
  timeout, // time out
  dataError, // data error
  crcError, // crc error
  dataTypeError, // Data type error
  noRecord, // No record
  parameterError, // Parameter error
  alarmNotExist, // Alarm clock does not exist
  alarmAlreadyExist, // Alarm already exists
  alarmCountLimit, // The number of alarms reaches the limit
  alarmTypeN
}
