<!--
Keep this document short & concise,
linking to external resources instead of including content in-line.
See 'release/text/readme.html' for the end user read-me.
-->

# Blender – Full Fresnel Extension

This repository is a research fork of Blender introducing a new Cycles
material shader node: **Full Fresnel**.

## Research context
This code accompanies the paper:

> *[Enhanced Multispectral Rendering with a Unified Complex-Index Fresnel Model in Blender.]*  
> *[The Visual Computer]*

The implementation enables evaluation of a physically complete Fresnel model describing the full Fresnel calculation at an
interface between two media with complex wavelength dependent IORs, without any approximations!
This unifies dielectric materials, conductors and everything in between for every spectral domain 
from UV to LWIR (long wave infrared) in Blender’s Cycles renderer.


## Code base
This work is based on Blender upstream commit:

- **Commit:** `38363fb2d7ff`
- **Tag in this repository:** `blender-base-38363fb2d7ff`

All Full Fresnel changes are contained in the branch:

- **Branch:** `full-fresnel`

### View changes
Compare the base code with the Full Fresnel implementation here:
https://github.com/PTHeerdt/blender-full-fresnel/compare/blender-base-38363fb2d7ff...full-fresnel

## Building
This repository follows Blender’s standard build process.
See the *Development* section below or Blender’s official documentation.

## License
This repository is distributed under the GNU General Public License v3,
consistent with Blender’s licensing.

-----------------------------------------------------------------------------------

Blender
=======

Blender is the free and open source 3D creation suite.
It supports the entirety of the 3D pipeline-modeling, rigging, animation, simulation, rendering, compositing,
motion tracking and video editing.

![Blender screenshot](https://code.blender.org/wp-content/uploads/2018/12/springrg.jpg "Blender screenshot")

Project Pages
-------------

- [Main Website](http://www.blender.org)
- [Reference Manual](https://docs.blender.org/manual/en/latest/index.html)
- [User Community](https://www.blender.org/community/)

Development
-----------

- [Build Instructions](https://developer.blender.org/docs/handbook/building_blender/)
- [Code Review & Bug Tracker](https://projects.blender.org)
- [Developer Forum](https://devtalk.blender.org)
- [Developer Documentation](https://developer.blender.org/docs/)


License
-------

Blender as a whole is licensed under the GNU General Public License, Version 3.
Individual files may have a different, but compatible license.

See [blender.org/about/license](https://www.blender.org/about/license) for details.
