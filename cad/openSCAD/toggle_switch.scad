length = 37;//52;
width = 47;//86;
height = 20;
wall = 3.5;
o_width = width + wall + wall;
o_length = length + wall + wall;
o_height = height + wall;

resolution = 100;

$fn = resolution;

// outer box
standoff_id=3.45;
standoff_od=14;
standoff_od_wall_depth = wall-2;
standoff_width = 55.8;
standoff_length = 28.4;
difference(){
union(){
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
        translate([width/2, length/2, wall+height/2]) {
            // standoff holes
            rotate([90,90,0]){
                cylinder(h=9999, d=standoff_od);
        }
        }
    }
}


mount_arm_surface_area = 20;
screwhole_size = 4.6;

//box_mount_hole_1
translate([0,0,0]) {
mirror([0,1,-1]) {
rotate(90){
    difference() {
            difference() {
                cube([mount_arm_surface_area+wall, 10, o_height-wall-wall]);
                // angled for strength
                translate([mount_arm_surface_area+wall, mount_arm_surface_area, -1]){
                    //cylinder(h=9999, r=mount_arm_surface_area);
                }
            }
        // screw hole
        rotate([0,90,0]) {
            translate([-o_height/2+6, 4, -1]){
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
                    difference() {
                        cube([mount_arm_surface_area+wall, 10, o_height-wall-wall]);
                        // angled for strength
                        translate([mount_arm_surface_area+wall, mount_arm_surface_area, -1]){
                            //cylinder(h=9999, r=mount_arm_surface_area);
                        }
                    }
                // screw hole
                rotate([0,90,0]) {
                    translate([-o_height/2+6, 4, -1]){
                        cylinder(h=9999, d=screwhole_size);
                    }
                }
            }
        }
    }
}
}
}
}
translate([-100, length-wall-wall-wall-wall-wall-wall+1, height-wall-wall]) {
            cube([width*20, 14, 30]);
    }
    translate([0, wall, 0]) {

     //cube([100, 100, 100]);
    }

}

