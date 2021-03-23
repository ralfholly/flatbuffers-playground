from conans import ConanFile, CMake, tools

class FlatbuffersPlayground(ConanFile):
    name        = "flatbuffers-playground"
    version     = "0.0.1"
    generators  = "cmake_find_package", "cmake_paths"
    settings    = "os", "compiler", "build_type", "arch"

    def requirements(self):
        self.requires("flatbuffers/1.12.0")
        self.requires("flatc/1.12.0")

    def configure(self):
        pass

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        pass

    def package_info(self):
        pass

    def imports(self):
        pass

    def build_directory(self):
        pass
