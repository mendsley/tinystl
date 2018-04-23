tinystl
=======
Tiny (as in minimal) implementation of some core STL functionality

Contact
-------
[@MatthewEndsley](https://twitter.com/#!/MatthewEndsley)  
<https://github.com/mendsley/tinystl>

License
-------
Copyright 2012-2018 Matthew Endsley

This project is governed by the BSD 2-clause license. For details see the file
titled LICENSE in the project root folder.

Compiling
---------
tinystl is a header only library. But there's some tests that need to be compiled if you want to run them.

1. Get the premake4 binary here: <https://premake.github.io/download.html>
2. It's one file, put it somewhere useful! (maybe we'll include it in tinystl later on)
3. Update git submodules: $ git submodule update --init
4. Generate project files
		premake5 vs2017
5. Open your project file. It's in tinystl/.build/projects/
6. Enjoy a tasty beverage
