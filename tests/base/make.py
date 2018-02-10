#!/usr/bin/env python3

from licant.cxx_modules import application, doit

from licant.libs import include
from licant.modules import submodule

include("g0")

application("target",
	sources = ["main.cpp"],

	include_modules = [
		submodule("g0.base"),
	]
)

doit("target")