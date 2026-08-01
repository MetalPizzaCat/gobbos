#pragma once

/// @brief Returns the largest value out of two given
/// @tparam T 
/// @param a 
/// @param b 
/// @return 
template <typename T>
inline T max(T a, T b) { return a > b ? a : b; }

/// @brief Returns the smaller value out of two given
/// @tparam T 
/// @param a 
/// @param b 
/// @return 
template <typename T>
inline T min(T a, T b) { return a < b ? a : b; }