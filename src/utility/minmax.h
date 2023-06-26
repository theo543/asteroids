#ifndef ASTEROIDS_MINMAX_H
#define ASTEROIDS_MINMAX_H

template <typename T>
void ref_min(T &dest, const T &source) {
    if (dest > source) dest = source;
}

template <typename T>
void ref_max(T &dest, const T &source) {
    if (dest < source) dest = source;
}

#endif //ASTEROIDS_MINMAX_H
