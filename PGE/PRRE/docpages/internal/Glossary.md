# Glossary

Intentionally not in alphabetical order.

TODO: add ToC

### Scene

A virtual 2- or 3-dimensional place or space where things happen, e.g. [CJ is chasing some vagos on a train in Los Santos](https://www.youtube.com/watch?v=6y7o3RNgWR8).

### Rendering

Generating an image of a 2- or 3-dimensional scene.

### Pipeline

Sequence of processing tasks arranged so that the output of each task is the input of the next task.

### Graphics / Rendering Pipeline

Software and/or hardware implementation of a [pipeline](#_Pipeline) where the input is a bunch of scene data and the output is an image of the [scene](#_Scene) as a result of the consecutive [rendering](#_Rendering) tasks. More details in [Rendering Pipeline Architecture](#_Rendering_Pipeline_Architecture).

### Pixel

TODO.

### Vertex

TODO.

### Vertex Shader

TODO.

### Pixel / Fragment Shader

TODO.

### Vertex Pipeline / Processor

Execution unit. Executes [vertex shader](#_Vertex_Shader) instructions. 1 pipeline processes 1 vertex.

### Vertex Processing / Shader Unit

SIMD principle. Contains multiple [vertex processors](#_Vertex_Pipeline_/).

### Pixel / Fragment Processor / Pipeline

Execution unit. Executes [pixel / fragment shader](#_Pixel_/_Fragment_1) instructions. 1 pipeline processes 1 [fragment](#_Fragment).

### Fragment Processing / Shader Unit

SIMD principle. Contains multiple [fragment processors](#_Pixel_/_Fragment).

### Unified Shader

TODO. Since Geforce 8xxx / Radeon HD2xxx series.

### Fragment

A screen-space position and some other data like output of a vertex shader. There will be at least one fragment produced for every pixel area covered by the primitive being rasterized.

### Raster Pipeline / ROP / Z-pipe

Execution unit. Executes scissor test, alpha test, stencil test, depth test and blending.

### Raster Operation Unit

Contains multiple [raster pipelines](#_Raster_Pipeline_/).

### Fragment Crossbar

Routes the [fragments](#_Fragment) coming from the [fragment pipelines](#_Pixel_/_Fragment) to the [ROPs](#_Raster_Pipeline_/) (this is needed as the number of [fragment pipelines](#_Pixel_/_Fragment) is not equal to the number of [ROPs](#_Raster_Pipeline_/)).

### Primitive

TODO.

### Texel

TODO.

### Texture

TODO.

### Texture Unit

TODO. The number of texture units defines the maximum number of [textures](#_Texture) accessed at the same time by the same [fragment shader](#_Pixel_/_Fragment_1). **Texture access** is aka **texture lookup** aka **texture fetching**.

### Texture Filtering

TODO.

### Isotropic Filtering

TODO.

### Anisotropic Filtering

TODO.

### Aliasing

TODO.

### Antialiasing, AA

TODO.

### Multisample Antialiasing, MSAA

TODO. The render output units super-sample only the Z buffers and stencil buffers, and using that information get greater geometry detail needed to determine if a pixel covers more than one polygonal object. This saves the pixel/fragment shader from having to render multiple fragments for pixels where the same object covers all of the same sub-pixels in a pixel. This method fails with texture maps which have varying transparency (e.g. a texture map that represents a chain link fence).

#### Quincunx Antialiasing (nVidia)

TODO. A blur filter that shifts the rendered image a half-pixel up and a half-pixel left in order to create sub-pixels which are then averaged together in a diagonal cross pattern, destroying both jagged edges but also some overall image detail.

Since Geforce 3.

#### AccuView Antialiasing (nVidia)

TODO. 4XS. Since Geforce 4.

#### Transparency Antialiasing (nVidia)

TODO. Since Geforce 7.

#### Coverage Sampling Antialiasing, CSAA (nVidia)

TODO. Since Geforce 8.

[http://www.nvidia.com/object/coverage-sampled-aa.html](http://www.nvidia.com/object/coverage-sampled-aa.html)

### Supersampled Antialiasing, SSAA

Render the scene large size internally then scale the result down to the output resolution. Slower than [MSAA](#_MultiSample_Antialiasing,_MSAA).

### Rendering Pipeline Architecture

Next I describe the usual steps in a rendering pipeline that produces a rendered image of a 3D object, with the help of the picture below.

TODO: add picture

source: [http://www.adobe.com/devnet/flashplayer/articles/how-stage3d-works.html](http://www.adobe.com/devnet/flashplayer/articles/how-stage3d-works.html)

First we define the [vertex](#_Vertex) stream by specifying the [vertex](#_Vertex) attributes (eg. position), the storage location of this stream (eg. host memory), and how to interpret the stream ([primitive](#_Primitive) type eg. triangles). Modeling-, view-, and projection transformations on the vertices including optional normals are done, so the [vertices](#_Vertex) are transformed from **object-space to clip-space**. [Primitives](#_Primitive) (e.g. triangles) are assembled from the transformed [vertices](#_Vertex), clipping is applied and then they are further transformed from **clip-space to screen/window-space**. Then we find out which pixels are covered by the incoming triangles, and interpolate [vertex](#_Vertex) attributes across the triangle. These generated values are fragments. Color, depth and stencil values are generated from each [fragment](#_Fragment) with the use of optional textures. The final steps include depth testing, blending. etc.
Following picture is an example of a rendering pipeline architecture implemented in HW. A bit more specific than the previous picture.

TODO: add picture  
  
source: [http://www.ozone3d.net/tutorials/gpu_sm3_dx9_3d_pipeline_p02.php](http://www.ozone3d.net/tutorials/gpu_sm3_dx9_3d_pipeline_p02.php)

Note: as seen in the picture, [Rasterizer](#_Rasterization_1) is not equal to [Raster Operation Unit](#_Raster_Operation_Unit).

In some sense, 3D chips have become physical incarnations of the pipeline, where data flows “downstream” from stage to stage. Computations in various stages of the pipeline can be overlapped, for improved performance. For example, because vertices and pixels are mutually independent of one another in both Direct3D and OpenGL, one triangle can be in the geometry stage while another is in the Rasterization stage. Furthermore, computations on two or more vertices in the Geometry stage and two or more pixels (from the same triangle) in the Rasterzation phase can be performed at the same time.

Another advantage of pipelining is that because no data is passed from one vertex to another in the geometry stage or from one pixel to another in the rendering stage, chipmakers have been able to implement multiple pixel pipes and gain considerable performance boosts using parallel processing of these independent entities. It’s also useful to note that the use of pipelining for real-time rendering, though it has many advantages, is not without downsides. For instance, once a triangle is sent down the pipeline, the programmer has pretty much waved goodbye to it. To get status or color/alpha information about that vertex once it’s in the pipe is very expensive in terms of performance, and can cause pipeline stalls, a definite no-no.

### HW T&L - Hardware Transformation and Lighting

Moving the [vertex](#_Vertex) transformation and lighting calculations from SW (CPU) to HW (GPU).

Since Geforce 256.

PURE implicitly benefits of this thanks to the vendors’ OpenGL implementation.

[http://www.anandtech.com/show/391/5](http://www.anandtech.com/show/391/5)(img_common%5Ctandl.png)
TODO: add picture  

source: [http://www.anandtech.com/show/391/5](http://www.anandtech.com/show/391/5)

#### Pre-Transform (pre-T&L) Cache

Stores the untransformed [vertices](#_Vertex). Optimizations regarding this part of the cache are simply sorting the [vertices](#_Vertex) in order of appearance. Typically extremely large, being able to hold ~64k [vertices](#_Vertex) on a Geforce 3 and up.

#### Post-Transform (post-T&L) Cache

This is a GPU FIFO buffer containing data of [vertices](#_Vertex) that have passed through this stage but not yet converted into [primitive](#_Primitive). Can be used with indexed rendering only (element arrays). 2 [vertices](#_Vertex) are considered equal if their index is the same within the same drawing command. If so, the processing of the current [vertex](#_Vertex) is skipped in this stage and the output of the appropriate previously-processed [vertex](#_Vertex) data is added to the output stream. Varies in size from effectively 10 (actual 16) [vertices](#_Vertex) on GeForce 256, GeForce 2, and GeForce 4 MX chipsets to effectively 18 (actual 24) on GeForce 3 and GeForce 4 Ti chipsets.

AMD Tootle is recommended for optimizing 3D-models: [http://developer.amd.com/tools-and-sdks/archive/legacy-cpu-gpu-tools/amd-tootle/](http://developer.amd.com/tools-and-sdks/archive/legacy-cpu-gpu-tools/amd-tootle/)

There is some other vertex cache optimization: [http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html](http://home.comcast.net/~tom_forsyth/papers/fast_vert_cache_opt.html)

[https://www.opengl.org/wiki/Post_Transform_Cache](https://www.opengl.org/wiki/Post_Transform_Cache)

[http://www.opentk.com/doc/advanced/vertex-cache-optimization](http://www.opentk.com/doc/advanced/vertex-cache-optimization)

### AGP Fast Writes

A method of allowing the CPU to send data directly to the AGP bus without having to use main system memory. Speeds up AGP reads. Doesn’t have effect on AGP writes.

Since Geforce 256.

PURE implicitly benefits if current HW and SW supports Fast Writes.

### HyperZ (ATi)

Z- and Stencil Buffer optimization techniques. See the 3 techniques below.

Since Radeon R100 (7xxx series).

TODO: add how PURE benefits.

#### Hierarchical Z, HiZ

We manage 8x8 blocks of [pixels](#_Pixel) (tiles) and store the maximum (LESS, LEQUAL) or minimum (GREATER, GEQUAL) Z-value for each tile. During [triangle rasterization](#_Rasterization_1), we calculate the minimum/maximum Z-value for each triangle. First we compare this value to the tile’s maximum/minimum Z-value: if the triangle’s minimum/maximum Z-value is greater/less than the tile’s maximum/minimum Z-value, it means the triangle is not visible at all and can be early rejected (**Early Z Reject**). This is much faster than comparing the triangle’s Z-values to the stored values in the depth buffer.

Before the HD2xxx series, this information was stored in on-chip memory. Since the HD2xxx series, it is stored in off-chip memory.

Since the HD2xxx series, HiZ is applied on the stencil buffer as well.

#### Z Compression

If a tile contains very few number (1-2) of triangles, instead of storing all Z-values in that tile, store the plane equation for that triangle(s). So this is a lossless compression. Does not reduce the amount of memory that is required to store the depth buffer. It only saves bandwidth. It will still need to allocate the full buffer to handle all potential uncompressed states.

#### Fast Z Clear

Benefit of Z Compression. Instead of writing the depth clear value across the entire depth buffer, we just reset the state of all tiles to “cleared” (by storing the plane equation for a constant Z=1 triangle).

### Lightspeed Memory Architecture, LMA (nVidia)

Memory bandwidth optimizations including similar features as [HyperZ](#_HyperZ_(ATi)). **Z-Occlusion Culling** ~ [HiZ](#_Hierarchical_Z,_HiZ). **Occlusion query**: determining if the geometry to be rendered will be visible by using a bounding box occlusion test first.

Since Geforce 3.

### Nvidia Shading Rasterizer, NSR (nVidia)

TODO. Programmable pixel pipeline. Since Geforce 2.

### Render to Vertex Buffer, R2VB

TODO. Since Radeon X1xxx series.

### High Dynamic Range Rendering, HDR

TODO. Since Geforce 6xxx series.

### Shadow Buffers (nVidia)

TODO. Since Geforce 3.

### UltraShadow (nVidia)

TODO. Since Geforce FX57xx series.

[http://www.tomshardware.com/reviews/nvidia-geforcefx-5900-ultra,630-4.html](http://www.tomshardware.com/reviews/nvidia-geforcefx-5900-ultra,630-4.html)

### UltraShadow II (nVidia)

TODO. Since Geforce 6xxx series.
<!--stackedit_data:
eyJwcm9wZXJ0aWVzIjoidGl0bGU6IFBVUkUgR2xvc3Nhcnlcbm
F1dGhvcjogUFIwMEY4OFxuZGF0ZTogJzIwMjAtMTEtMjInXG4i
LCJoaXN0b3J5IjpbLTk5OTIzNjQ5NywtOTkzMDY2Njg1XX0=
-->