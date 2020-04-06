{
  'variables': {
    'target_arch%': '<!(node preinstall.js --print-arch)>'
  },
  'targets': [
    {
      'target_name': 'secp256k1',
      'include_dirs' : [
        'secp256k1/include'
      ],
      'sources': [
        'binding.c',
        'secp256k1/src/modules/ecdh/main_impl.h'
      ],
      'xcode_settings': {
        'OTHER_CFLAGS': [
          '-g',
          '-O3',
          '-Wall',
          '-pedantic-errors',
        ]
      },
      'defines': [
        'ENABLE_MODULE_ECDH=1',
        'ENABLE_MODULE_RECOVERY=1',
        'ENABLE_EXPERIMENTAL'
      ],
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
