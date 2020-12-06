# port-engine
A C programming exercise for creating a pixelwise 2D rendering engine. All primitives are built up upon the manipulation of raw pixel buffer (also "video buffer", "framebuffer", or a simple array of bits & bytes in ARGB8888 format). The idea was to *understand* how early 80s, 90s computer graphics and game developer practitioners were able to create drawings, animation and space using just a *direct* access to a video buffer in RAM (without having advanced GPU's shader APIs). The sources are supposed to be a first approximation of it. However, using an *assembly* instead of 'high-level' C might be even more preferable but the information that is left on this matter is miserable.

*Fig 1. port conceptual model*

<img src="https://github.com/timfayz/port-engine/blob/master/examples/port%20Rendering%20Model.png" height="70%" width="70%"/>

<br>

*(the examples below may use early interations of port-engine API comparing to what is currently provided in `port.h`. Please, refer to the source file. It lists all the methods available for manipulating `Window` object at its very beginning)*

<br>

*Example 1. Draw line primitives*

<img src="https://github.com/timfayz/port-engine/blob/master/examples/port%20feature%20drawLine.gif" height="70%" width="70%"/>

<br>

*Example 2. Interpolation test (1 logical pixel becomes 64 of physical)*

<img src="https://github.com/timfayz/port-engine/blob/master/examples/port%20feature%20resize.gif" height="70%" width="70%"/>

<br>

*Example 3. Logical resize test*

<img src="https://github.com/timfayz/port-engine/blob/master/examples/port%20feature%20setLogicalSize%20(v0).gif" height="70%" width="70%"/>

<br>

*Example 4. Pixel animation*

<img src="https://github.com/timfayz/port-engine/blob/master/examples/port%20pixel%20falling.gif" height="70%" width="70%"/>

<br>

*Example 5. This is what you can get applying math and `drawLine()` primitive*

<img src="https://github.com/timfayz/port-engine/blob/master/examples/3D%20example.gif" height="70%" width="70%"/>


