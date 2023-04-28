tolerance = .1;
height = 4;
inner_diameter = 42;
// Two sides so twice the tolerance
inner_diameter_with_tolerance = inner_diameter + tolerance + tolerance;
inner_radius = inner_diameter/2;
inner_radius_with_tolerance = inner_diameter_with_tolerance/2;
outer_diameter = 83;



mount_screwhole_diameter = 8.8 + tolerance + tolerance;
mount_screwhole_inset = 69.85/2;

fn = 1000;

module middle_cylinder(){ translate([0,0, -0.5]) {
        cylinder (h=height+1, d=inner_diameter_with_tolerance, $fn=fn);
    }
}

module screwhole() {
            // Screw holes
            translate([0, mount_screwhole_inset, height/2]) {
                cylinder(h=height+1, d=mount_screwhole_diameter, $fn=fn, center=true);
            }
}

// Make the coupler
difference() {
    difference() {
        cylinder (h=height, d=outer_diameter, $fn=fn);
        middle_cylinder();
    }
    
    screwhole();
    rotate(90){
        screwhole();
    }
    rotate(180){
        screwhole();
    }  
    rotate(270){
        screwhole();
    }  
}

