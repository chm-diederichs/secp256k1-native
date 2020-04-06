  
{
  'variables': {
    'target_arch%': '<!(node preinstall.js --print-arch)>'
  },
  'target_default': {
    'cflags': [
      '-Wall',
      '-Wextra',
    ],
  },
  'targets': [
    {
      'target_name': 'secp256k1',
      'sources': [
        'secp256k1',
        'binding.c'
      ],
      'include_dirs': [
        'secp256k1/include',
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
        '-Wno-unused-function',
        '-Wno-nonnull-compare',
        '-g',
        '-O3',
        '-Wall',
        '-pedantic-errors'
      ],
      'defines': [
        'ECMULT_GEN_PREC_BITS=4',
        'ECMULT_WINDOW_SIZE=15',

        'ENABLE_MODULE_ECDH=1',
        'ENABLE_MODULE_RECOVERY=1',

        'USE_ENDOMORPHISM=1',

        'USE_NUM_NONE=1',
        'USE_FIELD_INV_BUILTIN=1',
        'USE_SCALAR_INV_BUILTIN=1',
      ],
      'libraries': [
        '<!(node preinstall.js --print-lib)'
      ],
      'conditions': [
        ['target_arch=="x64" and OS!="win"', {
          'defines': [
            'HAVE___INT128=1',
            'USE_ASM_X86_64=1',
            'USE_FIELD_5X52=1',
            'USE_SCALAR_4X64=1',
          ]
        }, {
          'defines': [
            'USE_FIELD_10X26=1',
            'USE_SCALAR_8X32=1',
          ]
        }],
                ['OS != "mac" and OS != "win"', {
          'link_settings': {
            'libraries': [ "-Wl,-rpath=\\$$ORIGIN"]
          }
        }]
      ],
    }
  ],
}


