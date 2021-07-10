# statvec

A stack-allocated, statically sized `constexpr` vector in the vein of `std::vector`. Currently provides the entire API, albeit slightly modified, of the latter save for

* assign
* get_allocator
* reserve
* shrink_to_fit
* insert
* emplace
* erase

## Catch

The unit tests use [Catch2](github.com/catchorg/Catch2) which is licensed under the BSL-1.0. See [LICENSE](LICENSE) for the license note.
