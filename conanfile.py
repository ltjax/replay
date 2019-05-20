from conans import ConanFile, CMake, tools


class ReplayConan(ConanFile):
    name = "replay"
    version = "1.0"
    license = "MIT"
    author = "Marius Elvert marius.elvert@googlemail.com"
    url = "https://github.com/ltjax/replay"
    description = "Lean library to observe file changes in a specific directory path."
    topics = ("games", "math")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"
    generators = "cmake"
    exports_sources = "source/*", "test/*", "include/*", "CMakeLists.txt"
    requires = "boost_filesystem/1.69.0@bincrafters/stable", "boost_iostreams/1.69.0@bincrafters/stable",\
       "boost_random/1.69.0@bincrafters/stable", "boost_test/1.69.0@bincrafters/stable", "boost_assign/1.69.0@bincrafters/stable" # Only needed for tests right now
    
    def _configured_cmake(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".", defs={'Replay_USE_CONAN': 'ON'})
        return cmake

    def build(self):
        self._configured_cmake().build()
    
    def package(self):
        self._configured_cmake().install()
    
    def package_info(self):
        self.cpp_info.libs = ["replay"]
