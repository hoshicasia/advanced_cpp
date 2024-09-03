template <bool B, class T, class F>
struct Cond {
    using Type = T;
};

template <class T, class F>
struct Cond<false, T, F> {
    using Type = F;
};


template<int n, int lo = 1, int hi = n>
struct Sqrt {
    enum { mid = (lo + hi + 1) / 2 };
    //typedef typename Cond<(mid * mid == n), Sqrt<n, mid, mid>, Sqrt<n, lo, mid - 1> >::Type T;
    //typedef typename Cond<(mid * mid >= n), F, Sqrt<n, mid, hi> >::Type T;
    typedef typename Cond<(mid * mid > n), Sqrt<n, lo, mid - 1>, Sqrt<n, mid, hi> >::Type T;
    enum { value = T::value };
};

template<int n, int m>
struct Sqrt<n, m, m> {
    enum { value = m };
};
