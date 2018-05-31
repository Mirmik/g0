#!/usr/bin/env python3

from licant.cxx_modules import application
from licant.modules import submodule
from licant.libs import include
import licant

#include("gxx")
include("g0")

application("target",
	sources = ["main.cpp"],
	include_paths = ["."],

	include_modules = [
		submodule("g0.base"),
		submodule("g0.inet"),
		submodule("gxx", "posix"),
		submodule("gxx.print", "cout"),
		submodule("gxx.dprint", "cout"),
		submodule("gxx.inet", "posix"),
	]
)

licant.ex("target")