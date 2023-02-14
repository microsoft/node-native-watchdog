{
  "targets": [
    {
      "target_name": "watchdog",
      "sources": [ "src/watchdog.cc" ],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'AdditionalOptions': [
            '/Qspectre',
            '/guard:cf',
            '/w34244',
            '/w34267',
            '/ZH:SHA_256'
          ]
        },
        'VCLinkerTool': {
          'AdditionalOptions': [
            '/guard:cf'
          ]
        }
      }
    }
  ]
}
