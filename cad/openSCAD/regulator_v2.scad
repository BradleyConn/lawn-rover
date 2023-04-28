length = 43;//52;
width = 88;//86;
height = 31;
wall = 3.5;
o_width = width + wall + wall;
o_length = length + wall + wall;
o_height = height + wall;

resolution = 100;

$fn = resolution;

// outer box
standoff_id=3.45;
standoff_od=7;
standoff_od_wall_depth = wall-2;
standoff_width = 55.8;
standoff_length = 28.4;
difference(){
    // main box
    cube([o_width, o_length, o_height]);
    translate([wall, wall, 0]) {
        translate([0, 0, wall]) {
            // 9999 to take out the whole face
            cube([width, length, 9999]);
        }
        //all the standoff holes
        //center of the box
        // shift it up a bit
        translate([width/2, length/2, 0]) {
            // standoff holes
            translate([standoff_width/2, standoff_length/2,0]) {
                cylinder(h=9999, d=standoff_id);
                cylinder(h=standoff_od_wall_depth, d=standoff_od);
            }
            translate([standoff_width/2, -standoff_length/2, 0]) {
                cylinder(h=9999, d=standoff_id);
                cylinder(h=standoff_od_wall_depth, d=standoff_od);

            }
            translate([-standoff_width/2, standoff_length/2, 0]) {
                cylinder(h=9999, d=standoff_id);
                cylinder(h=standoff_od_wall_depth, d=standoff_od);

            }
            translate([-standoff_width/2, -standoff_length/2, 0]) {
                cylinder(h=9999, d=standoff_id);
                cylinder(h=standoff_od_wall_depth, d=standoff_od);

            }
        }
    }
}


mount_arm_surface_area = 20;
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

//add the pin mechanism
pin_od = 12.5;
pin_id = 6.4;
wall_offset = 6;
translate([0, (1/4)*length+wall-pin_od/2, o_height]){
    difference() {
        union(){
            cube([wall, pin_od, wall_offset]);
            rotate([0,90,0]) {
                translate([-wall_offset,pin_od/2,0]) {
                    cylinder(h=wall, d=pin_od);
                }
            }
        }
        rotate([0,90,0]) {
            translate([-wall_offset,pin_od/2,0]) {
                cylinder(h=wall, d=pin_id);   
            }
        }
    }
}