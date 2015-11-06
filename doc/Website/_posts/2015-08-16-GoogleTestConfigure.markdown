---
layout: document
title: "Google Test Configure"
date: 2015-08-16 14:41:58
categories: claims
---
The following instructions guide you to use gtest in Claims.

1.Download gtest-1.7.0.zip
--

You can either download it from [https://code.google.com/p/googletest/downloads/list](https://code.google.com/p/googletest/downloads/list), or from imdb@:10.11.1.199:/home/imdb/repo.

2.Decompress gtest-1.7.0.zip
--

	    [~]$ unzip gtest-1.7.0.zip

3.Build
--

The aim of build is to create libgtest.a.

	    [~]$ cd gtest-1.7.0
	    [wangli@Robert gtest-1.7.0]$  g++ -isystem include -I./       -pthread -c src/gtest-all.cc
	    [wangli@Robert gtest-1.7.0]$ ar -rv libgtest.a gtest-all.o

4.Configure the settings of your eclipse
--

add include : /home/imdb/supports/gtest-1.7.0/include

add /home/imdb/supports/gtest-1.7.0/libgtest.a into "GCC C++ Linker-->Miscellaneous-->Other object"

	    If you have any question, do not hesitate to contract us. Email: wangli1426@gmail.com
