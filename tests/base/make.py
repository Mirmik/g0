#!/usr/bin/env python3
#coding: utf-8

import licant
from licant.cxx_modules import application
from licant.libs import include

licant.libs.include("gxx")
<<<<<<< HEAD
licant.libs.include("g0")
=======
>>>>>>> 08a606d12e8d590f6fb295ab9b8b036946e70521
licant.libs.include("g1")

application("target", 
	sources = ["main.cpp"],
	include_modules = [
		("g1"),
		("g1.allocator", "malloc"),
		("g1.time", "chrono"),
	
		("g0"),
		
		("gxx", "posix"),
		("gxx.inet", "posix"),
		("gxx.print", "cout"),
		("gxx.dprint", "cout"),
		("gxx.syslock", "mutex"),
<<<<<<< HEAD
=======
		("gxx.atomic_section", "mutex"),
>>>>>>> 08a606d12e8d590f6fb295ab9b8b036946e70521
	],
	cxx_flags = ""
)

licant.ex("target")