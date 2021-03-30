
# Introduction
This documentation describes how [my engine](https://github.com/proof88/PGE/) works and explains relevant technologies.  
You are reading the **internal documentation** which contains info about both the API usage and its implementation.  
If that is too much for you, you can always generate an **external documentation** with DoxyGen using the [PURE_Doxyfile_external config file](https://github.com/proof88/PGE/tree/master/PGE/Docs).  
I wrote this documentation with easy online Markdown editor [StackEdit](https://stackedit.io/).

## Motivation
Since my mid-childhood (~13 years old), I have always wanted to create not only PC games but also a 3D graphics engine. Inspired by the success of id Software’s [Quake III engine (id Tech 3)](https://en.wikipedia.org/wiki/Id_Tech_3), I am making my own now. The aim is to create & continuously develop a cross-platform codebase giving nice image quality with strong performance on newer hardware while maintaining compatibility with older hardware as well. The latter is important for me since I am interested in learning old-school techniques from the age of the first 3D-accelerators. :) Some parts of the engine was written from ground zero, some other parts are open-source libraries utilized by the engine.

## Copy & Paste
I quote extensively from ExtremeTech’s 3D Pipeline Tutorial article and other sources. I am not marking such copy-pasted sentences or paragraphs one-by-one with citation marks, but at the same time I am NOT declaring the content of this documentation to be 100% my own intellectual product.  
The text was formed by putting together parts of different articles and my additions into logical order so that it gives a good structured knowledge base in this area, especially for me if I happen to forget some of it a few months later. :)

## Sources
This documentation contains intellectual properties of various sources, listed below:

 - [https://www.opengl.org/wiki](https://www.opengl.org/wiki)
 - [https://www.extremetech.com/computing/49076-extremetech-3d-pipeline-tutorial](https://www.extremetech.com/computing/49076-extremetech-3d-pipeline-tutorial)
 - [https://fgiesen.wordpress.com/2011/07/09/a-trip-through-the-graphics-pipeline-2011-index/](https://fgiesen.wordpress.com/2011/07/09/a-trip-through-the-graphics-pipeline-2011-index/)
 - [http://www.songho.ca/opengl/index.html](http://www.songho.ca/opengl/index.html)
 - [http://www.techpowerup.com/gpudb/](http://www.techpowerup.com/gpudb/)
 - [Sammy Rogmans: A Generic Framework for Implementing Real-Time Stereo Matching Algorithms on the Graphics Processing Unit](https://www.researchgate.net/publication/265310715_A_Generic_Framework_for_Implementing_Real-Time_Stereo_Matching_Algorithms_on_the_Graphics_Processing_Unit)
 - [http://developer.amd.com/wordpress/media/2012/10/Depth_in-depth.pdf](http://developer.amd.com/wordpress/media/2012/10/Depth_in-depth.pdf)
 - [https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_BasicsTheory.html](https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_BasicsTheory.html)
 - [http://www.terathon.com/gdc07_lengyel.pdf](http://www.terathon.com/gdc07_lengyel.pdf)
 - [http://www.ozone3d.net/tutorials/gpu_sm3_dx9_3d_pipeline_p02.php](http://www.ozone3d.net/tutorials/gpu_sm3_dx9_3d_pipeline_p02.php)
 - [https://en.wikipedia.org/wiki/HyperZ](https://en.wikipedia.org/wiki/HyperZ)


<!--stackedit_data:
eyJwcm9wZXJ0aWVzIjoidGl0bGU6IFBVUkUgR2xvc3Nhcnlcbm
F1dGhvcjogUFIwMEY4OFxuZGF0ZTogJzIwMjAtMTEtMjInXG4i
LCJoaXN0b3J5IjpbLTcxMDI1MDE3NiwyNjU4MzIxODcsLTk5Mz
A2NjY4NV19
-->