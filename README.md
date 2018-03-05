# Tessellation---DirectX-11


Tessellation ia method of breaking down polygons into finer pieces. In DirectX tessellation is technique that allows you to generate primitives(triangles, lines, points, etc.) on the graphics-card. Specifically, it lets you repeatedly subdivide the current geometry into a finer mesh, thereby providing more detailed level of realism.

Tessellation allows you to load a relatively coarse mesh on your graphics card, generate more vertices and triangles dynamically and then have a mesh on the screen that looks much smoother.

Many popular games use tessellation and displacemnent mapping to produce smooth-looking models, and developers have done promising work on applying these techniques to their existing game characters.


References

http://www.gdcvault.com/play/1012740/direct3d
https://www.gamedev.net/articles/programming/graphics/d3d11-tessellation-in-depth-r3059/
