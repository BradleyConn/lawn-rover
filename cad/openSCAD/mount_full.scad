tolerance = .1;
height = 33;
inner_diameter = 17;
// Two sides so twice the tolerance
inner_diameter_with_tolerance = inner_diameter + tolerance + tolerance;
inner_radius = inner_diameter/2;
inner_radius_with_tolerance = inner_diameter_with_tolerance/2;
outer_diameter = 34.4 - tolerance - tolerance;
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
}

wheel_shaft_inset = height/2;
pipe_height = 44.4+wheel_shaft_inset;

pipe_inner_diameter = 41;
// Two sides so twice the tolerance
pipe_inner_diameter_with_tolerance = pipe_inner_diameter + tolerance + tolerance;
pipe_inner_radius = pipe_inner_diameter/2;
pipe_inner_radius_with_tolerance = pipe_inner_diameter_with_tolerance/2;
pipe_outer_diameter = 90.5;



pipe_mount_screwhole_diameter = 8.8 + tolerance + tolerance;
pipe_mount_screwhole_inset = 69.85/2;


module motor_shaft_cylinder(){ translate([0,0, -0.5]) {
        cylinder (h=pipe_height+1, d=inner_diameter_with_tolerance, $fn=fn);
    }
}

module wheel_shaft_cylinder(){ translate([0,0, -0.5]) {
        cylinder (h=pipe_height+1, d=pipe_inner_diameter_with_tolerance, $fn=fn);
    }
}

module screwhole() {
            // Screw holes
            translate([0, pipe_mount_screwhole_inset, pipe_height/2]) {
                cylinder(h=pipe_height+1, d=pipe_mount_screwhole_diameter, $fn=fn, center=true);
            }
}

// Make the coupler
difference() {
    union() {
        // Connected layer
        difference() {
            cylinder (h=wheel_shaft_inset, d=pipe_outer_diameter, $fn=fn);
            motor_shaft_cylinder();
        }
        
        // Pipe layer
        difference() {
            cylinder (h=pipe_height, d=pipe_outer_diameter, $fn=fn);
            wheel_shaft_cylinder();
        }
    }
    
    rotate(45 + 0){
        screwhole();
    }
    
    rotate(45 + 90){
        screwhole();
    }
    rotate(45 + 180){
        screwhole();
    }  
    rotate(45 +270){
        screwhole();
    }  
}

 