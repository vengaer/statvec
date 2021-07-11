#ifndef STATVEC_H
#define STATVEC_H

#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

template <typename T, std::size_t N>
class statvec;

namespace detail {

template <typename Derived, typename Pointer>
class iterbase {
    using derived_type = Derived;
    public:
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;

        constexpr iterbase() noexcept = default;

        template <typename T = Derived>
        constexpr typename T::reference operator*() noexcept;
        template <typename T = Derived>
        constexpr typename T::const_reference operator*() const noexcept;

        template <typename T = Derived>
        constexpr typename T::reference operator[](typename T::difference_type i) noexcept;
        template <typename T = Derived>
        constexpr typename T::const_reference operator[](typename T::difference_type i) const noexcept;

        template <typename T = Derived>
        constexpr typename T::pointer operator->() noexcept;
        template <typename T = Derived>
        constexpr typename T::const_pointer operator->() const noexcept;

        constexpr derived_type& operator++() noexcept;
        constexpr derived_type operator++(int) noexcept;

        constexpr derived_type& operator--() noexcept;
        constexpr derived_type operator--(int) noexcept;

        template <typename T = Derived>
        constexpr derived_type& operator+=(typename T::difference_type n) noexcept;
        template <typename T = Derived>
        constexpr derived_type& operator-=(typename T::difference_type n) noexcept;

        constexpr void swap(derived_type const& other) noexcept;

    protected:
        Pointer ptr_{};

        constexpr iterbase(Pointer ptr_) noexcept;

        template <typename, std::size_t>
        friend class ::statvec;
};

template <typename T>
struct iterator : iterbase<iterator<T>, typename T::pointer> {
    using value_type        = typename T::value_type;
    using reference         = typename T::reference;
    using const_reference   = typename T::const_reference;
    using pointer           = typename T::pointer;
    using const_pointer     = typename T::const_pointer;
    using difference_type   = typename iterbase<iterator<T>, pointer>::difference_type;
    using iterator_category = typename iterbase<iterator<T>, pointer>::iterator_category;

    using iterbase<iterator<T>, pointer>::iterbase;

    constexpr iterator(iterator const& other) noexcept = default;
    constexpr iterator& operator=(iterator const& other) & noexcept = default;

    template <typename U>
    friend constexpr bool operator==(iterator<U> const& lhs, iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator!=(iterator<U> const& lhs, iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator<=(iterator<U> const& lhs, iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator>=(iterator<U> const& lhs, iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator<(iterator<U> const& lhs, iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator>(iterator<U> const& lhs, iterator<U> const& rhs) noexcept;

    template <typename U>
    friend constexpr iterator<U> operator+(iterator<U> const& it, typename iterator<U>::difference_type n) noexcept;
    template <typename U>
    friend constexpr iterator<U>  operator+(typename iterator<U>::difference_type n, iterator<U> const& it) noexcept;
    template <typename U>
    friend constexpr iterator<U> operator-(iterator<U> const& it, typename iterator<U>::difference_type n) noexcept;
    template <typename U>
    friend constexpr typename iterator<U>::difference_type operator-(iterator<U> const& lhs, iterator<U> const& rhs) noexcept;

    template <typename>
    friend class const_iterator;
};

template <typename T>
struct const_iterator : iterbase<const_iterator<T>, typename T::const_pointer> {
    using value_type        = typename T::value_type;
    using reference         = typename T::const_reference;
    using const_reference   = typename T::const_reference;
    using pointer           = typename T::const_pointer;
    using const_pointer     = typename T::const_pointer;
    using difference_type   = typename iterbase<const_iterator<T>, pointer>::difference_type;
    using iterator_category = typename iterbase<const_iterator<T>, pointer>::iterator_category;

    using iterbase<const_iterator<T>, pointer>::iterbase;
    constexpr const_iterator(const_iterator const& other) noexcept = default;
    constexpr const_iterator(iterator<T> const& other) noexcept;

    constexpr const_iterator& operator=(iterator<T> const& other) & noexcept;
    constexpr const_iterator& operator=(const_iterator const& other) & noexcept = default;

    template <typename U>
    friend constexpr bool operator==(const_iterator<U> const& lhs, const_iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator!=(const_iterator<U> const& lhs, const_iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator<=(const_iterator<U> const& lhs, const_iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator>=(const_iterator<U> const& lhs, const_iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator<(const_iterator<U> const& lhs, const_iterator<U> const& rhs) noexcept;
    template <typename U>
    friend constexpr bool operator>(const_iterator<U> const& lhs, const_iterator<U> const& rhs) noexcept;

    template <typename U>
    friend constexpr const_iterator<U> operator+(const_iterator<U> const& it, typename const_iterator<U>::difference_type n) noexcept;
    template <typename U>
    friend constexpr const_iterator<U>  operator+(typename const_iterator<U>::difference_type n, const_iterator<U> const& it) noexcept;
    template <typename U>
    friend constexpr const_iterator<U> operator-(const_iterator<U> const& it, typename const_iterator<U>::difference_type n) noexcept;
    template <typename U>
    friend constexpr typename const_iterator<U>::difference_type operator-(const_iterator<U> const& lhs, const_iterator<U> const& rhs) noexcept;
};

template <typename>
struct is_std_array : std::false_type { };

template <typename T, std::size_t N>
struct is_std_array<std::array<T, N>> : std::true_type { };

template <typename T>
inline bool constexpr is_std_array_v = is_std_array<T>::value;

template <typename T>
struct remove_cvref
    : std::remove_cv<std::remove_reference_t<T>>
{ };

template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

template <typename>
struct is_statvec : std::false_type { };

template <typename T, std::size_t N>
struct is_statvec<statvec<T, N>> : std::true_type { };

template <typename T>
inline bool constexpr is_statvec_v = is_statvec<T>::value;

template <typename T, typename U = void>
using enable_variadic_constructor_t = std::enable_if_t<!is_std_array_v<T> && !is_statvec_v<T>, U>;

template <typename, typename = void>
struct is_input_iterator : std::false_type { };

template <typename T>
struct is_input_iterator<T, std::void_t<decltype(++std::declval<T&>()), decltype(*std::declval<T&>()),
                                        decltype(std::declval<T&>() == std::declval<T&>())>> : std::true_type { };

template <typename T>
inline bool constexpr is_input_iterator_v = is_input_iterator<T>::value;

template <typename T, typename U = void>
using enable_if_input_iterator_t = std::enable_if_t<is_input_iterator_v<T>, U>;

} // namespace detail

template <typename T, std::size_t N>
class statvec {
    static_assert(!std::is_reference_v<T>);
    static_assert(N);

    template <typename U>
    using remove_cvref_t = detail::remove_cvref_t<U>;
    template <typename U>
    using enable_variadic_constructor_t = detail::enable_variadic_constructor_t<U>;
    template <typename U>
    using enable_if_input_iterator_t = detail::enable_if_input_iterator_t<U>;

    public:
        using value_type             = typename std::array<T, N>::value_type;
        using reference              = typename std::array<T, N>::reference;
        using const_reference        = typename std::array<T, N>::const_reference;
        using pointer                = typename std::array<T, N>::pointer;
        using const_pointer          = typename std::array<T, N>::const_pointer;
        using size_type              = typename std::array<T, N>::size_type;

        using iterator               = detail::iterator<statvec<T, N>>;
        using const_iterator         = detail::const_iterator<statvec<T, N>>;
        using reverse_iterator       = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        constexpr statvec() noexcept = default;

        template <typename T0, typename... T1toN, typename = enable_variadic_constructor_t<remove_cvref_t<T0>>>
        constexpr statvec(T0&& first, T1toN&&... rest) noexcept(std::is_nothrow_constructible_v<T, T0&&> &&
                                                               (std::is_nothrow_constructible_v<T, T1toN&&> && ...) &&
                                                                std::is_nothrow_assignable_v<T, T0&&> &&
                                                               (std::is_nothrow_assignable_v<T, T1toN&&> && ...));
        template <std::size_t M>
        constexpr statvec(std::array<T, M> const& array) noexcept(std::is_nothrow_copy_constructible_v<T> &&
                                                                  std::is_nothrow_copy_assignable_v<T>);
        template <std::size_t M>
        constexpr statvec(std::array<T, M>&& array) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                             std::is_nothrow_move_assignable_v<T>);

        constexpr statvec(statvec const& other) noexcept(std::is_nothrow_copy_constructible_v<T>) = default;
        constexpr statvec(statvec&& other) noexcept(std::is_nothrow_move_constructible_v<T>) = default;

        constexpr statvec& operator=(statvec const& other) & noexcept(std::is_nothrow_copy_assignable_v<T>);
        constexpr statvec& operator=(statvec&& other) & noexcept(std::is_nothrow_move_assignable_v<T>);

        constexpr statvec& operator=(std::array<T, N> const& other) & noexcept(std::is_nothrow_copy_assignable_v<T>);
        constexpr statvec& operator=(std::array<T, N>&& other) & noexcept(std::is_nothrow_move_assignable_v<T>);

        constexpr bool assign(size_type count, T const& value) noexcept(std::is_nothrow_copy_assignable_v<T>);
        template <typename It, typename = enable_if_input_iterator_t<remove_cvref_t<It>>>
        constexpr bool assign(It first, It last) noexcept(std::is_nothrow_assignable_v<T, decltype(*first)>);

        constexpr reference operator[](size_type i) noexcept;
        constexpr const_reference operator[](size_type i) const noexcept;

        constexpr reference at(size_type i);
        constexpr const_reference at(size_type i) const;

        constexpr reference front() noexcept;
        constexpr const_reference front() const noexcept;

        constexpr reference back() noexcept;
        constexpr const_reference back() const noexcept;

        constexpr pointer data() noexcept;
        constexpr const_pointer data() const noexcept;

        constexpr bool empty() const noexcept;
        constexpr size_type size() const noexcept;
        constexpr size_type max_size() const noexcept;
        constexpr size_type capacity() const noexcept;

        constexpr void swap(statvec& other) noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>())));

        template <std::size_t M>
        [[nodiscard]] constexpr statvec<T, M> reserve() const & noexcept(std::is_nothrow_copy_constructible_v<T>);
        template <std::size_t M>
        [[nodiscard]] constexpr statvec<T, M> reserve() && noexcept(std::is_nothrow_move_constructible_v<T>);

        constexpr void clear() noexcept;
        constexpr bool resize(size_type size) noexcept;

        constexpr iterator insert(const_iterator pos, T const& value) noexcept(std::is_nothrow_copy_assignable_v<T> &&
                                                                               std::is_nothrow_move_assignable_v<T>);
        constexpr iterator insert(const_iterator pos, T&& value) noexcept(std::is_nothrow_move_assignable_v<T>);
        constexpr iterator insert(const_iterator pos, size_type count, T const& value) noexcept(std::is_nothrow_assignable_v<T>);
        template <typename It, typename = enable_if_input_iterator_t<remove_cvref_t<It>>>
        constexpr iterator insert(const_iterator pos, It first, It last) noexcept(std::is_nothrow_assignable_v<T, decltype(*first)> &&
                                                                                  std::is_nothrow_move_assignable_v<T>);

        template <typename... Ts>
        constexpr iterator emplace(const_iterator pos, Ts&&... args) noexcept(std::is_nothrow_constructible_v<T, Ts&&...> &&
                                                                              std::is_nothrow_move_assignable_v<T>);

        constexpr bool push_back(T const& value) noexcept(std::is_nothrow_copy_assignable_v<T>);
        constexpr bool push_back(T&& value) noexcept(std::is_nothrow_move_assignable_v<T>);

        template <typename... Ts>
        constexpr bool emplace_back(Ts&&... args) noexcept(std::is_nothrow_constructible_v<T, Ts&&...>);

        constexpr T pop_back() noexcept(std::is_nothrow_copy_constructible_v<T>);

        constexpr iterator erase(const_iterator pos) noexcept(std::is_nothrow_move_assignable_v<T>);
        constexpr iterator erase(const_iterator first, const_iterator last) noexcept(std::is_nothrow_move_assignable_v<T>);

        constexpr iterator begin() noexcept;
        constexpr iterator end() noexcept;

        constexpr const_iterator begin() const noexcept;
        constexpr const_iterator end() const noexcept;

        constexpr const_iterator cbegin() const noexcept;
        constexpr const_iterator cend() const noexcept;

        constexpr reverse_iterator rbegin() noexcept;
        constexpr reverse_iterator rend() noexcept;

        constexpr const_reverse_iterator rbegin() const noexcept;
        constexpr const_reverse_iterator rend() const noexcept;

        constexpr const_reverse_iterator crbegin() const noexcept;
        constexpr const_reverse_iterator crend() const noexcept;

        template <typename U, std::size_t M, std::size_t K>
        friend constexpr bool operator==(statvec<U, M> const& lhs, statvec<U, K> const& rhs) noexcept;
        template <typename U, std::size_t M, std::size_t K>
        friend constexpr bool operator!=(statvec<U, M> const& lhs, statvec<U, K> const& rhs) noexcept;
        template <typename U, std::size_t M, std::size_t K>
        friend constexpr bool operator<=(statvec<U, M> const& lhs, statvec<U, K> const& rhs) noexcept;
        template <typename U, std::size_t M, std::size_t K>
        friend constexpr bool operator>=(statvec<U, M> const& lhs, statvec<U, K> const& rhs) noexcept;
        template <typename U, std::size_t M, std::size_t K>
        friend constexpr bool operator<(statvec<U, M> const& lhs, statvec<U, K> const& rhs) noexcept;
        template <typename U, std::size_t M, std::size_t K>
        friend constexpr bool operator>(statvec<U, M> const& lhs, statvec<U, K> const& rhs) noexcept;

    private:
        std::array<T, N> buf_{};
        size_type size_{};

        template < typename... Ts, std::size_t... Is>
        static constexpr std::array<T, N> construct(std::index_sequence<Is...>, Ts&&... args) noexcept((std::is_nothrow_constructible_v<T, Ts&&> && ...));
        template <std::size_t M, typename Array, std::size_t... Is>
        static constexpr std::array<T, M> forward_array(Array&& array, std::index_sequence<Is...>) noexcept(std::is_nothrow_constructible_v<std::array<T, N>, Array&&>);
};

template <typename T0, typename... T1toN>
statvec(T0, T1toN...) -> statvec<T0, sizeof...(T1toN) + 1>;

template <typename T, std::size_t N>
statvec(std::array<T, N> const&) -> statvec<T, N>;

template <typename T, std::size_t N>
statvec(std::array<T, N>&&) -> statvec<T, N>;

template <typename T, std::size_t N>
template <typename T0, typename... T1toN, typename>
constexpr statvec<T, N>::statvec(T0&& first, T1toN&&... rest) noexcept(std::is_nothrow_constructible_v<T, T0&&> &&
                                                                      (std::is_nothrow_constructible_v<T, T1toN&&> && ...) &&
                                                                       std::is_nothrow_assignable_v<T, T0&&> &&
                                                                      (std::is_nothrow_assignable_v<T, T1toN&&> && ...))
    : buf_{construct(std::index_sequence_for<T0, T1toN...>{}, std::forward<T0>(first), std::forward<T1toN>(rest)...)},
      size_{sizeof...(rest) + 1u}
{
    static_assert(sizeof...(rest) + 1u <= N);
}

template <typename T,std::size_t N>
template <std::size_t M>
constexpr statvec<T, N>::statvec(std::array<T, M> const& array) noexcept(std::is_nothrow_copy_constructible_v<T> &&
                                                                         std::is_nothrow_copy_assignable_v<T>)
    : buf_{forward_array<N>(array, std::make_index_sequence<M>{})},
      size_{M}
{
    static_assert(M <= N);
}

template <typename T, std::size_t N>
template <std::size_t M>
constexpr statvec<T, N>::statvec(std::array<T, M>&& array) noexcept(std::is_nothrow_move_constructible_v<T> &&
                                                                    std::is_nothrow_move_assignable_v<T>)
    : buf_{forward_array<N>(std::move(array), std::make_index_sequence<M>{})},
      size_{M}
{
    static_assert(M <= N);
}

template <typename T, std::size_t N>
constexpr statvec<T, N>& statvec<T, N>::operator=(statvec const& other) & noexcept(std::is_nothrow_copy_assignable_v<T>) {
    std::copy(std::begin(other), std::end(other), std::begin(*this));
    size_ = other.size_;
    return *this;
}

template <typename T, std::size_t N>
constexpr statvec<T, N>& statvec<T, N>::operator=(statvec&& other) & noexcept(std::is_nothrow_move_assignable_v<T>) {
    std::move(std::begin(other), std::end(other), std::begin(*this));
    size_ = other.size_;
    return *this;
}

template <typename T, std::size_t N>
constexpr statvec<T, N>& statvec<T,N>::operator=(std::array<T, N> const& other) & noexcept(std::is_nothrow_copy_assignable_v<T>) {
    std::copy(std::begin(other), std::end(other), std::begin(*this));
    size_ = other.size();
    return *this;
}

template <typename T, std::size_t N>
constexpr statvec<T, N>& statvec<T, N>::operator=(std::array<T, N>&& other) & noexcept(std::is_nothrow_move_assignable_v<T>) {
    std::move(std::begin(other), std::end(other), std::begin(*this));
    size_ = other.size();
    return *this;
}

template <typename T, std::size_t N>
constexpr bool statvec<T, N>::assign(size_type count, T const& value) noexcept(std::is_nothrow_copy_assignable_v<T>) {
    if(count > capacity()) {
        for(size_ = 0u;  size_ < capacity(); ++size_) {
            buf_[size_] = value;
        }
        return false;
    }
    for(size_ = 0u; size_ < count; ++size_) {
        buf_[size_] = value;
    }
    return true;
}

template <typename T, std::size_t N>
template <typename It, typename>
constexpr bool statvec<T, N>::assign(It first, It last) noexcept(std::is_nothrow_assignable_v<T, decltype(*first)>) {
    auto diff = std::distance(first, last);
    if(diff > (decltype(diff))capacity()) {
        for(size_ = 0; size_ < capacity(); size_++) {
            buf_[size_] = *first++;
        }
        return false;
    }
    auto it = begin();
    while(first != last) {
        *it++ = *first++;
    }
    return true;
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::reference statvec<T, N>::operator[](size_type i) noexcept {
    return buf_[i];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_reference statvec<T, N>::operator[](size_type i) const noexcept {
    return buf_[i];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::reference statvec<T, N>::at(size_type i) {
    using namespace std::string_literals;
    if(i >= size()) {
        throw std::out_of_range("Cannot access element at index"s + std::to_string(i));
    }
    return buf_[i];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_reference statvec<T, N>::at(size_type i) const {
    using namespace std::string_literals;
    if(i >= size()) {
        throw std::out_of_range("Cannot access element at index"s + std::to_string(i));
    }
    return buf_[i];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::reference statvec<T,N>::front() noexcept {
    return buf_[0];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_reference statvec<T, N>::front() const noexcept {
    return buf_[0];
}

template <typename T, std::size_t N>
constexpr bool statvec<T, N>::empty() const noexcept {
    return !size();
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::reference statvec<T,N>::back() noexcept {
    return buf_[size() - 1u];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_reference statvec<T, N>::back() const noexcept {
    return buf_[size() - 1u];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::pointer statvec<T, N>::data() noexcept {
    return buf_.data();
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_pointer statvec<T, N>::data() const noexcept {
    return buf_.data();
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::size_type statvec<T, N>::size() const noexcept {
    return size_;
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::size_type statvec<T, N>::max_size() const noexcept {
    return capacity();
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::size_type statvec<T, N>::capacity() const noexcept {
    return N;
}

template <typename T, std::size_t N>
constexpr void statvec<T, N>::swap(statvec& other) noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>()))) {
    buf_.swap(other.buf_);
    std::swap(size_, other.size_);
}

template <typename T, std::size_t N>
template <std::size_t M>
[[nodiscard]] constexpr statvec<T, M> statvec<T, N>::reserve() const & noexcept(std::is_nothrow_copy_constructible_v<T>) {
    if constexpr(M > N) {
        return statvec<T, M>{forward_array<M>(buf_, std::make_index_sequence<N>{})};
    }
    else {
        return statvec<T, M>{forward_array<M>(buf_, std::make_index_sequence<M>{})};
    }
}

template <typename T, std::size_t N>
template <std::size_t M>
[[nodiscard]] constexpr statvec<T, M> statvec<T, N>::reserve() && noexcept(std::is_nothrow_move_constructible_v<T>) {
    if constexpr(M > N) {
        return statvec<T, M>{forward_array<M>(std::move(buf_), std::make_index_sequence<N>{})};
    }
    else {
        return statvec<T, M>{forward_array<M>(std::move(buf_), std::make_index_sequence<M>{})};
    }
}

template <typename T, std::size_t N>
constexpr void statvec<T, N>::clear() noexcept {
    size_ = 0u;
}

template <typename T, std::size_t N>
constexpr bool statvec<T, N>::resize(size_type size) noexcept {
    if(size > capacity()) {
        size_ = capacity();
        return false;
    }
    size_ = size;
    return true;
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::iterator
statvec<T, N>::insert(const_iterator pos, T const& value) noexcept(std::is_nothrow_copy_assignable_v<T> &&
                                                                   std::is_nothrow_move_assignable_v<T>)
{
    if(size() == capacity()) {
        return end();
    }
    ++size_;
    auto it = begin() + std::distance(cbegin(), pos);
    std::move_backward(it, end(), end() + 1);
    *it = value;
    return it;
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::iterator
statvec<T, N>::insert(const_iterator pos, T&& value) noexcept(std::is_nothrow_move_assignable_v<T>) {
    if(size() == capacity()) {
        return end();
    }
    auto it = begin() + std::distance(cbegin(), pos);
    std::move_backward(it, end(), end() + 1);
    *it = std::move(value);
    ++size_;
    return it;
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::iterator
statvec<T, N>::insert(const_iterator pos, size_type count, T const& value) noexcept(std::is_nothrow_assignable_v<T>) {
    if(size() + count > capacity()) {
        return end();
    }
    auto it = begin() + std::distance(cbegin(), pos);
    std::move_backward(it, end(), end() + count);
    while(count--) {
        *it++ = value;
    }
    size_ += count;
    return true;
}

template <typename T, std::size_t N>
template <typename It, typename>
constexpr typename statvec<T, N>::iterator
statvec<T, N>::insert(const_iterator pos, It first, It last) noexcept(std::is_nothrow_assignable_v<T, decltype(*first)> &&
                                                                      std::is_nothrow_move_assignable_v<T>) {
    auto diff = std::distance(first, last);

    if(size() + diff > capacity()) {
        return end();
    }
    auto it = begin() + std::distance(cbegin(), pos);
    if(first == last) {
        return it;
    }
    std::move_backward(it, end(), end() + diff);
    std::copy(first, last, it);
    size_ += diff;
    return it;
}

template <typename T, std::size_t N>
template <typename... Ts>
constexpr typename statvec<T, N>::iterator
statvec<T, N>::emplace(const_iterator pos, Ts&&... args) noexcept(std::is_nothrow_constructible_v<T, Ts&&...> &&
                                                                  std::is_nothrow_move_assignable_v<T>)
{
    if(size() == capacity()) {
        return end();
    }
    std::size_t diff = std::distance(cbegin(), pos);
    auto it = begin() + diff;
    std::move_backward(it, end(), end() + 1);
    ::new(&buf_[diff]) T(std::forward<Ts>(args)...);
    return it;
}

template <typename T, std::size_t N>
constexpr bool statvec<T, N>::push_back(T const& value) noexcept(std::is_nothrow_copy_assignable_v<T>) {
    if(size() == capacity()) {
        return false;
    }
    buf_[size_++] = value;
    return true;
}

template <typename T, std::size_t N>
constexpr bool statvec<T, N>::push_back(T&& value) noexcept(std::is_nothrow_move_assignable_v<T>) {
    if(size() == capacity()) {
        return false;
    }
    buf_[size_++] = std::move(value);
    return true;
}

template <typename T, std::size_t N>
template <typename... Ts>
constexpr bool statvec<T, N>::emplace_back(Ts&&... args) noexcept(std::is_nothrow_constructible_v<T, Ts&&...>) {
    if(size() == capacity()) {
        return false;
    }
    ::new(&buf_[size_++]) T(std::forward<Ts>(args)...);
    return true;
}

template <typename T, std::size_t N>
constexpr T statvec<T, N>::pop_back() noexcept(std::is_nothrow_copy_constructible_v<T>) {
    return buf_[size_-- - 1u];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::iterator
statvec<T, N>::erase(const_iterator pos) noexcept(std::is_nothrow_move_assignable_v<T>) {
    auto it = begin() + std::distance(cbegin(), pos);
    std::move(it + 1, end(), it);
    --size_;
    return it;
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::iterator
statvec<T, N>::erase(const_iterator first, const_iterator last) noexcept(std::is_nothrow_move_assignable_v<T>) {
    auto dst = begin() + std::distance(cbegin(), first);
    auto src = begin() + std::distance(cbegin(), last);
    std::move(src, end(), dst);
    size_ -= std::distance(first, last);
    return dst;
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::iterator statvec<T, N>::begin() noexcept {
    return &buf_[0];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::iterator statvec<T, N>::end() noexcept {
    return &buf_[size()];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_iterator statvec<T, N>::begin() const noexcept {
    return &buf_[0];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_iterator statvec<T, N>::end() const noexcept {
    return &buf_[size()];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_iterator statvec<T, N>::cbegin() const noexcept {
    return &buf_[0];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_iterator statvec<T, N>::cend() const noexcept {
    return &buf_[size()];
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::reverse_iterator statvec<T, N>::rbegin() noexcept {
    return reverse_iterator{end()};
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::reverse_iterator statvec<T, N>::rend() noexcept {
    return reverse_iterator{begin()};
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_reverse_iterator statvec<T, N>::rbegin() const noexcept {
    return const_reverse_iterator{cend()};
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_reverse_iterator statvec<T, N>::rend() const noexcept {
    return const_reverse_iterator{cbegin()};
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_reverse_iterator statvec<T, N>::crbegin() const noexcept {
    return const_reverse_iterator{cend()};
}

template <typename T, std::size_t N>
constexpr typename statvec<T, N>::const_reverse_iterator statvec<T, N>::crend() const noexcept {
    return const_reverse_iterator{cbegin()};
}

template <typename T, std::size_t N>
template < typename... Ts, std::size_t... Is>
constexpr std::array<T, N> statvec<T, N>::construct(std::index_sequence<Is...>, Ts&&... args) noexcept((std::is_nothrow_constructible_v<T, Ts&&> && ...)) {
    return {(std::forward<Ts>(args))...};
}

template <typename T, std::size_t N>
template <std::size_t M, typename Array, std::size_t... Is>
constexpr std::array<T, M> statvec<T, N>::forward_array(Array&& array, std::index_sequence<Is...>) noexcept(std::is_nothrow_constructible_v<std::array<T, N>, Array&&>) {
    if constexpr(std::is_rvalue_reference_v<decltype(array)>) {
        return {(std::move(array[Is]))...};
    }
    else {
        return {(array[Is])...};
    }
}

template <typename T, std::size_t N, std::size_t M>
constexpr bool operator==(statvec<T, N> const& lhs, statvec<T, M> const& rhs) noexcept {
    return std::equal(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs));
}

template <typename T, std::size_t N, std::size_t M>
constexpr bool operator!=(statvec<T, N> const& lhs, statvec<T, M> const& rhs) noexcept {
    return !(lhs == rhs);
}

template <typename T, std::size_t N, std::size_t M>
constexpr bool operator<=(statvec<T, N> const& lhs, statvec<T, M> const& rhs) noexcept {
    return !(lhs > rhs);
}

template <typename T, std::size_t N, std::size_t M>
constexpr bool operator>=(statvec<T, N> const& lhs, statvec<T, M> const& rhs) noexcept {
    return !(lhs < rhs);
}

template <typename T, std::size_t N, std::size_t M>
constexpr bool operator<(statvec<T, N> const& lhs, statvec<T, M> const& rhs) noexcept {
    return std::lexicographical_compare(std::begin(lhs), std::end(lhs), std::begin(rhs), std::end(rhs));
}

template <typename T, std::size_t N, std::size_t M>
constexpr bool operator>(statvec<T, N> const& lhs, statvec<T, M> const& rhs) noexcept {
    return std::lexicographical_compare(std::begin(rhs), std::end(rhs), std::begin(lhs), std::end(lhs));
}

template <std::size_t M, typename Vec>
[[nodiscard]] constexpr auto statvec_reserve(Vec&& vec) noexcept(noexcept(std::forward<Vec>(vec).template reserve<M>())) {
    return std::forward<Vec>(vec).template reserve<M>();
}

namespace detail {

template <typename T, typename P>
constexpr iterbase<T, P>::iterbase(P ptr) noexcept
    : ptr_{ptr} { }

template <typename T, typename P>
template <typename U>
constexpr typename U::reference iterbase<T, P>::operator*() noexcept {
    return *ptr_;
}

template <typename T, typename P>
template <typename U>
constexpr typename U::const_reference iterbase<T, P>::operator*() const noexcept {
    return *ptr_;
}

template <typename T, typename P>
template <typename U>
constexpr typename U::reference iterbase<T, P>::operator[](typename U::difference_type i) noexcept {
    return ptr_[i];
}

template <typename T, typename P>
template <typename U>
constexpr typename U::const_reference iterbase<T, P>::operator[](typename U::difference_type i) const noexcept {
    return ptr_[i];
}

template <typename T, typename P>
template <typename U>
constexpr typename U::pointer iterbase<T, P>::operator->() noexcept {
    return ptr_;
}

template <typename T, typename P>
template <typename U>
constexpr typename U::const_pointer iterbase<T, P>::operator->() const noexcept {
    return ptr_;
}

template <typename T, typename P>
constexpr typename iterbase<T, P>::derived_type& iterbase<T, P>::operator++() noexcept {
    ++ptr_;
    return static_cast<derived_type&>(*this);
}

template <typename T, typename P>
constexpr typename iterbase<T, P>::derived_type iterbase<T, P>::operator++(int) noexcept {
    auto it = static_cast<derived_type&>(*this);
    ++(*this);
    return it;
}

template <typename T, typename P>
constexpr typename iterbase<T, P>::derived_type& iterbase<T, P>::operator--() noexcept {
    --ptr_;
    return static_cast<derived_type&>(*this);
}

template <typename T, typename P>
constexpr typename iterbase<T, P>::derived_type iterbase<T, P>::operator--(int) noexcept {
    auto it = static_cast<derived_type&>(*this);
    --(*this);
    return it;
}

template <typename T, typename P>
template <typename U>
constexpr typename iterbase<T, P>::derived_type& iterbase<T, P>::operator+=(typename U::difference_type n) noexcept {
    ptr_ += n;
    return static_cast<derived_type&>(*this);
}

template <typename T, typename P>
template <typename U>
constexpr typename iterbase<T, P>::derived_type& iterbase<T, P>::operator-=(typename U::difference_type n) noexcept {
    return *this += -n;
}

template <typename T, typename P>
constexpr void iterbase<T, P>::swap(typename iterbase<T, P>::derived_type const& other) noexcept {
    std::swap(ptr_, other.ptr_);
}

/* iterator */

template <typename T>
constexpr bool operator==(iterator<T> const& lhs, iterator<T> const& rhs) noexcept {
    return lhs.ptr_ == rhs.ptr_;
}

template <typename T>
constexpr bool operator!=(iterator<T> const& lhs, iterator<T> const& rhs) noexcept {
    return !(lhs == rhs);
}

template <typename T>
constexpr bool operator<=(iterator<T> const& lhs, iterator<T> const& rhs) noexcept {
    return lhs.ptr_ <= rhs.ptr_;
}

template <typename T>
constexpr bool operator>=(iterator<T> const& lhs, iterator<T> const& rhs) noexcept {
    return lhs.ptr_ >= rhs.ptr_;
}

template <typename T>
constexpr bool operator<(iterator<T> const& lhs, iterator<T> const& rhs) noexcept {
    return !(lhs >= rhs);
}

template <typename T>
constexpr bool operator>(iterator<T> const& lhs, iterator<T> const& rhs) noexcept {
    return !(lhs <= rhs);
}

template <typename T>
constexpr iterator<T> operator+(iterator<T> const& it, typename iterator<T>::difference_type n) noexcept {
    return iterator<T>{it.ptr_ + n};
}

template <typename T>
constexpr iterator<T>  operator+(typename iterator<T>::difference_type n, iterator<T> const& it) noexcept {
    return it + n;
}

template <typename T>
constexpr iterator<T> operator-(iterator<T> const& it, typename iterator<T>::difference_type n) noexcept {
    return it + -n;
}

template <typename T>
constexpr typename iterator<T>::difference_type operator-(iterator<T> const& lhs, iterator<T> const& rhs) noexcept {
    return lhs.ptr_ - rhs.ptr_;
}

/* const_iterator */

template <typename T>
constexpr const_iterator<T>::const_iterator(iterator<T> const& other) noexcept
    : iterbase<const_iterator<T>, pointer>{other.ptr_} { }

template <typename T>
constexpr const_iterator<T>& const_iterator<T>::operator=(iterator<T> const& other) & noexcept {
    this->ptr_ = other.ptr_;
    return *this;
}

template <typename T>
constexpr bool operator==(const_iterator<T> const& lhs, const_iterator<T> const& rhs) noexcept {
    return lhs.ptr_ == rhs.ptr_;
}

template <typename T>
constexpr bool operator!=(const_iterator<T> const& lhs, const_iterator<T> const& rhs) noexcept {
    return !(lhs == rhs);
}

template <typename T>
constexpr bool operator<=(const_iterator<T> const& lhs, const_iterator<T> const& rhs) noexcept {
    return lhs.ptr_ <= rhs.ptr_;
}

template <typename T>
constexpr bool operator>=(const_iterator<T> const& lhs, const_iterator<T> const& rhs) noexcept {
    return lhs.ptr_ >= rhs.ptr_;
}

template <typename T>
constexpr bool operator<(const_iterator<T> const& lhs, const_iterator<T> const& rhs) noexcept {
    return !(lhs >= rhs);
}

template <typename T>
constexpr bool operator>(const_iterator<T> const& lhs, const_iterator<T> const& rhs) noexcept {
    return !(lhs <= rhs);
}

template <typename T>
constexpr const_iterator<T> operator+(const_iterator<T> const& it, typename const_iterator<T>::difference_type n) noexcept {
    return const_iterator<T>{it.ptr_ + n};
}

template <typename T>
constexpr const_iterator<T>  operator+(typename const_iterator<T>::difference_type n, const_iterator<T> const& it) noexcept {
    return it + n;
}

template <typename T>
constexpr const_iterator<T> operator-(const_iterator<T> const& it, typename const_iterator<T>::difference_type n) noexcept {
    return it + -n;
}

template <typename T>
constexpr typename const_iterator<T>::difference_type operator-(const_iterator<T> const& lhs, const_iterator<T> const& rhs) noexcept {
    return lhs.ptr_ - rhs.ptr_;
}

} // namespace detail

#endif /* STATVEC_H */
