///////////////////////////////////////////////////////////////////////////////
// disks.hpp
//
// Definitions for two algorithms that each solve the alternating disks
// problem.
//
// As provided, this header has four functions marked with comments.
// You need to write in your own implementation of these functions.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

// State of one disk, either light or dark.
enum disk_color { DISK_LIGHT, DISK_DARK };

// Data structure for the state of one row of disks.
class disk_state {
private:
	std::vector<disk_color> _colors;

public:
	disk_state(size_t light_count) : _colors(light_count * 2, DISK_LIGHT) {
		assert(light_count > 0);
		for (size_t i = 0; i < _colors.size(); i += 2) {
			_colors[i] = DISK_DARK;
		}
	}

	// Equality operator for unit tests.
	bool operator== (const disk_state& rhs) const {
		return std::equal(_colors.begin(), _colors.end(), rhs._colors.begin());
	}

	size_t total_count() const {
		return _colors.size();
	}

	size_t light_count() const {
		return total_count() / 2;
	}

	size_t dark_count() const {
		return light_count();
	}

	bool is_index(size_t i) const {
		return (i < total_count());
	}

	disk_color get(size_t index) const {
		assert(is_index(index));
		return _colors[index];
	}

	void swap(size_t left_index) {
		assert(is_index(left_index));
		auto right_index = left_index + 1;
		assert(is_index(right_index));
		std::swap(_colors[left_index], _colors[right_index]);
	}

	std::string to_string() const {
		std::stringstream ss;
		bool first = true;
		for (auto color : _colors) {
			if (!first) {
				ss << " ";
			}

			if (color == DISK_LIGHT) {
				ss << "L";
			} else {
				ss << "D";
			}

			first = false;
		}
		return ss.str();
	}

	bool is_alternating() const {
		bool s = false;
		// checks only even indices for dark disks
		for (unsigned i = 0; i < total_count(); i = i + 2) {
			if (get(i) == DISK_DARK) {
				s = true;
			} else {
				return false;
			}
		}
		return s;
	}

	bool is_sorted() const {
		bool s = false;
		// checks the first half of disks for light disks
		for (unsigned i = 0; i < (total_count() / 2); i++) {
			if (get(i) == DISK_LIGHT) {
				s = true;
			} else {
				return false;
			}
		}
		return s;
	}
};

// Data structure for the output of the alternating disks problem. That
// includes both the final disk_state, as well as a count of the number
// of swaps performed.
class sorted_disks {
private:
	disk_state _after;
	unsigned _swap_count;

public:
	sorted_disks(const disk_state& after, unsigned swap_count) : _after(after), _swap_count(swap_count) {

	}

	sorted_disks(disk_state&& after, unsigned swap_count) : _after(after), _swap_count(swap_count) {

	}

	const disk_state& after() const {
		return _after;
	}

	unsigned swap_count() const {
		return _swap_count;
	}
};

// Algorithm that sorts disks using the left-to-right algorithm.
sorted_disks sort_left_to_right(const disk_state& before) {

	int swap_count = 0;
	disk_state after = before;

	while(!after.is_sorted()) {
		for (size_t i = 0; i < after.total_count() - 1; i++) {
			// checks for a dark disk AND a dark disk one after another
			if (after.get(i) == DISK_DARK && after.get(i + 1) == DISK_LIGHT) {
				after.swap(i);
				swap_count++;
			}
		}
	}

	return sorted_disks(after, swap_count);
}

// Algorithm that sorts disks using the lawnmower algorithm.
sorted_disks sort_lawnmower(const disk_state& before) {
	int swap_count = 0;
	disk_state after = before;
	while(!after.is_sorted()) {
		// sorts going in the right direction
		for (size_t i = 0; i < after.total_count() - 1; i++) {
			if (after.get(i) == DISK_DARK && after.get(i + 1) == DISK_LIGHT) {
				after.swap(i);
				swap_count++;
			}
		}

		// sorts going in the left direction
		for (size_t j = after.total_count() - 1; j > 0; j--) {
			if (after.get(j) == DISK_LIGHT && after.get(j - 1) == DISK_DARK) {
				after.swap(j - 1);
				swap_count++;
			}
		}
	}

	return sorted_disks(after, swap_count);
}
