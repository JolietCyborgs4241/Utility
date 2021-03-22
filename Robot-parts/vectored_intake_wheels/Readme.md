# Vectored intake wheels

These not only provide a rolling force to draw in round items but they also provide a lateral force which can he used to move
items with a sideways force as well.  When used in matching pairs (including multiples of the same on each side), they can provide
a centering capability as an item is being loaded.

The OpenSCAD file can generate .STL files for both right an left-handed wheels.  All of the options are set at the top of the
file via global variables.

It can generate vectored intake sheels with varying:

* Diameters (the diameter of the wheel housing - the rollers ad some efective diameter to the finished assembly)
* Width
* Number of rollers
* Diameter of rollers (the model includes clearance for use of the specific diameter roller)
* Length of rollers
* Roller angle
* Roller "handness" (right or left handed)
* Roller axle diameter
* and several others...

The physical rollers should be the diameter specified in the code and generally should be
about 0.050" shorter than the length in the model.
