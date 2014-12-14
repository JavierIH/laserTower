module m3(H){
	cylinder(r=1.75,h=H,$fn=30,center=true);
}
module m4(H){
	cylinder(r=2.35,h=H,$fn=30,center=true);
}
module t_m4(H){
	cylinder(r=4.5,h=H,$fn=6);
}
module c_m(H){
	cylinder(r=6,h=H,$fn=30);
}

radio_base=20;
radio_barra=32.5/2;
servo_x=69;
servo_y=33;
c_servo_x=servo_x+40;
c_servo_y=servo_y;

module maxiservo(H){
	cube([servo_x,servo_y,H],center=true);
	translate([37.5,9,0])m4(H);
	translate([37.5,-9,0])m4(H);
	translate([-37.5,9,0])m4(H);
	translate([-37.5,-9,0])m4(H);
}

module base(){
	//%cylinder(r=radio_base,h=300,center=true);
	difference(){
		union(){
			hull(){
				cylinder(r=radio_barra+3,h=40);
				cube([2*radio_base+30,2*radio_base+30,6],center=true);
			}
			cylinder(r=radio_barra+3,h=60);
		}
		translate([0,0,1])cylinder(r=radio_barra,h=100);

		translate([radio_base+5,radio_base+5,1])m4(100);
		translate([radio_base+5,-(radio_base+5),1])m4(100);
		translate([-(radio_base+5),radio_base+5,1])m4(100);
		translate([-(radio_base+5),-(radio_base+5),1])m4(100);

		translate([radio_base+5,radio_base+5,0])t_m4(100);
		translate([radio_base+5,-(radio_base+5),0])t_m4(100);
		translate([-(radio_base+5),radio_base+5,0])t_m4(100);
		translate([-(radio_base+5),-(radio_base+5),0])t_m4(100);

		translate([0,0,45])rotate([0,90,0])m4(100);
		translate([0,0,55])rotate([90,0,0])m4(100);

	}
}

module adaptador(){
	difference(){
		hull(){
			translate([0,0,11])
				cube([2*radio_base+30,2*radio_base+30,22],center=true);
			translate([30,0,0])cylinder(r=34,h=22);
		}

		translate([(servo_x-servo_y)/2,0,0])maxiservo(100);
		translate([radio_base+5,radio_base+5,0])m4(100);
		translate([radio_base+5,-(radio_base+5),0])m4(100);
		translate([-(radio_base+5),radio_base+5,0])m4(100);
		translate([-(radio_base+5),-(radio_base+5),0])m4(100);
		translate([0,0,12])cylinder(r=22,h=100);	


		translate([0,0,10]){
			translate([radio_base+5,radio_base+5,0])c_m(100);
			translate([radio_base+5,-(radio_base+5),0])c_m(100);
			translate([-(radio_base+5),radio_base+5,0])c_m(100);
			translate([-(radio_base+5),-(radio_base+5),0])c_m(100);
			translate([0,0,10])cylinder(r=22,h=100);
		}

		translate([0,0,-1]){
			translate([radio_base+5,radio_base+5,0])c_m(6);
			translate([radio_base+5,-(radio_base+5),0])c_m(6);
			translate([-(radio_base+5),radio_base+5,0])c_m(6);
			translate([-(radio_base+5),-(radio_base+5),0])c_m(6);
		}

		translate([-26,-c_servo_y/2,10])
			cube([c_servo_x,c_servo_y,50]);
	
		cube([200,4,8],center=true);
	}
}

module pan(){
	difference(){
		union(){
			difference(){
				hull(){
					translate([0,0,0])cylinder(r=40,h=20);
					translate([12.3,0,50])rotate([0,45,0])cube([85,36,29],center=true);
				}
				translate([-100,18,10])cube([200,12,100]);
				translate([-100,-58,10])cube([200,40,100]);
			}
			hull(){
				translate([0,30,62])rotate([-90,0,0])cylinder(r=20,h=10);
				translate([0,35,10])cube([70,10,20],center=true);
			}
			hull(){
				cylinder(r=35,h=10);
				translate([0,35,5])cube([70,10,10],center=true);
			}
		}
		translate([12.3,0,50])rotate([90,45,0])maxiservo(50);
	}
}




base();
//adaptador();
//pan();