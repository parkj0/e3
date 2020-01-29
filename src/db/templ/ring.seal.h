// === BEGIN  ring.seal.h Name=$Name

extern e3::SealEvalKey * g_pek_$Name;

class $Name
{
        struct Init { Init() { init0(); } } init_object;

    protected:
        e3::SealNativeCiphertext x;

        static e3::SealEvalKey * pek;
        static const $Name * zero, * unit;

    public:
        static const bool is_signed = false;
        static const int size = 0;
        static const bool is_circuit = false;

        static const char * typname() { return "$Name"; }
        static const char * filname() { return "$Filencname"; }
        static const char * clsname() { return "$Clsencname"; }

        // Constructors
        static void init0()
        {
            if ( !pek )
            {
                initEvalKey(e3::KeyName {typname(), filname()}, pek, g_pek_$Name);
                if ( !unit ) unit = new $Name("$ArithUnit");
                if ( !zero ) zero = new $Name("$ArithZero");
            }
        }

        $Name(): x() {}
        $Name(const char * c) : $Name(std::string(c)) {}
        $Name(e3::SealNativeCiphertext a) : x(a) {}
        $Name(const $Name & a) : x(a.x) {}
        $Name(const std::string & s) : x( pek->decor(s, false), pek->key ) {}

        // public encryption
        explicit $Name(unsigned long long a) : $Name( pek->encrypt( std::to_string(a), 0 ) ) {}
        explicit $Name(const std::vector<unsigned long long> & a) : $Name( pek->encrypt( e3::util::merge(a, "_"), 0 ) ) {}

        // Operators
        $Name & operator=(const $Name & a) { $Name r(a); x = r.x; *this; }
        $Name & operator+=(const $Name & a);
        $Name & operator-=(const $Name & a);
        $Name & operator*=(const $Name & a);
        $Name operator+(const $Name & a) const;
        $Name operator-(const $Name & a) const;
        $Name operator*(const $Name & a) const;
        $Name & operator++() { return *this += *unit; }
        $Name & operator--() { return *this -= *unit; }
        $Name operator++(int) { $Name r(*this); ++*this; return r; }
        $Name operator--(int) { $Name r(*this); --*this; return r; }

        $Name operator*(unsigned long long u) const { return e3::multiply_by_ull(u, *this); }
        friend $Name operator*(unsigned long long u, const $Name & a) { return a * u; }
        // $Name & operator++() { return *this += *unit; }
        // $Name & operator--() { return *this -= *unit; }
        // $Name operator++(int) { $Name r(*this); ++*this; return r; }
        // $Name operator--(int) { $Name r(*this); --*this; return r; }

        // $Name operator+(const $Name & a) const;
        // $Name operator-(const $Name & a) const; // { return *this + (-a); }
        // $Name operator-() const;

        // $Name operator*(unsigned long long u) const { return multiply_by_ull(u, *this); }
        // friend $Name operator*(unsigned long long u, const $Name & a) { return a * u; }
        //
        // $Name operator+(unsigned long long u) const { return *this + (*unit * u); }
        // friend $Name operator+(unsigned long long u, const $Name & a) { return a + u; }
        //
        // $Name operator-(unsigned long long u) const { return *this - (*unit * u); }
        // friend $Name operator-(unsigned long long u, const $Name & a) { return a - u; }

        // Functions
        std::string str() const { return pek->decor(x.str(), true); }

        // $Name & operator*=(const $Name & a) = delete;
        // $Name operator*(const $Name & a) const = delete;
};

inline std::ostream & operator<<(std::ostream & os, const $Name & x) { return os << x.str(); }

$PostfixDefines

// === END  ring.seal.h Name=$Name
