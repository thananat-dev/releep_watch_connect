import 'package:flutter/services.dart';
import 'package:flutter_test/flutter_test.dart';
import 'package:releep_watch_connect/releep_watch_connect.dart';

void main() {
  const MethodChannel channel = MethodChannel('releep_watch_connect');

  TestWidgetsFlutterBinding.ensureInitialized();

  setUp(() {
    channel.setMockMethodCallHandler((MethodCall methodCall) async {
      return '42';
    });
  });

  tearDown(() {
    channel.setMockMethodCallHandler(null);
  });

  test('getPlatformVersion', () async {
    expect(await ReleepWatchConnect.platformVersion, '42');
  });
}
