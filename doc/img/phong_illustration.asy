settings.outformat = "pdf";
size(50cm, 0);

real radius        = 3.0;
real radius2       = 0.2;
real scale         = 1.0;
real angle_degree  = 75.0;
real angle_v1      = 90.0;
real angle_v2      = 60.0;
real angle_v3      = 30.0;
real vector_length = 1.0;

pair center       = (0, 0);

pair get_vector_point(real angle_deg, real length=vector_length, real radius=radius) {
    return (cos(radians(angle_deg)) * (radius + length), sin(radians(angle_deg)) * (radius + length));
}

pair base = (cos(radians(angle_degree)) * radius, sin(radians(angle_degree)) * radius);
pair base2 = (cos(radians(60)) * radius, sin(radians(60)) * radius);

pair normal_end = get_vector_point(angle_degree);
pair v1_end     = get_vector_point(angle_v1);
pair v2_end     = get_vector_point(angle_v2);
pair v3_end     = get_vector_point(angle_v3);
path circ       = arc(center, radius, 0.0, 180.0);
// pair tangent = dir(base, base);

draw(shift(0, 0) * xscale(scale) * yscale(scale) * circ);
// draw(shift(base.x, base.y) * scale(1/2) * ((0,0) -- tangent));
fill(circle(base, 0.02));

draw(shift(base.x, base.y) * xscale(0.1) * yscale(0.1) * arc(center, radius, 0.0, 180.0));
draw(arc(base, radius2, 0.0, 180.0)); 
draw(base -- normal_end, EndArrow(2));
draw(base -- v1_end, EndArrow(2));
draw(base -- v2_end, EndArrow(2));
draw(base_start -- v3_end, EndArrow(2));
