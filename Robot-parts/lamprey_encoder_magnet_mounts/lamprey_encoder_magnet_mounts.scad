// magnet mount for lamprey encoders

$fn = 200;               // facets per circle

use <MCAD/regular_shapes.scad>

MMtoIN        = 25.4;


// all dimensions in mm

MagnetOD        = 42.0;
MagnetID        = 30.8;             // actually 31mm but we need a little clearance
MagnetThickness = 4.5;

MountThickness  = 3;

HubThickness    = 8;
HubDiameter     = 0.8 * MMtoIN;     // 
HubGrubScrew    = .150 * MMtoIN;    // #21 drill is actually .159
                                    // this leaves a little for final drilling

HUB_TYPE_HEX    = 0;
HUB_TYPE_ROUND  = 1;

HubType         = HUB_TYPE_HEX;


// things get a little itchy when dealing with the hex bore hole
//
// the "radius" passed to the hexagon_prism() routine is the radius
// of the circle circumscribing the polygon (on the outside) but we can also
// consider the polygon to be inscribed on the circle
//
// what we want is to specify the radius of the circle inscribed in the hexagon
// so that we can specify the size of the hole as the distance between two
// opposing flats on the hexagon - just like bolts or nuts are sized
//
// for hexagons, the length of a side is equal to the radius of the circumscribing
// circle - this means if we can calculate the length of the side when the circle
// of our target radius is inscribed in the hexagon, we can use that to calculate
// the radius of the circle circumscribing the hexagon (which is what we need when
// we use the appropriate library routine)
//
// the length of a side based on the inscribed circle is:
//
//      2 * INSCRIBED_RADIUS * tan(PI/NUM_OF_SIDES) - angle in radians
// or
//      2 * INSCRIBED_RADIUS * tan(PI / NUM_OF_SIDES * 180 / PI) - angle in degrees
// or simplifying to:
//      INSCRIBED_DIAMETER * tan(180 / NUM_OF_SIDES)

HexBoreInscribedDiam       = 0.5 * MMtoIN;
HexBoreCircumscribedDiam   = HexBoreInscribedDiam * tan(180/6);  //Diam already in MM

// for round bores, we just need the diameter of the bore

HubRoundDiameter           = 0.25 * MMtoIN;


Extra          = 0.2;       // extra extrusion height to not leave a 0 thickness surface





difference() {
    
    union() {
        // we just start at 0 z and build layer on layer...
        cylinder(h=MagnetThickness, d=MagnetID);
        
        translate([0, 0, MagnetThickness]) {
            cylinder(h=MountThickness, d=MagnetOD);
            translate([0, 0, MountThickness]) {
                cylinder(h=HubThickness, d=HubDiameter);
            }
        }
        
        
    }
     
    // punch out the bore
    translate ([0, 0, -Extra]) {
        
        if (HubType == HUB_TYPE_HEX) {
            // hex bore
            hexagon_prism(MagnetThickness + MountThickness + HubThickness + Extra * 2,
                          HexBoreCircumscribedDiam);
        }
        
        if (HubType == HUB_TYPE_ROUND) {
            // round bore
            cylinder(h=MagnetThickness + MountThickness + HubThickness + Extra * 2,
                          d=HubRoundDiameter);
       }       
    }
    
    // grub screw hole - it just goes in the center of the hub thickness
    //
    // first off move the current Z to the middle of the hub thickness,
    // then rotate 90 on the X, and then punch the hole
    translate([0, 0, MagnetThickness + MountThickness + HubThickness / 2]) {
        rotate([90, 0, 0]) {
            cylinder(h=HubDiameter/2 + Extra, d=HubGrubScrew);
        }
    }
}
