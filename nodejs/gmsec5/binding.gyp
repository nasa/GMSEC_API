# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

{
  "targets": [
    {
      "target_name": "gmsec_nodejs",

      "sources" : [
          "interfaces/gmsec_nodejs_wrap.cxx"
      ],

      "conditions": [
          [
              'OS=="win"',
              {
                  'msvs_settings': {
                      'VCCLCompilerTool': {
                          "AdditionalOptions": [
                              "/execution-charset:utf-8",
                              "/source-charset:utf-8",
                              "/EHsc",
                              "/GR"
                          ],
                          'WarningLevel': 4,
                          'ExceptionHandling': 1,
                          'DisableSpecificWarnings': [
                              4100, 4127, 4201, 4244, 4267, 4302, 4311, 4459, 4506, 4611, 4702, 4714, 4512
                          ]
                      },
                  },

                  "include_dirs+": [
                      "../../framework/include",
                      "../../framework/include/gmsec5"
                  ],

                  "link_settings": {
                      "libraries": [ '../../../objects/Release/gmsec_api' ],
                  },
              },

              'OS=="mac"',
              {
                  "cflags_cc": [  '-IRelease/obj.target/include -IRelease/obj.target/include/gmsec5' ],

                  "xcode_settings": {
                      'GCC_ENABLE_CPP_RTTI': 'YES',
                      'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
                      'OTHER_CFLAGS': [ "-Wno-ignored-qualifiers -Wno-cast-function-type -Wno-deprecated-copy -Wno-unused-result -Wno-deprecated-declarations -stdlib=libc++ -std=c++17 -arch x86_64" ],
                  },

                  "include_dirs+": [
                      "$(GMSEC_HOME)/framework/include",
                      "$(GMSEC_HOME)/framework/include/gmsec5"
                  ],

                  "link_settings": {
                      "libraries": [ '-L$(GMSEC_HOME)/bin -lgmsec_api' ],
                  },
              },

              'OS=="linux"',
              {
                  "cflags_cc": [ '-IRelease/obj.target/include -IRelease/obj.target/include/gmsec5 -fexceptions -Wno-ignored-qualifiers -Wno-cast-function-type -Wno-deprecated-copy -Wno-unused-result -Wno-deprecated-declarations -frtti -fPIC' ],

                  "include_dirs+": [
                      "$(GMSEC_HOME)/framework/include",
                      "$(GMSEC_HOME)/framework/include/gmsec5"
                  ],

                  "link_settings": {
                      "libraries": [ '-L$(GMSEC_HOME)/bin -lgmsec_api' ],
                  },
              },
          ],
      ],
    }
  ]
}
