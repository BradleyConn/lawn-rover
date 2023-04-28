tolerance = .1;
height = 35;
inner_diameter = 17;
// Two sides so twice the tolerance
inner_diameter_with_tolerance = inner_diameter + tolerance + tolerance;
inner_radius = inner_diameter/2;
inner_radius_with_tolerance = inner_diameter_with_tolerance/2;
outer_diameter = 25;
key_width = 6;
// Two sides so twice the tolerance
key_width_with_tolerance = key_width - tolerance - tolerance;
key_height = 3.05;
key_height_with_tolerance = key_height - tolerance;

mount_height = 10;
mount_width = 10;
mount_length = 100;
mount_diameter = mount_length/2;

fn = 1000;

// Find how much you need to offset from the center for the rectangle cutout
// Pythagrean theorem!
inner_diameter_with_tol_sq = pow(inner_radius_with_tolerance, 2);
half_key_width_with_tol_sq = pow(key_width_with_tolerance/2, 2);
offset = sqrt(inner_diameter_with_tol_sq - half_key_width_with_tol_sq);



union () {
    // Make the coupler
    difference() {
        cylinder (h=height, d=outer_diameter, $fn=fn);
        translate([0,0, -0.5]) {
            cylinder (h=height+1, d=inner_diameter_with_tolerance, $fn=fn);
        }
    }

    // Go put the corners of the box on the radius of the circle
    translate([-key_width_with_tolerance/2, -offset,0]) {
        cube([key_width_with_tolerance, key_height_with_tolerance, height]);
    }
    // Fill in the gap
    // Extra tolerance overlap
    translate([-key_width_with_tolerance/2, -inner_radius_with_tolerance - tolerance,0]) {
        cube([key_width_with_tolerance, key_height_with_tolerance, height]);
    }
    
    
    
    
    
    
    // Now put the wheel mount part
    union() {
        difference() {
            rotate(45) {
                translate([0,0, mount_height/2]) {
                    cube([mount_length, mount_width, mount_height], center=true);
                }
            }
            translate([0,0, -0.5]) {
                cylinder (h=height+1, d=inner_diameter_with_tolerance, $fn=fn);
            }
        }
        difference() {
            rotate(45){
                translate([0,0, mount_height/2]) {
                    cube([mount_width, mount_length, mount_height], center=true);
                }
            }
            translate([0,0, -0.5]) {
                cylinder (h=height+1, d=inner_diameter_with_tolerance, $fn=fn);
            }
        }
        cylinder (h=height, r1=mount_length/,r2=outer_diameter/2);

        /*
        difference() {
        cylinder (h=mount_height, d=mount_diameter, $fn=fn);
            translate([0,0, -0.5]) {
                cylinder (h=height+1, d=inner_diameter_with_tolerance, $fn=fn);
            }
        }*/
    }
}
