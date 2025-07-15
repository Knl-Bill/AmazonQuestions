#include <bits/stdc++.h>
using namespace std;

// Returns the number of substrings of `s` in which some character appears at least `k` times
long long calculateRepetitiveness(const string &s, int k) {
    int n = s.size();

    // Edge case: if k <= 1, then every substring is valid
    if (k <= 1) {
        return 1LL * n * (n + 1) / 2;
    }

    // Step 1: Build a list of positions for each byte value [0..255]
    vector<vector<int>> pos(256);
    for (int i = 0; i < n; i++) {
        unsigned char c = s[i];
        pos[c].push_back(i);
    }

    // Step 2: Sliding pointers to track start of window for each character
    vector<int> pointers(256, 0);

    // Step 3: Min-heap (priority queue) of pairs (j, c)
    // Meaning: if window starts at i, substring ending at j is the earliest
    // that has k copies of c.
    using P = pair<int, unsigned char>;
    priority_queue<P, vector<P>, greater<P>> pq;

    for (int c = 0; c < 256; c++) {
        if ((int)pos[c].size() >= k) {
            // For i = 0, pointers[c] = 0, so k-th occurrence is at pos[c][k-1]
            pq.emplace(pos[c][k - 1], (unsigned char)c);
        }
    }

    long long ans = 0;

    // Step 4: Sweep i from 0 to n - 1
    for (int i = 0; i < n; i++) {
        // Remove stale entries from heap (entries not matching current pointer state)
        while (!pq.empty()) {
            auto [j, c] = pq.top();
            int p = pointers[c];

            // Check if the heap's top is still valid
            if (p + k - 1 < (int)pos[c].size() && pos[c][p + k - 1] == j) {
                break; // Valid entry
            }
            pq.pop(); // Stale entry
        }

        if (pq.empty()) {
            // No character can reach k copies from index i onward
            break;
        }

        // Get the minimum j such that s[i..j] has some char with freq ≥ k
        int jmin = pq.top().first;
        ans += (n - jmin); // All substrings s[i..jmin], s[i..jmin+1], ..., s[i..n-1] are valid

        // Advance window start from i to i+1: update for character s[i]
        unsigned char c0 = s[i];
        int &p0 = pointers[c0];

        // If current pointer was pointing at index i, move it forward
        if (p0 < (int)pos[c0].size() && pos[c0][p0] == i) {
            p0++;

            // If still at least k occurrences from new p0, push updated j into heap
            if (p0 + k - 1 < (int)pos[c0].size()) {
                pq.emplace(pos[c0][p0 + k - 1], c0);
            }
        }
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    int k;
    cin >> s >> k;

    cout << calculateRepetitiveness(s, k) << "\n";
    return 0;
}
