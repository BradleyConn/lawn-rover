length = 60;//52;
width = 10;//86;
height = 10;
wall = 3.5;
o_width = width + wall + wall;
o_length = length + wall + wall;
o_height = height + wall + wall;

resolution = 100;

$fn = resolution;

// outer box
standoff_id=3.45;
standoff_od=7;
standoff_od_wall_depth = wall-2;
standoff_width = 55.8;
standoff_length = 28.4;
// main box
//add the pin mechanism
pin_od = 11;
pin_id = 5.2*(4/3)-.1;
wall_offset = 6;
difference() {
    cube([o_width, o_length, o_height]);
    rotate([0,90,0]) {
        translate([-(height+wall+wall)/2,wall+wall,0]) {
            cylinder(h=9000, d=pin_id);   
        }
    }
}
//}


mount_arm_surface_area = 10;
screwhole_size = 4.6;
//box_mount_hole_1
translate([0,o_length,0]) {
mirror([0,1,0]) {
rotate(90){
    difference() {
        union() {
            difference() {
                cube([mount_arm_surface_area+wall, mount_arm_surface_area, o_height]);
                // angled for strength
                translate([mount_arm_surface_area+wall, mount_arm_surface_area, -1]){
                    cylinder(h=9999, r=mount_arm_surface_area);
                }
            }
            cube([wall, mount_arm_surface_area+screwhole_size*2, o_height]);
        }
        // screw hole
        rotate([0,90,0]) {
            translate([-o_height/2, mount_arm_surface_area, -1]){
                cylinder(h=9999, d=screwhole_size);
            }
        }
    }
}
//box mount hole 2
//copied and pasted but translated an mirrored
translate([o_width, 0, 0]){
    mirror([1,0,0]){
        rotate(90) {
            difference() {
                union() {
                    difference() {
                        cube([mount_arm_surface_area+wall, mount_arm_surface_area, o_height]);
                        // angled for strength
                        translate([mount_arm_surface_area+wall, mount_arm_surface_area, -1]){
                            cylinder(h=9999, r=mount_arm_surface_area);
                        }
                    }
                    cube([wall, mount_arm_surface_area+screwhole_size*2, o_height]);
                }
                // screw hole
                rotate([0,90,0]) {
                    translate([-o_height/2, mount_arm_surface_area, -1]){
                        cylinder(h=9999, d=screwhole_size);
                    }
                }
            }
        }
    }
}
}
}
