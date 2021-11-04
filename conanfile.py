from conans import ConanFile, CMake
from conans.errors import ConanInvalidConfiguration


class ReplayConan(ConanFile):
    name = "replay"
    version = "2.8"
    license = "MIT"
    author = "Marius Elvert marius.elvert@googlemail.com"
    url = "https://github.com/ltjax/replay"
    description = "Lean library to observe file changes in a specific directory path."
    topics = ("games", "math")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "use_stb": [True, False], "use_libpng": [True, False]}
    generators = "cmake"
    exports_sources = "source/*", "test/*", "include/*", "CMakeLists.txt", "replay.natvis"
    requires = "boost/1.75.0",
    build_requires = "catch2/2.13.4",
    default_options = {
      "shared": False,
      "use_stb": True,
      "use_libpng": False,
      "boost:header_only": True,
    }
    
    def _configured_cmake(self):
        cmake = CMake(self)
        cmake.configure(source_folder=".", defs={
            'Replay_USE_CONAN': True,
            'Replay_ENABLE_INSTALL': True,
            'Replay_USE_STBIMAGE': self.options.use_stb,
            'Replay_USE_STBIMAGE_WRITE': self.options.use_stb,
            'Replay_USE_STBIMAGE_FROM_CONAN': self.options.use_stb,
            'Replay_USE_LIBPNG': self.options.use_libpng})
        return cmake

    def configure(self):
        if self.options.use_libpng and self.options.use_stb:
            raise ConanInvalidConfiguration("Cannot use both stb and libpng")

    def requirements(self):
        if self.options.use_stb:
            self.requires("stb/20190512@conan/stable")
        if self.options.use_libpng:
            self.requires("libpng/1.6.37@bincrafters/stable")

    def build(self):
        self._configured_cmake().build()
    
    def package(self):
        self._configured_cmake().install()
    
    def package_info(self):
        self.cpp_info.libs = ["replay"]
        if self.options.use_stb:
            self.cpp_info.defines.extend(["REPLAY_USE_STBIMAGE", "REPLAY_USE_STBIMAGE_WRITE"])
