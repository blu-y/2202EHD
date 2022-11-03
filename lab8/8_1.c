








void draw_line(int x0, int x1, int y0, int y1){
    int is_steep = abs(y1-y0) > abs(x1 - x0); // bool
    if (is_steep){
        swap(x0, y0);
        sway(x1, y1);
    }
    if (x0 > x1){
        swap(x0, x1);
        swap(y0, y1);
    }
    int deltax = x1 - x0;
    int deltay = abs(y1-y0);
    int error = -(deltax/2);
    int y = y0;
    int y_step;
    if (y0 < y1) y_step = 1;
    else y_step = -1;

    for x f
}
