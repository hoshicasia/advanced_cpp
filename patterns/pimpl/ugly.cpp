#include <ugly.h>

struct SplineImpl {
    SplineImpl() = default;
    int BuildSpline(const std::vector<double>& x, const std::vector<double>& y, double a,
                    double b) {
        size_t i; /* loop index for matrix computation. */
        double* gam;
        double dx2; /* Change in x on left */
        double dx3; /* Change in x on right */
        double bet; /* 2 * (dx2 + dx3) (center of matrix) */
        double r_i;
        size_t n = x.size();
        n_ = n;
        /* Right side of equation */
        x_ = x;
        y_ = y;
        if (n < 1) {
            return 0;
        }
        y2_.resize(n);
        y2_[0] = a;
        if (n == 1) {
            return 0;
        }
        if (n == 2) {
            y2_[1] = b;
            return 0;
        }

        n -= 2;
        /* Allocate a vector of 0..n-1 inclusive. Note: a[i] = c[i-1] = dx */
        if ((gam = (double*)malloc(n * sizeof(double))) == NULL) {  // NOLINT
            return -1;
        }

        dx2 = x[1] - x[0];
        dx3 = x[2] - x[1];
        r_i = 6 * ((y[2] - y[1]) / dx3 - (y[1] - y[0]) / dx2) - dx2 * a;
        if (n == 1) {
            r_i += -dx3 * b;
        }
        bet = 2 * (x[2] - x[0]);
        if (bet == 0.0) {
            free(gam);
            return -2;
        }
        y2_[1] = r_i / bet;
        for (i = 1; i < n; ++i) {
            dx2 = x[i + 1] - x[i];
            dx3 = x[i + 2] - x[i + 1];
            r_i = 6 * ((y[i + 2] - y[i + 1]) / dx3 - (y[i + 1] - y[i]) / dx2);
            if (i == n - 1) {
                r_i += -dx3 * b;
            }
            gam[i] = dx2 / bet;
            /* I think: 1 > gam > 0 */
            bet = 2 * (x[i + 2] - x[i]) - dx2 * gam[i];
            /* I think: bet > 0 */
            if (bet == 0) {
                /* If x is monitonic increasing, then bet > dx2 => gam < 0 */
                /* => dx2 * gam < x[i + 2] - x[i] => bet > 0 */
                free(gam);
                return -2;
            }
            y2_[i + 1] = (r_i - dx2 * y2_[i]) / bet;
        }
        for (i = (n - 1); i >= 1; --i) {
            y2_[i] -= gam[i] * y2_[i + 1];
        }
        free(gam);
        /* I've subtracted 2 from n, so instead of y2[n - 1] we have y2[n + 1] */
        y2_[n + 1] = b;
        return 0;
    }

    double Interpolate(double x) {
        int lo = 0;      /* Index just below x, or 0. */
        int hi = n_ - 1; /* Index just above x, or n - 1. */
        int k;           /* Current midpoint in binary search. */
        double dx;       /* Change in x (xa[hi] - xa[lo]). */
        double a;        /* "upper" portion temp variable. */
        double b;        /* "lower" portion temp variable. */

        if (n_ == 0) {
            return -1;
        }
        /* Perform a binary search through xa to find values where: */
        /* 1) xa[lo] < x < xa[hi], and 2) hi - lo = 1 */
        while (hi - lo > 1) {
            k = (hi + lo) >> 1;
            if (x_[k] > x) {
                hi = k;
            } else {
                lo = k;
            }
        }
        dx = x_[hi] - x_[lo];
        if (dx == 0.0) {
            return -2;
        }
        a = (x_[hi] - x) / dx;
        b = (x - x_[lo]) / dx;
        double res = a * y_[lo] + b * y_[hi] +
                     ((a * a * a - a) * y2_[lo] + (b * b * b - b) * y2_[hi]) * dx * dx / 6.0;
        return res;
    }

private:
    size_t n_ = 0;
    std::vector<double> x_;
    std::vector<double> y_;
    std::vector<double> y2_;
};

Spline::Spline(const std::vector<double>& x, const std::vector<double>& y, double a, double b)
    : impl_(new SplineImpl()) {
    impl_->BuildSpline(x, y, a, b);
}

double Spline::Interpolate(double x) {
    return impl_->Interpolate(x);
}
