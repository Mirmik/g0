from licant.modules import module, submodule
from licant.libs import include

include("gxx")

module("g0.base",
	include_paths = ["include"],
	sources = [
		"src/service.cpp",
		"src/test.cpp",
	],
	include_modules = [
		submodule("gxx.include")
	],
)