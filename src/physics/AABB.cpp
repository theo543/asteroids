#include "AABB.h"
#include "utility/minmax.h"

AABB::AABB() : tl(0.f, 0.f), br(tl) {}

AABB::AABB(const std::vector<sf::Vector2f> &mesh) : tl(mesh.front()), br(mesh.front()) {
    for(const auto &point : mesh) {
        ref_min(tl.x, point.x);
        ref_max(br.x, point.x);
        ref_min(tl.y, point.y);
        ref_max(br.y, point.y);
    }
}

static bool intervals_intersect(float a_start, float a_end, float b_start, float b_end) {
    return (a_start <= b_end) && (a_end >= b_start);
}

bool AABB::collides(const AABB &other) const {
    return intervals_intersect(this->tl.x, this->br.x, other.tl.x, other.br.x)
        && intervals_intersect(this->tl.y, this->br.y, other.tl.y, other.br.y);
}

AABB AABB::transform(sf::Transform t) const {
    sf::Vector2f tr {br.x, tl.y}, bl {tl.x, br.y};
    return AABB{{t.transformPoint(tl), t.transformPoint(tr), t.transformPoint(bl), t.transformPoint(br)}};
}

sf::RectangleShape AABB::toRect() const {
    sf::RectangleShape rect;
    rect.setPosition(tl);
    rect.setSize(br - tl);
    return rect;
}

float AABB::getMaxX() const {
    return br.x;
}
