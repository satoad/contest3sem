int satsum(int v1, int v2) {
    if (v2 > v1) {
        int tmp = v2;
        v2 = v1;
        v1 = tmp;
    }

    int tmp = 0;
    tmp++;

    unsigned int max = 0;
    max = ~max >> tmp;
    int min = -max - tmp;

    if (v1 > 0) {
        if (v2 > ((int)max - v1)) {
            return max;
        }
    } else {
        if (v2 < min - v1) {
            return min;
        }
    }
    return v1 + v2;
}