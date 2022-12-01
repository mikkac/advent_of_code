from conans import ConanFile, CMake

class PocoTimerConan(ConanFile):
    requires = "spdlog/1.10.0"
    generators = "cmake"
    
    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()