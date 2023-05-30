#ifndef ASTEROIDS_MINMAX_H
#define ASTEROIDS_MINMAX_H

template <typename T>
void ref_min(T &dest, const T &source) {
    dest = std::min(dest, source);
}

template <typename T>
void ref_max(T &dest, const T &source) {
    dest = std::max(dest, source);
}

#endif //ASTEROIDS_MINMAX_H
