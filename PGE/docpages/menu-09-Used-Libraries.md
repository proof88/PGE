\page pge_used_libs Used 3rd Party Libraries

[TOC]

This page lists the 3rd party libraries used primarily by PGE, excluding those used by PURE.  
3rd party libraries used primarily by PURE are listed in [PURE's own documentation](https://proof88.github.io/pure-doc/index.html).

When choosing a 3rd party library to be used in my project, there are some **general requirements that must be satisfied by the lib**:
 - it shall be cross-platform, including Android (because I have plans for releasing to Android phones);
 - its license shall allow me to use it with my GPL-3.0/LGPL-3.0 software;
 - it shall be still maintained, so that at least severe security issues get fixed over time, and also I can apply stricter compiler warning/error settings when building the lib with recent C++ compilers;
 - unit tests and/or other tests are there so I can see the developer is doing some form of automated verification;
 - there are some actual users of the library, so I'm not the first human trying to actually use it;
 - C++ interface is highly appreciated over C.
 
So, for example, if there is a nice image loading library, working fine but not updated since 2004, then sorry but it is a no-go for me.

\section pge_used_libs_memory Memory

Vincenzo Barbato's [blIteratorAPI](https://github.com/navyenzo/blIteratorAPI) is used to easily add STD-style iterators to my custom containers like `PgeObjectPool`.  
Note that I cannot enable all functionality due to the compiler warnings with recent VC++ compiler, in the future I need to look into this.

\section pge_used_libs_audio Audio

The selected 3rd party library is elaborated on [this sub-page](#sound).

\section pge_used_libs_networking Networking

The selected 3rd party library is elaborated on [this sub-page](#networking).
