from licant.modules import module, submodule
from licant.libs import include

include("gxx")

module("g0.base",
	include_paths = ["include"],
	sources = [
		"src/service.cpp",
		"src/network.cpp",
		"src/test.cpp",
	],

	include_modules = [
		submodule("gxx.include")
	],
)

module("g0.inet",
	sources = [
		"src/udpgate.cpp",
	],
)