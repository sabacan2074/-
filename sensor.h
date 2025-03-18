
int getWhite() {
    return HTCS2readWhite(S2);
}

int getL() {
    return getColorReflected(LeftColor);
}

int getR() {
    return getColorReflected(RightColor);
}

bool isGreen(char sensor) {
    int r, g, b;
	getColorRGB(sensor, r, g, b);
    return (r + g + b) < 70 && (r + g + b) > 25 && r * 2.6 < g && g > b && r < b;
    // return getColorName(sensor) == 3;
}