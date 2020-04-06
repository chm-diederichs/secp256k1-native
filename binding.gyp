{
  'variables': {
    'target_arch%': '<!(node preinstall.js --print-arch)>'
  },
  'targets': [
    {
      'target_name': 'secp256k1',
      'include_dirs' : [
        'deps/secp256k1.gyp:secp256k1'
      ],
      'sources': [
        './binding.c',
      ],
      'xcode_settings': {
        'OTHER_CFLAGS': [
          '-g',
          '-O3',
          '-Wall',
          '-pedantic-errors',
        ]
      },
      'cflags': [
        '-g',
        '-O3',
        '-Wall',
        '-pedantic-errors',
      ],
      'libraries': [
        '<!(node preinstall.js --print-lib)'
      ],
      'conditions': [
        ['OS != "mac" and OS != "win"', {
          'link_settings': {
            'libraries': [ "-Wl,-rpath=\\$$ORIGIN"]
          }
        }],
        ['OS=="win"', {
          'link_settings': {
            'libraries': [
              '-lws2_32.lib'
            ]
          }
        }]
      ],
    }
  ]
}
