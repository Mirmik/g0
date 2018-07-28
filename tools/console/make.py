#!/usr/bin/env python3
#coding: utf-8

import licant
from licant.cxx_modules import application
from licant.libs import include

licant.libs.include("g0")
licant.libs.include("g1")
licant.libs.include("gxx")

application("target", 
	sources = ["main.cpp"],
	include_modules = [
		("g1"),
		("g1.allocator", "malloc"),
		("g1.time", "chrono"),
	
		("g0"),
		
		("gxx", "posix"),
		("gxx.log2", "impl"),
		("gxx.inet", "posix"),
		("gxx.print", "cout"),
		("gxx.dprint", "cout"),
		("gxx.syslock", "mutex"),
	],
	cxx_flags = "",
	libs = ["pthread"],
	include_paths = ["."]
)

licant.ex("target")