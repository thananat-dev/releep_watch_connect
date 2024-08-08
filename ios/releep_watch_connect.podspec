Pod::Spec.new do |s|
  s.name             = 'releep_watch_connect'
  s.version          = '0.0.9'
  s.summary          = 'A new flutter plugin project.'
  s.description      = <<-DESC
A new flutter plugin project.
                       DESC
  s.homepage         = 'http://example.com'
  s.license          = { :file => '../LICENSE' }
  s.author           = { 'Your Company' => 'email@example.com' }
  s.source           = { :path => '.' }
  s.source_files     = 'Classes/**/*'
  s.dependency       'Flutter'
  s.dependency       'SwiftyJSON'
  s.platform         = :ios, '12.3'

  # Flutter.framework does not contain an i386 slice.
  s.pod_target_xcconfig = {
    'DEFINES_MODULE' => 'YES',
    'EXCLUDED_ARCHS[sdk=iphonesimulator*]' => 'i386'
  }

  s.swift_version    = '5.2'

  # Combine xcconfig for all frameworks
  s.xcconfig = {
    'OTHER_LDFLAGS' => '-framework YCProductSDK -framework JLDialUnit -framework ZipZap -framework JL_BLEKit -framework JL_OTALib -framework JL_AdvParse -framework JL_HashPair -framework DFUnits -framework RTKLEFoundation -framework RTKOTASDK'
  }

  # Include vendored frameworks
  s.vendored_frameworks = [
    'YCProductSDK.framework',
    'JLDialUnit.framework',
    'ZipZap.framework',
    'JL_BLEKit.framework',
    'JL_OTALib.framework',
    'JL_AdvParse.framework',
    'JL_HashPair.framework',
    'DFUnits.framework',
    'RTKLEFoundation.framework',
    'RTKOTASDK.framework'
  ]

end
