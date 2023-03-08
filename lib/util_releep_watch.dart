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
