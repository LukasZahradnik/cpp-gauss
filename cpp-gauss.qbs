import qbs

Project {
    minimumQbsVersion: "1.7.1"

    CppApplication {
        consoleApplication: true
        files: [
            "main.cpp",
        ]

        cpp.cxxFlags: ["-mavx", "-O3", "-fopenmp"]
        cpp.dynamicLibraries: ["gomp"]
        cpp.optimization: "none"
        cpp.cxxLanguageVersion: "c++11"

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
