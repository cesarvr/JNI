{
    "targets": [{
        "target_name": "hello",
        "cflags": ["-std=c++11"],
        "sources": ["hello.cc"],
        "include_dirs": [
            'java/include/',
            'java/include/linux',
            "<!(node -e \"require('nan')\")",
        ],
        'libraries': [
            '-Ljava/jre/lib/amd64/server/'
        ]
    }]
}
