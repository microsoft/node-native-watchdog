{
  "targets": [
    {
      "target_name": "watchdog",
      "sources": [ "src/watchdog.cc" ],
      "cflags": ["-O2", "-D_FORTIFY_SOURCE=2"],
      'msvs_configuration_attributes': {
        'SpectreMitigation': 'Spectre'
      },
      'msvs_settings': {
        'VCCLCompilerTool': {
          'AdditionalOptions': [
            '/guard:cf',
            '/sdl',
            '/W3',
            '/ZH:SHA_256'
          ]
        },
        'VCLinkerTool': {
          'AdditionalOptions': [
            '/DYNAMICBASE',
            '/guard:cf'
          ]
        }
      }
    }
  ]
}
