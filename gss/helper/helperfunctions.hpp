typedef long long ll;

// Functions signature
ll pow(ll base, ll expo, ll mod);


// Functions implementations
ll pow(ll base, ll expo, ll mod) {
    ll result = 1;
    while(expo) {
        if (expo & 1) {
            result = (result*base) % mod; 
        }
        base = (base*base % mod);
        expo >>= 1;
    }
    return result;
}