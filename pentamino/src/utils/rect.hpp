#pragma once

#include <utility>
#include <iosfwd>
#include <algorithm> // min max
#include <cstdlib> // abs
#include <cstdint>
#include <cstdio> // snprintf

struct Rect {
    int16_t x;
    int16_t y;
    uint16_t cx;
    uint16_t cy;

    int16_t left() const {
        return this->x;
    }

    int16_t right() const {
        return static_cast<int16_t>(this->x + this->cx);
    }

    int16_t top() const {
        return this->y;
    }

    int16_t bottom() const {
        return static_cast<int16_t>(this->y + this->cy);
    }

    Rect() : x(0), y(0), cx(0), cy(0) {
    }

    Rect(int16_t left, int16_t top, uint16_t width, uint16_t height)
        : x(left), y(top), cx(width), cy(height)
    {
        // fast detection of overflow, works for valid width/height range 0..4096
        if (((width-1)|(height-1)) & 0x8000){
            this->cx = 0;
            this->cy = 0;
        }
    }

    bool contains_pt(int x, int y) const {
        return    x  >= this->x
                && y  >= this->y
                && x   < this->right()
                && y   < this->bottom();
    }

    bool has_intersection(int16_t x, int16_t y) const
    {
        return this->cx && this->cy
            && (x >= this->x && x < this->right())
            && (y >= this->y && y < this->bottom());
    }

    // special cases: contains returns true
    // - if both rects are empty
    // - if inner rect is empty
    bool contains(Rect inner) const {
        return (inner.x >= this->x
              && inner.y >= this->y
              && inner.right() <= this->right()
              && inner.bottom() <= this->bottom());
    }

    bool operator==(Rect const & other) const {
        return (other.x == this->x
             && other.y == this->y
             && other.cx == this->cx
             && other.cy == this->cy);
    }

    bool operator!=(Rect const & other) const {
        return !(*this == other);
    }

    // Rect constructor ensures that any empty rect will be (0, 0, 0, 0)
    // hence testing cx or cy is enough
    bool isempty() const {
        return (this->cx == 0) || (this->cy == 0);
    }

    int getCenteredX() const {
        return this->x + (this->cx / 2);
    }

    int getCenteredY() const {
        return this->y + (this->cy / 2);
    }

    Rect wh() const {
        return Rect(0, 0, this->cx, this->cy);
    }

    // compute a new rect containing old rect and given point
    Rect enlarge_to(int16_t x, int16_t y) const {
        if (this->isempty()){
            return Rect(x, y, 1, 1);
        }
        else {
            const int16_t x0 = std::min(this->x, x);
            const int16_t y0 = std::min(this->y, y);
            const int16_t x1 = std::max(static_cast<int16_t>(this->right() - 1), x);
            const int16_t y1 = std::max(static_cast<int16_t>(this->bottom() - 1), y);
            return Rect(x0, y0, uint16_t(x1 - x0 + 1), uint16_t(y1 - y0 + 1));
        }
    }

    Rect offset(int16_t dx, int16_t dy) const {
        return Rect(
            static_cast<int16_t>(this->x + dx),
            static_cast<int16_t>(this->y + dy),
            this->cx,
            this->cy
        );
    }

    Rect shrink(uint16_t margin) const {
        //REDASSERT((this->cx >= margin * 2) &&
        //          (this->cy >= margin * 2));
        return Rect(
            static_cast<int16_t>(this->x + margin),
            static_cast<int16_t>(this->y + margin),
            static_cast<uint16_t>(this->cx - margin * 2),
            static_cast<uint16_t>(this->cy - margin * 2)
        );
    }

    Rect expand(uint16_t margin) const {
        return Rect(
            static_cast<int16_t>(this->x - margin),
            static_cast<int16_t>(this->y - margin),
            static_cast<uint16_t>(this->cx + margin * 2),
            static_cast<uint16_t>(this->cy + margin * 2)
        );
    }

    //Rect upper_side() const {
    //    return Rect(this->x, this->y, this->cx, 1);
    //}

    //Rect left_side() const {
    //    return Rect(this->x, this->y, 1, this->cy);
    //}

    //Rect lower_side() const {
    //    return Rect(this->x, this->y + this->cy - 1, this->cx, 1);
    //}

    //Rect right_side() const {
    //    return Rect(this->x + this->cx - 1, this->y, 1, this->cy);
    //}

    Rect intersect(uint16_t width, uint16_t height) const
    {
        return this->intersect(Rect(0, 0, width, height));
    }

    Rect intersect(Rect in) const
    {
        int16_t max_x = std::max(in.x, this->x);
        int16_t max_y = std::max(in.y, this->y);
        int16_t min_right = std::min(in.right(), this->right());
        int16_t min_bottom = std::min(in.bottom(), this->bottom());

        return Rect(max_x, max_y, uint16_t(min_right - max_x), uint16_t(min_bottom - max_y));
    }

    Rect disjunct(Rect r) const
    {
        if (this->isempty()) {
            return r;
        }
        else if (r.isempty()) {
            return *this;
        }

        auto x = std::min(r.x, this->x);
        auto y = std::min(r.y, this->y);
        auto cx = std::max(r.right(), this->right()) - x;
        auto cy = std::max(r.bottom(), this->bottom()) - y;
        return Rect(x, y, uint16_t(cx), uint16_t(cy));
    }

    bool has_intersection(Rect in) const
    {
        return (this->cx && this->cy && !in.isempty()
        && ((in.x >= this->x && in.x < this->right()) || (this->x >= in.x && this->x < in.right()))
        && ((in.y >= this->y && in.y < this->bottom()) || (this->y >= in.y && this->y < in.bottom()))
        );
    }

    // Ensemblist difference
    template<class Fn>
    void difference(Rect a, Fn fn) const
    {
        const Rect intersect = this->intersect(a);

        if (!intersect.isempty()) {
            if (intersect.y > this->y) {
                fn(Rect(this->x, this->y,
                        this->cx, static_cast<uint16_t>(intersect.y - this->y)));
            }
            if (intersect.x > this->x) {
                fn(Rect(this->x, intersect.y,
                        static_cast<uint16_t>(intersect.x - this->x), intersect.cy));
            }
            if (this->right() > intersect.right()) {
                fn(Rect(intersect.right(), intersect.y,
                        static_cast<uint16_t>(this->right() - intersect.right()), intersect.cy));
            }
            if (this->y + this->cy > intersect.bottom()) {
                fn(Rect(this->x, intersect.bottom(),
                        this->cx, static_cast<uint16_t>(this->bottom() - intersect.bottom())));
            }
        }
        else {
            fn(*this);
        }
    }

    template<class Ch, class Tr>
    friend std::basic_ostream<Ch, Tr>& operator<<(std::basic_ostream<Ch, Tr>& out, Rect const & r)
    {
        return out << "(" << r.x << ", " << r.y << ", " << r.cx << ", " << r.cy << ")";
    }

    //             |                         |
    //             |                         |
    //             |                         |
    //    UP       |         UP              |      UP
    //    LEFT     |                         |      RIGHT
    //             |                         |
    //-------------+-------------------------+--------------------
    //             |/ / / / / / / / / / / / /|
    //             | / / / / / / / / / / / / |
    //             |/ / / / / / / / / / / / /|
    //    LEFT     | / / / /   IN  / / / / / |     RIGHT
    //             |/ / / / / Rect  / / / / /|
    //             | / / / / / / / / / / / / |
    //             |/ / / / / / / / / / / / /|
    //-------------+-------------------------+-------------------
    //             |                         |
    //    DOWN     |         DOWN            |     DOWN
    //    LEFT     |                         |     RIGHT
    //             |                         |
    //             |                         |

    enum  t_region {
        IN = 0x00,
        UP = 0x01,
        DOWN = 0x02,
        LEFT = 0x04,
        RIGHT = 0x08
    };
    // Region of a point outside rect
    // 0x00 means inside
    t_region region_pt(int16_t x, int16_t y) const {
        int res = IN;
        if (x < this->x) {
            res |= LEFT;
        }
        else if (x >= this->right()) {
            res |= RIGHT;
        }
        if (y < this->y) {
            res |= UP;
        }
        else if (y >= this->bottom()) {
            res |= DOWN;
        }
        return static_cast<t_region>(res);
    }

private:
    size_t str(const char* label, char* buffer, size_t size) const {
        size_t length = ::snprintf(buffer, size,
            "%s: Rect(%d %d %u %u)",
            label,
            this->x, this->y, this->cx, this->cy);
        return ((length < size) ? length : size - 1);
    }
};

struct Dimension {
    uint16_t w;
    uint16_t h;

    Dimension()
        : w(0)
        , h(0)
    {}

    Dimension(uint16_t w, uint16_t h)
        : w(w)
        , h(h)
    {}

private:
    size_t str(const char* label, char* buffer, size_t size) const {
        size_t length = ::snprintf(buffer, size,
            "%s: Dimension(%u %u)",
            label,
            static_cast<unsigned>(this->w), static_cast<unsigned>(this->h));
        return ((length < size) ? length : size - 1);
    }
};

struct Point {
    int x;
    int y;

    Point(int x, int y)
        : x(x)
        , y(y)
    {}

};

struct Segment {
    Point a;
    Point b;

    Segment(const Point & a, const Point & b)
        : a(a)
        , b(b)
    {}
};

struct LineEquation {
    Segment seg;
    Segment segin;

    int dX;
    int dY;
    int c;

    LineEquation(int aX, int aY, int bX, int bY)
        : seg(Segment(Point(aX, aY), Point(bX, bY)))
        , segin(Segment(Point(0, 0), Point(0, 0)))
        , dX(aX - bX)
        , dY(aY - bY)
        , c(bY*aX - aY*bX)
    {
    }

    int compute_x(int y) const {
        return (this->dX*y - this->c) / this->dY;
    }

    int compute_y(int x) const {
        return (this->dY*x + this->c) / this->dX;
    }

    bool compute_intersection(Rect rect, int region, int & x, int & y) const {
        int interX = 0;
        int interY = 0;
        bool found = false;

        if (region & Rect::LEFT) {
            int tmpy = this->compute_y(rect.x);
            if (tmpy >= rect.y && tmpy < rect.bottom()) {
                found = true;
                interX = rect.x;
                interY = tmpy;
            }
        }
        else if (region & Rect::RIGHT) {
            int tmpy = this->compute_y(rect.right() - 1);
            if (tmpy >= rect.y && tmpy < rect.bottom()) {
                found = true;
                interX = rect.right() - 1;
                interY = tmpy;
            }
        }

        if (region & Rect::UP) {
            int tmpx = this->compute_x(rect.y);
            if (tmpx >= rect.x && tmpx < rect.right()) {
                found = true;
                interX = tmpx;
                interY = rect.y;
            }
        }
        else if (region & Rect::DOWN) {
            int tmpx = this->compute_x(rect.bottom() - 1);
            if (tmpx >= rect.x && tmpx < rect.right()) {
                found = true;
                interX = tmpx;
                interY = rect.bottom() - 1;
            }
        }

        if (found) {
            x = interX;
            y = interY;
        }

        return found;
    }

    bool resolve(Rect rect) {
        Rect::t_region aPosition = rect.region_pt(int16_t(this->seg.a.x), int16_t(this->seg.a.y));
        Rect::t_region bPosition = rect.region_pt(int16_t(this->seg.b.x), int16_t(this->seg.b.y));

        if (aPosition & bPosition) {
            return false;
        }
        bool exist = true;
        if (!aPosition) {
            this->segin.a.x = this->seg.a.x;
            this->segin.a.y = this->seg.a.y;
        }
        else {
            exist &= this->compute_intersection(rect, aPosition, this->segin.a.x, this->segin.a.y);
        }

        if (!bPosition) {
            this->segin.b.x = this->seg.b.x;
            this->segin.b.y = this->seg.b.y;
        }
        else {
            exist &= this->compute_intersection(rect, bPosition, this->segin.b.x, this->segin.b.y);
        }
        return exist;
    }
};

// helper class used to compute differences between two rectangles
class DeltaRect {
    public:
    int dleft;
    int dtop;
    int dheight;
    int dwidth;

    DeltaRect(Rect r1, Rect r2)
    : dleft(r1.x - r2.x)
    , dtop(r1.y - r2.y)
    , dheight(r1.cy - r2.cy)
    , dwidth(r1.cx - r2.cx)
    {}

    bool fully_relative() const {
        return (abs(this->dleft) < 128)
            && (abs(this->dtop) < 128)
            && (abs(this->dwidth) < 128)
            && (abs(this->dheight) < 128)
            ;
    }
};
