length = 60-.4;//52;
width = 125-.4;//86;
height = 32;
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
standoff_width = 80.8;
standoff_length = 47.25;
//difference(){
    // main box
    //cube([o_width, o_length, o_height]);
    translate([wall, wall, 0]) {
        translate([0, 0, wall]) {
            // 9999 to take out the whole face
            difference(){
            cube([width, length, wall]);
                translate([width, length/2, 0]) {
                    cylinder(h=100, r=9);
                }
                translate([0, length/2, 0]) {
                    cylinder(h=100, r=5);
                }
                        //translate([17, 6, 0]) {

            //cube([width-27, length-12, wall]);
                        //}

            }
        }
    }
//}


//add the pin mechanism
pin_od = 11;
pin_id = 5.2;
pin_len = 16;
wall_offset = 6;
translate([wall, (1/4)*length+wall-pin_od/2, wall+wall]){
    difference() {
        union(){
            cube([pin_len-wall, pin_od, wall_offset]);
            rotate([0,90,0]) {
                translate([-wall_offset,pin_od/2,0]) {
                    cylinder(h=pin_len-wall, d=pin_od);
                }
            }
        }
        rotate([0,90,0]) {
            translate([-wall_offset,pin_od/2,0]) {
                cylinder(h=pin_len-wall, d=pin_id);   
            }
        }
    }
}