\page sound Sound

[TOC]

\section sound_intro Intro

PGE uses the [SoLoud](https://github.com/jarikomppa/soloud) sound library.  
Before selecting it, I spent some time with discovering about sounds libraries.  
The following requirements I had towards the sound library during the study:
 - shall be free;
 - shall be cross-platform, preferably with Android-support as well;
 - preferably with C++ API;
 - shall be able to play a few popular sound formats that games usually use;
 - 3D audio support;
 - shall have some development or maintenance support still nowadays, to make sure it will be compatible with future platforms;
 - easy to compile, shall not have many dependencies;
 - easy to integrate into PGE.
 
\section sound_libs Sound Libraries

I made some browsing and study instead of just going straight with the nice and popular [FMOD library](https://www.fmod.com/) which is non-free nowadays.  
Big lists of sound libraries can be found at these places:
 - https://www.reddit.com/r/gamedev/comments/4wlp2n/whats_the_best_crossplatform_audio_lib_these_days/
 - http://kktechkaizen.blogspot.com/2015/12/cross-platform-audio-libraries.html
 
Unfortunately, most of them doesn't have out-of-the-box 3D-audio support (I'm not talking about multiple channels support e.g. 5.1, I'm saying that they don't support spatial positioning of 3D Audio and play them according to a listener position).  
If you already use [SDL](https://github.com/libsdl-org/SDL) then it might be a good idea to go with it for audio support as well (I think it also supports 3D positional audio).

For me, it boiled down to the following 3 libraries:
 - **SoLoud**: https://github.com/jarikomppa/soloud
 - **JUCE**: https://github.com/juce-framework/JUCE
 - **OpenAL Soft**: https://github.com/kcat/openal-soft

I chose **SoLoud** because it has a nice C++ interface and documentation. By the time I'm writing this wiki, the last release of the lib was in 2020, which I agree was 2 years ago, but since then still lots of commits have been done to the lib.  
**JUCE** is still maintained but it is a bigger framework not only for audio but for other stuff as well like graphics.   
**OpenAL Soft** has pure C interface as I can see it, and I didn't like how the source code of example programs looked like.

