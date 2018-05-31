from licant.modules import module, submodule
from licant.libs import include

include("gxx")

module("g0.base",
	include_paths = ["."],
	sources = [
		"./g0/src/service.cpp",
		"./g0/src/test.cpp",
	],

	include_modules = [
		submodule("gxx.include")
	],
)

module("g0.inet",
	sources = [
		"./g0/src/udpgate.cpp",
	],
)