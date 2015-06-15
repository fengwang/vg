#ifndef RBMPKILMKEJNTUDSKXKSKHCFSALYJEBXYHQGHKMLYIRKIPICJJKDQVATCFXEAAAYEAVIMMKIN
#define RBMPKILMKEJNTUDSKXKSKHCFSALYJEBXYHQGHKMLYIRKIPICJJKDQVATCFXEAAAYEAVIMMKIN

#include <cstddef>
#include <cstdint>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <chrono>
#include <limits>

namespace vg
{

    // Seed

    inline auto make_seed() noexcept
    {
        typedef std::uint_fast64_t        seed_type;
        auto const now                  = std::chrono::system_clock::now();
        auto const duration             = now.time_since_epoch();
        seed_type  const nano_seed      = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
        seed_type const random_address  = reinterpret_cast<seed_type>( &nano_seed );
        seed_type ans                   = nano_seed + ( random_address | (random_address << 32) );
        ans                             = ( ans & 0x5555555555555555ULL ) <<  1 | ( ans & 0xAAAAAAAAAAAAAAAAULL ) >>  1;
        ans                             = ( ans & 0x3333333333333333ULL ) <<  2 | ( ans & 0xCCCCCCCCCCCCCCCCULL ) >>  2;
        ans                             = ( ans & 0x0F0F0F0F0F0F0F0FULL ) <<  4 | ( ans & 0xF0F0F0F0F0F0F0F0ULL ) >>  4;
        ans                             = ( ans & 0x00FF00FF00FF00FFULL ) <<  8 | ( ans & 0xFF00FF00FF00FF00ULL ) >>  8;
        ans                             = ( ans & 0x0000FFFF0000FFFFULL ) << 16 | ( ans & 0xFFFF0000FFFF0000ULL ) >> 16;
        ans                             = ( ans & 0x00000000FFFFFFFFULL ) << 32 | ( ans & 0xFFFFFFFF00000000ULL ) >> 32;
        ans                            ^= nano_seed;
        return                            ans;
    }

    // Engine

    inline auto linear_congruential() noexcept
    {
        static const std::uint_fast64_t a_ = 6364136223846793005ULL;
        static const std::uint_fast64_t c_ = 1442695040888963407ULL;
        std::uint_fast64_t              x_ = make_seed();

        return [=]() mutable noexcept
        {
            x_ *= a_;
            x_ += c_;
            return static_cast<long double>( x_ ) / static_cast<long double>( std::numeric_limits<std::uint_fast64_t>::max() );
        };
    }

    // Distribution

    inline auto normal() noexcept
    {
        return []() noexcept
        {
            return []( auto& engine ) noexcept
            {
                long double const one = 1;

                for (;;)
                {
                    auto const u1 = engine();
                    auto const u2 = engine();
                    auto const v1 = u1 + u1 - one;
                    auto const v2 = u2 + u2 - one;
                    auto const s = v1 * v1 + v2 * v2;

                    if ( s < one )
                    {
                        auto const tmp = -std::log( s );
                        return v1 * std::sqrt(( tmp + tmp ) / s );
                    }
                }
            };
        };
    }

    auto inline uniform() noexcept
    {
        return []( auto lower, auto upper ) noexcept
        {
            return [=]( auto& engine ) noexcept
            {
                return engine() * ( upper - lower ) + lower;
            };
        };
    }

    //Generator

    template< typename Distribution, typename Engine >
    auto make_generator( Distribution distribution, Engine engine ) noexcept
    {
        auto engine_ = engine();
        return [=]( auto ... args ) mutable noexcept
        {
            return [=]() mutable noexcept
            {
                return distribution()( args... )( engine_ );
            };
        };
    }

    template< typename Distribution >
    auto make_generator( Distribution distribution ) noexcept
    {
        return make_generator( distribution, linear_congruential );
    }

    //More Engines

    inline auto mt19937() noexcept
    {
        std::uint_fast64_t                      mt[312];
        std::uint_fast64_t                      mag01[2];
        std::size_t                             mti;

        mt[0]    = make_seed();
        mag01[0] = 0ULL;
        mag01[1] = 0xB5026F5AA96619E9ULL;

        for ( mti = 1; mti < 312; ++mti )
        {
            mt[mti] =  6364136223846793005ULL * ( mt[mti-1] ^( mt[mti-1] >> 62 ) );
            mt[mti] += mti;
        }

        return [=]() mutable noexcept
        {
            std::uint_fast64_t x;

            if ( mti > 311 )
            {
                for ( std::size_t i = 0; i < 156; ++i )
                {
                    x = ( mt[i] & 0xFFFFFFFF80000000ULL ) | ( mt[i+1] & 0x7FFFFFFFULL );
                    mt[i] = mt[i+156] ^( x >> 1 ) ^ mag01[x&1];
                }

                for ( std::size_t i = 156; i < 311; ++i )
                {
                    x = ( mt[i] & 0xFFFFFFFF80000000ULL ) | ( mt[i+1] & 0x7FFFFFFFULL );
                    mt[i] = mt[i-156] ^( x >> 1 ) ^ mag01[x&1];
                }

                x = ( mt[311] & 0xFFFFFFFF80000000ULL ) | ( mt[0] & 0x7FFFFFFFULL );
                mt[311] = mt[155] ^( x >> 1 ) ^ mag01[x&1];
                mti = 0;
            }

            x  = mt[mti++];
            x ^= ( x >> 29 ) & 0x5555555555555555ULL;
            x ^= ( x << 17 ) & 0x71D67FFFEDA60000ULL;
            x ^= ( x << 37 ) & 0xFFF7EEE000000000ULL;
            x ^= ( x >> 43 );

            return static_cast<long double>( x ) / static_cast<long double>( std::numeric_limits<std::uint_fast64_t>::max() );
        };
    }

    inline auto lagged_fibonacci() noexcept
    {
        std::uint_fast64_t data[44497];
        std::size_t mti = make_seed() % 21034;

        data[0] = 1;
        data[1] = 1;

        for ( std::size_t index = 2; index != 44497; ++index )
            data[index] = data[index-2] + data[index-1];

        return[=]() mutable noexcept
        {
            if ( mti > 21034 )
            {
                mti = 0;
                std::copy( data+21034, data+44497, data );
                for ( std::size_t index = 23463; index != 44497; ++index ) data[index] = data[index-2] + data[index-1];
            }

            auto const mtj  = mti + 23463;
            auto const x    = data[mti++] + data[mtj];
            auto const ans  = static_cast<long double>( x ) / static_cast<long double>( std::numeric_limits<std::uint_fast64_t>::max() );
            return ans;
        };
    }

    //More Non-Uniform Distributions

    inline auto gaussian() noexcept
    {
        return []( auto mean, auto variance ) noexcept
        {
            return [=]( auto& engine ) noexcept
            {
                long double const normal_ = normal()()( engine );
                return normal_ * variance + mean;
            };
        };
    }

    inline auto gamma() noexcept
    {
        return []( auto alpha, auto beta ) noexcept
        {
            assert( alpha > 0.0 );
            assert( beta > 0.0 );

            return [=]( auto& engine ) noexcept
            {
                long double const zero  = 0.0;
                long double const one   = 1.0;
                long double const three = 3.0;
                long double const a     = alpha < one ? alpha + one : alpha;
                long double const d     = a - one / three;
                long double const c     = one / (three * std::sqrt( d ));
                long double u           = zero;
                long double v           = zero;
                long double x           = zero;

                for ( ;; )
                {
                    for ( ;; )
                    {
                        x = normal()()( engine );
                        v = one + c * x;

                        if ( v > zero ) break;
                    }

                    const long double xx = x * x;

                    v *= v * v;

                    u = engine();

                    if ( u < one - 0.0331 * xx * xx ) 
                        break;

                    if ( std::log( u ) < 0.5 * xx + d * ( one - v + std::log( v ) ) )
                        break;
                }

                long double tmp = d * v;

                if ( alpha < one )
                    tmp *= std::pow( engine(), one / alpha );

                return tmp * beta;
            };
        };
    }

    inline auto beta() noexcept
    {
        return []( auto alpha, auto beta ) noexcept
        {
            return [=]( auto& engine ) noexcept
            {
                long double const one = 1.0;
                auto const a = gamma()( alpha, one )( engine );
                auto const b = gamma()( beta, one )( engine );
                return a / ( a + b );
            };
        };
    }

    inline auto arcsine() noexcept
    {
        return []() noexcept
        {
            return []( auto& engine ) noexcept
            {
                long double const half = 0.5;
                return beta()( half, half )( engine );
            };
        };
    }

    inline auto balding_nichols() noexcept
    {
        return []( auto p, auto F ) noexcept
        {
            assert( p >= 0.0  );
            assert( p <= 1.0 );
            assert( F >= 0.0 );
            assert( F <= 1.0 );
            return [=]( auto& engine ) noexcept
            {
                long double const one = 1.0;
                return beta()( ( one - F ) * p / F, ( one - F ) * ( one - p ) / F )( engine );
            };
        };
    }

    inline auto bernoulli() noexcept
    {
        return []( auto p ) noexcept
        {
            assert( p >= 0.0 );
            assert( p <= 1.0 );
            return [=]( auto& engine ) noexcept
            {
                if ( engine() > p ) return 0;
                return 1;
            };
        };
    }

    inline auto exponential() noexcept
    {
        return []( auto lambda ) noexcept
        {
            assert( lambda > 0.0 );
            return [=]( auto& engine ) noexcept
            {
                return - std::log( engine() ) / lambda;
            };
        };
    }

    inline auto binomial() noexcept
    {
        auto coin_flip = []( auto N, auto p ) noexcept
        {
            return [=]( auto& engine ) noexcept
            {
                unsigned long ans = 0;
                for ( decltype(N) index = 0; index != N; ++index )
                    if ( engine() < p ) ++ans;
                return ans;
            };
            
        };

        auto second_waiting_time = []( auto N, auto p ) noexcept
        {
            return [=]( auto& engine ) noexcept
            {
                long double const one  = 1.0;
                long double const half = 0.5;
                long double const p_   = p > half ? one - p : p;
                long double const q    = - std::log( one - p_ );
                unsigned long ans      = 0;
                long double sum        = 0.0;

                for ( ;; )
                {
                    sum += exponential()( one )( engine ) / ( N - ans );
                    if ( sum > q ) break;
                    ans++;
                }

                return p > half ? N - ans : ans;
            };
        };

        auto rejection = [=]( auto N, auto p ) noexcept
        {
            return [=]( auto& engine ) noexcept
            {
                long double const one   = 1.0;
                unsigned long n         = N;
                long double   p_        = p;
                unsigned long ans       = 0;

                while ( n > 8 )
                {
                    unsigned long const a = 1 + (n >> 1);
                    unsigned long const b = 1 + n - a;
                    long double const   X = beta()( a, b )( engine );

                    if ( X >= p_ )
                    {
                        n = a - 1;
                        p_ /= X;
                    }
                    else
                    {
                        ans += a;
                        n = b - 1;
                        p_ = ( p_ - X ) / ( one - X );
                    }
                }

                ans += coin_flip( n, p_ )( engine );
                return ans;
            };
        };

        return [=]( auto N, auto p ) noexcept
        {
            assert( N > 0 );
            assert( p >= 0.0 );
            assert( p <= 1.0 );

            return [=]( auto& engine ) noexcept
            {
                if ( N < 8 ) return coin_flip( N, p )( engine );
                if ( N*std::min(p,1.0-p) < 10.0 ) return second_waiting_time( N, p )( engine );
                return rejection( N, p )( engine );
            };
        };
    }

}//namespace vg

#endif//RBMPKILMKEJNTUDSKXKSKHCFSALYJEBXYHQGHKMLYIRKIPICJJKDQVATCFXEAAAYEAVIMMKIN

