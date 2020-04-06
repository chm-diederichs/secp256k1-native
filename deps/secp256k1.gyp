{

  'target_defaults': {
    'defines': [
      'POSIX'
    ],
  },
  'targets': [
    {
      'target_name': 'secp256k1',
      'type': 'static_library',
      'include_dirs' : [
        'secp256k1/include'
      ],
      'sources': [
        'secp256k1/src/modules/ecdh/main_impl.h'
      ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'CLANG_CXX_LANGUAGE_STANDARD': 'c++98',
            'OTHER_CFLAGS': [
              '-g',
              '-O3',
              '-Wall',
              '-pedantic-errors'
            ],
          },
        }],
      ],
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
    },
  ],
