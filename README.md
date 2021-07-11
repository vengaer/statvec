# statvec

A stack-allocated, statically sized `constexpr` vector in the vein of `std::vector`. Currently provides the entire API, albeit slightly modified, of the latter save for

* get_allocator
* shrink_to_fit

[[_TOC_]]

## Catch

The unit tests use [Catch2](https://github.com/catchorg/Catch2) which is licensed under the BSL-1.0. See [LICENSE](LICENSE) for the license note.

## Reference

### Synopsis

```c++
template <typename T, std::size_t N>
struct statvec {
    using value_type             = T;
    using reference              = value_type&;
    using const_reference        = value_type const&;
    using pointer                = value_type*;
    using const_pointer          = value_type const*;
    using size_type              = std::size_t;

    using iterator               = /* Implementation defined random access iterator */
    using const_iterator         = /* Implementation defined random access iterator */
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /* ... */
};
```

### Constructors

```c++
constexpr statvec() noexcept
```

Default constructs a `statvec`.

```c++
template <typename T0, typename T1toN...>
constexpr statvec(T0&& first, T1toN&&... rest)
```

Constructs a `statvec` by perfect forwarding `first` and `rest...`. No intermediary copies are made. `sizeof...(T1toN)` must be less than `N - 1`. The constructor participates in overload resolution only if `T0` is not a potentially cvref-qualified instance of either `statvec` or `std::array`. The constructor is `noexcept` if `T` is both nothrow constructible and nothrow assignable from each of the types `T0&&` and `T1toN&&...`.

```c++
template <std::size_t M>
constexpr statvec(std::array<T, M> const& array)
```
Constructs a `statvec` by copying each element in `array`. `M` must be less than or equal to the capacity of the `statvec`. No intermedia copies are made. The constructor is `noexcept` if `T` is both nothrow copy constructible and nothrow copy assignable.

```c++
template <std::size_t M>
constexpr statvec(std::array<T, M>&& array)
```

Constructs a `statvec` by moving each element in `array`. `M` must be less than or equal to the capacity of the `statvec`. No intermediate copies are performed. The constructor is `noexcept` if `T` is both nothrow move constructible and nothrow move assignable.

```c++
constexpr statvec(statvec const& other)
```

Copy constructs a `statvec`. Note that `N` elements are copied even if the vector is not filled to the brim to support non-default constructible types. The constructor is `noexcept` if `T` is nothrow copy constructible.

```c++
constexpr statvec(statvec&& other)
```

Move constructs a `statvec`. `N` elements are moved even if the size of `other` is less than `N` to support non-default constructible types. The constructor is `noexcept` if `T` is nothrow move constructible.

### Assignments Operators

```c++
constexpr statvec& operator=(statvec const& other) &
```

Copies exactly `other.size()` elements from `other` to `*this`. The operator is `noexcept` if `T` is nothrow copy assignable.

```c++
constexpr statvec& operator=(statvec&& other) &
```

Moves exactly `other.size()` elements from `other` to `*this`. The operator is `noexcept` if `T` is nothrow move assignable.

```c++
constexpr statvec& operator=(std::array<T, N> const& other) &
```

Copies exactly `N` elements from `other` to `*this`. The operator is `noexcept` if `T` is nothrow copy assignable.

```c++
constexpr statvec& operator=(std::array<T, N>&& other) &
```

Moves exactly `N` elements from `other` to `*this`. The operator is `noexcept` if `T` is nothrow move assignable.

```c++
constexpr bool assign(size_type count, T const& value)
```

Assigns `value` to the `std::min(count, this->capacity())` first elements of `*this`. Returns `true` if `count <= this->capacity()`, otherwise `false`. The function is `noexcept` if `T` is nothrow copy assignable.

```c++
template <typename InputIt>
constexpr bool assign(InputIt first, InputIt last)
```

Copies `std::min(std::distance(first, last), this->capacity())` elements from the range `[first, last)` to `*this`, starting at slot 0. Returns `true` if `std::distance(first, last) <= this->capacity()`, otherwise `false`. The function template is `noexcept` if `T` is nothrow copy assignable. The template participates in overload resolution only if `InputIt` is incrementable, dereferencable and comparable.

### Accessors

```c++
constexpr reference operator[](size_type index) noexcept
constexpr const_reference operator[](size_type index) const noexcept
```

Accesses the element at index `index`. No bounds checking is performed.

```c++
constexpr reference at(size_type index) 
constexpr const_reference at(size_type index) const
```

Accesses the element at index `index`. If `index >= this->size()`, a `std::out_of_range` is thrown.

```c++
constexpr reference front() noexcept
constexpr const_reference front() const noexcept
```

Accesses the first element in the `statvec`. It is the responsibility of the user to ensure this is not called on an empty `statvec`.

```c++
constexpr reference back() noexcept
constexpr const_reference back() const noexcept
```

Accesses the last element (i.e. the one before `end()`). The user must ensure there are elements in the vector.

```c++
constexpr pointer data() noexcept
constexpr const_pointer data() const noexcept
```

Returns a pointer to the underlying memory, the size of which is always `N * sizeof(T)` bytes.

### Size, Capacity, etc.

```c++
constexpr bool empty() const noexcept
```

Returns the equivalent of `size() == 0`.

```c++
constexpr size_type size() const noexcept
```

Returns the current size of the `statvec`.

```c++
constexpr size_type max_capacity() const noexcept
```

Returns the capacity `N` of the `statvec`.

```c++
constexpr size_type capacity() const noexcept
```

Equivalent to `max_capacity()`.

### Mutators

```c++
constexpr void swap(statvec& other)
```

Swaps the content of `*this` with `other`. The function is `noexcept` if `std::swap(std::declval<T&>(), std::declval<T&>())` is `noexcept`.

```c++
template <std::size_t M>
constexpr statvec<T, M> reserve() const &
constexpr statvec<T, M> reserve() &&

template <std::size_t M, typename Vec>
constexpr auto statvec_reserve(Vec&& vec)
```

Creates a new vector with capacity `M` by either copying or moving `std::min(M, N)` elements from `*this`. The free function is provided for avoiding the need for the `.template` syntax (c.f. the rationale behind `std::get()`). The function templates are `noexcept` if `T` is nothrow copy constructible, nothrow move constructible or if the expressions `std::forward<Vec>(vec).template reserve<M>()` is `noexcept`, respectively. 

```c++
constexpr void clear() noexcept
```

Clears `*this` by setting the size to 0.

```c++
constexpr bool resize(size_type size) noexcept
```

Resizes the `statvec` to `std::min(size, capacity())` and returns `true` if `size <= capacity()`, otherwise `false`.

```c++
constexpr iterator insert(const_iterator pos, T& const value) 
constexpr iterator insert(const_iterator pos, T&& value) 
```

If `size() < capacity()`, copy or move assigns, respectively, `value` at the position just before `pos` and returns an iterator to the newly inserted element. Otherwise, `end()` is returned. The function is `noexcept` if `T` both is nothrow copy and move assignable or just nothrow move assignable, respectively.

```c++
constexpr iterator insert(const_iterator pos, size_type count, T& const value)
```

If `size() + count <= capacity()`, inserts `value` in `count` elements starting at the position just before `pos` and returns an iterator to the first newly inserted element. Otherwise, no elements are inserted an `end()` is returned. The function is `noexcept` if `T` is nothrow assignable.

```c++
template <typename InputIt>
constexpr iterator insert(const_iterator pos, InputIt first, InputIt last)
```

If `size() + std::distance(first, last) > capacity()`, `end()` is returned and no elements are inserted. If `first == last`, no elements are inserted and `pos` is returned. Otherwise, each element in the range `[first, last)` is inserted, starting at the position just before `pos` and an iterator to the first newly inserted element is returned. The function template is `noexcept` if the expression `std::declval<T&>() = *first` is `noexcept` it participates in overload resolution only if `InputIt` is incrementable, dereferencable and comparable.

```c++
template <typename... Ts>
constexpr iterator emplace(const_iterator pos, Ts&&... args)
``` 

If `size() < capacity()`, constructs an object of type `T` in-place at the position just before `pos` and returns an iterator to the newly inserted element. Otherwise, no element is inserted `end()` is returned. The function is `noexcept` if `T` is nothrow move assignable and in-place construction itself is `noexcept`.

```c++
constexpr bool push_back(T const& value)
constexpr bool push_back(T&& value)
```

If `size() < capacity()`, increments the size, copy or move assigns, respectively, `value` to the last element and returns `true`. Otherwise, the `statvec` remains unchanged and `false` is returned. The functions are `noexcept` if `T` is nothrow copy assignable and nothrow move constructible, respectively.

```c++
template <typename... Ts>
constexpr bool emplace_back(Ts&&... args)
```

If `size() < capacity()`, increments the size, constructs an object of type `T` in-place at the end of the `statvec` and returns `true`. Otherwise, `false` is returned and neither the `statvec` nor its size modified. The function template is `noexcept` if the in-place construction itself is.

```c++
constexpr T pop_back()
```

Pops the last element from the `statvec` and returns it. No check against `size()` is performed, meaning that poping from an empty `statvec` will underflow. The function is `noexcept` if `T` is nothrow copy constructible.

```c++
constexpr iterator erase(const_iterator pos)
```

Erases the element at `pos`, shifts each element in the range `[pos + 1, cend())` down one step and returns an iterator to the first element of said range. The function is `noexcept` is `T` is nothrow move assignable.

```c++
constexpr iterator erase(const_iterator first, const_iterator last)
```

Erases each element in the range `[first, last)` by replacing them with the elements in the range `[last, cend())` and returns an iterator to the first element in the latter range. The function is `noexcept` if `T` is nothrow move assignable.

### Iterators

```c++
constexpr iterator begin() noexcept
constexpr const_iterator begin() const noexcept
constexpr const_iterator cbegin() const noexcept

constexpr iterator end() noexcept
constexpr const_iterator end() const noexcept
constexpr const_iterator cend() const noexcept

constexpr reverse_iterator rbegin() noexcept
constexpr const_reverse_iterator rbegin() const noexcept
constexpr const_Reverse_iterator crbegin() const noexcept

constexpr reverse_iterator rend() noexcept
constexpr const_reverse_iterator rend() const noexcept
constexpr const_reverse_iterator crend() const noexcept
```

Provide access to the iterator ranges of the `statvec`.

### Comparison Operators

```c++
bool operator==(statvec const& lhs, statvec const& rhs) noexcept
bool operator!=(statvec const& lhs, statvec const& rhs) noexcept
bool operator<=(statvec const& lhs, statvec const& rhs) noexcept
bool operator>=(statvec const& lhs, statvec const& rhs) noexcept
bool operator<(statvec const& lhs, statvec const& rhs) noexcept
bool operator>(statvec const& lhs, statvec const& rhs) noexcept
```

Perform lexicographical comparisons of `statvec`s.
