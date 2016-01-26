{
    "targets": [{
        "target_name": "jvm",
        "cflags": ["-std=c++11"],
        "sources": ["jvm.cc", "include/jvm_handler.cc"],

        'conditions': [
            ['OS=="linux"', {
                    "include_dirs": [
                        'include/',
                        'java/include/',
                        'java/include/linux',
                        "<!(node -e \"require('nan')\")",
                    ],
                    'libraries': [
                        '-Ljava/jre/lib/amd64/server/', '-Linclude/'
                    ],
                },

                'OS=="mac"', {
                    "include_dirs": [
                        'libs/',
                        'include/',
                        '/Library/Java/JavaVirtualMachines/jdk1.8.0_72.jdk/Contents/Home/include/darwin',
                        "<!(node -e \"require('nan')\")",
                    ],
                    'libraries': [
                        '-L/Library/Java/JavaVirtualMachines/jdk1.8.0_72.jdk/Contents/MacOS/libjli.dylib', "-framework",
                    ],
                    'xcode_settings': {
                        'OTHER_CPLUSPLUSFLAGS': ['-std=gnu++11'],
                    },
                }
            ]
        ]
    }]
}
