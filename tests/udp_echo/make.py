#!/usr/bin/env python3

from licant.cxx_modules import application, doit
from licant.modules import submodule
from licant.libs import include

#include("gxx")
include("g0")

application("target",
	sources = ["main.cpp"],
	include_paths = ["."],

	include_modules = [
		submodule("g0.base"),
		submodule("g0.inet"),
		submodule("gxx"),
		submodule("gxx.print", "cout"),
		submodule("gxx.dprint", "cout"),
		submodule("gxx.inet", "unix"),
	]
)

doit("target")