$fn = 200;               // facets per circle

use <MCAD/regular_shapes.scad>

// chamfer is from https://github.com/SebiTimeWaster/Chamfers-for-OpenSCAD
include <Chamfers-for-OpenSCAD/Chamfer.scad>;


MMtoIN        = 25.4;

difference() {
    
    chamferCylinder(h=1.950 * MMtoIN, r= 0.5 * MMtoIN, ch=.15 * MMtoIN);
    
    translate([0, 0, -0.1]) {
        cylinder(h = 1.950 * MMtoIN + 0.2, r=3.0 / 32.0 * MMtoIN);
    }
}

