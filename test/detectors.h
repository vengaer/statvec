#ifndef DETECTORS_H
#define DETECTORS_H

struct copy_detector {
    copy_detector() noexcept = default;
    copy_detector(copy_detector const&) noexcept {
        ++copy_constructions;
        ++copies;
    }
    copy_detector& operator=(copy_detector const&) & noexcept {
        ++copy_assignments;
        ++copies;
        return *this;
    }

    copy_detector(copy_detector&&) noexcept = default;
    copy_detector& operator=(copy_detector&&) & noexcept = default;

    unsigned copy_assignments{};
    unsigned copy_constructions{};
    unsigned copies{};
};

struct move_detector {
    move_detector() noexcept = default;
    move_detector(move_detector const&) noexcept = default;
    move_detector& operator=(move_detector const&) & noexcept = default;

    move_detector(move_detector&&) noexcept {
        ++move_constructions;
        ++moves;
    }
    move_detector& operator=(move_detector&&) & noexcept {
        ++move_assignments;
        ++moves;
        return *this;
    }

    unsigned move_assignments{};
    unsigned move_constructions{};
    unsigned moves{};
};

struct forward_detector {
    forward_detector() noexcept = default;
    forward_detector(int i, int j) noexcept {
        i ^= j;
    }

    forward_detector(forward_detector const& other) noexcept
        : constructions{other.constructions + 1u} { }

    forward_detector& operator=(forward_detector const& other) & noexcept {
        constructions = other.constructions + 1u;
        return *this;
    }

    unsigned constructions{1u};
};

#endif /* DETECTORS_H */
