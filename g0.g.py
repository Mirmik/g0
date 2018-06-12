#!/usr/bin/env python3
#coding: utf-8

import licant
from licant.modules import module
from licant.libs import include

include("g1")

module("g0", 
	sources = ["g0/src/core.cpp"],
	include_paths = ["."],
)
