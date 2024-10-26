from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout


class ReplayConan(ConanFile):
    name = "replay"
    version = "2.10"

    # Metadata
    license = "MIT"
    author = "Marius Elvert marius.elvert@googlemail.com"
    url = "https://github.com/ltjax/replay"
    description = "lightweight game development utility library."
    topics = ("games", "math")
    
    # Building
    exports_sources = "source/*", "test/*", "include/*", "CMakeLists.txt", "replay.natvis"
    test_requires = "catch2/2.13.10",

    # Configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "use_stb": [True, False], "use_libpng": [True, False], "fPIC": [True, False]}
    default_options = {
      "shared": False,
      "use_stb": True,
      "use_libpng": False,
      "boost/*:header_only": True,
      "fPIC": True
    }

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()

        toolchain = CMakeToolchain(self)
        toolchain.variables['Replay_USE_CONAN'] = True
        toolchain.variables['Replay_ENABLE_INSTALL'] = True
        toolchain.variables['Replay_USE_STBIMAGE'] = self.options.use_stb
        toolchain.variables['Replay_USE_STBIMAGE_WRITE'] = self.options.use_stb
        toolchain.variables['Replay_USE_STBIMAGE_FROM_CONAN'] = self.options.use_stb
        toolchain.variables['Replay_USE_LIBPNG'] = self.options.use_libpng
        toolchain.generate()

    def configure(self):
        if self.options.use_libpng and self.options.use_stb:
            raise ConanInvalidConfiguration("Cannot use both stb and libpng")

    def requirements(self):
        self.requires("boost/1.75.0", transitive_headers=True)
        if self.options.use_stb:
            self.requires("stb/cci.20200203")
        if self.options.use_libpng:
            self.requires("libpng/1.6.44")

    def build(self):        
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
    
    def package(self):
        cmake = CMake(self)
        cmake.install()
    
    def layout(self):
        cmake_layout(self)

    def package_info(self):
        self.cpp_info.libs = ["replay"]
        if self.options.use_stb:
            self.cpp_info.defines.extend(["REPLAY_USE_STBIMAGE", "REPLAY_USE_STBIMAGE_WRITE"])
