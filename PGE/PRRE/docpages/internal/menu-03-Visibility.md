\page visibility Visibility Optimizations

[TOC]

Usually the rendering speed defines the overall speed of a game due to its complexity compared to other parts that also need processing (sounds, input, etc.).  
There are 2 major ways to increase rendering speed:
 - do not render objects that would end up not visible in the final image anyway;
 - reduce quality of rendering without reducing the quality of the final image.

There are numerous ways to decide if a 3D object or part of it is visible from the player's point of view or not.  
**There is not a single best way to do it but different methods** that are operated at different stages of the rendering pipeline, and should be used together for best performance.  
We also call these as **Hidden Surface Removal** techniques or **Culling**.  
There are some other simple ways as well like defining maximum rendering distance of an object to the viewer.
On this page I'm mentioning all techniques I have read about although not all of them are implemented in PURE engine. This is always explicitly noted.

I'm grouping techniques into 2:
 - pre-pipe;
 - in-pipe.
 
Pre-pipe techniques do not need the graphics pipeline to be executed to decide if an object can be ignored for rendering or not. To maximize speed, we want to make use of such techniques
as much as we can.  
In-pipe techniques operate within the graphics pipeline because they need data for their visibility decisions that are produced within the pipeline. Even if an object is sent to the pipeline,
it is still better to cull it in an earlier than in a later stage.

\section prepipe Pre-Pipe Techniques

\subsection structures Spatial Hierarchies / Acceleration Structures

Following few hierarchies can be used to avoid doing visibility tests on single objects, but instead test against group of objects, for more efficient decision making.  
They can also provide a recommended order of iterating over objects or polygons. Although these are basically structures, they can be combined with visibility decision maker
algorithms such as \ref vfc for more efficiency.

\subsubsection pvs PVS: Potentially Visible Set

PURE engine does not use PVS.  
The scene is divided into cells, and for every single cell, we list all other visible cells from that cell. This is a preprocessing step. During real-time rendering, we just need to decide
in which cell the camera is currently in and render only those objects that are in the visible cells. This technique can be used independent of and prior to feeding the graphics pipeline, thus
it can save a lot of time because we can skip sending objects to the pipeline. Although this technique requires preprocessing, this can be used in realtime mobile games too.  

\subsubsection bsp BSP: Binary Space Partitioning

PURE engine does not use BSP.  
We recursively divide the scene into two until some requirements are satisfied. For example, we can place polygons left or right in the tree based on if they are in front of or behind a plane,
and do this recursively. As a result, we can traverse the tree that gives back-to-front or front-to-back order of polygons from view position, and send them in such order into the rendering pipeline,
enabling proper occlusion of polygons even without using a depth buffer! This was actually used in games for the painter's algorithm before depth buffering widespread.  
Note that BSPs are still used nowadays for different purposes such as helping generating \ref pvs.

\subsubsection quadtree Quadtree

TODO  
PURE engine does not use Quadtree.  
We insert scene objects into a tree structure, where each node can have exactly 4 children nodes. Quadtree is the two-dimensional analog of \ref octree and is most often used to partition a
two-dimensional space by recursively subdividing it into four quadrants or regions. 

\subsubsection octree Octree

TODO  
PURE engine does not use Octree.  
We insert scene objects into a tree structure, where each node can have exactly 8 children nodes. A node is considered as a cubic volume. Its 8 children equally partition it.  
When we insert a new object in the tree, we do it in a top-down fashion, so we start from the root node and progress down to the leaves. If the current node is a leaf and doesn't contain any object yet,
we insert the new object into it. Otherwise if it already contains an object, we split that node into 8 cells and insert both the already contained and the new objects in the proper cells.
Note that an object can be placed into multiple cells at the same time if it overlaps multiple cells.

##### Loose Octree

TODO  
PURE engine does not use Loose Octree.  

https://sakura7.blog.hu/2010/07/05/loose_octree  
http://www.tulrich.com/geekstuff/partitioning.html

\subsubsection bvh BVH: Bounding Volume Hierarchy

TODO PURE engine does not support this.  
The bounding volumes of objects are put in a hierarchical tree.  
With the help of an octree, we can easily build the hierarchy.  
First insert the objects into the octree as described earlier.  
Then we traverse the tree from bottom to top: starting with the leaves, we calculate the bounding boxes of the objects contained by each of the leaves. Then, 1 level above, we calculate the overall
bounding volumes for the node grouping all its children together. We recursively progressing up towards the root node, where the biggest bounding volume containing all previously calculated
bounding volumes is created.  
With the help of such spatial hierarchy, a lot of operations such as ray intersection tests will take much less time, since we start the tests with the root node, and continue the testing with
children bounding volumes only if the test passed.

https://www.scratchapixel.com/lessons/advanced-rendering/introduction-acceleration-structure/bounding-volume-hierarchy-BVH-part2  
https://github.com/LeoGuo98/csc418-A4-bounding-volume-hierarchy  
https://github.com/brandonpelfrey/Fast-BVH  
https://github.com/leonardo-domingues/atrbvh  
https://github.com/taqu/BoundingVolumeHierarchy  
https://github.com/mirkoalicastro/bounding-volume-hierarchy  
https://github.com/GrandPiaf/Raytracer

\subsubsection portal Portal

PURE engine does not use portals.  
Portal is a surface that connects 2 adjacent 3D spaces that are visible from each other through this surface, e.g. a door connecting 2 rooms. So a portal is the inverse of an occluder because
it tells which polygons/objects are definitely visible behind it! Suitable for indoor maps only and needs preprocessing for identifying portals or level designer needs to mark portals.

\subsubsection antiportal Antiportal

PURE engine does not use antiportals.  
The inverse of a portal. A surface that is technically an occluder, with a shape of a rectangle. This rectangle is considered as the "near plane" of a view frustum, and objects falling into this
frustum can be culled.  
Needs no or less preprocessing compared to portals. Can be used in both in- and outdoor maps. Needs already existing \ref structures though to be efficient.

\subsection lod LOD: Level of Detail

PURE engine does not support this.  
This is not about culling the object but to reduce its processing time without causing any image quality degradation.  
If an object consists of thousand of polygons but is far from the viewer, there is not much use of rendering such complex geometry that would finally end up as just a few pixels due to its
distance from the camera. Still we need to render something because the object is visible for sure. In such cases it is reasonable to render a lower level of geometry, which won't cause
any noticeable difference in the rendered image. There can be multiple levels of detail of an object, where the first level is the original geometry with 10000 vertices, the second level
is a reduced complexity level with 3000 vertices, and so on, and the LOD level is chosen automatically based on the object's distance to the viewer.  
There are other methods such as dynamic level of detail for terrain, where no precalculated reduced level of geometry is stored.

\subsection rendering_distances Minimum and Maximum Rendering Distance

TODO PURE engine does not support this.  
Sometimes it is reasonable to define minimum and/or maximum rendering distance for specific objects, e.g. tiny pickupable items, that are not needed to be rendered if they are too far from
the viewer. Note that this is not related to camera's near and far clipping plane settings!

\section inpipe In-Pipe Techniques

\subsection vfc VFC: View Frustum Culling

This is done in Clip Space of the \ref geometry_stage of the graphics pipeline.  
Although low-level HW rendering APIs such as OpenGL implement clipping planes and automatically do the primitive clipping or culling for us in Clip Space, there are still disadvantages
of just relying on this simple way of culling:
 - with many objects in the scene, many objects are needed to be transformed into Clip Space one-by-one to decide if they can be culled away;
 - this also means many objects are needed to be checked against the clipping planes.

To speed things up, we can use the bounding box of such object instead of its full geometry and test that against the clipping planes. If the bounding box is completely outside the view frustum
we can cull the object thus we could win some time by not transforming its complex geometry from model-space to clip-space. More on this in \ref architecture.  
With this approach, we manage to save on transforming complex geometries, because we just transform their bounding boxes to decide if we should cull them. But still, does
this bring actual speedup? With modern GPUs the bottleneck is usually NOT the transformation of complex geometries but rasterizing them. This depends on how complex shading programs are
bound to them. Also, stalling the rendering pipeline by introducing the check on CPU side if the bounding box is within the view frustum or not brings in slowdown.

http://www.lighthouse3d.com/tutorials/view-frustum-culling/  
https://www.gamedev.net/tutorials/programming/general-and-gameplay-programming/frustum-culling-r4613/  
https://bruop.github.io/frustum_culling/  
http://www.cse.chalmers.se/~uffe/vfc_bbox.pdf  
https://cesium.com/blog/2015/08/04/fast-hierarchical-culling/  
https://www.researchgate.net/publication/2803988_Optimized_View_Frustum_Culling_Algorithms_for_Bounding_Boxes  

\subsubsection hvfc Hierarchical View Frustum Culling

TODO PURE uses this technique.  
Same as VFC but 3D objects are organized into some kind of \ref structures such as \ref bvh. This way we check if bigger bounding volumes are in the view frustum - if not then
all objects within that bounding volume can be easily culled without further testing. This reduces the number of bounding box tests even with many objects. Less testing leads to more speed.

\subsection occlusion Occlusion Culling

Although I put this method after VFC since this needs later stage of the rendering pipeline as well to operate (i.e. depth-testing of an object bounding box), it is not necessarily worse
than VFC. It can actually replace VFC, and if implemented in a sophisticated way such as CHC, it can also outperform it. For CHC, and details of Occlusion Culling, a specific page is
maintained: \subpage occlusion_culling.

Summary: somehow we can decide if an object is occluded by another object or not. If so, we can skip it.  

\subsubsection h_occlusion Hierarchical Occlusion Culling

TODO PURE uses hierarchical HW occlusion culling.  
Same as occlusion culling but as like as \ref hvfc, we take advantage of organizing 3D objects into \ref structures, so we might be able to do less occlusion testing by starting initial
occlusion queries for the bigger spatial groups such as bounding volumes first.  
Note that details are not described here but on the own page of \ref occlusion_culling.

\subsection backface Backface Culling

PURE does HW Backface Culling.  
A primitive can be discarded based on its facing. More on this in \ref architecture.  
This is done in the \ref raster_stage which is pretty late in the pipeline, so culling an object at this point can save fill rate only.

\subsection depth_buffering Depth Buffering

PURE does HW Depth Buffering.  
This is not specifically for optimizing rendering speed but to decide if a pixel should be written to the frame buffer or not.  
Happens at the very end of the rendering pipeline thus not much speed can be gained with it.  
More on this in Architecture.



